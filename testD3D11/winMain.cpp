#include "main.h"

#include <map>
#include <unordered_map>

/*
class proxy
{
	int a;
};

class thingIterator
{
public:
	
	thingIterator& operator++()
	{
		current++;
		return *this;
	}

	proxy *current;
	proxy *end;
};

inline thing& proxyCast(proxy &a)
{
	return *((thing*)(&a));
}

*/
class foo;
typedef std::unordered_map<int, foo> thing;

class foo
{
public:
	foo(const thing::iterator &_f)
	{
		f = _f;
	}
	thing::iterator f;
};

int PASCAL WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, 
				   LPSTR lpszCmdLine, int nCmdShow)
{
	Console::openLogFile("console.txt");
	AppTest callback;
	ApplicationWin32 app(hInstance, 1280, 1024, "D3D11 Test", GraphicsDeviceTypeD3D11, callback);
	app.messageLoop();

	thing stuff;
	std::unordered_map<int, foo>::iterator a = stuff.begin();
	stuff.insert(std::make_pair(3, foo(stuff.begin())));
	stuff.insert(std::make_pair(4, foo(stuff.begin())));
}