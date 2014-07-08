#ifndef _COREMESH_TRIMESHCOLLISIONACCELERATOR_INL_
#define _COREMESH_TRIMESHCOLLISIONACCELERATOR_INL_

namespace ml {

template<class T>
void TriMeshCollisionAcceleratorBruteForce<T>::initInternal(const std::vector< std::pair<const TriMesh<T> *, mat4f> > &meshes, bool storeLocalCopy)
{
    //
    // for simplicity, since brute-force collisions are far too slow, just always store a local copy.
    //
    storeLocalCopy = true;
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
            
            for (int i = 0; i < 3; i++)
                tri.pos[i] = p.second * p.first->getVertices()[indices[i]].position;

            m_tris.push_back(tri);
        }
        meshIndex++;
    }
}

int tri_tri_intersect(float V0[3], float V1[3], float V2[3],
                      float U0[3], float U1[3], float U2[3]);

template<class T>
bool TriMeshCollisionAcceleratorBruteForce<T>::collision(const TriMeshCollisionAcceleratorBruteForce<T> &accel) const
{
    if (m_tris.size() > 0)
    {
        for (const Triangle &triA : m_tris)
        {
            for (const Triangle &triB : accel.m_tris)
            {
                if (math::triangleIntersectTriangle(triA.pos, triB.pos))
                    return true;
            }
        }
    }
       
    return false;
}

} // ml

#endif