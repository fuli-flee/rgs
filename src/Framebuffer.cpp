//帧缓存
#pragma once
#include "Base.h"
#include "Framebuffer.h"

namespace RGS
{
	Framebuffer::Framebuffer(const int width, const int height)
		:m_Width(width), m_Height(height)
	{
		ASSERT((width > 0) && (height > 0));
		m_PixelSize = m_Width * m_Height;
		m_ColorBuffer = new Vec3[m_PixelSize]();//初始化颜色缓冲区
		m_DepthBuffer = new float[m_PixelSize]();//初始化深度缓冲区
		Clear({ 0.0f, 0.0f, 0.0f });
		ClearDepth(1.0f);
	}

	Framebuffer::~Framebuffer()
	{
		delete[] m_ColorBuffer;
		delete[] m_DepthBuffer;
		m_ColorBuffer = nullptr;
		m_DepthBuffer = nullptr;
	}

	void Framebuffer::SetColor(const int x, const int y, const Vec3& color) 
	{
		int index = GetPixelIndex(x, y);
		if (index < m_PixelSize && index >= 0)
		{
			m_ColorBuffer[index] = color;//设置颜色
		}
		else  //为什么用else?
				//因为如果index超出范围，说明是越界了，不应该再设置颜色了，所以用else
		{
			ASSERT(false);
		}
	}

	Vec3 Framebuffer::GetColor(const int x, const int y) const 
	{
		int index = GetPixelIndex(x, y);
		if (index < m_PixelSize && index >= 0)
		{
			return m_ColorBuffer[index];//获取颜色
		}
		else
		{
			ASSERT(false);
			return { 0.0f, 0.0f, 0.0f };
		}
	}

	void Framebuffer::SetDepth(const int x, const int y, const float depth) 
	{
		int index = GetPixelIndex(x, y);
		if (index < m_PixelSize && index >= 0)
		{
			m_DepthBuffer[index] = depth;//设置深度
		}
		else
		{
			ASSERT(false);
		}
	}

	float Framebuffer::GetDepth(const int x, const int y)const 
	{
		int index = GetPixelIndex(x, y);
		if (index < m_PixelSize && index >= 0)
		{
			float depth = m_DepthBuffer[index];
			return depth;//获取深度
		}
		else
		{
			ASSERT(false);
			return 1.0f;
		}
	}
	
	void Framebuffer::Clear(const Vec3& color) 
	{
		for (int i = 0;i < m_PixelSize;i++)
		{
			m_ColorBuffer[i] = color;
		}
	}

	void Framebuffer::ClearDepth(float depth) 
	{
		for (int i = 0; i < m_PixelSize; i++)
		{
			m_DepthBuffer[i] = depth;
		}
	}
}
