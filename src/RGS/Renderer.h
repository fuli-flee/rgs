#pragma once
#include "RGS/Framebuffer.h"
#include "RGS/Base.h"
#include "RGS/Maths.h"

#include <type_traits>
#include <cmath>

namespace RGS
{
	template<typename vertex_t>	//对于不同的数据结构所构成的三角形, 这里只需要知道其顶点的类型即可
	struct Triangle
	{
		static_assert(std::is_base_of_v<VertexBase, vertex_t>, "vertex_t 必须继承自 RGS::VertexBase");

		vertex_t Vertex[3];

		vertex_t& operator[](size_t i) { return Vertex[i]; }
		const vertex_t& operator[](size_t i) const { return Vertex[i]; }

		Triangle() = default;
	};

	template<typename vertex_t, typename uniforms_t, typename varyings_t>//vertex为输入顶点类型, uniforms为顶点着色器所需的uniform变量, varyings为输出变量
	struct Program
	{
		using vertex_shader_t = void (*)(varyings_t&, const vertex_t&, const uniforms_t&);//(*)此为函数指针
		vertex_shader_t VertexShader;//顶点着色器

		using fragment_shader_t = Vec4(*) (bool& discard, const varyings_t&, const uniforms_t&);//discard为片段是否丢弃的标志, 若为true, 则丢弃此片段
		fragment_shader_t FragmentShader;//片段着色器

		Program(const vertex_shader_t vertex_shader,const fragment_shader_t fragment_shader)
			: VertexShader(vertex_shader), FragmentShader(fragment_shader) {}
	};

	class Renderer
	{
	private:
		static constexpr int RGS_MAX_VARYINGS = 9;

	private:
		enum class Plane
		{
			POSITIVE_W,
			POSITIVE_X,
			NEGATIVE_X,
			POSITIVE_Y,
			NEGATIVE_Y,
			POSITIVE_Z,
			NEGATIVE_Z,
		};

		struct BoundingBox {int MinX, MinY,  MaxX, MaxY;};
		

		static bool IsVertexVisible(const Vec4& clipPos);
		static bool IsInsidePlane(const Vec4& clipPos, const Plane plane);
		static bool IsInsideTriangle(float(&weights)[3]);
		

		static float GetIntersectRatio(const Vec4& prev, const Vec4& curr, const Plane plane);
		static BoundingBox GetBoundingBox(const Vec4(&fragCoords)[3], const int width, const int height);
		static void CalculateWeights(float(&screenWeights)[3], float(&weights)[3], const Vec4(&fragCoords)[3], const Vec2& screenPoint);

		template <typename varyings_t>
		static void LerpVaryings(varyings_t& out, const varyings_t& start, const varyings_t& end, const float ratio)
		{
			constexpr uint32_t floatNum = sizeof(varyings_t) / sizeof(float);
			float* startFloat = (float*)&start;
			float* endFloat = (float*)&end;
			float* outFloat = (float*)&out;

			for (int i = 0; i < (int)floatNum; i++)
			{
				outFloat[i] = Lerp(startFloat[i], endFloat[i], ratio);
			}
		}

		template<typename varyings_t>
		static void LerpVaryings(varyings_t& out, const varyings_t(&varyings)[3], float(&weights)[3]) 
		{
			constexpr uint32_t floatNum = sizeof(varyings_t) / sizeof(float);
			float* v0 = (float*)&varyings[0];
			float* v1 = (float*)&varyings[1];
			float* v2 = (float*)&varyings[2];
			float* outFloat = (float*)&out;

			for (int i = 0; i < (int)floatNum; i++)
			{
				outFloat[i] = v0[i] * weights[0] + v1[i] * weights[1] + v2[i] * weights[2];
			}
		}


