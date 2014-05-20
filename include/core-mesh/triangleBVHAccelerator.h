#pragma once

#ifndef _TRIANGLEBVHACCELERATOR_H_
#define _TRIANGLEBVHACCELERATOR_H_

namespace ml {

template <class FloatType>
struct TriangleBVHNode {
	TriangleBVHNode() : parent(0), rChild(0), lChild(0), leafTri(0) {}
	~TriangleBVHNode() {
		SAFE_DELETE(rChild);
		SAFE_DELETE(lChild);
	}

	//wait for vs 2013
	//template<class T>
	//using Triangle = TriMesh::Triangle<T>;

	BoundingBox3d<FloatType> boundingBox;
	typename TriMesh<FloatType>::Triangle<FloatType>* leafTri;


	TriangleBVHNode<FloatType> *parent;
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
			lChild->parent = rChild->parent = this;

			const unsigned int newSortAxis = (lastSortAxis+1)%3;
			lChild->split(begin, begin + ((end-begin)/2), newSortAxis);
			rChild->split(begin + ((end-begin)/2), end, newSortAxis);

		} else {
			assert(end - begin == 1);
			leafTri = *begin;	//found a leaf
		}
	}

	bool intersect(const Ray<FloatType> &r, FloatType& t, FloatType& u, FloatType& v, typename TriMesh<FloatType>::Triangle<FloatType>* &triangle, FloatType& tmin, FloatType& tmax, bool intersectOnlyFrontFaces = false) const {
		if (t < tmin || t > tmax)	return false;	//early out (warning t must be initialized)
		if (boundingBox.intersect(r, tmin, tmax)) {
			bool b = false;
			if (!lChild && !rChild) {
				if (leafTri->intersect(r, t, u, v, tmin, tmax, intersectOnlyFrontFaces))	{
					triangle = leafTri;
					b = true; ///intersect !!!!
					tmax = t;
				}
			} else {
				if (lChild->intersect(r, t, u, v, triangle, tmin, tmax, intersectOnlyFrontFaces))	b = true;
				if (rChild->intersect(r, t, u, v, triangle, tmin, tmax, intersectOnlyFrontFaces))	b = true;
			}
			return b;
		} 
		return false;
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
class TriangleBVHAccelerator
{
public:

	TriangleBVHAccelerator(void) {
		m_Root = NULL;
	}
	TriangleBVHAccelerator( const TriMesh<FloatType>& triMesh, bool useParallelBuild = true) {
		m_Root = NULL;
		build(triMesh, useParallelBuild);
	}

	~TriangleBVHAccelerator(void) {
		destroy();
	}

	void build( const TriMesh<FloatType>& triMesh, bool useParallelBuild = true )
	{
		SAFE_DELETE(m_Root);	//in case there is already a mesh before...
		
		createTrianglePointers(triMesh.getVertices(), triMesh.getIndices());

		if (useParallelBuild) {
			buildParallel(m_TrianglePointers);
		} else {
			buildRecursive(m_TrianglePointers);
		}

		std::cout << "Info: TriangleBVHAccelerator build done ( " << m_TrianglePointers.size() << " tris )" << std::endl;
		std::cout << "Info: Tree depth " << m_Root->getTreeDepthRec() << std::endl;
		std::cout << "Info: NumNodes " << m_Root->getNumNodesRec() << std::endl;
		std::cout << "Info: NumLeaves " << m_Root->getNumLeaves() << std::endl;
	}

	void destroy() {
		SAFE_DELETE(m_Root);
		for (size_t i = 0; i < m_TrianglePointers.size(); i++) {
			SAFE_DELETE(m_TrianglePointers[i]);
		}
	}
	bool intersect(const Ray<FloatType> &r, FloatType& t, FloatType& u, FloatType& v, typename TriMesh<FloatType>::Triangle<FloatType>* &triangle, FloatType tmin = (FloatType)0, FloatType tmax = std::numeric_limits<FloatType>::max(), bool intersectOnlyFrontFaces = false) const {
		u = v = std::numeric_limits<FloatType>::max();	
		t = tmax;	//TODO MATTHIAS: probably we don't have to track tmax since t must always be smaller than the prev
		triangle = NULL;
		return m_Root->intersect(r, t, u, v, triangle, tmin, tmax, intersectOnlyFrontFaces);
	}
private:

	void createTrianglePointers(const std::vector<typename TriMesh<FloatType>::Vertex<FloatType>>& vertices, const std::vector<vec3ui>& indices) {
		for (size_t i = 0; i < m_TrianglePointers.size(); i++) {
			SAFE_DELETE(m_TrianglePointers[i]);
		}
		m_TrianglePointers.resize(indices.size());
		for (size_t i = 0; i < indices.size(); i++) {
			typename TriMesh<FloatType>::Triangle<FloatType>* tri = new typename TriMesh<FloatType>::Triangle<FloatType>(&vertices[indices[i].x], &vertices[indices[i].y], &vertices[indices[i].z]);
			m_TrianglePointers[i] = tri;
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

					lChild->parent = rChild->parent = node;

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

	std::vector<typename TriMesh<FloatType>::Triangle<FloatType>*> m_TrianglePointers;
};

typedef TriangleBVHAccelerator<float> TriangleBVHAcceleratorf;
typedef TriangleBVHAccelerator<double> TriangleBVHAcceleratord;

} // namespace ml

#endif
