#ifndef _COREMESH_TRIMESHRAYACCELERATOR_H_
#define _COREMESH_TRIMESHRAYACCELERATOR_H_

namespace ml {

template<class T>
class TriMeshRayAccelerator
{
public:
    struct Intersection
    {
        point3d<T> pos;
        
        // dist of intersect from ray origin
        T dist;

        int meshIndex;
        int triangleIndex;
        point2d<T> uv;

        // normal of the contact triangle
        point3d<T> normal;
    };

    template<class Accelerator>
    static bool getFirstIntersection(const ml::Rayf &ray,
        const std::vector< Accelerator > &objectAccelerators,
        Intersection &intersect,
        UINT &objectIndex);

    template<class Accelerator>
    static bool getFirstIntersectionDirect(const ml::Rayf &ray,
        const std::vector< Accelerator > &objectAccelerators,
        Intersection &intersect,
        UINT &objectIndex);

    void init(const TriMesh<T> &mesh, bool storeLocalCopy)
    {
        init(mesh, ml::mat4f::identity(), storeLocalCopy);
    }
    void init(const TriMesh<T> &mesh, const mat4f &transform, bool storeLocalCopy)
    {
        std::vector< std::pair<const TriMesh<T> *, mat4f> > meshes;
        meshes.push_back(std::make_pair(&mesh, transform));
        init(meshes, storeLocalCopy);
    }
    void init(const std::vector< std::pair<const TriMesh<T> *, mat4f> > &meshes, bool storeLocalCopy)
    {
        initInternal(meshes, storeLocalCopy);
    }
    virtual bool intersect(const Ray<T> &ray, Intersection &result) const = 0;

private:
    virtual void initInternal(const std::vector< std::pair<const TriMesh<T> *, mat4f> > &meshes, bool storeLocalCopy) = 0;
    
};

template<class T>
class TriMeshRayAcceleratorBruteForce : public TriMeshRayAccelerator<T>
{
public:
    struct Triangle
    {
        point3d<T> pos[3];
        UINT meshIndex;

        point3d<T> getPos(const point2d<T> &uv) const
        {
            return (pos[0] + (pos[1] - pos[0]) * uv.x + (pos[2] - pos[0]) * uv.y);
        }
        point3d<T> normal() const
        {
            return math::triangleNormal(pos[0], pos[1], pos[2]);
        }

        //
        // TODO: this belongs in a utility class, certainly not here nor in TriMesh<T>::Triangle
        //
        bool intersect(const Ray<T> &r, T& _t, T& _u, T& _v, T tmin = (T)0, T tmax = std::numeric_limits<T>::max()) const;
    };

    void initInternal(const std::vector< std::pair<const TriMesh<T> *, mat4f> > &meshes, bool storeLocalCopy);
    bool intersect(const Ray<T> &ray, TriMeshRayAccelerator<T>::Intersection &result) const;

private:
    //
    // exactly one of m_tris or m_meshes contains data, depending on storeLocalCopy
    //
    std::vector< Triangle > m_tris;
    std::vector< std::pair<const TriMesh<T> *, mat4f> > m_meshes;
    BoundingBox3d<T> m_bbox;
};

typedef TriMeshRayAccelerator<float> TriMeshRayAcceleratorf;
typedef TriMeshRayAccelerator<double> TriMeshRayAcceleratord;

} // ml

#include "TriMeshRayAccelerator.inl"

#endif