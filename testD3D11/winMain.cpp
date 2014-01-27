#include "main.h"

int PASCAL WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, 
				   LPSTR lpszCmdLine, int nCmdShow)
{
	ApplicationWin32 app(hInstance, 1280, 1024, "D3D11 Test");
	app.messageLoop();
}