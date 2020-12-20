
#include <application-base/applicationWin32.h>
#include <application-d3d11/D3D11GraphicsDevice.h>

ml::ApplicationWin32::ApplicationWin32(HINSTANCE instance, UINT windowWidth, UINT windowHeight, const std::string &name, GraphicsDeviceType graphicsType, ApplicationCallback &callback, unsigned int initWindowPosX, unsigned int initWindowPosY) :
	m_callback(callback),
	m_window(*this)
{
	m_initialized = false;
	m_window.init(instance, windowWidth, windowHeight, name, m_callback.msgProcCallback, initWindowPosX, initWindowPosY);

	switch(graphicsType)
	{
	case GraphicsDeviceTypeD3D11:
		m_graphics = new D3D11GraphicsDevice();
		break;
	default:
		MLIB_ERROR("invalid graphics device type");
	}
	m_graphics->init(m_window);

	m_data = new ApplicationData(&m_window, m_graphics, &m_input);

	m_callback.init(*m_data);
}

ml::ApplicationWin32::~ApplicationWin32()
{
	delete m_graphics;
	delete m_data;

	m_bResizeEvent = false;
}

void ml::ApplicationWin32::messageLoop(bool vsync /*= false*/)
{
	bool messageReceived;
	MSG msg;
	msg.message = WM_NULL;
	PeekMessage( &msg, nullptr, 0U, 0U, PM_NOREMOVE );

	m_initialized = true;

	while( WM_QUIT != msg.message )
	{
		// Use PeekMessage() so we can use idle time to render the scene. 
		messageReceived = ( PeekMessage( &msg, nullptr, 0U, 0U, PM_REMOVE ) != 0 );

		if( messageReceived )
		{
			TranslateMessage( &msg );
			DispatchMessage( &msg );
		}
		else
		{
			if (m_bResizeEvent) {
				m_bResizeEvent = false;	//not 100% sure if that doesn't lead to a raise condition and may cause resize misses...
				data().graphics.resize(data().window);
				callback().resize(data());
			}


			for(int i = 0; i < m_input.keyCount; i++)
				if(m_input.keys[i]) m_callback.keyPressed(*m_data, i);

			m_graphics->renderBeginFrame();
			m_callback.render(*m_data);
			m_graphics->renderEndFrame(vsync);
		}
	}
}
