#include "BlinnShader.h"

#include "RGS/Maths.h"

#include <cmath>  

namespace RGS
{
	void BlinnVertexShader(BlinnVaryings& varyings, const BlinnVertex& vertex, const BlinnUniforms& uniforms)//varyings不加const就不能输出了
	{
		varyings.ClipPos = uniforms.MVP * vertex.ModelPos;//vertex顶点在模型空间通过uniforms.MVP变换到裁剪空间
	}

	Vec4 BlinnFragmentShader(bool& discard, const BlinnVaryings& varyings, const BlinnUniforms& uniforms)
	{	
		discard = false;
		return { varyings.NdcPos.X / 2 + 0.5f, varyings.NdcPos.Y / 2 + 0.5f, 0.0f ,1.0f };
	}
}