#include "main.h"

int PASCAL WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, 
				   LPSTR lpszCmdLine, int nCmdShow)
{
	AppTest callback;
	ApplicationWin32 app(hInstance, 1280, 1024, "D3D11 Test", GraphicsDeviceTypeD3D11, callback);
	app.messageLoop();
}