		template<typename varyings_t>
		static int ClipAgainstPlane(varyings_t(&outVaryings)[RGS_MAX_VARYINGS],
									const varyings_t(&inVaryings)[RGS_MAX_VARYINGS],
									const Plane plane,
									const int inVertexNum)
		{
			ASSERT(inVertexNum >= 3);

			int outVertexNum = 0;
			for (int i = 0; i < inVertexNum; i++)
			{
				//上一个点
				int prevIndex = (i - 1 + inVertexNum) % inVertexNum;
				//当前点
				int currIndex = i;

				const varyings_t& prevVaryings = inVaryings[prevIndex];
				const varyings_t& currVaryings = inVaryings[currIndex];

				const bool prevInside = IsInsidePlane(prevVaryings.ClipPos, plane);
				const bool currInside = IsInsidePlane(currVaryings.ClipPos, plane);

				if (currInside != prevInside)//不相等说明一点在内侧，另一点在外侧，需要裁剪
				{
					//加入交点
					float redio = GetIntersectRatio(prevVaryings.ClipPos, currVaryings.ClipPos, plane);
					LerpVaryings(outVaryings[outVertexNum], prevVaryings, currVaryings, redio);
					outVertexNum++;
				}
				if (currInside)
				{
					outVaryings[outVertexNum] = inVaryings[currIndex];
					outVertexNum++;
				}
			}

			ASSERT(outVertexNum <= RGS_MAX_VARYINGS);
			return outVertexNum;
		}

		template<typename varyings_t> 
		static int Clip(varyings_t(&varyings)[RGS_MAX_VARYINGS])
		{
			//判断顶点是否在视锥体内
			bool v0_Visible = IsVertexVisible(varyings[0].ClipPos);
			bool v1_Visible = IsVertexVisible(varyings[1].ClipPos);
			bool v2_Visible = IsVertexVisible(varyings[2].ClipPos);
			if (v0_Visible && v1_Visible && v2_Visible)
				return 3;
			
			//裁剪
			int vertexNum = 3;
			varyings_t varyings_[RGS_MAX_VARYINGS];//双重缓存
			vertexNum = ClipAgainstPlane(varyings_, varyings, Plane::POSITIVE_W, vertexNum);
			if (vertexNum == 0) return 0;
			vertexNum = ClipAgainstPlane(varyings, varyings_, Plane::POSITIVE_X, vertexNum);
			if (vertexNum == 0) return 0;
			vertexNum = ClipAgainstPlane(varyings_, varyings, Plane::POSITIVE_X, vertexNum);
			if (vertexNum == 0) return 0;
			vertexNum = ClipAgainstPlane(varyings, varyings_, Plane::POSITIVE_Y, vertexNum);
			if (vertexNum == 0) return 0;
			vertexNum = ClipAgainstPlane(varyings_, varyings, Plane::POSITIVE_Y, vertexNum);
			if (vertexNum == 0) return 0;
			vertexNum = ClipAgainstPlane(varyings, varyings_, Plane::POSITIVE_Z, vertexNum);
			if (vertexNum == 0) return 0;
			vertexNum = ClipAgainstPlane(varyings_, varyings, Plane::POSITIVE_Z, vertexNum);
			if (vertexNum == 0) return 0;
			memcpy(varyings, varyings_, sizeof(varyings_));

			return vertexNum;
		}

		template<typename varyings_t>
		static void CaculateNdcPos(varyings_t(&varyings)[RGS_MAX_VARYINGS], const int vertexNum)
		{
			for (int i = 0; i < vertexNum; i++)
			{
				float w = varyings[i].ClipPos.W;
				varyings[i].NdcPos = varyings[i].ClipPos / w;
				varyings[i].NdcPos.W = 1.0f / w;
			}
		}

		template <typename varyings_t>//varyings_t为输出变量类型
		//CaculateFragPos是将顶点的NDC坐标转换为屏幕坐标
		static void CaculateFragPos(varyings_t(&varyings)[RGS_MAX_VARYINGS],
									const int vertexNum,
									const float width,
									const float height)//width和height为屏幕的宽高
		{
			for (int i = 0; i < vertexNum; i++)
			{
				float x = (varyings[i].NdcPos.X + 1.0f) * 0.5f * width;
				float y = (varyings[i].NdcPos.Y + 1.0f) * 0.5f * height;
				float z = (varyings[i].NdcPos.Z + 1.0f) * 0.5f;
				float w = varyings[i].NdcPos.W;

				varyings[i].FragPos.X = x;
				varyings[i].FragPos.Y = y;
				varyings[i].FragPos.Z = z;
				varyings[i].FragPos.W = w; 
			}
		}

