
#ifndef _COREMESH_TRIMESHRAYACCELERATOR_H_
#define _COREMESH_TRIMESHRAYACCELERATOR_H_

namespace ml {

//////////////////////////////////////////////////////////////////////////
// Interface for TriMesh-Ray Acceleration Structures
//////////////////////////////////////////////////////////////////////////

template<class FloatType>
class TriMeshRayAccelerator
{
public:
    struct Intersection
    {
		Intersection() {}
		Intersection(Intersection&& other) {
			*this = std::move(other);
		}
		void operator==(Intersection&& other) {
			*this = std::move(other);
		}

		bool isValid() const {
			return triangle != NULL;
		}
		
		typename TriMesh<FloatType>::Vertex<FloatType> getSurfaceVertex() const {
			return triangle->getSurfaceVertex(u,v);
		}
		point3d<FloatType> getSurfacePosition() const {
			return triangle->getSurfacePosition(u,v);
		}
		point4d<FloatType> getSurfaceColor() const {
			return triangle->getSurfaceColor(u,v);
		}
		point3d<FloatType> getSurfaceNormal() const {
			return triangle->getSurfaceNormal(u,v);
		}
		point2d<FloatType> getSurfaceTexCoord() const {
			return triangle->getSurfaceTexCoord(u,v); 
		}

		unsigned int getTriangleIndex() const {
			return triangle->getIndex();
		}
		unsigned int getMeshIndex() const {
			return triangle->getMeshIndex();
		}

		FloatType t, u, v;	
		const typename TriMesh<FloatType>::Triangle<FloatType>* triangle;
    };

	//////////////////////////////////////////////////////////////////////////
	// Interface Definition
	//////////////////////////////////////////////////////////////////////////

	virtual typename const TriMesh<FloatType>::Triangle<FloatType>* intersect(const Ray<FloatType>& r, FloatType& t, FloatType& u, FloatType& v, FloatType tmin = (FloatType)0, FloatType tmax = std::numeric_limits<FloatType>::max(), bool onlyFrontFaces = false) const = 0;


	//////////////////////////////////////////////////////////////////////////
	// API helpers
	//////////////////////////////////////////////////////////////////////////

	void build(const TriMesh<FloatType> &mesh, bool storeLocalCopy = false) {
		std::vector<const TriMesh<FloatType>* > meshes;
		meshes.push_back(&mesh);
		build(meshes, storeLocalCopy);
	}

	void build(const std::vector<const TriMesh<FloatType>* >& triMeshes, bool storeLocalCopy = false) {
		destroy();
		std::vector<const std::vector<typename TriMesh<FloatType>::Vertex<FloatType>>*> vertices(triMeshes.size());
		std::vector<const std::vector<vec3ui>*> indices(triMeshes.size());

		if (storeLocalCopy) {
			m_VerticesCopy.resize(triMeshes.size());
			for (size_t i = 0; i < triMeshes.size(); i++) {
				m_VerticesCopy[i] = triMeshes[i]->getVertices();
				vertices[i] = &m_VerticesCopy[i];
				indices[i] = &triMeshes[i]->getIndices();
			}
		} else {
			for (size_t i = 0; i < triMeshes.size(); i++) {
				vertices[i] = &triMeshes[i]->getVertices();
				indices[i] = &triMeshes[i]->getIndices();
			}
		}
		createTrianglePointers(vertices, indices);

		buildInternal();	//construct the acceleration structure
	}

	void build(const TriMesh<FloatType>& mesh, const Matrix4x4<FloatType>& transform) {
		std::vector< std::pair<const TriMesh<FloatType>*, Matrix4x4<FloatType>> > meshes;
		meshes.push_back(std::make_pair(&mesh, transform));
		build(meshes);
	}

	//! constructs the acceleration structure; always generates an internal copy
	void build(const std::vector<std::pair<const TriMesh<FloatType>*, Matrix4x4<FloatType>>>& triMeshPairs) {
		destroy();

		std::vector<const std::vector<typename TriMesh<FloatType>::Vertex<FloatType>>*> vertices(triMeshPairs.size());
		std::vector<const std::vector<vec3ui>*> indices(triMeshPairs.size());

		m_VerticesCopy.resize(triMeshPairs.size());
		for (size_t i = 0; i < triMeshPairs.size(); i++) {
			m_VerticesCopy[i] = triMeshPairs[i].first->getVertices();
			//apply the transform locally
			for (auto& v : m_VerticesCopy[i]) {
				v.position = triMeshPairs[i].second * v.position;
			}
			vertices[i] = &m_VerticesCopy[i];
			indices[i] = &triMeshPairs[i].first->getIndices();
		}
		createTrianglePointers(vertices, indices);

		buildInternal();	//construct the acceleration structure
	}


	bool intersect(const Ray<FloatType>& r, Intersection& i, FloatType tmin = (FloatType)0, FloatType tmax = std::numeric_limits<FloatType>::max(), bool onlyFrontFaces = false) const {
		i.triangle = intersect(r, i.t, i.u, i.v, tmin, tmax, onlyFrontFaces);
		return i.isValid();
	}

