#pragma once
#include <string>

namespace RGS
{
	struct Vec2
	{
		float X, Y;

		constexpr Vec2()
			:X(0.0f), Y(0.0f) {}
		constexpr Vec2(float x, float y)
			: X(x), Y(y) {}
	};

	struct Vec3
	{
		float X, Y, Z;

		constexpr Vec3()
			: X(0.0f), Y(0.0f), Z(0.0f) {}
		constexpr Vec3(float x, float y, float z)
			: X(x), Y(y), Z(z) {}

		operator Vec2() const { return{ X , Y }; }//当Vec3转换为Vec2时，只取X和Y两个值
	};

	
	struct Vec4 
	{
		float X, Y, Z, W;

		constexpr Vec4()
			: X(0.0f), Y(0.0f), Z(0.0f), W(0.0f) {}
		constexpr Vec4(float x, float y, float z, float w)
			: X(x), Y(y), Z(z), W(w) {}
		constexpr Vec4(const Vec3& vec3, float w)
			:X(vec3.X), Y(vec3.Y), Z(vec3.Z), W(w) {}

		operator Vec2() const { return{ X, Y }; }
		operator Vec3() const { return{ X, Y, Z }; }

		operator std::string() const
		{
			std::string res;
			res += "(";
			res += std::to_string(X);
			res += ", ";
			res += std::to_string(Y);
			res += ", ";
			res += std::to_string(Z);
			res += ", ";
			res += std::to_string(W);
			res += ")";
			return res;
		}
	};

	//行优先初始化
	struct Mat4
	{
		float M[4][4];
		Mat4() 
		{
			for (int i = 0; i < 4; i++)
			{
				for (int j = 0; j < 4; j++)
				{
					M[i][j] = 0.0f;
				}
			}
		}

		//按列存入
		Mat4(const Vec4& v0, const Vec4& v1, const Vec4& v2, const Vec4& v3);

		operator const std::string() const 
		{
			std::string res;
			res += "(";

			for (int i = 0; i < 4; i++)
			{
				for (int j = 0; j < 4; j++)
				{
					res += std::to_string(M[i][j]);
					res += (i == 3 && j == 3) ? ")" : ",";
				}
			}

			return res;
		};
	};

	Vec4 operator*(const Mat4& mat4, const Vec4& vec4);
	Mat4 operator*(const Mat4& left, const Mat4& right);
	Mat4& operator*=(Mat4& left, const Mat4& right);

	Mat4 Mat4Identity();
	Mat4 Mat4Scale(float sx, float sy, float sz);
	Mat4 Mat4Translate(float tx, float ty, float tz);
	Mat4 Mat4RotateX(float angle);
	Mat4 Mat4RotateY(float angle);
	Mat4 Mat4RotateZ(float angle);
	Mat4 Mat4LookAt(const Vec3& xAxis, const Vec3& yAxis, const Vec3& zAxis, const Vec3& eye);

	unsigned char Float2UChar(const float f);
	float UChar2Float(const unsigned char c);
}