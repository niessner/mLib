
ApplicationWin32::ApplicationWin32(HINSTANCE instance, UINT windowWidth, UINT windowHeight, const String &name)
{
	m_data.window.init(instance, windowWidth, windowHeight, name);
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
			//DXUTRender3DEnvironment();
		}
	}
}
