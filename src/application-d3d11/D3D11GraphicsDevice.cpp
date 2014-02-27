
void D3D11GraphicsDevice::init(const WindowWin32 &window)
{
	UINT width = window.width();
	UINT height = window.height();

	UINT createDeviceFlags = 0;
#ifdef _DEBUG
	createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

	DXGI_SWAP_CHAIN_DESC swapChainDesc;
	ZeroMemory( &swapChainDesc, sizeof( swapChainDesc ) );
	swapChainDesc.BufferCount = 1;
	swapChainDesc.BufferDesc.Width = width;
	swapChainDesc.BufferDesc.Height = height;
	swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	swapChainDesc.BufferDesc.RefreshRate.Numerator = 60;
	swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapChainDesc.OutputWindow = window.handle();
	swapChainDesc.SampleDesc.Count = 1;
	swapChainDesc.SampleDesc.Quality = 0;
	swapChainDesc.Windowed = TRUE;

	D3D_FEATURE_LEVEL featureLevels[] =
	{
		D3D_FEATURE_LEVEL_11_0,
		D3D_FEATURE_LEVEL_10_1,
		D3D_FEATURE_LEVEL_10_0,
	};
	UINT numFeatureLevels = ARRAYSIZE( featureLevels );

	D3D_VALIDATE(D3D11CreateDeviceAndSwapChain( NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, createDeviceFlags, featureLevels, numFeatureLevels,
				 D3D11_SDK_VERSION, &swapChainDesc, &m_swapChain, &m_device, &m_featureLevel, &m_context ));

    //
	// Create a render target view
    //
	ID3D11Texture2D* backBuffer = NULL;
	D3D_VALIDATE(m_swapChain->GetBuffer( 0, __uuidof( ID3D11Texture2D ), ( LPVOID* )&backBuffer ));
	
	D3D_VALIDATE(m_device->CreateRenderTargetView( backBuffer, NULL, &m_renderView ));
	backBuffer->Release();

    //
	// Setup the viewport
    //
	D3D11_VIEWPORT viewport;
	viewport.Width = (FLOAT)width;
	viewport.Height = (FLOAT)height;
	viewport.MinDepth = 0.0f;
	viewport.MaxDepth = 1.0f;
	viewport.TopLeftX = 0;
	viewport.TopLeftY = 0;
	m_context->RSSetViewports( 1, &viewport );

    //
	// Setup the rasterizer state
    //
	m_rasterDesc.AntialiasedLineEnable = false;
	m_rasterDesc.CullMode = D3D11_CULL_NONE;
	m_rasterDesc.DepthBias = 0;
	m_rasterDesc.DepthBiasClamp = 0.0f;
	m_rasterDesc.DepthClipEnable = true;
	m_rasterDesc.FillMode = D3D11_FILL_SOLID;
	m_rasterDesc.FrontCounterClockwise = false;
	m_rasterDesc.MultisampleEnable = false;
	m_rasterDesc.ScissorEnable = false;
	m_rasterDesc.SlopeScaledDepthBias = 0.0f;

	D3D_VALIDATE(m_device->CreateRasterizerState(&m_rasterDesc, &m_rasterState));
	m_context->RSSetState(m_rasterState);

    //
    // Create the depth buffer
    //
    D3D11_TEXTURE2D_DESC depthDesc;
    depthDesc.Width = width;
    depthDesc.Height = height;
    depthDesc.MipLevels = 1;
    depthDesc.ArraySize = 1;
    depthDesc.Format = DXGI_FORMAT_D32_FLOAT;
    depthDesc.SampleDesc.Count = 1;
    depthDesc.SampleDesc.Quality = 0;
    depthDesc.Usage = D3D11_USAGE_DEFAULT;
    depthDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
    depthDesc.CPUAccessFlags = 0;
    depthDesc.MiscFlags = 0;
    D3D_VALIDATE(m_device->CreateTexture2D( &depthDesc, NULL, &m_depthBuffer ));

    //
    // Setup the depth state
    //
    D3D11_DEPTH_STENCIL_DESC depthStateDesc;
    depthStateDesc.DepthEnable = true;
    depthStateDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
    depthStateDesc.DepthFunc = D3D11_COMPARISON_LESS;
    depthStateDesc.StencilEnable = false;
    D3D_VALIDATE(m_device->CreateDepthStencilState(&depthStateDesc, &m_depthState));
    m_context->OMSetDepthStencilState(m_depthState, 1);

    //
    // Setup the depth view
    //
    D3D11_DEPTH_STENCIL_VIEW_DESC depthViewDesc;
    depthViewDesc.Flags = 0;
    depthViewDesc.Format = DXGI_FORMAT_D32_FLOAT;
    depthViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
    depthViewDesc.Texture2D.MipSlice = 0;

    // Create the depth stencil view
    D3D_VALIDATE(m_device->CreateDepthStencilView( m_depthBuffer, &depthViewDesc, &m_depthView ));

    m_context->OMSetRenderTargets( 1, &m_renderView, m_depthView );

#ifdef _DEBUG
	m_device->QueryInterface(__uuidof(ID3D11Debug), reinterpret_cast<void**>(&m_debug));
#endif
}

void D3D11GraphicsDevice::registerAsset(GraphicsAsset *asset)
{
	if(!m_assets.contains(asset)) m_assets.pushBack(asset);
}

void D3D11GraphicsDevice::renderBeginFrame()
{
	float clearColor[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
	m_context->ClearRenderTargetView( m_renderView, clearColor );
    m_context->ClearDepthStencilView( m_depthView, D3D11_CLEAR_DEPTH, 1.0f, 0);
}

void D3D11GraphicsDevice::renderEndFrame()
{
	m_swapChain->Present( 1, 0 );
}
