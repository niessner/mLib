
#include <d3d11.h>
#include <d3dcompiler.h>

class GraphicsDeviceD3D11 : public GraphicsDevice
{
public:
	GraphicsDeviceD3D11()
	{
		m_device = NULL;
		m_context = NULL;
		m_view = NULL;
		m_swapChain = NULL;
		m_featureLevel = D3D_FEATURE_LEVEL_9_1;
	}
	void init(const WindowWin32 &window);
	void renderBeginFrame();
	void renderEndFrame();

private:
	ID3D11Device *m_device;
	ID3D11DeviceContext *m_context;
	ID3D11RenderTargetView *m_view;
	IDXGISwapChain *m_swapChain;
	D3D_FEATURE_LEVEL m_featureLevel;
};

