//窗口类实现2
//ps: 这个类是Windows平台下的窗口类实现，主要是实现窗口的创建、显示、消息处理等功能
//平台相关的实现都在这个类中，其他平台的实现类只需要继承这个类，并实现相关的平台相关的功能即可
#include"Base.h"
#include"Windows.h"
#include "Math.h"
#include"WindowsWindow.h"

#include<windows.h>

#define RGS_WINDOW_ENTRY_NAME "Entry"
#define RGS_WINDOW_CLASS_NAME "Class"

namespace RGS
{
	// WindowsWindow类的静态成员变量，用于标记是否已初始化
	bool WindowsWindow::s_Inited = false;

	// 初始化Windows窗口
	void WindowsWindow::Init()
	{
		ASSERT(!s_Inited);//断点 
		Register();
		s_Inited = true;
	}

	// 终止Windows窗口
	void WindowsWindow::Terminate()
	{
		ASSERT(s_Inited);
		Unregister();
		s_Inited = false;
	}

	// 注册窗口类
	void WindowsWindow::Register()
	{
		ATOM atom;
		WNDCLASS wc = { 0 };
		wc.cbClsExtra = 0;
		wc.cbWndExtra = 0;
		wc.hbrBackground = (HBRUSH)(WHITE_BRUSH);//设置窗口背景色
		wc.hCursor = NULL;								//设置鼠标样式
		wc.hIcon = NULL;								//设置图标		
		wc.hInstance = GetModuleHandle(NULL);
		wc.lpfnWndProc = WindowsWindow::WndProc;		//设置窗口消息处理函数
		wc.lpszClassName = RGS_WINDOW_CLASS_NAME;
		wc.style = CS_HREDRAW | CS_VREDRAW;
		wc.lpszMenuName = NULL;
		atom = RegisterClass(&wc);
	}

	// 注销窗口类
	void WindowsWindow::Unregister()
	{
		UnregisterClass(RGS_WINDOW_CLASS_NAME, GetModuleHandle(NULL));
	}

	// 构造函数，初始化窗口
	WindowsWindow::WindowsWindow(const std::string title, const int width, const int height)
		: Window(title, width, height)
	{
		ASSERT(s_Inited, "未初始化");

		DWORD style = WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX;
		RECT rect;
		rect.left = 0;
		rect.top = 0;
		rect.bottom = (long)height;
		rect.right = (long)width;
		AdjustWindowRect(&rect, style, false);

		m_Handle = CreateWindow(RGS_WINDOW_CLASS_NAME, m_Title.c_str(), style,
			CW_USEDEFAULT, 0, rect.right - rect.left, rect.bottom - rect.top,
			NULL, NULL, GetModuleHandle(NULL), NULL);
		ASSERT(m_Handle != nullptr);
		m_Closed = false;
		SetProp(m_Handle, RGS_WINDOW_ENTRY_NAME, this);



		HDC windowDC = GetDC(m_Handle);
		m_MemoryDC = CreateCompatibleDC(windowDC);

		BITMAPINFOHEADER biHeader = {};
		HBITMAP newBitmap;
		HBITMAP oldBitmap;

		biHeader.biSize = sizeof(BITMAPINFOHEADER);
		biHeader.biWidth = ((long)m_Width);
		biHeader.biHeight = -((long)m_Height);
		biHeader.biPlanes = 1;
		biHeader.biBitCount = 24;
		biHeader.biCompression = BI_RGB;

		//分配空间
		newBitmap = CreateDIBSection(windowDC, (BITMAPINFO*)&biHeader, DIB_RGB_COLORS, (void**)&m_Buffer, nullptr, 0);
		ASSERT(newBitmap != nullptr);
		constexpr int channelCount = 3;
		int size = m_Width * m_Height * channelCount * sizeof(unsigned char);
		memset(m_Buffer, 0, size);
		oldBitmap = (HBITMAP)SelectObject(m_MemoryDC, newBitmap);

		DeleteObject(oldBitmap);
		ReleaseDC(m_Handle, windowDC);

		Show();
	}

