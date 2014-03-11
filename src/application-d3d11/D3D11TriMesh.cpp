
const D3D11_INPUT_ELEMENT_DESC ml::D3D11TriMesh::layout[layoutElementCount] =
{
	{ "position", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "normal", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "attributeA", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "attributeB", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
};

//TODO load from arbitrary meshes ;)
void ml::D3D11TriMesh::load(GraphicsDevice &g, const TriMesh& mesh)
{
	g.castD3D11().registerAsset(this);
    m_mesh = mesh;
	reset(g);
}

void ml::D3D11TriMesh::updateAttributeA(GraphicsDevice &g, const std::vector<vec4f> &newValues)
{
    for(UINT vertexIndex = 0; vertexIndex < m_mesh.vertices().size(); vertexIndex++)
    {
        m_mesh.vertices()[vertexIndex].attributeA = newValues[vertexIndex];
    }
    reset(g);
}

void ml::D3D11TriMesh::load(GraphicsDevice &g, TriMesh&& mesh)
{
	g.castD3D11().registerAsset(this);
	m_mesh = mesh;
	reset(g);
}

void ml::D3D11TriMesh::release(GraphicsDevice &g)
{
	SAFE_RELEASE(m_vertexBuffer);
	SAFE_RELEASE(m_indexBuffer);
}

void ml::D3D11TriMesh::reset(GraphicsDevice &g)
{
	release(g);
	initVB(g);
	initIB(g);
}

void ml::D3D11TriMesh::initVB(GraphicsDevice &g)
{
    if (m_mesh.vertices().size() == 0) return;
	auto &device = g.castD3D11().device();

	D3D11_BUFFER_DESC bufferDesc;
	ZeroMemory( &bufferDesc, sizeof(bufferDesc) );
	bufferDesc.Usage = D3D11_USAGE_DEFAULT;
	bufferDesc.ByteWidth = sizeof( D3D11TriMeshVertex ) * (UINT)m_mesh.vertices().size();
	bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bufferDesc.CPUAccessFlags = 0;

	D3D11_SUBRESOURCE_DATA data;
	ZeroMemory( &data, sizeof(data) );
	data.pSysMem = &m_mesh.vertices()[0];

	D3D_VALIDATE(device.CreateBuffer( &bufferDesc, &data, &m_vertexBuffer ));
}

void ml::D3D11TriMesh::initIB(GraphicsDevice &g)
{
    if (m_mesh.indices().size() == 0) return;
	auto &device = g.castD3D11().device();

	D3D11_BUFFER_DESC bufferDesc;
	ZeroMemory( &bufferDesc, sizeof(bufferDesc) );
	bufferDesc.Usage = D3D11_USAGE_DEFAULT;
	bufferDesc.ByteWidth = sizeof( UINT ) * (UINT)m_mesh.indices().size();
	bufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	bufferDesc.CPUAccessFlags = 0;

	D3D11_SUBRESOURCE_DATA data;
	ZeroMemory( &data, sizeof(data) );
	data.pSysMem = &m_mesh.indices()[0];

	D3D_VALIDATE(device.CreateBuffer( &bufferDesc, &data, &m_indexBuffer ));
}

void ml::D3D11TriMesh::render(GraphicsDevice &g)
{
    if (m_mesh.indices().size() == 0) return;
	auto &context = g.castD3D11().context();

	context.IASetIndexBuffer( m_indexBuffer, DXGI_FORMAT_R32_UINT, 0 );

	UINT stride = sizeof( D3D11TriMeshVertex );
	UINT offset = 0;
	context.IASetVertexBuffers( 0, 1, &m_vertexBuffer, &stride, &offset );

	context.IASetPrimitiveTopology( D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST );

	context.DrawIndexed( (UINT)m_mesh.indices().size(), 0, 0 );
}
