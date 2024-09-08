#pragma once
#include "RGS/Shaders/ShaderBase.h"

#include "RGS/Maths.h"

namespace RGS
{
	struct BlinnVertex : public VertexBase
	{

	/*	//Blinn着色
		Vec3 ModelNormal;
		Vec2 TexCoord = { 0.0f, 0.0f };*/
	};

	struct BlinnVaryings : public VaryingsBase
	{
		/* //Blinn着色
		Vec2 TexCoord;*/
	};

	struct BlinnUniforms : public UniformsBase
	{
		bool IsAnother = false; 
	};

	void BlinnVertexShader(BlinnVaryings& varyings, const BlinnVertex& vertex, const BlinnUniforms& uniforms);

	Vec4 BlinnFragmentShader(bool& discard, const BlinnVaryings& varyings, const BlinnUniforms& uniforms);
}