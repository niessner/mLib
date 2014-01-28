
void D3D11VertexShader::load(D3D11GraphicsDevice &g, const String &filename)
{
	m_filename = filename;
	g.registerAsset(this);
	reset(g);
}

void D3D11VertexShader::release(GraphicsDevice &g)
{
	SAFE_RELEASE(m_shader);
}

void D3D11VertexShader::reset(GraphicsDevice &g)
{
	release(g);

	auto &device = g.castD3D11().device();
	
	ID3DBlob *blob = D3D11Utility::CompileShader(m_filename, "vertexShaderMain", "vs_4_0");
	MLIB_ASSERT(blob != NULL, "CompileShader failed");

	D3D_VALIDATE(device.CreateVertexShader(blob->GetBufferPointer(), blob->GetBufferSize(), NULL, &m_shader));
	blob->Release();
}
