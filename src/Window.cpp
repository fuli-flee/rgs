//窗口类实现1
#include"base.h"
#include"Window.h"
#include"WindowsWindow.h"

namespace RGS
{
	// 窗口类的构造函数，初始化窗口的标题、宽度和高度
	Window::Window(std::string title, const int width, const int height)
		: m_Title(title), m_Width(width), m_Height(height) 
	{
		ASSERT((m_Width > 0)  && (m_Height > 0));
		memset(m_Keys, RGS_RELEASE, RGS_KEY_MAX_COUNT);
	}

	// 初始化窗口
	void Window::Init()
	{
		WindowsWindow::Init();
	}

	// 终止窗口
	void Window::Terminate()
	{
		WindowsWindow::Terminate();
	}

	// 创建一个新的窗口实例
	Window* Window::Create(const std::string title, const int width, const int height)
	{
		return new WindowsWindow(title, width, height);
	}

	void Window::PollInputEvents()//不加static是因为在CPP文件中加上static意为该函数仅在当前文件中有效，而在头文件中加上static意为全局有效，所以在CPP文件中加上static是为了避免重复定义
	{
		WindowsWindow::PollInputEvents();
	}
}
