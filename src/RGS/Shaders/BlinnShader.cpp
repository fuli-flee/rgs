#include "BlinnShader.h"

#include "RGS/Maths.h"

#include <cmath>  

namespace RGS
{
	void BlinnVertexShader(BlinnVaryings& varyings, const BlinnVertex& vertex, const BlinnUniforms& uniforms)//varyings����const�Ͳ��������
	{
		varyings.ClipPos = uniforms.MVP * vertex.ModelPos;//vertex������ģ�Ϳռ�ͨ��uniforms.MVP�任���ü��ռ�
	}

	Vec4 BlinnFragmentShader(bool& discard, const BlinnVaryings& varyings, const BlinnUniforms& uniforms)
	{	
		if (uniforms.IsAnother)
		{
			Vec4 color = varyings.NdcPos * 0.5 + 0.5f;
			color.W = 1.0f;
			return color;
		}
		else
		{
			int x = varyings.FragPos.X;
			if ((x + 10) % 10 > 4)
			{
				discard = true;
			}
			return Vec4{ 1.0f,1.0f,1.0f,0.5f };
		}
	}
}