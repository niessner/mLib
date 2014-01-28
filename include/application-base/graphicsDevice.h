
enum GraphicsDeviceType
{
	GraphicsDeviceTypeD3D11,
};

class GraphicsDevice
{
public:
	virtual void init(const WindowWin32 &window) = 0;
	virtual void renderBeginFrame() = 0;
	virtual void renderEndFrame() = 0;
};
