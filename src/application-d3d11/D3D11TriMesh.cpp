
const D3D11_INPUT_ELEMENT_DESC D3D11TriMesh::layout[layoutElementCount] =
{
	{ "position", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "normal", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "attributeA", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "attributeB", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
};

//TODO load from arbitrary meshes ;)
void D3D11TriMesh::load(GraphicsDevice &g, const TriMesh& mesh)
{
	g.castD3D11().registerAsset(this);
	m_MeshVertices.clear();	m_MeshVertices.resize(mesh.vertices().size());
	for (unsigned int i = 0; i < mesh.vertices().size(); i++) {
		m_MeshVertices[i].position = mesh.vertices()[i].position;
		m_MeshVertices[i].normal = mesh.vertices()[i].normal;
		m_MeshVertices[i].attributeA = mesh.vertices()[i].attributeA;
		m_MeshVertices[i].attributeB = mesh.vertices()[i].attributeB;
	}
	m_MeshIndices = mesh.indices();
	reset(g);
}

//void D3D11TriMesh::load(GraphicsDevice &g, TriMesh&& mesh)
//{
//	g.castD3D11().registerAsset(this);
//	//TODO(ms): Debug into here to check move semantics are working
//	m_mesh = mesh;
//	reset(g);
//}

void D3D11TriMesh::release(GraphicsDevice &g)
{
	SAFE_RELEASE(m_vertexBuffer);
	SAFE_RELEASE(m_indexBuffer);
}

void D3D11TriMesh::reset(GraphicsDevice &g)
{
	release(g);
	initVB(g);
	initIB(g);
}

void D3D11TriMesh::initVB(GraphicsDevice &g)
{
	auto &device = g.castD3D11().device();

	D3D11_BUFFER_DESC bufferDesc;
	ZeroMemory( &bufferDesc, sizeof(bufferDesc) );
	bufferDesc.Usage = D3D11_USAGE_DEFAULT;
	bufferDesc.ByteWidth = sizeof( D3D11TriMeshVertex ) * (UINT)m_MeshVertices.size();
	bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bufferDesc.CPUAccessFlags = 0;

	D3D11_SUBRESOURCE_DATA data;
	ZeroMemory( &data, sizeof(data) );
	data.pSysMem = m_MeshVertices.ptr();


	const bool randomizeColor = true;
	if(randomizeColor)
	{
		
	}

	D3D_VALIDATE(device.CreateBuffer( &bufferDesc, &data, &m_vertexBuffer ));
}

void D3D11TriMesh::initIB(GraphicsDevice &g)
{
	auto &device = g.castD3D11().device();

	D3D11_BUFFER_DESC bufferDesc;
	ZeroMemory( &bufferDesc, sizeof(bufferDesc) );
	bufferDesc.Usage = D3D11_USAGE_DEFAULT;
	bufferDesc.ByteWidth = sizeof( UINT ) * (UINT)m_MeshIndices.size();
	bufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	bufferDesc.CPUAccessFlags = 0;

	D3D11_SUBRESOURCE_DATA data;
	ZeroMemory( &data, sizeof(data) );
	data.pSysMem = m_MeshIndices.ptr();

	D3D_VALIDATE(device.CreateBuffer( &bufferDesc, &data, &m_indexBuffer ));
}

void D3D11TriMesh::render(GraphicsDevice &g)
{
	auto &context = g.castD3D11().context();

	context.IASetIndexBuffer( m_indexBuffer, DXGI_FORMAT_R32_UINT, 0 );

	UINT stride = sizeof( D3D11TriMeshVertex );
	UINT offset = 0;
	context.IASetVertexBuffers( 0, 1, &m_vertexBuffer, &stride, &offset );

	context.IASetPrimitiveTopology( D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST );

	context.DrawIndexed( (UINT)m_MeshIndices.size(), 0, 0 );
}