		template<typename varyings_t,typename uniforms_t,typename vertex_t>
		static void ProcessPixel(Framebuffer& framebuffer,
									const int x,
									const int y,
									const varyings_t& varyings,
									const Program<vertex_t, uniforms_t, varyings_t>& program,
									const uniforms_t& uniforms)
		{
			bool discard = false;
			Vec4 color{ 0.0f, 0.0f, 0.0f, 0.0f };
			color = program.FragmentShader(discard,varyings, uniforms);//调用顶点着色器
			if (discard)
			{
				return;
			}
			color.X = Clamp(color.X, 0.0f, 1.0f);
			color.Y = Clamp(color.Y, 0.0f, 1.0f);
			color.Z = Clamp(color.Z, 0.0f, 1.0f);
			color.W = Clamp(color.W, 0.0f, 1.0f);

			framebuffer.SetColor(x, y, color);
		}

		template<typename varyings_t,typename uniforms_t,typename vertex_t>
		static void RasterizeTriangle(Framebuffer& framebuffer,
									const Program<vertex_t, uniforms_t, varyings_t>& program,//program将会存储片段着色器的相关信息
									const varyings_t(&varyings)[3],
									const uniforms_t& uniforms)
		{
			Vec4 fragCoords[3];
			fragCoords[0] = varyings[0].FragPos;
			fragCoords[1] = varyings[1].FragPos;
			fragCoords[2] = varyings[2].FragPos;
			BoundingBox bBox = GetBoundingBox(fragCoords, framebuffer.GetWidth(), framebuffer.GetHeight());

			for (int y = bBox.MinY; y <= bBox.MaxY; y++)
			{
				for (int x = bBox.MinX; x <= bBox.MaxX; x++)
				{
					float screenWeights[3];
					float weights[3];
					Vec2 screenPoint{ (float)x + 0.5f, (float)y + 0.5f };

					CalculateWeights(screenWeights, weights, fragCoords, screenPoint);
					if(!IsInsideTriangle(weights))
						continue;

					varyings_t pixVaryings;
					LerpVaryings(pixVaryings, varyings, weights);

					ProcessPixel(framebuffer, x, y, pixVaryings, program, uniforms);
				}
			}

			/*for (int i = 0; i < 3; i++)
			{
				int x = (int)varyings[i].FragPos.X;
				int y = (int)varyings[i].FragPos.Y;

				for (int j = -5; j < 6; j++)
				{
					for (int k = -5; k < 6; k++)
					{
						framebuffer.SetColor(x + j, y + k, { 1.0f, 1.0f, 1.0f });
					}
				}
			}*/
		}


	public:
		template<typename vertex_t, typename uniforms_t, typename varyings_t>
		static void Draw(Framebuffer& framebuffer,
						const Program<vertex_t, uniforms_t, varyings_t>& program, //program用于指定顶点着色器
						const Triangle<vertex_t>& triangle, 
						const uniforms_t& uniforms)
		{
			static_assert(std::is_base_of_v<VertexBase, vertex_t>, "vertex_t 必须继承自 RGS::VertexBase");
			static_assert(std::is_base_of_v<VaryingsBase, varyings_t>, "varyings_t 必须继承自 RGS::VaryingsBase");

			varyings_t varyings[RGS_MAX_VARYINGS];
			for (int i = 0; i < 3; i++)
			{
				program.VertexShader(varyings[i], triangle[i], uniforms);//将三角的三个顶点通过VertexShader(顶点着色器)变换之后存入varyings数组中
			}

			//裁剪
			int vertexNum = Clip(varyings);


			//屏幕坐标转换
			CaculateNdcPos(varyings, vertexNum);
			int fWidth = framebuffer.GetWidth();
			int fHeight = framebuffer.GetHeight();
			CaculateFragPos(varyings, vertexNum, (float)fWidth, (float)fHeight);

			//三角形绘制
			for (int i = 0; i < vertexNum - 2; i++)//多边形可以由它的 顶点数-2 个三角形组成
			{
				varyings_t triVaryings[3];
				triVaryings[0] = varyings[0];
				triVaryings[1] = varyings[i + 1];
				triVaryings[2] = varyings[i + 2];

				RasterizeTriangle(framebuffer, program,triVaryings,uniforms);
			}
		}
	};
} 

