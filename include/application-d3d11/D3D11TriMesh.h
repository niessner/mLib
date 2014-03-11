
#ifndef APPLICATION_D3D11_D3D11TRIMESH_H_
#define APPLICATION_D3D11_D3D11TRIMESH_H_

namespace ml {

class D3D11TriMesh : public GraphicsAsset
{
public:
	struct D3D11TriMeshVertex {
		D3D11TriMeshVertex() : position(Vec3f::origin), normal(Vec3f::origin), attributeA(Vec4f::origin), attributeB(Vec4f::origin) { }
		explicit D3D11TriMeshVertex(const Vec3f& _position) : position(_position) {}
		D3D11TriMeshVertex(const Vec3f& _p, const Vec3f& _n, const Vec4f& _a, const Vec4f& _b) : position(_p), normal(_n), attributeA(_a), attributeB(_b) { }
		Vec3f position;
		Vec3f normal;
		Vec4f attributeA;
		Vec4f attributeB;
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

	void updateAttributeA(GraphicsDevice &g, const std::vector<Vec4f> &vertexColors);

	const TriMesh& triMesh() const
	{
		return m_mesh;
	}

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

}  // namespace ml

#endif  // APPLICATION_D3D11_D3D11TRIMESH_H_