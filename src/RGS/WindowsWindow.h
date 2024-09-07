#pragma once
#include"RGS/Window.h"

#include<string>
#include<Windows.h>

namespace RGS
{
	class WindowsWindow : public Window
	{
	public:
		// 构造函数，初始化窗口
		WindowsWindow(const std::string title, const int width, const int height);
		// 析构函数，清理资源
		~WindowsWindow();

		// 显示窗口
		virtual void Show() const override;
		//绘制帧缓存
		virtual void DrawFramebuffer(const Framebuffer& framebuffer);//override 覆盖函数,对函数进行重写

	public:
		// 初始化窗口系统
		static void Init();
		// 终止窗口系统
		static void Terminate();
		// 处理输入事件
		static void PollInputEvents();//按住ALT+↑↓可以将该行移动
	private:
		// 注册窗口类
		static void Register();
		// 注销窗口类
		static void Unregister();

		static void KeyPressImpl(WindowsWindow* window, const WPARAM wParam, const char state);//wParam为虚拟键码，state为按下或弹起状态
		// 窗口过程函数，处理窗口消息
		static LRESULT CALLBACK WndProc(const HWND hWnd, const UINT msgID, const WPARAM wParam, const LPARAM lParam);



	private:
		HWND m_Handle; // 窗口句柄
		HDC m_MemoryDC; // 内存设备上下文
		unsigned char* m_Buffer; // 缓冲区

		static bool s_Inited; // 初始化标志
	};
}
