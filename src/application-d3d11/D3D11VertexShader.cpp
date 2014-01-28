
void D3D11VertexShader::load(D3D11GraphicsDevice &g, const String &filename)
{
	m_filename = filename;
	g.registerAsset(this);
	reset(g);
}

void D3D11VertexShader::reset(GraphicsDevice &g)
{
	auto &device = g.castD3D11().device();
	g.CreateVertexShader();
}
