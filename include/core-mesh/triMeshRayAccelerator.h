
#ifndef _COREMESH_TRIMESHRAYACCELERATOR_H_
#define _COREMESH_TRIMESHRAYACCELERATOR_H_

namespace ml {

template<class FloatType>
class TriMeshRayAccelerator
{
public:
    struct Intersection
    {
        FloatType t;
        point2d<FloatType> uv;	
		//const typename TriMesh<FloatType>::Triangle<FloatType>* triangle;
    };

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

    void build(const TriMesh<FloatType> &mesh, bool storeLocalCopy = false)
    {
		typename TriMesh<FloatType>::Triangle<FloatType> t;

        build(mesh, Matrix4x4<FloatType>::identity(), storeLocalCopy);
    }
    void build(const TriMesh<FloatType>& mesh, const Matrix4x4<FloatType>& transform, bool storeLocalCopy)
    {
        std::vector< std::pair<const TriMesh<FloatType>*, Matrix4x4<FloatType>> > meshes;
        meshes.push_back(std::make_pair(&mesh, transform));
        build(meshes, storeLocalCopy);
    }
    void build(const std::vector< std::pair<const TriMesh<FloatType> *, Matrix4x4<FloatType>> > &meshes, bool storeLocalCopy)
    {
        buildInternal(meshes, storeLocalCopy);
    }
    virtual bool intersect(const Ray<FloatType> &ray, Intersection &result) const = 0;

private:
    virtual void buildInternal(const std::vector< std::pair<const TriMesh<FloatType> *, Matrix4x4<FloatType>> > &meshes, bool storeLocalCopy) = 0;
    
};

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

#endif
