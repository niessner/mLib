
namespace ml
{

void D3D11RenderTarget::load(GraphicsDevice &g, const UINT width, const UINT height)
{
    m_graphics = &g.castD3D11();
    m_width = width;
    m_height = height;
    
    g.castD3D11().registerAsset(this);

    reset();
}

void D3D11RenderTarget::release()
{
	SAFE_RELEASE(m_renderView);
	SAFE_RELEASE(m_depthView);
	SAFE_RELEASE(m_texture);
	SAFE_RELEASE(m_captureTexture);
	SAFE_RELEASE(m_depthBuffer);
	SAFE_RELEASE(m_captureDepth);
}

void D3D11RenderTarget::reset()
{
    release();

    if (m_width == 0 || m_height == 0)
        return;

    auto &device = m_graphics->getDevice();
	auto &context = m_graphics->getContext();

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
    D3D_VALIDATE(device.CreateDepthStencilView(m_depthBuffer, nullptr, &m_depthView));

    //
    // Create the color capture buffer
    //
    renderDesc.BindFlags = 0;
    renderDesc.CPUAccessFlags = D3D11_CPU_ACCESS_READ | D3D11_CPU_ACCESS_WRITE;
    renderDesc.Usage = D3D11_USAGE_STAGING;
    D3D_VALIDATE(device.CreateTexture2D(&renderDesc, nullptr, &m_captureTexture));

    //
    // Create the depth capture buffer
    //
    depthDesc.BindFlags = 0;
    depthDesc.CPUAccessFlags = D3D11_CPU_ACCESS_READ | D3D11_CPU_ACCESS_WRITE;
    depthDesc.Usage = D3D11_USAGE_STAGING;
    D3D_VALIDATE(device.CreateTexture2D(&depthDesc, nullptr, &m_captureDepth));
}

void D3D11RenderTarget::bind()
{
    if (m_texture == nullptr)
        return;

	auto &context = m_graphics->getContext();
    context.OMSetRenderTargets(1, &m_renderView, m_depthView);

    D3D11_VIEWPORT viewport;
    viewport.Width = (FLOAT)m_width;
    viewport.Height = (FLOAT)m_height;
    viewport.MinDepth = 0.0f;
    viewport.MaxDepth = 1.0f;
    viewport.TopLeftX = 0;
    viewport.TopLeftY = 0;
    context.RSSetViewports(1, &viewport);
}

void D3D11RenderTarget::clear(const ml::vec4f &clearColor)
{
	auto &context = m_graphics->getContext();
    context.ClearRenderTargetView(m_renderView, clearColor.array);
    context.ClearDepthStencilView(m_depthView, D3D11_CLEAR_DEPTH, 1.0f, 0);
}

void D3D11RenderTarget::clearColorBuffer(const ml::vec4f &clearColor)
{
	auto &context = m_graphics->getContext();
    context.ClearRenderTargetView(m_renderView, clearColor.array);
}

void D3D11RenderTarget::captureDepthBuffer(ColorImageR32 &result, const mat4f &perspectiveTransform)
{
    captureDepthBuffer(result);

    auto inv = perspectiveTransform.getInverse();

    for (UINT y = 0; y < result.getHeight(); y++)
    {
        for (UINT x = 0; x < result.getWidth(); x++)
        {
            float &v = result(x, y);
            if (v >= 1.0f)
                v = std::numeric_limits<float>::infinity();
            else
            {
                float dx = math::linearMap(0.0f, result.getWidth() - 1.0f, -1.0f, 1.0f, (float)x);
                float dy = math::linearMap(0.0f, result.getHeight() - 1.0f, -1.0f, 1.0f, (float)y);
                v = (inv * vec3f(dx, dy, v)).length();
            }
        }
    }
}

void D3D11RenderTarget::captureDepthBuffer(ColorImageR32 &result)
{
	auto &context = m_graphics->getContext();
    context.CopyResource(m_captureDepth, m_depthBuffer);

	result.allocate(m_width, m_height);

    D3D11_MAPPED_SUBRESOURCE resource;
    UINT subresource = D3D11CalcSubresource(0, 0, 0);
    HRESULT hr = context.Map(m_captureDepth, subresource, D3D11_MAP_READ, 0, &resource);
    const BYTE *data = (BYTE *)resource.pData;

    for (UINT y = 0; y < m_height; y++)
    {
        memcpy(&result(0U, y), data + resource.RowPitch * y, m_width * sizeof(float));
    }

    context.Unmap(m_captureDepth, subresource);
}

void D3D11RenderTarget::captureColorBuffer(ColorImageR8G8B8A8 &result)
{
	auto &context = m_graphics->getContext();
    context.CopyResource(m_captureTexture, m_texture);

	result.allocate(m_width, m_height);

    D3D11_MAPPED_SUBRESOURCE resource;
    UINT subresource = D3D11CalcSubresource(0, 0, 0);
    HRESULT hr = context.Map(m_captureTexture, subresource, D3D11_MAP_READ, 0, &resource);
    const BYTE *data = (BYTE *)resource.pData;

    for (UINT y = 0; y < m_height; y++)
    {
        memcpy(&result(0U, y), data + resource.RowPitch * y, m_width * sizeof(ml::vec4uc));
    }

    context.Unmap(m_captureTexture, subresource);
}

}