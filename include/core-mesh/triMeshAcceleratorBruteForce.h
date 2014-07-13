
#ifndef _TRIMESH_ACCELERATOR_BRUTEFORCE_H_
#define _TRIMESH_ACCELERATOR_BRUTEFORCE_H_

namespace ml {

template <class FloatType>
class TriMeshAcceleratorBruteForce : public TriMeshRayAccelerator<FloatType>
{
public:
	TriMeshAcceleratorBruteForce(const TriMesh<FloatType>& triMesh, bool storeLocalCopy = false) {
		build(triMesh, storeLocalCopy);
	}

	typename const TriMesh<FloatType>::Triangle<FloatType>* intersect(const Ray<FloatType>& r, FloatType& t, FloatType& u, FloatType& v, FloatType tmin = (FloatType)0, FloatType tmax = std::numeric_limits<FloatType>::max(), bool onlyFrontFaces = false) const {
		
		for (size_t i = 0; i < m_TrianglePointers.size(); i++) {

		}
	}


private:
	void buildInternal() {
		//nothing to do here
	}

};


} //namespace ml

#endif