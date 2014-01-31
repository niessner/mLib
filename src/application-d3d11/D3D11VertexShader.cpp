
void D3D11VertexShader::load(GraphicsDevice &g, const String &filename)
{
	release(g);
	SAFE_RELEASE(m_blob);

	m_filename = filename;
	m_hash64 = m_filename.hash64();
	g.castD3D11().registerAsset(this);

	m_blob = D3D11Utility::CompileShader(m_filename, "vertexShaderMain", "vs_4_0");
	MLIB_ASSERT(m_blob != NULL, "CompileShader failed");

	reset(g);
}

void D3D11VertexShader::release(GraphicsDevice &g)
{
	SAFE_RELEASE(m_shader);
	SAFE_RELEASE(m_standardLayout);
}

void D3D11VertexShader::reset(GraphicsDevice &g)
{
	release(g);

	auto &device = g.castD3D11().device();
	
	D3D_VALIDATE(device.CreateVertexShader(m_blob->GetBufferPointer(), m_blob->GetBufferSize(), NULL, &m_shader));

	device.CreateInputLayout( MeshVertex::layout, MeshVertex::layoutElementCount, m_blob->GetBufferPointer(), m_blob->GetBufferSize(), &m_standardLayout );
}

void D3D11VertexShader::bind(GraphicsDevice &g)
{
	auto &context = g.castD3D11().context();
	context.VSSetShader( m_shader, NULL, 0 );
	context.IASetInputLayout( m_standardLayout );
}