#ifndef INCLUDE_CORE_MESH_TRIMESH_H_
#define INCLUDE_CORE_MESH_TRIMESH_H_

struct MeshVertex
{
    MeshVertex() : position(vec3f::origin), normal(vec3f::origin), attributeA(vec4f::origin), attributeB(vec4f::origin) { }
    explicit MeshVertex(const vec3f& _position) : position(_position) {}
    MeshVertex(const vec3f& _p, const vec3f& _n, const vec4f& _a, const vec4f& _b) : position(_p), normal(_n), attributeA(_a), attributeB(_b) { }
    vec3f position;
    vec3f normal;
    vec4f attributeA;
    vec4f attributeB;
};

class TriMesh
{
    public:
        TriMesh() : m_vertices(), m_indices() { }
        TriMesh(const Vector<MeshVertex>& _vertices, const Vector<UINT>& _indices)
        {
            m_vertices = _vertices;
            m_indices = _indices;
        }
        TriMesh(TriMesh&& t)
        {
            m_vertices = std::move(t.m_vertices);
            m_indices = std::move(t.m_indices);
        }
        void operator=(TriMesh && t)
        {
            m_vertices = std::move(t.m_vertices);
            m_indices = std::move(t.m_indices);
        }
        void applyTransform(const mat4f& m)
        {
            for (MeshVertex& v : m_vertices) { v.position = m * v.position; }
        }

        void stretch(float s) { stretch(vec3f(s, s, s)); }

        void stretch(const vec3f& v)
        {
            for (MeshVertex& mv : m_vertices) for (UINT i = 0; i < 3; i++) { mv.position[i] *= v[i]; }
        }

        const Vector<MeshVertex>& vertices() const { return m_vertices; }

        const Vector<UINT>& indices() const { return m_indices; }

    private:
        Vector<MeshVertex> m_vertices;
        Vector<UINT> m_indices;
};

#endif  // INCLUDE_CORE_MESH_TRIMESH_H_
