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
		Vertex() : position(point3d<FloatType>::origin), normal(point3d<FloatType>::origin), color(point4d<FloatType>::origin), texCoord(point2d<FloatType>::origin) { }
		Vertex(const point3d<FloatType>& _position) : position(_position) { }
		Vertex(const point3d<FloatType>& _p, const point3d<FloatType>& _n) : position(_p), normal(_n) { }
		Vertex(const point3d<FloatType>& _p, const point3d<FloatType>& _n, const point4d<FloatType>& _c, const point2d<FloatType>& _t) : position(_p), normal(_n), color(_c), texCoord(_t) { }

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

        //
        // If you change the order of these fields, you must update the layout fields in D3D11TriMesh::layout 
        //
		point3d<FloatType> position;
		point3d<FloatType> normal;
		point4d<FloatType> color;
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

		Triangle(const Vertex<FloatType> *v0, const Vertex<FloatType> *v1, const Vertex<FloatType> *v2, unsigned int triIdx = 0, unsigned int meshIdx = 0) {
			assert (v0 && v1 && v2);
			this->v0 = v0;
			this->v1 = v1;
			this->v2 = v2;
			m_Center = (v0->position + v1->position + v2->position)/(FloatType)3.0;
			m_TriangleIndex = triIdx;
			m_MeshIndex = meshIdx;
		}
		~Triangle() {
		}


		Vertex<FloatType> getSurfaceVertex(FloatType u, FloatType v) const {
			return *v0 *((FloatType)1.0 - u - v) + *v1 *u + *v2 *v;
		}
		point3d<FloatType> getSurfacePosition(FloatType u, FloatType v) const 	{
			return v0->position*((FloatType)1.0 - u - v) + v1->position*u + v2->position*v;
		}
		point4d<FloatType> getSurfaceColor(FloatType u, FloatType v) const {
			return v0->color*((FloatType)1.0 - u - v) + v1->color*u + v2->color*v;
		}
		point3d<FloatType> getSurfaceNormal(FloatType u, FloatType v) const {
			return v0->normal*((FloatType)1.0 - u - v) + v1->normal*u + v2->normal*v;
		}
		point2d<FloatType> getSurfaceTexCoord(FloatType u, FloatType v) const {
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

		bool intersects(const Triangle<FloatType>& other) const {
			return intersection::intersectTriangleTriangle(v0->position,v1->position,v2->position, other.v0->position,other.v1->position,other.v2->position);
		}

		void includeInBoundingBox(BoundingBox3<FloatType> &bb) const {
			bb.include(v0->position);
			bb.include(v1->position);
			bb.include(v2->position);
		}

		BoundingBox3<FloatType> getBoundingBox() const {
			BoundingBox3<FloatType> bb;
			includeInBoundingBox(bb);
			return bb;
		}

		const point3d<FloatType>& getCenter() const {
			return m_Center;
		}

		const Vertex<FloatType>& getV0() const {
			return *v0;
		}
		const Vertex<FloatType>& getV1() const {
			return *v1;
		}
		const Vertex<FloatType>& getV2() const {
			return *v2;
		}
		
		unsigned int getIndex() const {
			return m_TriangleIndex;
		}
		unsigned int getMeshIndex() const {
			return m_MeshIndex;
		}

	private:
		const Vertex<FloatType> *v0, *v1, *v2;			
		point3d<FloatType> m_Center;	//TODO check if we want to store the center
		unsigned int m_TriangleIndex;	//! 0-based triangle index within it's mesh 
		unsigned int m_MeshIndex;		//! 0-based mesh index; used for accelerators that take an std::vector of triMeshes 
	};
	typedef Triangle<float> Trianglef;
	typedef Triangle<double> Triangled;



	// ********************************
	// TriMesh itself
	// ********************************
	TriMesh() : m_Vertices(), m_Indices() {
		m_bHasNormals = false;
		m_bHasTexCoords = false;
		m_bHasColors = false;
	}
	TriMesh(const MeshData<FloatType>& meshData);

	TriMesh(const std::vector<Vertex<FloatType>>& vertices, const std::vector<unsigned int>& indices, const bool recomputeNormals = false,
          const bool hasNormals = false, const bool hasTexCoords = false, const bool hasColors = false) {
		if (indices.size()%3 != 0)	throw MLIB_EXCEPTION("not a tri mesh");
		m_Vertices = vertices;
		m_Indices.resize(indices.size()/3);
		memcpy(&m_Indices[0], &indices[0], indices.size()*sizeof(unsigned int));
    m_bHasNormals = hasNormals;
    m_bHasTexCoords = hasTexCoords;
    m_bHasColors = hasColors;
    if (recomputeNormals) {
      computeNormals();
    }
	}

  TriMesh(const std::vector<Vertex<FloatType>>& vertices, const std::vector<vec3ui>& indices, bool recomputeNormals = false,
          const bool hasNormals = false, const bool hasTexCoords = false, const bool hasColors = false) {
	  m_Vertices = vertices;
	  m_Indices = indices;
    m_bHasNormals = hasNormals;
    m_bHasTexCoords = hasTexCoords;
    m_bHasColors = hasColors;
    if (recomputeNormals) {
      computeNormals();
    }
  }

	TriMesh(
		const std::vector<point3d<FloatType>>& vertices, 
		const std::vector<unsigned int>& indices, 
		const std::vector<point4d<FloatType>>& colors,
		const std::vector<point3d<FloatType>>& normals,
		const std::vector<point3d<FloatType>>& texCoords) :
	TriMesh(vertices.size(), indices.size(), 
		&vertices[0], &indices[0],
		colors.size() > 0 ? &colors[0] : nullptr,
		normals.size() > 0 ? &normals[0] : nullptr,
		texCoords.size() > 0 ? &texCoords[0] : nullptr) 
	{
	}

	TriMesh(const std::vector<point3d<FloatType> >& vertices, const std::vector<unsigned int>& indices) : TriMesh(vertices.size(), indices.size(), &vertices[0], &indices[0]) {}

	TriMesh(
		size_t numVertices, size_t numIndices,
		const point3d<FloatType>* vertices, 
		const unsigned int* indices, 
		const point4d<FloatType>* colors = nullptr, 
		const point3d<FloatType>* normals = nullptr, 
		const point2d<FloatType>* texCoords = nullptr) 
	{
		if (numIndices % 3 != 0) throw MLIB_EXCEPTION("not a tri mesh");
    m_bHasColors = colors != nullptr;
    m_bHasNormals = normals != nullptr;
    m_bHasTexCoords = texCoords != nullptr;
    m_Vertices.resize(numVertices);
		for (size_t i = 0; i < numVertices; i++) {
			m_Vertices[i].position = vertices[i];
			if (colors) m_Vertices[i].color = colors[i];
			if (normals) m_Vertices[i].normal = normals[i];
			if (texCoords) m_Vertices[i].texCoord = texCoords[i];
		}
		m_Indices.resize(numIndices/3);
		for (size_t i = 0; i < numIndices/3; i++) {
			m_Indices[i] = vec3ui(indices[3*i+0],indices[3*i+1],indices[3*i+2]);
		}
	}

	TriMesh(const BoundingBox3<FloatType>& bbox, const point4d<FloatType>& color = point4d<FloatType>(1.0,1.0,1.0,1.0)) {
		std::vector<point3d<FloatType>> vertices;
		std::vector<vec3ui> indices;
		std::vector<point3d<FloatType>> normals;
		bbox.makeTriMesh(vertices, indices, normals);

		m_Vertices.resize(vertices.size());
		for (size_t i = 0; i < vertices.size(); i++) {
			m_Vertices[i].color = color;
			m_Vertices[i].position = vertices[i];
			m_Vertices[i].normal = normals[i];
		}
		m_Indices = indices;
    m_bHasColors = m_bHasNormals = m_bHasTexCoords = true;
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


	TriMesh(const BinaryGrid3d& grid, FloatType voxelSize = (FloatType)1, bool withNormals = false, const point4d<FloatType>& color = point4d<FloatType>(0.5,0.5,0.5,0.5)) {
		for (unsigned int z = 0; z < grid.slices(); z++) {
			for (unsigned int y = 0; y < grid.rows(); y++) {
				for (unsigned int x = 0; x < grid.cols(); x++) {
					if (grid.isVoxelSet(x,y,z)) {
						point3d<FloatType> p((FloatType)x,(FloatType)y,(FloatType)z);
						p = p * voxelSize;
						BoundingBox3<FloatType> bb;
						bb.include(p - voxelSize*0.5f);	bb.include(p + voxelSize*0.5f);


						if (withNormals) {
							point3d<FloatType> verts[24];
							vec3ui indices[12];
							point3d<FloatType> normals[24];
							bb.makeTriMesh(verts,indices,normals);

							unsigned int vertIdxBase = (unsigned int)m_Vertices.size();
							for (unsigned int i = 0; i < 24; i++) {
								m_Vertices.push_back(Vertex<FloatType>(verts[i], normals[i]));
							}
							for (unsigned int i = 0; i < 12; i++) {
								indices[i] += vertIdxBase;
								m_Indices.push_back(indices[i]);
							}
						} else {
							point3d<FloatType> verts[8];
							vec3ui indices[12];
							bb.makeTriMesh(verts, indices);

							unsigned int vertIdxBase = (unsigned int)m_Vertices.size();
							for (unsigned int i = 0; i < 8; i++) {
								m_Vertices.push_back(Vertex<FloatType>(verts[i]));
							}
							for (unsigned int i = 0; i < 12; i++) {
								indices[i] += vertIdxBase;
								m_Indices.push_back(indices[i]);
							}
						}
					}
				}
			}
			for (unsigned int i = 0; i < m_Vertices.size(); i++) {
				m_Vertices[i].color = color;
			}
		}
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

	void clear() {
		m_Vertices.clear();
		m_Indices.clear();
		m_bHasNormals = false;
		m_bHasTexCoords = false;
		m_bHasColors = false;
	}

	void transform(const Matrix4x4<FloatType>& m) {
		for (Vertex<FloatType>& v : m_Vertices) { v.position = m * v.position; }
	}

	void scale(FloatType s) { scale(point3d<FloatType>(s, s, s)); }

	void scale(const point3d<FloatType>& v) {
		for (Vertex<FloatType>& mv : m_Vertices) for (UINT i = 0; i < 3; i++) { mv.position[i] *= v[i]; }
	}

	//! overwrites/sets the mesh color
	void setColor(const point4d<FloatType>& c) {
		for (auto& v : m_Vertices) {
			v.color = c;
		}
	}
		
	//! Computes the bounding box of the mesh (not cached!)
	BoundingBox3<FloatType> getBoundingBox() const {
		BoundingBox3<FloatType> bb;
		for (size_t i = 0; i < m_Vertices.size(); i++) {
			bb.include(m_Vertices[i].position);
		}
		return bb;
	}

	//! Computes the vertex normals of the mesh
	void computeNormals();


	const std::vector<Vertex<FloatType>>& getVertices() const { return m_Vertices; }
	const std::vector<vec3ui>& getIndices() const { return m_Indices; }
	
	std::vector<Vertex<FloatType>>& getVertices() { return m_Vertices; }
    std::vector<vec3ui>& getIndices() { return m_Indices; }

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
			meshData.m_FaceIndicesVertices[i][0] = m_Indices[i].x;
			meshData.m_FaceIndicesVertices[i][1] = m_Indices[i].y;
			meshData.m_FaceIndicesVertices[i][2] = m_Indices[i].z;
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


	bool hasNormals() const {
		return m_bHasNormals;
	}
	bool hasColors() const {
		return m_bHasColors;
	}
	bool hasTexCoords() const {
		return m_bHasTexCoords;
	}

	BinaryGrid3d voxelize(FloatType voxelSize, const BoundingBox3<FloatType>& bounds = BoundingBox3<FloatType>(), bool solid = false) const {

		BoundingBox3<FloatType> bb = getBoundingBox();
		bb.include(point3d<FloatType>(0.0f,0.0f,0.0f));
		bb.scale(1.2f);
		//TODO use bounding box to compute offset such that all voxels are positive...

		BinaryGrid3d grid(vec3ui(bb.getExtent() / voxelSize));
		for (size_t i = 0; i < m_Indices.size(); i++) {
			voxelizeTriangle(m_Vertices[m_Indices[i].x].position, m_Vertices[m_Indices[i].y].position, m_Vertices[m_Indices[i].z].position, grid, voxelSize, vec3ui(0,0,0));
		}
		return grid;
	}
private:

	void voxelizeTriangle(const point3d<FloatType>& v0, const point3d<FloatType>& v1, const point3d<FloatType>& v2, BinaryGrid3d& grid, FloatType voxelSize, const vec3ui& voxelOffset) const {
		FloatType diagLenSq = (FloatType)3.0*voxelSize*voxelSize;
		if ((v0-v1).lengthSq() < diagLenSq && (v0-v2).lengthSq() < diagLenSq &&	(v1-v2).lengthSq() < diagLenSq) {
			BoundingBox3<FloatType> bb;
			bb.include(v0);	bb.include(v1);	bb.include(v2);
			vec3ui minI = math::floor(bb.getMin()/voxelSize);
			vec3ui maxI = math::ceil(bb.getMax()/voxelSize);

			//test for accurate voxel-triangle intersections
			for (unsigned int i = minI.x; i <= maxI.x; i++) {
				for (unsigned int j = minI.y; j <= maxI.y; j++) {
					for (unsigned int k = minI.z; k <= maxI.z; k++) {
						point3d<FloatType> v((FloatType)i,(FloatType)j,(FloatType)k);
						BoundingBox3<FloatType> voxel;
						const FloatType eps = (FloatType)0.00001;
						voxel.include((v - (FloatType)0.5-eps)*voxelSize);
						voxel.include((v + (FloatType)0.5+eps)*voxelSize);
						if (voxel.intersects(v0, v1, v2)) {
							//project to xy-plane
							point2d<FloatType> p0(v0);
							point2d<FloatType> p1(v1);
							point2d<FloatType> p2(v2);
							point2d<FloatType> pv(v);
							BoundingBox2<FloatType> pBox;	
							pBox.include(p0);
							pBox.include(p1);
							pBox.include(p2);
							grid.setVoxel(i,j,k);
						}
					}
				}
			} 
		} else {
			vec3f e0 = (v0 + v1)/2.0f;
			vec3f e1 = (v1 + v2)/2.0f;
			vec3f e2 = (v2 + v0)/2.0f;
			voxelizeTriangle(v0,e0,e2, grid, voxelSize, voxelOffset);
			voxelizeTriangle(e0,v1,e1, grid, voxelSize, voxelOffset);
			voxelizeTriangle(e1,v2,e2, grid, voxelSize, voxelOffset);
			voxelizeTriangle(e0,e1,e2, grid, voxelSize, voxelOffset);
		}
	}

    friend class boost::serialization::access;
		
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
