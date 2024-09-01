//������ʵ��1
#include"base.h"
#include"Window.h"
#include"WindowsWindow.h"

namespace RGS
{
	// ������Ĺ��캯������ʼ�����ڵı��⡢��Ⱥ͸߶�
	Window::Window(std::string title, const int width, const int height)
		: m_Title(title), m_Width(width), m_Height(height) 
	{
		ASSERT((m_Width > 0)  && (m_Height > 0));
		memset(m_Keys, RGS_RELEASE, RGS_KEY_MAX_COUNT);
	}

	// ��ʼ������
	void Window::Init()
	{
		WindowsWindow::Init();
	}

	// ��ֹ����
	void Window::Terminate()
	{
		WindowsWindow::Terminate();
	}

	// ����һ���µĴ���ʵ��
	Window* Window::Create(const std::string title, const int width, const int height)
	{
		return new WindowsWindow(title, width, height);
	}

	void Window::PollInputEvents()//����static����Ϊ��CPP�ļ��м���static��Ϊ�ú������ڵ�ǰ�ļ�����Ч������ͷ�ļ��м���static��Ϊȫ����Ч��������CPP�ļ��м���static��Ϊ�˱����ظ�����
	{
		WindowsWindow::PollInputEvents();
	}
}
