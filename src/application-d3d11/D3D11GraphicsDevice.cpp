
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

	// Create a render target view
	ID3D11Texture2D* backBuffer = NULL;
	D3D_VALIDATE(m_swapChain->GetBuffer( 0, __uuidof( ID3D11Texture2D ), ( LPVOID* )&backBuffer ));
	
	D3D_VALIDATE(m_device->CreateRenderTargetView( backBuffer, NULL, &m_view ));
	backBuffer->Release();

	m_context->OMSetRenderTargets( 1, &m_view, NULL );

	// Setup the viewport
	D3D11_VIEWPORT viewport;
	viewport.Width = (FLOAT)width;
	viewport.Height = (FLOAT)height;
	viewport.MinDepth = 0.0f;
	viewport.MaxDepth = 1.0f;
	viewport.TopLeftX = 0;
	viewport.TopLeftY = 0;
	m_context->RSSetViewports( 1, &viewport );

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
	m_context->ClearRenderTargetView( m_view, clearColor );
}

void D3D11GraphicsDevice::renderEndFrame()
{
	m_swapChain->Present( 0, 0 );
}
