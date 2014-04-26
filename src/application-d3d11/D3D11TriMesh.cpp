
const D3D11_INPUT_ELEMENT_DESC ml::D3D11TriMesh::layout[layoutElementCount] =
{
	{ "position", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "normal", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "attributeA", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "attributeB", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
};

////TODO load from arbitrary meshes ;)
//void ml::D3D11TriMesh::load(GraphicsDevice &g, const TriMeshOld& mesh)
//{
//	//assert(false);	//TODO MATTHIAS
//	MLIB_WARNING("Function not supported anymore...");
//	g.castD3D11().registerAsset(this);
//	m_Vertices.resize(mesh.vertices().size());
//	for (size_t i = 0; i < m_Vertices.size(); i++) {
//		m_Vertices[i].position = mesh.vertices()[i].position;
//		m_Vertices[i].normal = mesh.vertices()[i].normal;
//		m_Vertices[i].attributeA = mesh.vertices()[i].attributeA;
//		m_Vertices[i].attributeB = mesh.vertices()[i].attributeB;
//	}
//	m_Indices.resize(mesh.indices().size());
//	for (size_t i = 0; i < mesh.indices().size(); i++) {
//		m_Indices[i] = mesh.indices()[i];
//	}
//	reset(g);
//}
//
//
//void ml::D3D11TriMesh::load(GraphicsDevice &g, TriMeshOld&& mesh)
//{
//	//assert(false);	//TODO MATTHIAS
//	MLIB_WARNING("Function not supported anymore...");
//	g.castD3D11().registerAsset(this);
//	m_Vertices.resize(mesh.vertices().size());
//	for (size_t i = 0; i < m_Vertices.size(); i++) {
//		m_Vertices[i].position = mesh.vertices()[i].position;
//		m_Vertices[i].normal = mesh.vertices()[i].normal;
//		m_Vertices[i].attributeA = mesh.vertices()[i].attributeA;
//		m_Vertices[i].attributeB = mesh.vertices()[i].attributeB;
//	}
//	m_Indices.resize(mesh.indices().size());
//	for (size_t i = 0; i < mesh.indices().size(); i++) {
//		m_Indices[i] = mesh.indices()[i];
//	}
//	reset(g);
//}

void ml::D3D11TriMesh::updateAttributeA(GraphicsDevice &g, const std::vector<vec4f> &newValues)
{
	if (newValues.size() != m_Vertices.size())	throw MLIB_EXCEPTION("vertex buffer size doesn't match");

    for (size_t i = 0; i < newValues.size(); i++) {
		m_Vertices[i].attributeA = newValues[i];
    }
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
    if (m_Vertices.size() == 0) return;
	auto &device = g.castD3D11().device();

	D3D11_BUFFER_DESC bufferDesc;
	ZeroMemory( &bufferDesc, sizeof(bufferDesc) );
	bufferDesc.Usage = D3D11_USAGE_DEFAULT;
	bufferDesc.ByteWidth = sizeof( D3D11TriMeshVertex ) * (UINT)m_Vertices.size();
	bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bufferDesc.CPUAccessFlags = 0;

	D3D11_SUBRESOURCE_DATA data;
	ZeroMemory( &data, sizeof(data) );
	data.pSysMem = &m_Vertices[0];

	D3D_VALIDATE(device.CreateBuffer( &bufferDesc, &data, &m_vertexBuffer ));
}

void ml::D3D11TriMesh::initIB(GraphicsDevice &g)
{
    if (m_Indices.size() == 0) return;
	auto &device = g.castD3D11().device();

	D3D11_BUFFER_DESC bufferDesc;
	ZeroMemory( &bufferDesc, sizeof(bufferDesc) );
	bufferDesc.Usage = D3D11_USAGE_DEFAULT;
	bufferDesc.ByteWidth = sizeof( UINT ) * (UINT)m_Indices.size();
	bufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	bufferDesc.CPUAccessFlags = 0;

	D3D11_SUBRESOURCE_DATA data;
	ZeroMemory( &data, sizeof(data) );
	data.pSysMem = &m_Indices[0];

	D3D_VALIDATE(device.CreateBuffer( &bufferDesc, &data, &m_indexBuffer ));
}

void ml::D3D11TriMesh::render(GraphicsDevice &g) const
{
    if (m_Indices.size() == 0) return;
	auto &context = g.castD3D11().context();

	context.IASetIndexBuffer( m_indexBuffer, DXGI_FORMAT_R32_UINT, 0 );

	UINT stride = sizeof( D3D11TriMeshVertex );
	UINT offset = 0;
	context.IASetVertexBuffers( 0, 1, &m_vertexBuffer, &stride, &offset );

	context.IASetPrimitiveTopology( D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST );

	context.DrawIndexed( (UINT)m_Indices.size(), 0, 0 );
}
