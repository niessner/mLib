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
	~D3D11TriMesh()
	{
		SAFE_RELEASE(m_layout);
		SAFE_RELEASE(m_vertexBuffer);
		SAFE_RELEASE(m_indexBuffer);
	}
	void load(GraphicsDevice &g, const TriMesh& mesh);
	void load(GraphicsDevice &g, TriMesh&& mesh);

	void release(GraphicsDevice &g);
	void reset(GraphicsDevice &g);

	void render(GraphicsDevice &g);

	static const UINT layoutElementCount = 4;
	static const D3D11_INPUT_ELEMENT_DESC layout[layoutElementCount];

private:

	void initVB(GraphicsDevice &g);
	void initIB(GraphicsDevice &g);

	ID3D11InputLayout *m_layout;
	ID3D11Buffer *m_vertexBuffer;
	ID3D11Buffer *m_indexBuffer;
    TriMesh m_mesh;
};