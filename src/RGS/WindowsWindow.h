#pragma once
#include"RGS/Window.h"

#include<string>
#include<Windows.h>

namespace RGS
{
	class WindowsWindow : public Window
	{
	public:
		// ���캯������ʼ������
		WindowsWindow(const std::string title, const int width, const int height);
		// ����������������Դ
		~WindowsWindow();

		// ��ʾ����
		virtual void Show() const override;
		//����֡����
		virtual void DrawFramebuffer(const Framebuffer& framebuffer);//override ���Ǻ���,�Ժ���������д

	public:
		// ��ʼ������ϵͳ
		static void Init();
		// ��ֹ����ϵͳ
		static void Terminate();
		// ���������¼�
		static void PollInputEvents();//��סALT+�������Խ������ƶ�
	private:
		// ע�ᴰ����
		static void Register();
		// ע��������
		static void Unregister();

		static void KeyPressImpl(WindowsWindow* window, const WPARAM wParam, const char state);//wParamΪ������룬stateΪ���»���״̬
		// ���ڹ��̺�������������Ϣ
		static LRESULT CALLBACK WndProc(const HWND hWnd, const UINT msgID, const WPARAM wParam, const LPARAM lParam);



	private:
		HWND m_Handle; // ���ھ��
		HDC m_MemoryDC; // �ڴ��豸������
		unsigned char* m_Buffer; // ������

		static bool s_Inited; // ��ʼ����־
	};
}
