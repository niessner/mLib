
namespace ml
{

void D3D11Texture3D::load(GraphicsDevice &g, const Grid3<RGBColor> &data)
{
    m_graphics = &g.castD3D11();
    release();
    
    g.castD3D11().registerAsset(this);
    m_data = data;

    reset();
}

void D3D11Texture3D::release()
{
    SAFE_RELEASE(m_texture);
    SAFE_RELEASE(m_view);
}

void D3D11Texture3D::reset()
{
    release();

    if (m_data.getDimX() == 0)
        return;

    auto &device = m_graphics->getDevice();
	auto &context = m_graphics->getContext();

    D3D11_TEXTURE3D_DESC desc;
    desc.Width = (UINT)m_data.getDimX();
	desc.Height = (UINT)m_data.getDimY();
	desc.Depth = (UINT)m_data.getDimZ();
    desc.MipLevels = 0;
    desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    desc.Usage = D3D11_USAGE_DEFAULT;
    desc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET;
    desc.CPUAccessFlags = 0;
    desc.MiscFlags = D3D11_RESOURCE_MISC_GENERATE_MIPS;

    D3D_VALIDATE(device.CreateTexture3D(&desc, nullptr, &m_texture));
    D3D_VALIDATE(device.CreateShaderResourceView(m_texture, nullptr, &m_view));

	context.UpdateSubresource(m_texture, 0, nullptr, m_data.ptr(), (UINT)m_data.getDimX() * sizeof(RGBColor), (UINT)m_data.getDimX() * (UINT)m_data.getDimY() * sizeof(RGBColor));

    context.GenerateMips(m_view);
}

void D3D11Texture3D::bind() const
{
    if (m_view == nullptr)
        return;
	m_graphics->getContext().PSSetShaderResources(0, 1, &m_view);
}

}