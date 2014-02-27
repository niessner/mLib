
enum MouseButtonState
{
	MouseButtonNone,
	MouseButtonLeft,
	MouseButtonRight,
};

struct InputState
{
public:
	InputState()
	{
		for(bool &b : keys) b = false;
		mouseX = mouseY = prevMouseX = prevMouseY = 0;
		mouseState = prevMouseState = MouseButtonNone;
	}

	static const UINT keyCount = 256;
	bool keys[keyCount];
	
	int mouseX, mouseY;
	MouseButtonState mouseState;

	int prevMouseX, prevMouseY;
	MouseButtonState prevMouseState;
};

struct ApplicationData
{
	ApplicationData(WindowWin32 *window_ptr, GraphicsDevice *graphics_ptr, InputState *input_ptr) :
		window(*window_ptr),
		graphics(*graphics_ptr),
		input(*input_ptr)
	{

	}
	WindowWin32 &window;
	GraphicsDevice &graphics;
	InputState &input;
};

class ApplicationCallback
{
public:
	virtual void init(ApplicationData &app) = 0;
	virtual void render(ApplicationData &app) = 0;
	virtual void keyDown(ApplicationData &app, UINT key) = 0;
	virtual void keyPressed(ApplicationData &app, UINT key) = 0;
	virtual void mouse(ApplicationData &app, int x, int y, int prevX, int prevY) = 0;
};

class ApplicationWin32
{
public:
	ApplicationWin32(HINSTANCE instance, UINT windowWidth, UINT windowHeight, const std::string &name, GraphicsDeviceType graphicsType, ApplicationCallback &callback);
	~ApplicationWin32();

	void messageLoop();

	inline ApplicationData& data()
	{
		return *m_data;
	}
	inline ApplicationCallback& callback()
	{
		return m_callback;
	}

private:
	//
	// m_data is just a view to encapsulate all externally-visible application
	// components. THe actual data is owned by m_window, m_device, etc.
	//
	ApplicationData *m_data;
	
	WindowWin32 m_window;
	InputState m_input;
	GraphicsDevice *m_graphics;

	HINSTANCE m_instance;

	ApplicationCallback &m_callback;
};
