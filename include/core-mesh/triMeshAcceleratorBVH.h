#pragma once

#ifndef _TRIMESH_ACCELERATOR_BVH_H_
#define _TRIMESH_ACCELERATOR_BVH_H_

namespace ml {

template <class FloatType>
struct TriangleBVHNode {
	TriangleBVHNode() : rChild(0), lChild(0), leafTri(0) {}
	~TriangleBVHNode() {
		SAFE_DELETE(rChild);
		SAFE_DELETE(lChild);
	}

	//wait for vs 2013
	//template<class T>
	//using Triangle = TriMesh::Triangle<T>;

	BoundingBox3d<FloatType> boundingBox;
	typename TriMesh<FloatType>::Triangle<FloatType>* leafTri;


	TriangleBVHNode<FloatType> *lChild;
	TriangleBVHNode<FloatType> *rChild;

	void computeBoundingBox() {
		boundingBox.reset();
		
		if (!lChild && !rChild) {
			leafTri->includeInBoundingBox(boundingBox);
		} else {
			if (lChild)	{
				lChild->computeBoundingBox();
				boundingBox.include(lChild->boundingBox);
			}
			if (rChild) {
				rChild->computeBoundingBox();
				boundingBox.include(rChild->boundingBox);
			}
		}
	}


	void split(typename std::vector<typename TriMesh<FloatType>::Triangle<FloatType>*>::iterator& begin, typename std::vector<typename TriMesh<FloatType>::Triangle<FloatType>*>::iterator& end, unsigned int lastSortAxis) {
		if (end - begin > 1) {
			if (lastSortAxis == 0)		std::stable_sort(begin, end, cmpX);
			else if (lastSortAxis == 1)	std::stable_sort(begin, end, cmpY);
			else						std::stable_sort(begin, end, cmpZ);

			lChild = new TriangleBVHNode;
			rChild = new TriangleBVHNode;

			const unsigned int newSortAxis = (lastSortAxis+1)%3;
			lChild->split(begin, begin + ((end-begin)/2), newSortAxis);
			rChild->split(begin + ((end-begin)/2), end, newSortAxis);

		} else {
			assert(end - begin == 1);
			leafTri = *begin;	//found a leaf
		}
	}


	typename const TriMesh<FloatType>::Triangle<FloatType>* intersect(const Ray<FloatType> &r, FloatType& t, FloatType& u, FloatType& v, FloatType& tmin, FloatType& tmax, bool onlyFrontFaces = false) const {
		if (t < tmin || t > tmax)	return NULL;	//early out (warning t must be initialized)
		if (boundingBox.intersect(r, tmin, tmax)) {
			if (!lChild && !rChild) { //is leaf
				if (leafTri->intersect(r, t, u, v, tmin, tmax, onlyFrontFaces))	{
					tmax = t;
					return leafTri;
				}
			} else {
				typename const TriMesh<FloatType>::Triangle<FloatType>* t0 = lChild->intersect(r, t, u, v, tmin, tmax, onlyFrontFaces);
				typename const TriMesh<FloatType>::Triangle<FloatType>* t1 = rChild->intersect(r, t, u, v, tmin, tmax, onlyFrontFaces);
				if (t1)	return t1;
				if (t0)	return t0;
			}
		} 
		return NULL;
	}

	unsigned int getTreeDepthRec() const {
		unsigned int maxDepth = 0;
		if (lChild)	maxDepth = std::max(maxDepth, lChild->getTreeDepthRec());	
		if (rChild) maxDepth = std::max(maxDepth, rChild->getTreeDepthRec());
		return maxDepth+1;
	}

	unsigned int getNumNodesRec() const {
		unsigned int numNodes = 1;
		if (lChild)	numNodes += lChild->getNumNodesRec();	
		if (rChild) numNodes += rChild->getNumNodesRec();
		return numNodes;
	}

