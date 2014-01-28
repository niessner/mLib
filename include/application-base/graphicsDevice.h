
enum GraphicsDeviceType
{
	GraphicsDeviceTypeD3D11,
};

class GraphicsDevice
{
public:
	virtual void init(const WindowWin32 &window) = 0;
};
