
#include <windows.h>

class WindowWin32
{
public:
	WindowWin32(ApplicationWin32 &parent) :
		m_parent(parent)
	{
		m_className = "uninitialized";
		m_handle = NULL;
		ZeroMemory(&m_class, sizeof(m_class));
	}
	~WindowWin32();

	void init(HINSTANCE instance, int width, int height, const std::string &name);
	void destroy();
	void resize(UINT newWidth, UINT newHeight);
	void rename(const std::string &name);
	
	UINT width() const;
	UINT height() const;

	HWND handle() const
	{
		return m_handle;
	}
	ApplicationWin32& parent()
	{
		return m_parent;
	}

private:
	std::string    m_className;
	ApplicationWin32 &m_parent;
	WNDCLASSA m_class;
	HWND      m_handle;
};
