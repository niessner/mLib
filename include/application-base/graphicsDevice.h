
enum GraphicsDeviceType
{
	GraphicsDeviceTypeD3D11,
};

class GraphicsDevice
{
public:
	virtual ~GraphicsDevice() {}
	virtual void init(const WindowWin32 &window) = 0;
    virtual void resize(UINT width, UINT height) = 0;
	virtual void renderBeginFrame() = 0;
	virtual void renderEndFrame() = 0;
    virtual void captureBackBuffer(Bitmap &result) = 0;

	GraphicsDeviceType type() const
	{
		return m_type;
	}
	D3D11GraphicsDevice& castD3D11() const
	{
		return *((D3D11GraphicsDevice*)this);
	}

protected:
	GraphicsDeviceType m_type;
};
