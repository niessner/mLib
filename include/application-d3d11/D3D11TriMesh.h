
//
// This should be moved outside the context of D3D11TriMesh
//
struct MeshVertex
{
	MeshVertex() {}
	MeshVertex(const vec3f &_position) :
		position(_position)
	{}

	vec3f position;
	vec3f normal;
	vec4f attributeA;
	vec4f attributeB;

	static const UINT layoutElementCount = 4;
	static const D3D11_INPUT_ELEMENT_DESC layout[layoutElementCount];
};

class D3D11TriMesh : public GraphicsAsset
{
public:
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
	void load(GraphicsDevice &g, const Vector<MeshVertex> &vertices, const Vector<UINT> &indices);

	void release(GraphicsDevice &g);
	void reset(GraphicsDevice &g);
	
	void render(GraphicsDevice &g);

private:
	void initVB(GraphicsDevice &g);
	void initIB(GraphicsDevice &g);

	//
	// These are CPU copies of the data buffers. They should be replaced by a non-D3D mesh structure.
	//
	Vector<MeshVertex> m_vertices;
	Vector<UINT> m_indices;

	ID3D11InputLayout *m_layout;
	ID3D11Buffer *m_vertexBuffer;
	ID3D11Buffer *m_indexBuffer;
};