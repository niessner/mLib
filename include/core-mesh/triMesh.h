#ifndef CORE_MESH_TRIMESH_H_
#define CORE_MESH_TRIMESH_H_

namespace ml {

class TriMesh
{
    public:
		struct TriMeshVertex
		{
			TriMeshVertex() : position(Vec3f::origin), normal(Vec3f::origin), attributeA(Vec4f::origin), attributeB(Vec4f::origin) { }
			explicit TriMeshVertex(const Vec3f& _position) : position(_position) {}
			TriMeshVertex(const Vec3f& _p, const Vec3f& _n, const Vec4f& _a, const Vec4f& _b) : position(_p), normal(_n), attributeA(_a), attributeB(_b) { }
			Vec3f position;
			Vec3f normal;
			Vec4f attributeA;
			Vec4f attributeB;
		};

        TriMesh() : m_vertices(), m_indices() { }
		TriMesh(const MeshDataf& meshData) {
			m_vertices.resize(meshData.m_Vertices.size());
			//bool hasNormals = meshData.m_Normals.size() > 0;
			bool hasColors = meshData.m_Colors.size() > 0;
			for (unsigned int i = 0; i < m_vertices.size(); i++) {
				m_vertices[i].position = meshData.m_Vertices[i];
				if (hasColors)	m_vertices[i].attributeA = meshData.m_Colors[i];
			}

			for (size_t i = 0; i < meshData.m_FaceIndicesVertices.size(); i++) {
				if (meshData.m_FaceIndicesVertices[i].size() == 3) {
					for (size_t k = 0; k < meshData.m_FaceIndicesVertices[i].size(); k++) 
					m_indices.push_back(meshData.m_FaceIndicesVertices[i][k]);
				} else {
					MLIB_WARNING("non triangle face found - ignoring it");
				}
			}
		}
        TriMesh(const std::vector<TriMeshVertex>& _vertices, const std::vector<UINT>& _indices)
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
        void applyTransform(const Mat4f& m)
        {
            for (TriMeshVertex& v : m_vertices) { v.position = m * v.position; }
        }

        void stretch(float s) { stretch(Vec3f(s, s, s)); }

        void stretch(const Vec3f& v)
        {
            for (TriMeshVertex& mv : m_vertices) for (UINT i = 0; i < 3; i++) { mv.position[i] *= v[i]; }
        }

        std::vector<TriMeshVertex>& vertices() { return m_vertices; }

        std::vector<UINT>& indices() { return m_indices; }

        const std::vector<TriMeshVertex>& vertices() const { return m_vertices; }

        const std::vector<UINT>& indices() const { return m_indices; }

    private:
        std::vector<TriMeshVertex> m_vertices;
        std::vector<UINT> m_indices;
};

}  // namespace ml

#endif  // INCLUDE_CORE_MESH_TRIMESH_H_
