
#ifndef CORE_UTIL_NEARESTNEIGHBORSEARCH_H_
#define CORE_UTIL_NEARESTNEIGHBORSEARCH_H_

namespace ml
{

template<class D>
class NearestNeighborSearch
{
public:
	void init(const std::vector< const D* > &points, UINT dimension, UINT maxK)
	{
		initInternal(points, dimension, maxK);
	}
	void kNearest(const D *query, UINT k, D epsilon, std::vector<UINT> &result) const
	{
		kNearestInternal(query, k, epsilon, result);
	}
	
	void init(const std::vector< std::vector<D> > &points, UINT maxK)
	{
		//Vector< const D* > v = points.map([](const Vector<D> &x) {return x.ptr();});
		std::vector< const D* > v(points.size());
		for(UINT i = 0; i < points.size(); i++)
			v[i] = &points[i][0];
		init(v, (UINT)points[0].size(), maxK);
	}

	void kNearest(const std::vector<D> &query, UINT k, D epsilon, std::vector<UINT> &result) const
	{
		kNearestInternal(&query[0], k, epsilon, result);
	}

	std::vector<UINT> kNearest(const std::vector<D> &query, UINT k, D epsilon) const
	{
		std::vector<UINT> result;
		kNearestInternal(&query[0], k, epsilon, result);
		return result;
	}

	std::vector<UINT> kNearest(const D *query, UINT k, D epsilon) const
	{
		std::vector<UINT> result;
		kNearestInternal(query, k, epsilon, result);
		return result;
	}

    std::vector<UINT> fixedRadius(const std::vector<D> &query, UINT k, D radiusSq) const
	{
		std::vector<UINT> result;
		fixedRadiusInternal(&query[0], k, radiusSq, 0.0f, result);
		return result;
	}

    std::vector<UINT> fixedRadius(const D *query, UINT k, D radiusSq) const
    {
        std::vector<UINT> result;
        fixedRadiusInternal(query, k, radiusSq, 0.0f, result);
        return result;
    }

    std::vector< std::pair<UINT, D> > fixedRadiusDist(const D *query, UINT k, D radiusSq) const
    {
        std::vector< std::pair<UINT, D> > result;
        fixedRadiusInternalDist(query, k, radiusSq, 0.0f, result);
        return result;
    }

private:
	virtual void initInternal(const std::vector< const D* > &points, UINT dimension, UINT maxK) = 0;
	virtual void kNearestInternal(const D *query, UINT k, D epsilon, std::vector<UINT> &result) const = 0;
    virtual void fixedRadiusInternal(const D *query, UINT k, D radiusSq, D epsilon, std::vector<UINT> &result) const = 0;
    virtual void fixedRadiusInternalDist(const D *query, UINT k, D radiusSq, D epsilon, std::vector< std::pair<UINT, D> > &result) const = 0;
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

    KNearestNeighborQueue(UINT k, D clearValue)
    {
        init(k, clearValue);
    }

	void init(UINT k, D clearValue)
	{
		if(m_queue.size() != k) m_queue.resize(k);
		clear(clearValue);
	}

	void clear(D clearValue)
	{
		m_queue.assign(m_queue.size(), NeighborEntry(-1, clearValue));
		m_farthestDist = clearValue;
	}

    inline void insert(int index, D dist)
    {
        insert(NeighborEntry(index, dist));
    }

	inline void insert(const NeighborEntry &entry)
	{
		if(entry.dist < m_farthestDist)
		{
			m_queue.back() = entry;
			const int queueLength = (int)m_queue.size();
			if(queueLength > 1)
			{
				NeighborEntry *data = &m_queue[0];
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

	const std::vector<NeighborEntry>& queue() const
	{
		return m_queue;
	}

private:
	D m_farthestDist;
	std::vector<NeighborEntry> m_queue;
};

template<class D>
class NearestNeighborSearchBruteForce : public NearestNeighborSearch<D>
{
public:
	void initInternal(const std::vector< const D* > &points, UINT dimension, UINT maxK)
	{
		m_dimension = dimension;
		m_pointData.resize(points.size() * dimension);
		int pointIndex = 0;
		for(auto p : points)
		{
			m_points.push_back(&m_pointData[0] + pointIndex);
			for(UINT d = 0; d < m_dimension; d++)
				m_pointData[pointIndex++] = p[d];
		}
	}

	void kNearestInternal(const D *query, UINT k, D epsilon, std::vector<UINT> &result) const
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

		if(result.size() != k) result.resize(k);
		UINT resultIndex = 0;
		for(const auto &e : m_queue.queue())
		{
			result[resultIndex++] = e.index;
		}
	}

    void fixedRadiusInternal(const D *query, UINT k, D radiusSq, D epsilon, std::vector<UINT> &result) const
    {
        throw MLIB_EXCEPTION("fixedRadiusInternal not implemented");
    }

    void fixedRadiusInternalDist(const D *query, UINT k, D radiusSq, D epsilon, std::vector< std::pair<UINT, D> > &result) const
    {
        throw MLIB_EXCEPTION("fixedRadiusInternalDist not implemented");
    }

private:
	UINT m_dimension;
	std::vector<D> m_pointData;
	std::vector< const D* > m_points;
	mutable KNearestNeighborQueue<D> m_queue;
};

}  // namespace ml

#endif  // CORE_UTIL_NEARESTNEIGHBORSEARCH_H_