	unsigned int getNumLeaves() const {
		unsigned int numLeaves = 0;
		if (lChild) numLeaves += lChild->getNumLeaves();
		if (rChild) numLeaves += rChild->getNumLeaves();
		if (!lChild && !rChild) {
			assert(leafTri);
			numLeaves++;
		}
		return numLeaves;
	}


	static bool cmpX(typename TriMesh<FloatType>::Triangle<FloatType> *t0, typename TriMesh<FloatType>::Triangle<FloatType> *t1) {
		return t0->getCenter().x < t1->getCenter().x;
	}

	static bool cmpY(typename TriMesh<FloatType>::Triangle<FloatType> *t0, typename TriMesh<FloatType>::Triangle<FloatType> *t1) {
		return t0->getCenter().y < t1->getCenter().y;
	}

	static bool cmpZ(typename TriMesh<FloatType>::Triangle<FloatType> *t0, typename TriMesh<FloatType>::Triangle<FloatType> *t1) {
		return t0->getCenter().z < t1->getCenter().z;
	}
};

template <class FloatType>
class TriMeshAcceleratorBVH : public TriMeshRayAccelerator<FloatType>
{
public:

	TriMeshAcceleratorBVH(void) {
		m_Root = NULL;
	}
	TriMeshAcceleratorBVH( const TriMesh<FloatType>& triMesh, bool storeLocalCopy = false, bool useParallelBuild = true) {
		m_Root = NULL;
		build(triMesh, storeLocalCopy, useParallelBuild);
	}

	~TriMeshAcceleratorBVH(void) {
		destroy();
	}

	void build( const TriMesh<FloatType>& triMesh, bool storeLocalCopy = false, bool useParallelBuild = true)
	{
		destroy();	//in case there is already a mesh before...
		
		if (storeLocalCopy) {
			m_VerticesCopy = triMesh.getVertices();
			createTrianglePointers(m_VerticesCopy, triMesh.getIndices());
		} else {
			createTrianglePointers(triMesh.getVertices(), triMesh.getIndices());
		}

		if (useParallelBuild) {
			buildParallel(m_TrianglePointers);
		} else {
			buildRecursive(m_TrianglePointers);
		}


	}

	void destroy() {
		SAFE_DELETE(m_Root);
		m_Triangles.clear();
		m_TrianglePointers.clear();
		m_VerticesCopy.clear();
	}

	typename const TriMesh<FloatType>::Triangle<FloatType>* intersect(const Ray<FloatType> &r, FloatType& t, FloatType& u, FloatType& v, FloatType tmin = (FloatType)0, FloatType tmax = std::numeric_limits<FloatType>::max(), bool onlyFrontFaces = false) const {
		u = v = std::numeric_limits<FloatType>::max();	
		t = tmax;	//TODO MATTHIAS: probably we don't have to track tmax since t must always be smaller than the prev
		return m_Root->intersect(r, t, u, v, tmin, tmax, onlyFrontFaces);
	}

	void printInfo() const {
		std::cout << "Info: TriangleBVHAccelerator build done ( " << m_TrianglePointers.size() << " tris )" << std::endl;
		std::cout << "Info: Tree depth " << m_Root->getTreeDepthRec() << std::endl;
		std::cout << "Info: NumNodes " << m_Root->getNumNodesRec() << std::endl;
		std::cout << "Info: NumLeaves " << m_Root->getNumLeaves() << std::endl;
	}
private:

	void createTrianglePointers(const std::vector<typename TriMesh<FloatType>::Vertex<FloatType>>& vertices, const std::vector<vec3ui>& indices) {
		m_Triangles.clear();
		m_Triangles.reserve(indices.size());
		for (size_t i = 0; i < indices.size(); i++) {
			m_Triangles.push_back(typename TriMesh<FloatType>::Triangle<FloatType>(&vertices[indices[i].x], &vertices[indices[i].y], &vertices[indices[i].z]));
		}

		m_TrianglePointers.resize(m_Triangles.size());
		for (size_t i = 0; i < m_Triangles.size(); i++) {
			m_TrianglePointers[i] = &m_Triangles[i];
		}
	}

