
#ifndef EXT_ANN_NEARESTNEIGHBORSEARCHANN_H_
#define EXT_ANN_NEARESTNEIGHBORSEARCHANN_H_

namespace ml
{

template<class FloatType>
class NearestNeighborSearchANN : public NearestNeighborSearch<FloatType>
{
public:
	NearestNeighborSearchANN()
	{
		m_dimension = 0;
		m_maxK = 0;
		m_indices = nullptr;
		m_dists = nullptr;
		m_queryPt = nullptr;
		m_points = nullptr;
		m_tree = nullptr;
	}

	~NearestNeighborSearchANN()
	{
		SAFE_DELETE_ARRAY(m_indices);
		SAFE_DELETE_ARRAY(m_dists);
		SAFE_DELETE(m_tree);
		if(m_queryPt) annDeallocPt(m_queryPt);
		if(m_points) annDeallocPts(m_points);
	}


private:
	void initInternal(const std::vector< const FloatType* > &points, UINT dimension, UINT maxK)
	{
		if (points.size() >= std::numeric_limits<int>::max()) throw MLIB_EXCEPTION("ANN only supports 'int'");

		const UINT pointCount = (UINT)points.size();
		m_dimension = dimension;
		m_maxK = maxK;
		m_queryPt = annAllocPt(m_dimension);
		m_points = annAllocPts(pointCount, m_dimension);
		m_indices = new ANNidx[m_maxK];
		m_dists = new ANNdist[m_maxK];
		for (unsigned int i = 0; i < pointCount; i++) {
			for (unsigned int elementIndex = 0; elementIndex < m_dimension; elementIndex++) {
				m_points[i][elementIndex] = points[i][elementIndex];
			}
		}
		const bool useBruteForce = false;
		if (useBruteForce) m_tree = new ANNbruteForce(m_points, pointCount, m_dimension);
		else m_tree = new ANNkd_tree(m_points, pointCount, m_dimension);
	}

	void kNearestInternal(const FloatType *query, UINT k, FloatType epsilon, std::vector<UINT> &result) const
	{
		for(UINT elementIndex = 0; elementIndex < m_dimension; elementIndex++)
			m_queryPt[elementIndex] = query[elementIndex];
		m_tree->annkSearch(  // search
			m_queryPt,       // query point
			k,               // number of near neighbors
			m_indices,       // nearest neighbors (returned)
			m_dists,         // distance (returned)
			epsilon);        // error bound
		if(result.size() < k) result.resize(k);
		for(UINT i = 0; i < k; i++)
			result[i] = m_indices[i];
	}

    void fixedRadiusInternal(const FloatType *query, UINT k, FloatType radiusSq, FloatType epsilon, std::vector<UINT> &result) const
    {
        for(UINT elementIndex = 0; elementIndex < m_dimension; elementIndex++)
            m_queryPt[elementIndex] = query[elementIndex];
        int count = m_tree->annkFRSearch(  // search
            m_queryPt,       // query point
            radiusSq,
            k,               // number of near neighbors
            m_indices,       // nearest neighbors (returned)
            m_dists,         // distance (returned)
            epsilon);        // error bound

        result.resize(math::min(count, (int)k));
        for(int i = 0; i < result.size(); i++)
            result[i] = m_indices[i];
    }

    void fixedRadiusInternalDist(const FloatType *query, UINT k, FloatType radiusSq, FloatType epsilon, std::vector< std::pair<UINT, FloatType> > &result) const
    {
        for(UINT elementIndex = 0; elementIndex < m_dimension; elementIndex++)
            m_queryPt[elementIndex] = query[elementIndex];
        int count = m_tree->annkFRSearch(  // search
            m_queryPt,       // query point
            radiusSq,
            k,               // number of near neighbors
            m_indices,       // nearest neighbors (returned)
            m_dists,         // distance (returned)
            epsilon);        // error bound

        result.resize(math::min(count, (int)k));
        for(int i = 0; i < result.size(); i++)
        {
            result[i] = std::make_pair(m_indices[i], (FloatType)m_dists[i]);
        }
    }

	UINT                 m_maxK;        // Maximum value of k
	UINT                 m_dimension;   // dimensionality of KDTree
	ANNpointArray        m_points;      // data points
	mutable ANNpoint     m_queryPt;     // query point
	mutable ANNidxArray  m_indices;     // near neighbor indices
	mutable ANNdistArray m_dists;       // near neighbor distances
	ANNpointSet*         m_tree;        // search structure
};

typedef NearestNeighborSearchANN<float> NearestNeighborSearchANNf;
typedef NearestNeighborSearchANN<double> NearestNeighborSearchANNd;

}  // namespace ml

#endif  // EXT_ANN_NEARESTNEIGHBORSEARCHANN_H_
