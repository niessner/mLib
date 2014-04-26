#ifndef CORE_MESH_TRIMESHOLD_H_
#define CORE_MESH_TRIMESHOLD_H_

namespace ml {

class TriMeshOld
{
    public:
		struct TriMeshOldVertex
		{
			TriMeshOldVertex() : position(vec3f::origin), normal(vec3f::origin), attributeA(vec4f::origin), attributeB(vec4f::origin) { }
			explicit TriMeshOldVertex(const vec3f& _position) : position(_position) {}
			TriMeshOldVertex(const vec3f& _p, const vec3f& _n, const vec4f& _a, const vec4f& _b) : position(_p), normal(_n), attributeA(_a), attributeB(_b) { }
			vec3f position;
			vec3f normal;
			vec4f attributeA;
			vec4f attributeB;
		};

        TriMeshOld() : m_vertices(), m_indices() { }
		TriMeshOld(const MeshDataf& meshData) {
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
        TriMeshOld(const std::vector<TriMeshOldVertex>& _vertices, const std::vector<UINT>& _indices)
        {
            m_vertices = _vertices;
            m_indices = _indices;
        }
        TriMeshOld(TriMeshOld&& t)
        {
            m_vertices = std::move(t.m_vertices);
            m_indices = std::move(t.m_indices);
        }
        void operator=(TriMeshOld && t)
        {
            m_vertices = std::move(t.m_vertices);
            m_indices = std::move(t.m_indices);
        }
        void applyTransform(const mat4f& m)
        {
            for (TriMeshOldVertex& v : m_vertices) { v.position = m * v.position; }
        }

        void stretch(float s) { stretch(vec3f(s, s, s)); }

        void stretch(const vec3f& v)
        {
            for (TriMeshOldVertex& mv : m_vertices) for (UINT i = 0; i < 3; i++) { mv.position[i] *= v[i]; }
        }

        std::vector<TriMeshOldVertex>& vertices() { return m_vertices; }

        std::vector<UINT>& indices() { return m_indices; }

        const std::vector<TriMeshOldVertex>& vertices() const { return m_vertices; }

        const std::vector<UINT>& indices() const { return m_indices; }

        bbox3f bbox() const
        {
            bbox3f result(m_vertices.front().position, m_vertices.front().position);
            for(const TriMeshOldVertex &v : m_vertices)
                result.include(v.position);
            return result;
        }

    private:
        std::vector<TriMeshOldVertex> m_vertices;
        std::vector<UINT> m_indices;
};

}  // namespace ml

#endif  // INCLUDE_CORE_MESH_TRIMESH_H_
