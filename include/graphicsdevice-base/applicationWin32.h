
struct ApplicationData
{
	WindowWin32 window;
	std::unique_ptr<GraphicsDevice> device;
};

class ApplicationWin32
{
public:
	ApplicationWin32(HINSTANCE instance, UINT windowWidth, UINT windowHeight, const String &name);

	void messageLoop();

	inline ApplicationData& data()
	{
		return m_data;
	}

private:
	ApplicationData m_data;
	HINSTANCE m_instance;
};
