
#pragma warning ( disable : 4005)

#include <d3d11.h>
#include <d3dx11.h>
#include <d3dcompiler.h>

class D3D11GraphicsDevice : public GraphicsDevice
{
public:
	D3D11GraphicsDevice()
	{
		m_type = GraphicsDeviceTypeD3D11;
		m_device = NULL;
		m_context = NULL;
		m_renderView = NULL;
		m_debug = NULL;
		m_swapChain = NULL;
        m_depthBuffer = NULL;
        m_depthState = NULL;
        m_rasterState = NULL;
        m_depthView = NULL;
        m_captureBuffer = NULL;
		m_featureLevel = D3D_FEATURE_LEVEL_9_1;
	}
	~D3D11GraphicsDevice()
	{
		SAFE_RELEASE(m_rasterState);
		SAFE_RELEASE(m_context);
		SAFE_RELEASE(m_renderView);
		SAFE_RELEASE(m_swapChain);
		SAFE_RELEASE(m_device);
        SAFE_RELEASE(m_depthBuffer);
        SAFE_RELEASE(m_depthState);
        SAFE_RELEASE(m_depthView);
        SAFE_RELEASE(m_captureBuffer);

		if(m_debug)
		{
			//m_debug->ReportLiveDeviceObjects(D3D11_RLDO_SUMMARY);
		}
		SAFE_RELEASE(m_debug);
	}
	void init(const WindowWin32 &window);
    void resize(UINT width, UINT height);
	void renderBeginFrame();
	void renderEndFrame();
	void registerAsset(GraphicsAsset *asset);
    void captureBackBuffer(Bitmap &result);

	ID3D11Device& device()
	{
		return *m_device;
	}

	ID3D11DeviceContext& context()
	{
		return *m_context;
	}

private:
	ID3D11Device *m_device;
	ID3D11DeviceContext *m_context;
	ID3D11RenderTargetView *m_renderView;
	ID3D11Debug *m_debug;
	IDXGISwapChain *m_swapChain;
	
	ID3D11RasterizerState *m_rasterState;
	D3D11_RASTERIZER_DESC m_rasterDesc;

    ID3D11Texture2D *m_depthBuffer;
    ID3D11DepthStencilState *m_depthState;
    ID3D11DepthStencilView *m_depthView;

    ID3D11Texture2D *m_captureBuffer;

	D3D_FEATURE_LEVEL m_featureLevel;

	
	std::set<GraphicsAsset*> m_assets;
};



