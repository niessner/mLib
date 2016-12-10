
namespace ml
{

void D3D11Texture2D::init(GraphicsDevice &g, const ColorImageR8G8B8A8 &image)
{
    m_graphics = &g.castD3D11();
    releaseGPU();
    
    //g.castD3D11().registerAsset(this);
    m_image = image;

    createGPU();
}

void D3D11Texture2D::releaseGPU()
{
    SAFE_RELEASE(m_texture);
    SAFE_RELEASE(m_srv);
}

void D3D11Texture2D::createGPU()
{
    releaseGPU();

	if (m_image.getWidth() == 0 || m_image.getHeight() == 0)
        return;

    auto &device = m_graphics->getDevice();
	auto &context = m_graphics->getContext();

    D3D11_TEXTURE2D_DESC desc;
    desc.Width = (UINT)m_image.getWidth();
    desc.Height = (UINT)m_image.getHeight();
    desc.MipLevels = 0;
    desc.ArraySize = 1;
    desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    desc.SampleDesc.Count = 1;
    desc.SampleDesc.Quality = 0;
    desc.Usage = D3D11_USAGE_DEFAULT;
    desc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET;
    desc.CPUAccessFlags = 0;
    desc.MiscFlags = D3D11_RESOURCE_MISC_GENERATE_MIPS;

    D3D_VALIDATE(device.CreateTexture2D(&desc, nullptr, &m_texture));
    D3D_VALIDATE(device.CreateShaderResourceView(m_texture, nullptr, &m_srv));

    context.UpdateSubresource(m_texture, 0, nullptr, m_image.getData(), (UINT)m_image.getWidth() * sizeof(vec4uc), (UINT)m_image.getWidth() * (UINT)m_image.getHeight() * sizeof(vec4uc));

    context.GenerateMips(m_srv);
}

void D3D11Texture2D::bind(unsigned int slot /* = 0 */) const
{
    if (m_srv == nullptr) return;
	m_graphics->getContext().PSSetShaderResources(slot, 1, &m_srv);
}


void D3D11Texture2D::unbind(unsigned int slot /*= 0*/) const
{
	ID3D11ShaderResourceView* srvNULL = nullptr;
	m_graphics->getContext().PSSetShaderResources(slot, 1, &srvNULL);
}


}