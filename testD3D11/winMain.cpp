#include "main.h"

int PASCAL WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, 
				   LPSTR lpszCmdLine, int nCmdShow)
{
	Console::openLogFile("console.txt");
	AppTest callback;
	ApplicationWin32 app(hInstance, 1280, 1024, "D3D11 Test", GraphicsDeviceTypeD3D11, callback);
	app.messageLoop();
}

int main()
{
    AppTest callback;
    ApplicationWin32 app(NULL, 1280, 1024, "D3D11 Test", GraphicsDeviceTypeD3D11, callback);
    app.messageLoop();
}
