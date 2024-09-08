#include "BlinnShader.h"

#include "RGS/Maths.h"

#include <cmath>  

namespace RGS
{
	void BlinnVertexShader(BlinnVaryings& varyings, const BlinnVertex& vertex, const BlinnUniforms& uniforms)//varyings不加const就不能输出了
	{
		varyings.ClipPos = uniforms.MVP * vertex.ModelPos;//vertex顶点在模型空间通过uniforms.MVP变换到裁剪空间

		//Blinn着色
		//varyings.TexCoord = vertex.TexCoord;
	}

	Vec4 BlinnFragmentShader(bool& discard, const BlinnVaryings& varyings, const BlinnUniforms& uniforms)
	{	
		//Blinn着色
		/*return Vec4{ varyings.TexCoord.X, varyings.TexCoord.Y, 0.0f, 1.0f };
		Vec4 color = varyings.NdcPos * 0.5 + 0.5f;
		color.W = 1.0f;
		return color;*/

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