	void buildParallel(std::vector<typename TriMesh<FloatType>::Triangle<FloatType>*>& tris) {
		struct NodeEntry {
			size_t begin;
			size_t end;
			TriangleBVHNode<FloatType> *node;
		};


		std::vector<NodeEntry> currLevel(1);
		m_Root = new TriangleBVHNode<FloatType>;
		currLevel[0].node = m_Root;
		currLevel[0].begin = 0;
		currLevel[0].end = tris.size();
		
		
		unsigned int lastSortAxis = 0;
		bool needFurtherSplitting = true;
		while(needFurtherSplitting) {
			needFurtherSplitting = false;

			std::vector<NodeEntry> nextLevel(currLevel.size()*2);
#pragma omp parallel for	
			for (int i = 0; i < (int)std::min(currLevel.size(),tris.size()); i++) {
				const size_t begin = currLevel[i].begin;
				const size_t end = currLevel[i].end;

				if (end - begin > 1) {
					if (lastSortAxis == 0)		std::stable_sort(tris.begin()+begin, tris.begin()+end, TriangleBVHNode<FloatType>::cmpX);
					else if (lastSortAxis == 1)	std::stable_sort(tris.begin()+begin, tris.begin()+end, TriangleBVHNode<FloatType>::cmpY);
					else						std::stable_sort(tris.begin()+begin, tris.begin()+end, TriangleBVHNode<FloatType>::cmpZ);

					TriangleBVHNode<FloatType>* node = currLevel[i].node;
					TriangleBVHNode<FloatType>* lChild = new TriangleBVHNode<FloatType>;
					TriangleBVHNode<FloatType>* rChild = new TriangleBVHNode<FloatType>;
					node->lChild = lChild;
					node->rChild = rChild;

					nextLevel[2*i+0].begin = begin;
					nextLevel[2*i+0].end = begin + ((end-begin)/2);
					nextLevel[2*i+1].begin = begin + ((end-begin)/2);
					nextLevel[2*i+1].end = end;

					nextLevel[2*i+0].node = currLevel[i].node->lChild;
					nextLevel[2*i+1].node = currLevel[i].node->rChild;
					
					if (nextLevel[2*i+0].end - nextLevel[2*i+0].begin < 2) lChild->leafTri = tris[nextLevel[2*i+0].begin];
					else needFurtherSplitting = true;
					if (nextLevel[2*i+1].end - nextLevel[2*i+1].begin < 2) rChild->leafTri = tris[nextLevel[2*i+1].begin];
					else needFurtherSplitting = true;
				} 
			}

			if (needFurtherSplitting) {
				currLevel = nextLevel;
				lastSortAxis = (lastSortAxis+1)%3;
			}
		}

		m_Root->computeBoundingBox();
	}

	void buildRecursive(std::vector<typename TriMesh<FloatType>::Triangle<FloatType>*>& tris) {
		assert(tris.size() > 2);
		m_Root = new TriangleBVHNode<FloatType>;
		m_Root->split(tris.begin(), tris.end(), 0);
		m_Root->computeBoundingBox();
	}


	//! private data
	TriangleBVHNode<FloatType>* m_Root;

	std::vector<typename TriMesh<FloatType>::Vertex<FloatType>>		m_VerticesCopy;
	std::vector<typename TriMesh<FloatType>::Triangle<FloatType>>	m_Triangles;
	std::vector<typename TriMesh<FloatType>::Triangle<FloatType>*>	m_TrianglePointers;
};

typedef TriMeshAcceleratorBVH<float>		TriMeshAcceleratorBVHf;
typedef TriMeshAcceleratorBVH<double>	TriMeshAcceleratorBVHd;

} // namespace ml

#endif
