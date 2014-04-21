
#ifndef APPLICATION_D3D11_D3D11TRIMESH_H_
#define APPLICATION_D3D11_D3D11TRIMESH_H_

namespace ml {

class D3D11TriMesh : public GraphicsAsset
{
public:
	struct D3D11TriMeshVertex {
		D3D11TriMeshVertex() : position(vec3f::origin), normal(vec3f::origin), attributeA(vec4f::origin), attributeB(vec4f::origin) { }
		explicit D3D11TriMeshVertex(const vec3f& _position) : position(_position) {}
		D3D11TriMeshVertex(const vec3f& _p, const vec3f& _n, const vec4f& _a, const vec4f& _b) : position(_p), normal(_n), attributeA(_a), attributeB(_b) { }
		vec3f position;
		vec3f normal;
		vec4f attributeA;
		vec4f attributeB;
	};
	D3D11TriMesh()
	{
		m_layout = NULL;
		m_vertexBuffer = NULL;
		m_indexBuffer = NULL;
	}
	template<class T>
	D3D11TriMesh(GraphicsDevice &g, const MeshData<T>& meshData) {
		m_layout = NULL;
		m_vertexBuffer = NULL;
		m_indexBuffer = NULL;
		load(g, meshData);
	}
	~D3D11TriMesh()
	{
		SAFE_RELEASE(m_layout);
		SAFE_RELEASE(m_vertexBuffer);
		SAFE_RELEASE(m_indexBuffer);
	}

	void load(GraphicsDevice &g, const TriMeshOld& mesh);
	void load(GraphicsDevice &g, TriMeshOld&& mesh);


	template<class T>
	void load(GraphicsDevice &g, const MeshData<T>& meshData) {
		m_Vertices.resize(meshData.m_Vertices.size());
		//bool hasNormals = meshData.m_Normals.size() > 0;
		bool hasColors = meshData.m_Colors.size() > 0;
		for (unsigned int i = 0; i < m_Vertices.size(); i++) {
			m_Vertices[i].position = meshData.m_Vertices[i];
			if (hasColors)	m_Vertices[i].attributeA = meshData.m_Colors[i];
		}

		m_Indices.clear();
		for (size_t i = 0; i < meshData.m_FaceIndicesVertices.size(); i++) {
			if (meshData.m_FaceIndicesVertices[i].size() == 3) {
				for (size_t k = 0; k < meshData.m_FaceIndicesVertices[i].size(); k++) 
					m_Indices.push_back(meshData.m_FaceIndicesVertices[i][k]);
			} else {
				MLIB_WARNING("non triangle face found - ignoring it");
			}
		}
		reset(g);
	}

	void release(GraphicsDevice &g);
	void reset(GraphicsDevice &g);

	void render(GraphicsDevice &g) const;

	//! updates the first attribute which is typically color
	void updateAttributeA(GraphicsDevice &g, const std::vector<vec4f> &vertexColors);


	static const UINT layoutElementCount = 4;
	static const D3D11_INPUT_ELEMENT_DESC layout[layoutElementCount];


	const std::vector<D3D11TriMeshVertex>& getVertices() const {
		return m_Vertices;
	}
private:

	void initVB(GraphicsDevice &g);
	void initIB(GraphicsDevice &g);

	ID3D11InputLayout *m_layout;
	ID3D11Buffer *m_vertexBuffer;
	ID3D11Buffer *m_indexBuffer;
	
	std::vector<D3D11TriMeshVertex> m_Vertices;
	std::vector<unsigned int>		m_Indices;
};

}  // namespace ml

#endif  // APPLICATION_D3D11_D3D11TRIMESH_H_