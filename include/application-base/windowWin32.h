
#include <windows.h>

class WindowWin32
{
public:
	WindowWin32();
	~WindowWin32();

	void init(HINSTANCE instance, int width, int height, const String &name);
	void destroy();
	void resize(UINT newWidth, UINT newHeight);
	void rename(const String &name);

	UINT width() const;
	UINT height() const;

private:
	String    m_className;
	WNDCLASSA m_class;
	HWND      m_handle;
};
