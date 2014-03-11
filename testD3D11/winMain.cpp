#include "main.h"

int PASCAL WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, 
				   LPSTR lpszCmdLine, int nCmdShow)
{
	ml::Console::openLogFile("console.txt");
	AppTest callback;
	ml::ApplicationWin32 app(hInstance, 1280, 1024, "D3D11 Test", ml::GraphicsDeviceTypeD3D11, callback);
	app.messageLoop();
}

int main()
{
    AppTest callback;
    ml::ApplicationWin32 app(NULL, 1280, 1024, "D3D11 Test", ml::GraphicsDeviceTypeD3D11, callback);
    app.messageLoop();
}
