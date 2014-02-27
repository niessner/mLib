#ifndef INCLUDE_CORE_MESH_TRIMESH_H_
#define INCLUDE_CORE_MESH_TRIMESH_H_

struct MeshVertex {
  MeshVertex() {}
  explicit MeshVertex(const vec3f& _position) : position(_position) {}
  MeshVertex(const vec3f& _p, const vec3f& _n, const vec4f& _a, const vec4f& _b)
    : position(_p), normal(_n), attributeA(_a), attributeB(_b) { }
  vec3f position;
  vec3f normal;
  vec4f attributeA;
  vec4f attributeB;
};

class TriMesh {
 public:
  TriMesh() : m_vertices(), m_indices() { }
  TriMesh(const Vector<MeshVertex>& _vertices, const Vector<UINT>& _indices) {
    m_vertices = _vertices;
    m_indices = _indices;

	const bool randomizeColor = true;
	if(randomizeColor)
	{
		for(MeshVertex &v : m_vertices)
		{
			v.normal = vec3f(rand() / (float)RAND_MAX, rand() / (float)RAND_MAX, rand() / (float)RAND_MAX);
			v.attributeA = vec4f(rand() / (float)RAND_MAX, rand() / (float)RAND_MAX, rand() / (float)RAND_MAX, rand() / (float)RAND_MAX);
			v.attributeB = vec4f(rand() / (float)RAND_MAX, rand() / (float)RAND_MAX, rand() / (float)RAND_MAX, rand() / (float)RAND_MAX);
		}
	}
  }

  const Vector<MeshVertex>& vertices() const { return m_vertices; }

  const Vector<UINT>& indices() const { return m_indices; }

 private:
  Vector<MeshVertex> m_vertices;
  Vector<UINT> m_indices;
};

#endif  // INCLUDE_CORE_MESH_TRIMESH_H_
