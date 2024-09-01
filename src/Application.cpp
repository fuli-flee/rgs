//应用程序类实现
#include "Application.h"

#include "Window.h"
#include "FrameBuffer.h"

#include<iostream>
#include <string>



namespace RGS 
{
	Application::Application(std::string name,const int width, const int height)
		: m_Name(name), m_Width(width), m_Height(height)
	{
 		Init();
	}

	Application::~Application()
	{
		Terminate();
	}

	void Application::Init()
	{
		Window::Init();
		m_Window = Window::Create(m_Name, m_Width, m_Height);
	}

	void Application::Terminate()
	{
		delete m_Window;
		Window::Terminate();
	}

	void Application::Run()
	{
		while (!m_Window->Closed())
		{
			OnUpdate();

			Window::PollInputEvents();
		}
	}

	void Application::OnUpdate()
	{
		if (m_Window->GetKey(RGS_KEY_0) == RGS_PRESS)
			std::cout << "0被按下" << std::endl;
		if (m_Window->GetKey(RGS_KEY_A) == RGS_PRESS)
			std::cout << "A被按下" << std::endl;
		if (m_Window->GetKey(RGS_KEY_B) == RGS_PRESS)
			std::cout << "B被按下" << std::endl;
		if (m_Window->GetKey(RGS_KEY_O) == RGS_PRESS)
			std::cout << "O被按下" << std::endl;

		if (m_Window->GetKey(RGS_KEY_R) == RGS_PRESS)
			std::cout << "R被按下" << std::endl;
		if (m_Window->GetKey(RGS_KEY_G) == RGS_PRESS)
			std::cout << "G被按下" << std::endl;
		if (m_Window->GetKey(RGS_KEY_S) == RGS_PRESS)
			std::cout << "S被按下" << std::endl;

		Framebuffer framebuffer(m_Width, m_Height);
		framebuffer.Clear({200. / 255., 224. / 255., 228. / 255.});
		m_Window->DrawFramebuffer(framebuffer);
	}
}

