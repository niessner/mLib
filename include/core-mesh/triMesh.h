#ifndef CORE_MESH_TRIMESH_H_
#define CORE_MESH_TRIMESH_H_

namespace ml {

template<class FloatType>
class TriMesh
{
    public:

		template<class FloatType>
		class Vertex {
		public:
			Vertex() : position(point3d<FloatType>::origin), normal(point3d<FloatType>::origin), color(point3d<FloatType>::origin), texCoord(point2d<FloatType>::origin) { }
			explicit Vertex(const vec3f& _position) : position(_position) {}
			Vertex (const vec3f& _p, const vec3f& _n, const vec3f& _c, const vec2f& _t) : position(_p), normal(_n), color(_c), texCoord(_t) { }
			point3d<FloatType> position;
			point3d<FloatType> normal;
			point3d<FloatType> color;
			point2d<FloatType> texCoord;
		private:
		};

		template<class FloatType>
		class Triangle {
		public:
			Triangle(Vertex<FloatType> *v0, Vertex<FloatType> *v1, Vertex<FloatType> *v2) {
				assert (v0 && v1 && v2);

				this->v0 = v0;
				this->v1 = v1;
				this->v2 = v2;
				m_Center = (v0->position + v1->position + v2->position)/(FloatType)3.0;
			}
		private:
			Vertex<FloatType> *v0, *v1, *v2;
			point3d<FloatType> m_Center;
		};
		
        TriMesh() : m_Vertices(), m_Indices(), m_TrianglePointers {
			m_bNormalsComputed = false;
			m_bTrianglePointersCreated = false;
		}
		TriMesh(const MeshData<FloatType>& meshData) {
			m_bNormalsComputed = false;
			m_bTrianglePointersCreated = false;

			m_Vertices.resize(meshData.m_Vertices.size());

			bool hasNormals = meshData.m_Normals.size() > 0;
			bool hasColors = meshData.m_Colors.size() > 0;
			bool hasTexCoords = meshData.m_TextureCoords.size() > 0;

			for (unsigned int i = 0; i < m_Vertices.size(); i++) {
				m_Vertices[i].position = meshData.m_Vertices[i];
				if (hasColors)	m_Vertices[i].color = meshData.m_Colors[i];
			}

			for (size_t i = 0; i < meshData.m_FaceIndicesVertices.size(); i++) {
				if (meshData.m_FaceIndicesVertices[i].size() == 3) {
					m_Indices.push_back(vec3ui(meshData.m_FaceIndicesVertices[i][0], meshData.m_FaceIndicesVertices[i][1], meshData.m_FaceIndicesVertices[i][2]));
					if (hasNormals) {
						//we are ignoring the fact that sometimes there might be vertex split required (if a vertex has two different normals)
						m_Vertices[m_Indices[i][0]].normal = meshData.m_Normals[meshData.m_FaceIndicesNormals[i][0]];
						m_Vertices[m_Indices[i][1]].normal = meshData.m_Normals[meshData.m_FaceIndicesNormals[i][1]];
						m_Vertices[m_Indices[i][2]].normal = meshData.m_Normals[meshData.m_FaceIndicesNormals[i][2]];
					}
					if (hasTexCoords) {
						//we are ignoring the fact that sometimes there might be vertex split required (if a vertex has two different normals)
						m_Vertices[m_Indices[i][0]].texCoord = meshData.m_TextureCoords[meshData.m_FaceIndicesTextureCoords[i][0]];
						m_Vertices[m_Indices[i][1]].texCoord = meshData.m_TextureCoords[meshData.m_FaceIndicesTextureCoords[i][1]];
						m_Vertices[m_Indices[i][2]].texCoord = meshData.m_TextureCoords[meshData.m_FaceIndicesTextureCoords[i][2]];
					}
				} else {
					MLIB_WARNING("non triangle face found - ignoring it");
				}
			}

			if (!hasNormals)	computeNormals();
			createTrianglePointers();
		}
        TriMesh(TriMesh&& t)
        {
            m_Vertices = std::move(t.m_Vertices);
            m_Indices = std::move(t.m_Indices);
			m_TrianglePointers = std::move(t.m_TrianglePointers);
        }

		~TriMesh() {
			for (size_t i = 0; i < m_TrianglePointers.size(); i++) {
				SAFE_DELETE(m_TrianglePointers[i]);
			}
		}

        void operator=(TriMesh&& t) {
            m_Vertices = std::move(t.m_Vertices);
            m_Indices = std::move(t.m_Indices);
			m_TrianglePointers = std::move(t.m_TrianglePointers);
        }
		void applyTransform(const mat4f& m) {
			for (Vertex& v : m_Vertices) { v.position = m * v.position; }
		}

        void stretch(float s) { stretch(vec3f(s, s, s)); }

        void stretch(const vec3f& v) {
            for (Vertex& mv : m_Vertices) for (UINT i = 0; i < 3; i++) { mv.position[i] *= v[i]; }
        }

        const std::vector<Vertex<FloatType>>& getVertices() const { return m_Vertices; }
        const std::vector<vec3ui>& getIndices() const { return m_Indices; }
		const std::vector<Triangle<FloatType>*> getTrianglePointers() const { return m_TrianglePointers; }

    private:

		//! Computes the vertex normals of the mesh
		void computeNormals() {

			for (int i = 0; i < (int)m_Vertices.size(); i++) {
				m_Vertices[i].normal = point3d<FloatType>::origin;
			}

			for (int i = 0; i < (int)m_Indices.size(); i++) {
				point3d<FloatType> faceNormal = 
					(m_Vertices[m_Indices[i].y].position - m_Vertices[m_Indices[i].x].position)^(m_Vertices[m_Indices[i].z].position - m_Vertices[m_Indices[i].x].position);

				m_Vertices[m_Indices[i].x].normal += faceNormal;
				m_Vertices[m_Indices[i].y].normal += faceNormal;
				m_Vertices[m_Indices[i].z].normal += faceNormal;
			}
			for (int i = 0; i < (int)m_Vertices.size(); i++) {
				m_Vertices[i].normal.normalize();
			}

			m_bNormalsComputed = true;
		}


		//! generates the triangle pointers (triangle pointer vector)
		void createTrianglePointers() {

			for (size_t i = 0; i < m_TrianglePointers.size(); i++) {
				SAFE_DELETE(m_TrianglePointers[i]);
			}
			m_TrianglePointers.resize(m_Indices.size());
#pragma omp parallel for
			for (int i = 0; i < (int)m_Indices.size(); i++) {
				Triangle<FloatType>* tri = new Triangle<FloatType>(&m_Vertices[m_Indices[i].x], &m_Vertices[m_Indices[i].y], &m_Vertices[m_Indices[i].z]);
				m_TrianglePointers[i] = tri;
			}

			m_bTrianglePointersCreated = true;
		}

		bool m_bNormalsComputed;
		bool m_bTrianglePointersCreated;

        std::vector<Vertex<FloatType>>		m_Vertices;
        std::vector<vec3ui>					m_Indices;
		std::vector<Triangle<FloatType>*>	m_TrianglePointers;
};

typedef TriMesh<float> TriMeshf;
typedef TriMesh<double> TriMeshd;

}  // namespace ml

#endif  // INCLUDE_CORE_MESH_TRIMESH_H_