	Intersection intersect(const Ray<FloatType> &r, FloatType tmin = (FloatType)0, FloatType tmax = std::numeric_limits<FloatType>::max(), bool onlyFrontFaces = false) const {
		Intersection i;
		i.triangle = intersect(r, i.t, i.u, i.v, tmin, tmax, onlyFrontFaces);
		return i;
	}


/*
	template<class Accelerator>
	static bool getFirstIntersection(
		const Ray<FloatType>& ray,  
		const std::vector< Accelerator >& objectAccelerators,
		Intersection& intersect,
		UINT &objectIndex);

	template<class Accelerator>
	static bool getFirstIntersectionDirect(const Ray<FloatType>& ray,
		const std::vector< Accelerator > &objectAccelerators,
		Intersection &intersect,
		UINT &objectIndex);
*/
	
protected:

	std::vector<std::vector<typename TriMesh<FloatType>::Vertex<FloatType>> >			m_VerticesCopy;
	std::vector<typename TriMesh<FloatType>::Triangle<FloatType>>						m_Triangles;
	std::vector<typename TriMesh<FloatType>::Triangle<FloatType>*>						m_TrianglePointers;
	
private:

	//! takes a vector of meshes: including vertices and indices
	void createTrianglePointers(
		const std::vector<const std::vector<typename TriMesh<FloatType>::Vertex<FloatType>>*>& verticesVec,
		const std::vector<const std::vector<vec3ui>*>& indicesVec) 
	{
		//reserver memory
		m_Triangles.clear();
		size_t numTris = 0;
		for (size_t i = 0; i < indicesVec.size(); i++) {
			numTris += indicesVec[i]->size();
		}
		m_Triangles.reserve(numTris);

		//loop over meshes
		for (size_t m = 0; m < indicesVec.size(); m++) {
			const auto& indices = *indicesVec[m];
			const auto& vertices = *verticesVec[m];
			//loop over tris within a mesh
			for (size_t i = 0; i < indices.size(); i++) {
				//generate triangle with triangle and mesh index
				m_Triangles.push_back(typename TriMesh<FloatType>::Triangle<FloatType>(&vertices[indices[i].x], &vertices[indices[i].y], &vertices[indices[i].z], (unsigned int)i, (unsigned int)m));
			}
		}

		//create triangle pointers
		m_TrianglePointers.resize(m_Triangles.size());
		for (size_t i = 0; i < m_Triangles.size(); i++) {
			m_TrianglePointers[i] = &m_Triangles[i];
		}
	}

	void destroy() {
		m_Triangles.clear();
		m_TrianglePointers.clear();
		m_VerticesCopy.clear();
	}

	//! given protected data above filed, the data structure is constructed
	virtual void buildInternal() = 0;

	////! local copy may be created if requested
	//virtual void buildInternal(const std::vector< const TriMesh<FloatType>* >& meshes, bool storeLocalCopy) = 0;

	////! in this case always a local copy is stored
	//virtual void buildInternal(const std::vector< std::pair<const TriMesh<FloatType> *, Matrix4x4<FloatType>> > &meshPair) = 0;

};

} //ml

/*

template<class FloatType>
class TriMeshRayAcceleratorBruteForce : public TriMeshRayAccelerator<FloatType>
{
public:

    struct Triangle
    {
        point3d<FloatType> pos[3];
        UINT meshIndex;

        point3d<FloatType> getPos(const point2d<FloatType> &uv) const
        {
            return (pos[0] + (pos[1] - pos[0]) * uv.x + (pos[2] - pos[0]) * uv.y);
        }
        point3d<FloatType> normal() const
        {
            return math::triangleNormal(pos[0], pos[1], pos[2]);
        }

        //
        // TODO: this belongs in a utility class, certainly not here nor in TriMesh<T>::Triangle
        //
        bool intersect(const Ray<FloatType> &r, FloatType& _t, FloatType& _u, FloatType& _v, FloatType tmin = (FloatType)0, FloatType tmax = std::numeric_limits<FloatType>::max()) const;
    };

    void initInternal(const std::vector< std::pair<const TriMesh<FloatType> *, mat4f> > &meshes, bool storeLocalCopy);
    bool intersect(const Ray<FloatType> &ray, TriMeshRayAccelerator<FloatType>::Intersection &result) const;

private:
    //
    // exactly one of m_tris or m_meshes contains data, depending on storeLocalCopy
    //
    std::vector< Triangle > m_tris;
    std::vector< std::pair<const TriMesh<FloatType> *, mat4f> > m_meshes;
    BoundingBox3d<FloatType> m_bbox;
};

typedef TriMeshRayAccelerator<float> TriMeshRayAcceleratorf;
typedef TriMeshRayAccelerator<double> TriMeshRayAcceleratord;

} // ml

#include "TriMeshRayAccelerator.inl"
*/

#endif
