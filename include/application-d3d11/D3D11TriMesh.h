
#ifndef APPLICATION_D3D11_D3D11TRIMESH_H_
#define APPLICATION_D3D11_D3D11TRIMESH_H_

namespace ml {

class D3D11TriMesh : public GraphicsAsset
{
public:
    static const UINT layoutElementCount = 4;	//accessed by D3D11VertexShader
    static const D3D11_INPUT_ELEMENT_DESC layout[layoutElementCount];

	D3D11TriMesh()
	{
        m_graphics = nullptr;
		m_vertexBuffer = nullptr;
		m_indexBuffer = nullptr;
	}

	template<class T>
	D3D11TriMesh(GraphicsDevice& g, const MeshData<T>& meshData)
    {
		m_vertexBuffer = nullptr;
		m_indexBuffer = nullptr;
		load(g, meshData);
	}

	template<class T>
	D3D11TriMesh(GraphicsDevice& g, const TriMesh<T>& triMesh)
    {
		m_vertexBuffer = nullptr;
		m_indexBuffer = nullptr;
		load(g, triMesh);
	}

	~D3D11TriMesh()
	{
		release();
	}

    void load(GraphicsDevice& g, const D3D11TriMesh& mesh)
    {
		m_graphics = &g.castD3D11();
        m_triMesh = mesh.m_triMesh;
        reset();
    }

	template<class T>
	void load(GraphicsDevice& g, const TriMesh<T>& triMesh)
    {
        m_graphics = &g.castD3D11();
        m_triMesh = triMesh;
		reset();
	}

	template<class T>
	void load(GraphicsDevice& g, const MeshData<T>& meshData)
    {
        load(g, TriMesh<T>(meshData));
	}

	void release();
	void reset();

	void render() const;

	//! \deprecated Backwards-compatible version of updateColors with unused GraphicsDevice& argument
	void updateColors(GraphicsDevice& g, const std::vector<vec4f> &vertexColors);

	//! Updates colors of this D3D11TriMesh to vertexColors. Precondition: vertexColors has same length as vertices otherwise exception is thrown
	void updateColors(const std::vector<vec4f>& vertexColors);

	//! computes and returns the bounding box; no caching
	BoundingBox3f computeBoundingBox() const
    {
        return m_triMesh.computeBoundingBox();
    }

    const TriMeshf& getTriMesh() const
    {
        return m_triMesh;
	}

	void getMeshData(MeshDataf& meshData) const
    {
        meshData = m_triMesh.getMeshData();
	}

	MeshDataf getMeshData() const {
		MeshDataf meshData;
		getMeshData(meshData);
		return meshData;
	}

    D3D11TriMesh(const D3D11TriMesh &t)
    {
        m_vertexBuffer = nullptr;
        m_indexBuffer = nullptr;
        load(*t.m_graphics, t);
    }
    D3D11TriMesh(D3D11TriMesh &&t)
    {
        m_graphics = t.m_graphics; t.m_graphics = nullptr;
        m_vertexBuffer = t.m_vertexBuffer; t.m_vertexBuffer = nullptr;
        m_indexBuffer = t.m_indexBuffer; t.m_indexBuffer = nullptr;
        m_triMesh = std::move(t.m_triMesh);
    }

    void operator = (const D3D11TriMesh& t)
    {
        m_vertexBuffer = nullptr;
        m_indexBuffer = nullptr;
        load(*t.m_graphics, t);
    }

    void operator = (D3D11TriMesh&& t)
    {
        m_graphics = t.m_graphics; t.m_graphics = nullptr;
        m_vertexBuffer = t.m_vertexBuffer; t.m_vertexBuffer = nullptr;
        m_indexBuffer = t.m_indexBuffer; t.m_indexBuffer = nullptr;
        m_triMesh = std::move(t.m_triMesh);
    }

private:
	D3D11GraphicsDevice *m_graphics;
    void initVB(GraphicsDevice &g);
	void initIB(GraphicsDevice &g);

	ID3D11Buffer *m_vertexBuffer;
	ID3D11Buffer *m_indexBuffer;
	
    TriMeshf m_triMesh;
};

}  // namespace ml

#endif  // APPLICATION_D3D11_D3D11TRIMESH_H_