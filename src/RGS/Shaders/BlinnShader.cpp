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
		discard = false;
		return { varyings.NdcPos.X / 2 + 0.5f, varyings.NdcPos.Y / 2 + 0.5f, 0.0f ,1.0f };
	}
}