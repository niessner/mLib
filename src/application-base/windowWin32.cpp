
//
// In a multi-window world, s_windowMap would be needed.  In a single-window world, s_mainWindow is sufficient.
//
//std::map<HWND, WindowWin32*> s_windowMap;
WindowWin32* s_mainWindow = NULL;

LRESULT WINAPI WindowCallback( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam )
{
	if(s_mainWindow == NULL) return DefWindowProc( hWnd, msg, wParam, lParam );
    switch( msg )
    {
    case WM_SYSCOMMAND:
        {
            switch (wParam)
            {
                case SC_SCREENSAVE:      // screensaver trying To start
                case SC_MONITORPOWER:    // monitor trying to enter powersave
                return 0;
            }
            break;
        }

    case WM_CLOSE:
        PostQuitMessage(0);
        break;

    case WM_KEYDOWN:
        switch( wParam )                 //key pressed
        {
        case VK_ESCAPE:
            PostQuitMessage(0);        //if escape pressed, exit
            break;
        default:
            s_mainWindow->parent().callback().keyPress(s_mainWindow->parent().data(), (UINT)wParam);
            break;
        }
        break;

    case WM_KEYUP:
        //s_mainWindow->parent().callback().keyPress(s_mainWindow->parent().data(), (UINT)wParam);
        break;

    /*case WM_LBUTTONDOWN:
        g_WndProcContext->SetMouseState(MouseButtonLeft, true);
        break;

    case WM_LBUTTONUP:
        g_WndProcContext->SetMouseState(MouseButtonLeft, false);
        break;

    case WM_RBUTTONDOWN:
        g_WndProcContext->SetMouseState(MouseButtonRight, true);
        break;

    case WM_RBUTTONUP:
        g_WndProcContext->SetMouseState(MouseButtonRight, false);
        break;

    case WM_MBUTTONDOWN:
        g_WndProcContext->SetMouseState(MouseButtonMiddle, true);
        break;

    case WM_MBUTTONUP:
        g_WndProcContext->SetMouseState(MouseButtonMiddle, false);
        break;

    case WM_MOUSEMOVE:
        {
            POINTS P = MAKEPOINTS(lParam);
            Vec2i NewPos(P.x, P.y);
            g_WndProcContext->UpdateMousePos(NewPos);
        }
        break;

    case WM_MOUSEWHEEL:
        g_WndProcContext->UpdateWheelState(GET_WHEEL_DELTA_WPARAM(wParam));
        break;*/
    }

    return DefWindowProc( hWnd, msg, wParam, lParam );
}

WindowWin32::~WindowWin32()
{
	destroy();
	s_mainWindow = NULL;
}

void WindowWin32::init(HINSTANCE instance, int width, int height, const std::string &name)
{
	m_className = name;
	m_class.style = CS_HREDRAW | CS_VREDRAW;
	m_class.lpfnWndProc = (WNDPROC) WindowCallback; 
	m_class.cbClsExtra = 0;
	m_class.cbWndExtra = 0;
	m_class.hInstance = instance;
	m_class.hIcon = LoadIcon(NULL, IDI_WINLOGO);
	m_class.hCursor = LoadCursor(NULL, IDC_ARROW);
	m_class.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH); 
	m_class.lpszMenuName =  NULL;
	m_class.lpszClassName = m_className.c_str();
	RegisterClassA(&m_class);

	s_mainWindow = this;
	m_handle = CreateWindowA(
		m_className.c_str(), 
		m_className.c_str(), 
		WS_OVERLAPPEDWINDOW, 
		0, //CW_USEDEFAULT
		0, //CW_USEDEFAULT
		width, 
		height, 
		(HWND) NULL, 
		(HMENU) NULL, 
		instance,
		(LPVOID) NULL);
	
	ShowWindow(m_handle, SW_SHOW);
	UpdateWindow(m_handle);
}

void WindowWin32::destroy()
{
	DestroyWindow(m_handle);
	UnregisterClassA( m_className.c_str(), m_class.hInstance );
}

UINT WindowWin32::width() const
{
	RECT rect;
	GetClientRect(m_handle, &rect);
	return rect.right - rect.left;
}

UINT WindowWin32::height() const
{
	RECT rect;
	GetClientRect(m_handle, &rect);
	return rect.bottom - rect.top;
}
