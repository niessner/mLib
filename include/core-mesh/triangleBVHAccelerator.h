#pragma once

#ifndef _TRIANGLEBVHACCELERATOR_H_
#define _TRIANGLEBVHACCELERATOR_H_

namespace ml {

template <class FloatType>
struct TriangleBVHNode {
	TriangleBVHNode() : parent(0), rChild(0), lChild(0), lastSort(0) {}
	~TriangleBVHNode() {
		SAFE_DELETE(rChild);
		SAFE_DELETE(lChild);
	}

	//wait for vs 2013
	//template<class T>
	//using Triangle = TriMesh::Triangle<T>;

	BoundingBox3d<FloatType> boundingBox;

	typename std::vector<Triangle<FloatType>*>::iterator begin;
	typename std::vector<Triangle<FloatType>*>::iterator end;

	unsigned int lastSort;

	TriangleBVHNode<FloatType> *parent;
	TriangleBVHNode<FloatType> *lChild;
	TriangleBVHNode<FloatType> *rChild;

	void computeBoundingBox() {
		boundingBox.reset();
		
		if (!lChild && !rChild) {
			assert(end - begin == 1);
			for (std::vector<Triangle<FloatType>*>::iterator iter = begin; iter != end; iter++) {
				(*iter)->includeInBoundingBox(boundingBox);
			} 
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


	void split() {
		if (end - begin > 1) {
			if (lastSort == 0)		std::stable_sort(begin, end, cmpX);
			else if (lastSort == 1)	std::stable_sort(begin, end, cmpY);
			else					std::stable_sort(begin, end, cmpZ);

			lChild = new TriangleBVHNode;
			rChild = new TriangleBVHNode;

			lChild->lastSort = rChild->lastSort = (lastSort+1)%3;
			lChild->parent = rChild->parent = this;

			lChild->begin = begin;
			lChild->end = begin + ((end-begin)/2);
			rChild->begin = lChild->end;
			rChild->end = end;

			lChild->split();
			rChild->split();
		}
	}

	bool intersect(const Ray<FloatType> &r, FloatType& t, FloatType& u, FloatType& v, Triangle<FloatType>* &triangle, FloatType tmin = (FloatType)0, FloatType tmax = std::numeric_limits<FloatType>::max(), bool intersectOnlyFrontFaces = false) const {
		if (t < tmin || t > tmax)	return false;	//early out (warning t must be initialized)
		if (boundingBox.intersect(r, tmin, tmax)) {
			bool b = false;
			if (!lChild && !rChild) {
				assert(end - begin == 1);
				FloatType currTMax = tmax;
				for (typename std::vector<Triangle<FloatType>*>::iterator iter = begin; iter != end; iter++) {
					if ((*iter)->intersect(r, currTMax, u, v, tmin, currTMax, intersectOnlyFrontFaces))	{
						triangle = *iter;
						b = true; ///intersect !!!!
						t = currTMax;
					}
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
		if (!lChild && !rChild) numLeaves++;
		return numLeaves;
	}


	static bool cmpX(Triangle<FloatType> *t0, Triangle<FloatType> *t1) {
		return t0->getCenter().x < t1->getCenter().x;
	}

	static bool cmpY(Triangle<FloatType> *t0, Triangle<FloatType> *t1) {
		return t0->getCenter().y < t1->getCenter().y;
	}

	static bool cmpZ(Triangle<FloatType> *t0, Triangle<FloatType> *t1) {
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
	TriangleBVHAccelerator(std::vector<Triangle<FloatType>*>& tris) {
		m_Root = NULL;
		build(tris);
	}

	~TriangleBVHAccelerator(void) {
		destroy();
	}

	void build( std::vector<Triangle<FloatType>*>& tris )
	{
		SAFE_DELETE(m_Root);	//in case there is already a mesh before...

		//std::vector<Triangle<FloatType>*>& tris = mesh->GetTris();
		assert(tris.size() > 2);
		m_Root = new TriangleBVHNode<FloatType>;
		m_Root->begin = tris.begin();
		m_Root->end = tris.end();
		m_Root->split();
		m_Root->computeBoundingBox();

		std::cout << "Info: TriangleBVHAccelerator build done ( " << tris.size() << " tris )" << std::endl;
		//std::cout << "Info: Tree depth " << m_Root->getTreeDepthRec() << std::endl;
		//std::cout << "Info: NumNodes " << m_Root->getNumNodesRec() << std::endl;
		//std::cout << "Info: NumLeaves " << m_Root->getNumLeaves() << std::endl;
		//TODO parallel build
	}
	void destroy() {
			SAFE_DELETE(m_Root);
	}
	bool intersect(const Ray<FloatType> &r, FloatType& t, FloatType& u, FloatType& v, Triangle<FloatType>* &triangle, FloatType tmin = (FloatType)0, FloatType tmax = std::numeric_limits<FloatType>::max(), bool intersectOnlyFrontFaces = false) const {
		t = u = v = std::numeric_limits<FloatType>::max();
		triangle = NULL;
		return m_Root->intersect(r, t, u, v, triangle, tmin, tmax, intersectOnlyFrontFaces);
	}
private:
	TriangleBVHNode<FloatType>* m_Root;
};

typedef TriangleBVHAccelerator<float> TriangleBVHAcceleratorf;
typedef TriangleBVHAccelerator<double> TriangleBVHAcceleratord;

} // namespace ml

#endif
