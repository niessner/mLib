
namespace ml
{

void D3D11RenderTarget::load(GraphicsDevice &g, const UINT width, const UINT height)
{
    m_width = width;
    m_height = height;
    release(g);
    
    g.castD3D11().registerAsset(this);

    reset(g);
}

void D3D11RenderTarget::release(GraphicsDevice &g)
{
    SAFE_RELEASE(m_renderView);
    SAFE_RELEASE(m_depthView);
    SAFE_RELEASE(m_texture);
    SAFE_RELEASE(m_depthBuffer);
}

void D3D11RenderTarget::reset(GraphicsDevice &g)
{
    release(g);

    if (m_width == 0 || m_height == 0)
        return;

    auto &device = g.castD3D11().device();
    auto &context = g.castD3D11().context();

    //
    // Create the render target
    //
    D3D11_TEXTURE2D_DESC renderDesc;
    renderDesc.Width = m_width;
    renderDesc.Height = m_height;
    renderDesc.MipLevels = 0;
    renderDesc.ArraySize = 1;
    renderDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    renderDesc.SampleDesc.Count = 1;
    renderDesc.SampleDesc.Quality = 0;
    renderDesc.Usage = D3D11_USAGE_DEFAULT;
    renderDesc.BindFlags = D3D11_BIND_RENDER_TARGET;
    renderDesc.CPUAccessFlags = 0;
    renderDesc.MiscFlags = 0;

    D3D_VALIDATE(device.CreateTexture2D(&renderDesc, nullptr, &m_texture));
    
    //
    // Create the render target view
    //
    D3D_VALIDATE(device.CreateRenderTargetView(m_texture, nullptr, &m_renderView));
    
    //
    // Create the depth buffer
    //
    D3D11_TEXTURE2D_DESC depthDesc;
    depthDesc.Width = m_width;
    depthDesc.Height = m_height;
    depthDesc.MipLevels = 1;
    depthDesc.ArraySize = 1;
    depthDesc.Format = DXGI_FORMAT_D32_FLOAT;
    depthDesc.SampleDesc.Count = 1;
    depthDesc.SampleDesc.Quality = 0;
    depthDesc.Usage = D3D11_USAGE_DEFAULT;
    depthDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
    depthDesc.CPUAccessFlags = 0;
    depthDesc.MiscFlags = 0;
    D3D_VALIDATE(device.CreateTexture2D(&depthDesc, nullptr, &m_depthBuffer));

    //
    // Create the depth view
    //
    D3D11_DEPTH_STENCIL_VIEW_DESC depthViewDesc;
    depthViewDesc.Flags = 0;
    depthViewDesc.Format = DXGI_FORMAT_D32_FLOAT;
    depthViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
    depthViewDesc.Texture2D.MipSlice = 0;

}

void D3D11RenderTarget::bind(GraphicsDevice &g)
{
    if (m_texture == nullptr)
        return;

    auto &context = g.castD3D11().context();
    context.OMSetRenderTargets(1, &m_renderView, m_depthView);
}

void D3D11RenderTarget::clear(GraphicsDevice &g, const ml::vec4f &clearColor)
{
    auto &context = g.castD3D11().context();
    context.ClearRenderTargetView(m_renderView, clearColor.array);
    context.ClearDepthStencilView(m_depthView, D3D11_CLEAR_DEPTH, 1.0f, 0);
}

}