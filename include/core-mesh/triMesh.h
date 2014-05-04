#ifndef CORE_MESH_TRIMESH_H_
#define CORE_MESH_TRIMESH_H_

namespace ml {



	template<class FloatType>
	class TriMesh
	{
	public:

		///MOVE VERTEX AND TRIANGLE BACK ONCE WE HAVE TEMPLATE ALIASING !!!! (for triangle BVH Accelerator)

		// ********************************
		// Vertex class of the Tri Mesh
		// ********************************
		template<class FloatType>
		class Vertex {
		public:
			Vertex() : position(point3d<FloatType>::origin), normal(point3d<FloatType>::origin), color(point3d<FloatType>::origin), texCoord(point2d<FloatType>::origin) { }
			Vertex(const vec3f& _position) : position(_position) { }
			Vertex(const vec3f& _p, const vec3f& _n, const vec3f& _c, const vec2f& _t) : position(_p), normal(_n), color(_c), texCoord(_t) { }

			Vertex operator*(FloatType t) const {
				return Vertex(position*t, normal*t, color*t, texCoord*t);
			}
			Vertex operator/(FloatType t) const {
				return Vertex(position/t, normal/t, color/t, texCoord/t);
			}
			Vertex operator+(const Vertex& other) const {
				return Vertex(position+other.position, normal+other.normal, color+other.color, texCoord+other.texCoord);
			}
			Vertex operator-(const Vertex& other) const {
				return Vertex(position-other.position, normal-other.normal, color-other.color, texCoord-other.texCoord);
			}

			void operator*=(FloatType t) {
				*this = *this * t;
			}
			void operator/=(FloatType t) const {
				*this = *this / t;
			}
			void operator+=(const Vertex& other) const {
				*this = *this + other;
			}
			void operator-=(const Vertex& other) const {
				*this = *this - other;
			}

			point3d<FloatType> position;
			point3d<FloatType> normal;
			point3d<FloatType> color;
			point2d<FloatType> texCoord;
		private:
		};
		typedef Vertex<float> Vertexf;
		typedef Vertex<double> Vertexd;

		// ********************************
		// Triangle class of the Tri Mesh
		// ********************************
		template<class FloatType>
		class Triangle {
		public:
			Triangle(const Vertex<FloatType> *v0, const Vertex<FloatType> *v1, const Vertex<FloatType> *v2) {
				assert (v0 && v1 && v2);
				this->v0 = v0;
				this->v1 = v1;
				this->v2 = v2;
				m_Center = (v0->position + v1->position + v2->position)/(FloatType)3.0;
			}
			~Triangle() {
			}

			Vertex<FloatType> getSurfaceVertex(FloatType u, FloatType v) const {
				return *v0 *((FloatType)1.0 - u - v) + *v1 *u + *v2 *v;
			}
			point3d<FloatType> getSurfacePosition(FloatType u, FloatType v) const 	{
				return v0->position*((FloatType)1.0 - u - v) + v1->position*u + v2->position*v;
			}
			point3d<FloatType> getSurfaceColor(FloatType u, FloatType v) const {
				return v0->color*((FloatType)1.0 - u - v) + v1->color*u + v2->color*v;
			}
			point2d<FloatType> getSurfaceNormal(FloatType u, FloatType v) const {
				return v0->normal*((FloatType)1.0 - u - v) + v1->normal*u + v2->normal*v;
			}
			point3d<FloatType> getSurfaceTexCoord(FloatType u, FloatType v) const {
				return v0->texCoord*((FloatType)1.0 - u - v) + v1->texCoord*u + v2->texCoord*v;
			}

			bool intersect(const Ray<FloatType> &r, FloatType& _t, FloatType& _u, FloatType& _v, FloatType tmin = (FloatType)0, FloatType tmax = std::numeric_limits<FloatType>::max(), bool intersectOnlyFrontFaces = false) const {
				const point3d<FloatType> &d = r.direction();
				const point3d<FloatType> &p = r.origin();

				point3d<FloatType> e1 = v1->position - v0->position;
				point3d<FloatType> e2 = v2->position - v0->position;

				if (intersectOnlyFrontFaces) {
					point3d<FloatType> n = e1^e2; n.normalize();
					if ((d | n) > (FloatType)0.0) return false;
				}

				point3d<FloatType> h = d^e2;
				FloatType a = e1 | h;

				//if (a > -0.0000000001 && a < 0.0000000001) return false;
				if (a == (FloatType)0.0 || a == -(FloatType)0.0)	return false;

				FloatType f = (FloatType)1.0/a;
				point3d<FloatType> s = p - v0->position;
				FloatType u = f * (s | h);

				if (u < (FloatType)0.0 || u > (FloatType)1.0)	return false;

				point3d<FloatType> q = s^e1;
				FloatType v = f * (d | q);

				if (v < (FloatType)0.0 || u + v > (FloatType)1.0)	return false;

				// at this stage we can compute t to find out where the intersection point is on the line
				FloatType t = f * (e2 | q);

				//if (t > 0.0000000001 && t < r.t) {
				//if (t < _t) {
				if (t <= tmax && t >= tmin) {
					_t = t;
					_u = u;
					_v = v;
					return true;
				} else {
					return false;
				}
			}


