
#ifndef APPLICATION_D3D11_D3D11GRAPHICSDEVICE_H_
#define APPLICATION_D3D11_D3D11GRAPHICSDEVICE_H_

namespace ml {

class D3D11GraphicsDevice : public GraphicsDevice
{
public:
    D3D11GraphicsDevice()
    {
        m_type = GraphicsDeviceTypeD3D11;
        m_device = nullptr;
        m_context = nullptr;
        m_renderTargetView = nullptr;
        m_debug = nullptr;
        m_swapChain = nullptr;
        m_depthBuffer = nullptr;
        m_depthState = nullptr;
        m_rasterState = nullptr;
        m_depthStencilView = nullptr;
        m_captureBuffer = nullptr;
        m_samplerState = nullptr;
		m_featureLevel = D3D_FEATURE_LEVEL_9_1;


		ZeroMemory(&m_swapChainDesc, sizeof(m_swapChainDesc));
		m_swapChainDesc.BufferCount = 1;
		m_swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		m_swapChainDesc.BufferDesc.RefreshRate.Numerator = 60;
		m_swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
		m_swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;		
		m_swapChainDesc.SampleDesc.Count = 1;
		m_swapChainDesc.SampleDesc.Quality = 0;
		m_swapChainDesc.Windowed = TRUE;
		//set in init
		//m_swapChainDesc.OutputWindow = window.handle();
		//m_swapChainDesc.BufferDesc.Width = m_width;
		//m_swapChainDesc.BufferDesc.Height = m_height;

    }

    ~D3D11GraphicsDevice()
    {
        SAFE_RELEASE(m_rasterState);
        SAFE_RELEASE(m_context);
        SAFE_RELEASE(m_renderTargetView);
        SAFE_RELEASE(m_swapChain);
        SAFE_RELEASE(m_device);
        SAFE_RELEASE(m_depthBuffer);
        SAFE_RELEASE(m_depthState);
        SAFE_RELEASE(m_depthStencilView);
        SAFE_RELEASE(m_captureBuffer);
        SAFE_RELEASE(m_samplerState);

        if (m_debug)
        {
            //m_debug->ReportLiveDeviceObjects(D3D11_RLDO_SUMMARY);
        }
        SAFE_RELEASE(m_debug);
    }

    void init(const WindowWin32 &window);
	void resize(const WindowWin32 &window);
    void renderBeginFrame();
    void renderEndFrame();
    void registerAsset(GraphicsAsset *asset);

    void setCullMode(D3D11_CULL_MODE mode);
    void toggleCullMode();
    void toggleWireframe();
    void clear(const ml::vec4f &clearColor);
    void bindRenderDepth();

    D3D11ShaderManager& getShaderManager()
	{
        return m_shaderManager;
    }

    ID3D11Device& getDevice()
    {
        return *m_device;
    }

    ID3D11DeviceContext& getContext()
    {
        return *m_context;
    }

	UINT getWidth() const {
		return m_width;
	}

	UINT getHeight() const {
		return m_height;
	}

	//! maps from integer pixel coordinates to NDC space [-1;1]^2
	vec2f pixelToNDC(const vec2i& p) const {
		return vec2f(
			2.0f*(float)p.x / ((float)getWidth() - 1.0f) - 1.0f,
			1.0f - 2.0f*(float)p.y / ((float)getHeight() - 1.0f));
	}

private:

	//! creates depth buffer, depth stencil view, rendertarget view, and sets the view port to the current width/height (called by init and resize)
	void createViews();

    void registerDefaultShaders();

    UINT m_width, m_height;
    ID3D11Device *m_device;
    ID3D11DeviceContext *m_context;
    ID3D11RenderTargetView *m_renderTargetView;
    ID3D11Debug *m_debug;

    IDXGISwapChain *m_swapChain;
	DXGI_SWAP_CHAIN_DESC m_swapChainDesc;	//settings


    ID3D11RasterizerState *m_rasterState;
    D3D11_RASTERIZER_DESC m_rasterDesc;

    ID3D11Texture2D *m_depthBuffer;
    ID3D11DepthStencilState *m_depthState;
    ID3D11DepthStencilView *m_depthStencilView;

    ID3D11SamplerState *m_samplerState;

    ID3D11Texture2D *m_captureBuffer;

    std::set<GraphicsAsset*> m_assets;

    D3D11ShaderManager m_shaderManager;

	D3D_FEATURE_LEVEL m_featureLevel;
	

protected:
	void captureBackBufferInternal(ColorImageR8G8B8A8 &result);
};

}  // namespace ml

#endif  // APPLICATION_D3D11_D3D11GRAPHICSDEVICE_H_