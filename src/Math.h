#pragma once

namespace RGS
{
	struct Vec3
	{
		float X, Y, Z;

		constexpr Vec3()
			: X(0.0f), Y(0.0f), Z(0.0f) {}
		constexpr Vec3(float x, float y, float z)
			: X(x), Y(y), Z(z) {}
	};

	unsigned char Float2UChar(const float f);
	float UChar2Float(const unsigned char c);
}