			void includeInBoundingBox(BoundingBox3d<FloatType> &bb) {
				bb.include(v0->position);
				bb.include(v1->position);
				bb.include(v2->position);
			}

			const point3d<FloatType>& getCenter() const {
				return m_Center;
			}
		private:
			const Vertex<FloatType> *v0, *v1, *v2;			
			point3d<FloatType> m_Center;
		};
		typedef Triangle<float> Trianglef;
		typedef Triangle<double> Triangled;



		// ********************************
		// TriMesh itself
		// ********************************
		TriMesh() : m_Vertices(), m_Indices(), m_TrianglePointers() {
			m_bHasNormals = false;
			m_bHasTexCoords = false;
			m_bHasColors = false;
			m_bTrianglePointersCreated = false;
		}
		TriMesh(const MeshData<FloatType>& meshData);

		TriMesh(const std::vector<Vertex<FloatType>>& vertices, const std::vector<unsigned int>& indices) {
			if (indices.size()%3 != 0)	throw MLIB_EXCEPTION("not a tri mesh");
			m_Vertices = vertices;
			m_Indices.resize(indices.size()/3);
			memcpy(&m_Indices[0], &indices[0], indices.size()*sizeof(unsigned int));
		}
		TriMesh(const std::vector<Vertex<FloatType>>& vertices, const std::vector<vec3ui>& indices) {
			m_Vertices = vertices;
			m_Indices = indices;
		}

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
		}

		TriMesh(const TriMesh& other) {
			m_Vertices = other.m_Vertices;
			m_Indices = other.m_Indices;
			m_bHasNormals = other.m_bHasNormals;
			m_bHasTexCoords = other.m_bHasTexCoords;
			m_bHasColors = other.m_bHasColors;
		}
		TriMesh(TriMesh&& t) {
			m_Vertices = std::move(t.m_Vertices);
			m_Indices = std::move(t.m_Indices);
			m_bHasNormals = t.m_bHasNormals;
			m_bHasTexCoords = t.m_bHasTexCoords;
			m_bHasColors = t.m_bHasColors;
		}

		~TriMesh() {
		}

		void operator=(TriMesh&& t) {
			m_Vertices = std::move(t.m_Vertices);
			m_Indices = std::move(t.m_Indices);
			m_bHasNormals = t.m_bHasNormals;
			m_bHasTexCoords = t.m_bHasTexCoords;
			m_bHasColors = t.m_bHasColors;
		}

		void operator=(const TriMesh& other) {
			m_Vertices = other.m_Vertices;
			m_Indices = other.m_Indices;
			m_bHasNormals = other.m_bHasNormals;
			m_bHasTexCoords = other.m_bHasTexCoords;
			m_bHasColors = other.m_bHasColors;
		}

		void applyTransform(const Matrix4x4<FloatType>& m) {
			for (Vertex<FloatType>& v : m_Vertices) { v.position = m * v.position; }
		}

		void scale(FloatType s) { scale(point3d<FloatType>(s, s, s)); }

		void scale(const point3d<FloatType>& v) {
			for (Vertex<FloatType>& mv : m_Vertices) for (UINT i = 0; i < 3; i++) { mv.position[i] *= v[i]; }
		}
		
		//! Computes the bounding box of the mesh (not cached!)
		BoundingBox3d<FloatType> getBoundingBox() const {
			BoundingBox3d<FloatType> bb;
			for (size_t i = 0; i < m_Vertices.size(); i++) {
				bb.include(m_Vertices[i].position);
			}
			return bb;
		}

		//! Computes the vertex normals of the mesh
		void computeNormals();


		const std::vector<Vertex<FloatType>>& getVertices() const { return m_Vertices; }
		const std::vector<vec3ui>& getIndices() const { return m_Indices; }
		
		void getMeshData(MeshData<FloatType>& meshData) const {
			meshData.clear();

			meshData.m_Vertices.resize(m_Vertices.size());
			meshData.m_FaceIndicesVertices.resize(m_Indices.size());
			if (m_bHasColors) {
				meshData.m_Colors.resize(m_Vertices.size());
			}
			if (m_bHasNormals)	{
				meshData.m_Normals.resize(m_Vertices.size());
			}
			if (m_bHasTexCoords) {
				meshData.m_TextureCoords.resize(m_Vertices.size());
			}
			for (size_t i = 0; i < m_Indices.size(); i++) {
				std::vector<unsigned int> ci(3);
				ci[0] = m_Indices[i].x;	ci[1] = m_Indices[i].y;	ci[2] = m_Indices[i].z;
				meshData.m_FaceIndicesVertices[i] = ci;
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
		
		bool m_bHasNormals;
		bool m_bHasTexCoords;
		bool m_bHasColors;

		std::vector<Vertex<FloatType>>		m_Vertices;
		std::vector<vec3ui>					m_Indices;
	};

	typedef TriMesh<float> TriMeshf;
	typedef TriMesh<double> TriMeshd;

}  // namespace ml

#include "triMesh.cpp"

#endif  // INCLUDE_CORE_MESH_TRIMESH_H_
