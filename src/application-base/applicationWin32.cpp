
ApplicationWin32::ApplicationWin32(HINSTANCE instance, UINT windowWidth, UINT windowHeight, const String &name, GraphicsDeviceType graphicsType, ApplicationCallback &callback) :
	m_callback(callback)
{
	m_window.init(instance, windowWidth, windowHeight, name);

	switch(graphicsType)
	{
	case GraphicsDeviceTypeD3D11:
		m_device = new GraphicsDeviceD3D11();
		break;
	default:
		MLIB_ERROR("invalid graphics device type");
	}
	m_device->init(m_window);

	m_data = new ApplicationData(&m_window, m_device);

	m_callback.init(*m_data);
}

ApplicationWin32::~ApplicationWin32()
{
	delete m_data;
}

void ApplicationWin32::messageLoop()
{
	bool messageReceived;
	MSG msg;
	msg.message = WM_NULL;
	PeekMessage( &msg, NULL, 0U, 0U, PM_NOREMOVE );

	while( WM_QUIT != msg.message )
	{
		// Use PeekMessage() so we can use idle time to render the scene. 
		messageReceived = ( PeekMessage( &msg, NULL, 0U, 0U, PM_REMOVE ) != 0 );

		if( messageReceived )
		{
			TranslateMessage( &msg );
			DispatchMessage( &msg );
		}
		else
		{
			m_device->renderBeginFrame();
			m_callback.render(*m_data);
			m_device->renderEndFrame();
		}
	}
}
