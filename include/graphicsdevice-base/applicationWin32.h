
struct ApplicationData
{
	ApplicationData(WindowWin32 *window_ptr, GraphicsDevice *device_ptr) :
		window(*window_ptr),
		device(*device_ptr)
	{

	}
	WindowWin32 &window;
	GraphicsDevice &device;
};

class ApplicationWin32
{
public:
	ApplicationWin32(HINSTANCE instance, UINT windowWidth, UINT windowHeight, const String &name, GraphicsDeviceType graphicsType);
	~ApplicationWin32();

	void messageLoop();

	inline ApplicationData& data()
	{
		return *m_data;
	}

private:
	//
	// m_data is just a view to encapsulate all externally-visible application
	// components. THe actual data is owned by m_window, m_device, etc.
	//
	ApplicationData *m_data;
	
	WindowWin32 m_window;
	GraphicsDevice *m_device;

	HINSTANCE m_instance;
};
