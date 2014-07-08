#ifndef _COREMESH_TRIMESHCOLLISIONACCELERATOR_H_
#define _COREMESH_TRIMESHCOLLISIONACCELERATOR_H_

namespace ml {

template<class T>
class TriMeshCollisionAccelerator
{
public:
    
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

    //
    // this is not actually a virtual function, since we only expect each accelerator to handle collisions
    // with its own accelerator type.
    //
    //virtual bool collision(const TriMeshCollisionAccelerator<T> &accel) const = 0;

private:
    virtual void initInternal(const std::vector< std::pair<const TriMesh<T> *, mat4f> > &meshes, bool storeLocalCopy) = 0;
    
};

template<class T>
class TriMeshCollisionAcceleratorBruteForce : public TriMeshCollisionAccelerator<T>
{
public:
    struct Triangle
    {
        point3d<T> pos[3];
    };

    void initInternal(const std::vector< std::pair<const TriMesh<T> *, mat4f> > &meshes, bool storeLocalCopy);
    bool collision(const TriMeshCollisionAcceleratorBruteForce<T> &accel) const;

private:
    //
    // exactly one of m_tris or m_meshes contains data, depending on storeLocalCopy
    //
    std::vector< Triangle > m_tris;
    std::vector< std::pair<const TriMesh<T> *, mat4f> > m_meshes;
};

} // ml

#include "TriMeshCollisionAccelerator.inl"

#endif