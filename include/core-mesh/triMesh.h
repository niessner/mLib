#ifndef CORE_MESH_TRIMESH_H_
#define CORE_MESH_TRIMESH_H_

namespace ml {

template<class FloatType>
class TriMesh
{
    public:
		// Vertex class of the Tri Mesh
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

		// Triangle class of the tri Mesh
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
			m_bHasNormals = false;
			m_bHasTexCoords = false;
			m_bHasColors = false;
			m_bTrianglePointersCreated = false;
		}
		TriMesh(const MeshData<FloatType>& meshData);

		TriMesh(
			const std::vector<point3d<FloatType>>& vertices, 
			const std::vector<unsigned int> indices, 
			const std::vector<point3d<FloatType>>& colors,
			const std::vector<point3d<FloatType>>& normals,
			const std::vector<point3d<FloatType>>& texCoords) :
				TriMesh(vertices.size(), indices.size(), 
					&vertices[0], &indices[0],
					colors.size() > 0 ? &colors[0] : NULL,
					normals.size() > 0 ? &normals[0] : NULL,
					texCoords.size() > 0 ? &texCoords[0] : NULL) 
		{
		}
		
		TriMesh(const std::vector<point3d<FloatType>>& vertices, const std::vector<unsigned int> indices) : TriMesh(vertices.size(), indices.size(), &vertices[0], &indices[0]) {}

		TriMesh(
			size_t numVertices, size_t numIndices,
			const point3d<FloatType>* vertices, 
			const unsigned int* indices, 
			const point3d<FloatType>* colors = NULL, 
			const point3d<FloatType>* normals = NULL, 
			const point2d<FloatType>* texCoords = NULL) 
		{
			if (numIndices % 3 != 0) throw MLIB_EXCEPTION("not a tri mesh");

			m_Vertices.resize(numVertices);
			for (size_t i = 0; i < numVertices; i++) {
				m_Vertices[i].position = vertices[i];
				if (colors) m_Vertices[i].color = colors[i];
				if (normals) m_Vertices[i].normals = normals[i];
				if (texCoords) m_Vertices[i].texCoord = texCoords[i];
			}
			m_Indices.resize(numIndices/3);
			for (size_t i = 0; i < numIndices/3; i++) {
				m_Indices[i] = vec3ui(indices[3*i+0],indices[3*i+1],indices[3*i+2]);
			}
			createTrianglePointers();
		}

		TriMesh(const TriMesh& other) {
			m_Vertices = other.m_Vertices;
			m_Indices = other.m_Indices;
			m_bHasNormals = other.m_bHasNormals;
			m_bHasTexCoords = other.m_bHasTexCoords;
			m_bHasColors = other.m_bHasColors;
			m_bTrianglePointersCreated = other.m_bTrianglePointersCreated;
			createTrianglePointers();
		}
        TriMesh(TriMesh&& t) {
            m_Vertices = std::move(t.m_Vertices);
            m_Indices = std::move(t.m_Indices);
			m_TrianglePointers = std::move(t.m_TrianglePointers);
			m_bHasNormals = other.m_bHasNormals;
			m_bHasTexCoords = other.m_bHasTexCoords;
			m_bHasColors = other.m_bHasColors;
			m_bTrianglePointersCreated = other.m_bTrianglePointersCreated;
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
			m_bHasNormals = other.m_bHasNormals;
			m_bHasTexCoords = other.m_bHasTexCoords;
			m_bHasColors = other.m_bHasColors;
			m_bTrianglePointersCreated = other.m_bTrianglePointersCreated;
        }

		void operator=(const TriMesh& other) {
			m_Vertices = other.m_Vertices;
			m_Indices = other.m_Indices;
			m_bHasNormals = other.m_bHasNormals;
			m_bHasTexCoords = other.m_bHasTexCoords;
			m_bHasColors = other.m_bHasColors;
			m_bTrianglePointersCreated = other.m_bTrianglePointersCreated;
			createTrianglePointers();
		}

		void applyTransform(const Matrix4x4<FloatType>& m) {
			for (Vertex<FloatType>& v : m_Vertices) { v.position = m * v.position; }
		}

        void scale(FloatType s) { scale(point3d<FloatType>(s, s, s)); }

        void scale(const point3d<FloatType>& v) {
            for (Vertex<FloatType>& mv : m_Vertices) for (UINT i = 0; i < 3; i++) { mv.position[i] *= v[i]; }
        }


		//! Computes the vertex normals of the mesh
		void computeNormals();


        const std::vector<Vertex<FloatType>>& getVertices() const { return m_Vertices; }
        const std::vector<vec3ui>& getIndices() const { return m_Indices; }
		const std::vector<Triangle<FloatType>*> getTrianglePointers() const { return m_TrianglePointers; }

		void getMeshData(MeshData<FloatType>& meshData) const {
			meshData.clear();

			meshData.m_Vertices.resize(m_Vertices.size());
			meshData.m_FaceIndicesVertices.resize(m_Indices.size());
			if (m_bHasColors) {
				meshData.m_Colors.resize(m_Vertices.size());
			}
			if (m_bHasNormals)	{
				meshData.m_Normals.resize(m_Vertices.size());
				meshData.m_FaceIndicesNormals.resize(m_Indices.size());
			}
			if (m_bHasTexCoords) {
				meshData.m_TextureCoords.resize(m_Vertices.size());
				meshData.m_FaceIndicesTextureCoords.resize(m_Indices.size());
			}
			for (size_t i = 0; i < m_Indices.size(); i++) {
				std::vector<unsigned int> ci(3);
				ci[0] = m_Indices[i].x;	ci[1] = m_Indices[i].y;	ci[2] = m_Indices[i].z;
				meshData.m_FaceIndicesVertices[i] = ci;
				if (m_bHasNormals)		meshData.m_FaceIndicesNormals[i] = ci;
				if (m_bHasTexCoords)	meshData.m_FaceIndicesTextureCoords[i] = ci;
			}

			for (size_t i = 0; i < m_Vertices.size(); i++) {
				meshData.m_Vertices[i] = m_Vertices[i].position;
				if (m_bHasColors)		meshData.m_Colors[i] = m_Vertices[i].color;
				if (m_bHasNormals)		meshData.m_Normals[i] = m_Vertices[i].normal;
				if (m_bHasTexCoords)	meshData.m_TextureCoords[i] = m_Vertices[i].texCoord;
			}
		}

		MeshData<FloatType> getMeshData() const {
			MeshData<FloatType> meshData;
			getMeshData(meshData);
			return meshData;
		}
    private:

		//! generates the triangle pointers (triangle pointer vector)
		void createTrianglePointers();

		bool m_bHasNormals;
		bool m_bHasTexCoords;
		bool m_bHasColors;
		bool m_bTrianglePointersCreated;

        std::vector<Vertex<FloatType>>		m_Vertices;
        std::vector<vec3ui>					m_Indices;
		std::vector<Triangle<FloatType>*>	m_TrianglePointers;
};

typedef TriMesh<float> TriMeshf;
typedef TriMesh<double> TriMeshd;

}  // namespace ml

#include "triMesh.cpp"

#endif  // INCLUDE_CORE_MESH_TRIMESH_H_
