
#pragma once
#ifndef _NEAREST_NEIGHBOR_SEARCH_H_
#define _NEAREST_NEIGHBOR_SEARCH_H_

template<class D>
class NearestNeighborSearch
{
public:
	void init(const Vector< const D* > &points, UINT dimension, UINT maxK)
	{
		initInternal(points, dimension, maxK);
	}
	void kNearest(const D *query, UINT k, D epsilon, Vector<UINT> &result) const
	{
		kNearestInternal(query, k, epsilon, result);
	}
	
	void init(const Vector< Vector<D> > &points, UINT maxK)
	{
		//Vector< const D* > v = points.map([](const Vector<D> &x) {return x.ptr();});
		Vector< const D* > v(points.size());
		for(UINT i = 0; i < points.size(); i++)
			v[i] = points[i].ptr();
		init(v, (UINT)points[0].size(), maxK);
	}

	void kNearest(const Vector<D> &query, UINT k, D epsilon, Vector<UINT> &result) const
	{
		kNearestInternal(query.ptr(), k, epsilon, result);
	}


	Vector<UINT> kNearest(const Vector<D> &query, UINT k, D epsilon) const
	{
		Vector<UINT> result;
		kNearestInternal(query.ptr(), k, epsilon, result);
		return result;
	}

	Vector<UINT> kNearest(const D *query, UINT k, D epsilon) const
	{
		Vector<UINT> result;
		kNearestInternal(query, k, epsilon, result);
		return result;
	}

private:
	virtual void initInternal(const Vector< const D* > &points, UINT dimension, UINT maxK) = 0;
	virtual void kNearestInternal(const D *query, UINT k, D epsilon, Vector<UINT> &result) const = 0;
};

template<class D>
class KNearestNeighborQueue
{
public:
	struct NeighborEntry
	{
		NeighborEntry() {}
		NeighborEntry(int _index, D _dist)
		{
			index = _index;
			dist = _dist;
		}
		int index;
		D dist;
	};

	void init(UINT k, D clearValue)
	{
		if(m_queue.size() != k) m_queue.allocate(k);
		clear(clearValue);
	}

	void clear(D clearValue)
	{
		m_queue.clear(NeighborEntry(-1, clearValue));
		m_farthestDist = clearValue;
	}

	inline void insert(const NeighborEntry &entry)
	{
		if(entry.dist < m_farthestDist)
		{
			m_queue.back() = entry;
			const int queueLength = (int)m_queue.size();
			if(queueLength > 1)
			{
				NeighborEntry *data = m_queue.ptr();
				for(int index = queueLength - 2; index >= 0; index--)
				{
					if(data[index].dist > data[index + 1].dist)
					{
						std::swap(data[index], data[index + 1]);
					}
				}
			}
			m_farthestDist = m_queue.back().dist;
		}
	}

	const Vector<NeighborEntry>& queue() const
	{
		return m_queue;
	}

private:
	D m_farthestDist;
	Vector<NeighborEntry> m_queue;
};

template<class D>
class NearestNeighborSearchBruteForce : public NearestNeighborSearch<D>
{
public:
	void initInternal(const Vector< const D* > &points, UINT dimension, UINT maxK)
	{
		m_dimension = dimension;
		m_pointData.allocate(points.size() * dimension);
		int pointIndex = 0;
		for(auto p : points)
		{
			m_points.pushBack(m_pointData.ptr() + pointIndex);
			for(UINT d = 0; d < m_dimension; d++)
				m_pointData[pointIndex++] = p[d];
		}
	}

	void kNearestInternal(const D *query, UINT k, D epsilon, Vector<UINT> &result) const
	{
		m_queue.init(k, std::numeric_limits<D>::max());
		
		for(UINT pointIndex = 0; pointIndex < m_points.size(); pointIndex++)
		{
			D dist = 0.0f;
			const D* point = m_points[pointIndex];
			for(UINT d = 0; d < m_dimension; d++)
			{
				D diff = point[d] - query[d];
				dist += diff * diff;
			}
			m_queue.insert(typename KNearestNeighborQueue<D>::NeighborEntry(pointIndex, dist));
		}

		if(result.size() != k) result.allocate(k);
		UINT resultIndex = 0;
		for(const auto &e : m_queue.queue())
		{
			result[resultIndex++] = e.index;
		}
	}

private:
	UINT m_dimension;
	Vector<D> m_pointData;
	Vector< const D* > m_points;
	mutable KNearestNeighborQueue<D> m_queue;
};

#endif // _NEAREST_NEIGHBOR_SEARCH_H_