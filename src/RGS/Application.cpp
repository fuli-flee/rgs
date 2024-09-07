//应用程序类实现
#include "RGS/Application.h"

#include "RGS/Base.h"
#include "RGS/Window.h"
#include "RGS/Framebuffer.h"
#include "RGS/Maths.h"
#include "RGS/Shaders/BlinnShader.h"
#include "RGS/Renderer.h"

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
		framebuffer.Clear();//200. / 255., 224. / 255., 228. / 255.

		//在这里绘制一些东西
		Program program(BlinnVertexShader, BlinnFragmentShader);
		Triangle <BlinnVertex>tri;
		tri[0].ModelPos = { 0.0f, 0.0f, -8.0f, 1.0f };//{ -10.0f, 10.0f, -10.0f, 1.0f }左上角
		tri[1].ModelPos = { -10.0f, -10.0f, -10.0f, 1.0f };//{ -10.0f, -10.0f, -10.0f, 1.0f }左下角
		tri[2].ModelPos = { 30.0f, -10.0f, -10.0f, 1.0f };//
		BlinnUniforms uniforms;
		uniforms.MVP = Mat4Perspective(90.0f / 180.0f * 3.1415926f, 1.0f, 0.1f, 10.0f);

		Renderer::Draw(framebuffer,program,tri,uniforms);

		m_Window->DrawFramebuffer(framebuffer);
	}
}

