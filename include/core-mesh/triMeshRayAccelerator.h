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
    };

    virtual void init(const std::vector< std::pair<const TriMesh<T> *, mat4f> > &meshes, bool storeLocalCopy) = 0;
    virtual bool intersect(const Ray<T> &ray, Intersection &result) const = 0;
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

        //
        // TODO: this belongs in a utility class, certainly not here nor in TriMesh<T>::Triangle
        //
        bool intersect(const Ray<T> &r, T& _t, T& _u, T& _v, T tmin = (T)0, T tmax = std::numeric_limits<T>::max()) const
        {
            const point3d<T> &d = r.direction();
            const point3d<T> &p = r.origin();

            point3d<T> e1 = pos[1] - pos[0];
            point3d<T> e2 = pos[2] - pos[0];

            point3d<T> h = d ^ e2;
            T a = e1 | h;

            if (a == (T)0.0 || a == -(T)0.0) return false;

            T f = (T)1.0 / a;
            point3d<T> s = p - pos[0];
            T u = f * (s | h);

            if (u < (T)0.0 || u >(T)1.0) return false;

            point3d<T> q = s ^ e1;
            T v = f * (d | q);

            if (v < (T)0.0 || u + v >(T)1.0) return false;

            // at this stage we can compute t to find out where the intersection point is on the line
            T t = f * (e2 | q);

            if (t <= tmax && t >= tmin)
            {
                _t = t;
                _u = u;
                _v = v;
                return true;
            }
            return false;
        }
    };

    void init(const std::vector< std::pair<const TriMesh<T> *, mat4f> > &meshes, bool storeLocalCopy)
    {
        if (!storeLocalCopy)
        {
            m_meshes = meshes;
            return;
        }
        
        size_t triCount = 0;
        for (const auto &p : meshes)
            triCount += p.first->getIndices().size();

        m_tris.clear();
        m_tris.reserve(triCount);

        int meshIndex = 0;
        for (const auto &p : meshes)
        {
            for (const auto &indices : p.first->getIndices())
            {
                Triangle tri;
                
                tri.meshIndex = meshIndex;

                for (int i = 0; i < 3; i++)
                    tri.pos[i] = p.second * p.first->getVertices()[indices[i]].position;

                m_tris.push_back(tri);
            }
            meshIndex++;
        }
    }

    bool intersect(const Ray<T> &ray, TriMeshRayAccelerator<T>::Intersection &result) const
    {
        result.dist = std::numeric_limits<float>::max();
        result.meshIndex = -1;
        result.triangleIndex = -1;

        if (m_tris.size() > 0)
        {
            int triangleIndex = 0;
            for (const Triangle &tri : m_tris)
            {
                T dist, u, v;
                if (tri.intersect(ray, dist, u, v) && dist < result.dist)
                {
                    result.dist = dist;
                    result.meshIndex = tri.meshIndex;
                    result.triangleIndex = triangleIndex;
                    result.uv.x = u;
                    result.uv.y = v;
                    result.pos = tri.getPos(result.uv);
                }
                triangleIndex++;
            }
        }
        else if (m_meshes.size() > 0)
        {
            int meshIndex = 0, triangleIndex = 0;
            for (const auto &p : m_meshes)
            {
                for (const auto &indices : p.first->getIndices())
                {
                    Triangle tri;

                    tri.meshIndex = meshIndex;
                    for (int i = 0; i < 3; i++)
                        tri.pos[i] = p.second * p.first->getVertices()[indices[i]].position;

                    T dist, u, v;
                    if (tri.intersect(ray, dist, u, v) && dist < result.dist)
                    {
                        result.dist = dist;
                        result.meshIndex = tri.meshIndex;
                        result.triangleIndex = triangleIndex;
                        result.uv.x = u;
                        result.uv.y = v;
                        result.pos = tri.getPos(result.uv);
                    }

                    triangleIndex++;
                }
                meshIndex++;
            }
        }
        
        return (result.meshIndex != -1);
    }

private:
    //
    // exactly one of m_tris or m_meshes contains data, depending on storeLocalCopy
    //
    std::vector< Triangle > m_tris;
    std::vector< std::pair<const TriMesh<T> *, mat4f> > m_meshes;
};

typedef TriMeshRayAccelerator<float> TriMeshRayAcceleratorf;
typedef TriMeshRayAccelerator<double> TriMeshRayAcceleratord;

} // ml

#endif