#pragma once
#include "RGS/Window.h"
#include "RGS/Renderer.h"
#include "RGS/Shaders/BlinnShader.h"


#include <string>
#include <vector>
#include <chrono>

namespace RGS
{
	class Application
	{
	public:
		Application(const std::string name,const int width,const int height);
		~Application();

		void Run();

	private:
		void Init();
		void Terminate();

		void OnUpdate();

	private:
		std::string m_Name;
		int m_Width;
		int m_Height;

		Window* m_Window;

	};

}

