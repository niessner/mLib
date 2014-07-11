
#ifndef _TRIMESH_ACCELERATOR_BRUTEFORCE_H_
#define _TRIMESH_ACCELERATOR_BRUTEFORCE_H_

namespace ml {

template <class FloatType>
class TriMeshRayAcceleratorBruteForce
{
public:

	TriMeshRayAcceleratorBruteForce() {}
	TriMeshRayAcceleratorBruteForce(const TriMesh<FloatType>& triMesh, bool storeLocalCopy = false) {

		build(triMesh, storeLocalCopy, useParallelBuild);
	}

	void build (const TriMesh<FloatType>& triMesh, bool storeLocalCopy = false) {

	}
private:

};

} //namespace ml

#endif