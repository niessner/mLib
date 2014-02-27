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

  //
  // These are CPU copies of the data buffers. They should be replaced by a non-D3D mesh structure.
  //
  TriMesh m_mesh;

  ID3D11InputLayout *m_layout;
  ID3D11Buffer *m_vertexBuffer;
  ID3D11Buffer *m_indexBuffer;
};