	void WindowsWindow::KeyPressImpl(WindowsWindow* window, const WPARAM wParam, const char state)
	{
		//偷懒了，直接用数组来存
		if (wParam >= '0' && wParam <= '9')
		{
			window->m_Keys[wParam] = state;
			return;
		}
		if (wParam >= 'A' && wParam <= 'Z')
		{
			window->m_Keys[wParam] = state;
			return;
		}

		switch (wParam)
		{
		case VK_SPACE:
			window->m_Keys[RGS_KEY_SPACE] = state;
			break;
		case VK_SHIFT:
			window->m_Keys[RGS_KEY_LEFT_SHIFT] = state;
			window->m_Keys[RGS_KEY_RIGHT_SHIFT] = state;
			break;
		default:
			break;
		}
	}

	// 窗口消息处理函数
	LRESULT CALLBACK WindowsWindow::WndProc(const HWND hwnd, const UINT msgID, const WPARAM wParam, const LPARAM lParam)
	{
		WindowsWindow* window = (WindowsWindow*)GetProp(hwnd, RGS_WINDOW_ENTRY_NAME);
		if (window == nullptr)
		{
			return DefWindowProc(hwnd, msgID, wParam, lParam);
		}
		switch (msgID)
		{
		case WM_DESTROY:
			window->m_Closed = true;
			return 0;
		case WM_KEYDOWN:
			KeyPressImpl(window, wParam, RGS_PRESS);
			return 0;
		case WM_KEYUP:
			KeyPressImpl(window, wParam, RGS_RELEASE);
			return 0;
		}
		return DefWindowProc(hwnd, msgID, wParam, lParam);
	}

	void WindowsWindow::PollInputEvents()
	{
		MSG message;
		while (PeekMessage(&message, NULL, 0, 0, PM_REMOVE))//不用Getmessage是因为当消息队列为空时，Getmessage会一直停止,
																// 而PeekMessage不会停止，没有消息的话就会搁这儿一边歇着,所以可以提高效率			
		{
			TranslateMessage(&message);
			DispatchMessage(&message);
		}
	}

	// 显示窗口
	void WindowsWindow::Show() const
	{
		HDC windowDC = GetDC(m_Handle);
		BitBlt(windowDC, 0, 0, m_Width, m_Height, m_MemoryDC, 0, 0, SRCCOPY);
		ShowWindow(m_Handle, SW_SHOW);
		ReleaseDC(m_Handle, windowDC);
	}

	void WindowsWindow::DrawFramebuffer(const Framebuffer& framebuffer)
	{
		//尽可能显示
		const int fWidth = framebuffer.GetWidth();
		const int fHeight = framebuffer.GetHeight();
		const int width = m_Width < fWidth? m_Width : fWidth;
		const int height = m_Height < fHeight? m_Height : fHeight;
		for (int i = 0; i < height; i++)
		{
			for (int j = 0; j < width; j++)
			{
				//翻转RGB显示
				constexpr int channelCount = 3;
				constexpr int rChannel = 2;
				constexpr int gChannel = 1;
				constexpr int bChannel = 0;
				
				Vec3 color = framebuffer.GetColor(j, fHeight - 1 - i);
				const int pixStart = (i * m_Width + j) * channelCount;
				const int rIndex = pixStart + rChannel;
				const int gIndex = pixStart + gChannel;
				const int bIndex = pixStart + bChannel;

				m_Buffer[rIndex] = Float2UChar(color.X);
				m_Buffer[gIndex] = Float2UChar(color.Y);
				m_Buffer[bIndex] = Float2UChar(color.Z);
			}
		}
		Show();
	}

	// 析构函数，清理资源
	WindowsWindow::~WindowsWindow()
	{
		ShowWindow(m_Handle, SW_HIDE);
		RemoveProp(m_Handle, RGS_WINDOW_ENTRY_NAME);
		DeleteDC(m_MemoryDC);
		DestroyWindow(m_Handle);
	}
	
}
