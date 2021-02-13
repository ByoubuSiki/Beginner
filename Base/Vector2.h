#pragma once
#include<iostream>

namespace Beginner
{

	//2ŽŸŒ³ƒxƒNƒgƒ‹
	struct Vector2
	{
		Vector2(const float vecX, const float vecY) :x(vecX), y(vecY) {}
		Vector2(const Vector2& vec2) : Vector2(vec2.x, vec2.y) {}
		Vector2() :Vector2(NULL, NULL) {}

		float x, y;

#pragma region operator

		const Vector2 operator+(const int& value) const
		{
			Vector2 vec;
			vec.x = x + value;
			vec.y = y + value;

			return vec;
		}
		const Vector2 operator-(const int& value) const
		{
			Vector2 vec;
			vec.x = x - value;
			vec.y = y - value;

			return vec;
		}
		const Vector2 operator*(const int& value) const
		{
			Vector2 vec;
			vec.x = x * value;
			vec.y = y * value;

			return vec;
		}
		const Vector2 operator/(const int& value) const
		{
			Vector2 vec;
			vec.x = x / value;
			vec.y = y / value;

			return vec;
		}

		const Vector2 operator+(const Vector2& value) const
		{
			Vector2 vec;
			vec.x = x + value.x;
			vec.y = y + value.y;

			return vec;
		}
		const Vector2 operator-(const Vector2& value) const
		{
			Vector2 vec;
			vec.x = x - value.x;
			vec.y = y - value.y;

			return vec;
		}
		const Vector2 operator*(const Vector2& value) const
		{
			Vector2 vec;
			vec.x = x * value.x;
			vec.y = y * value.y;

			return vec;
		}
		const Vector2 operator/(const Vector2& value) const
		{
			Vector2 vec;
			vec.x = x / value.x;
			vec.y = y / value.y;

			return vec;
		}

		Vector2& operator+=(const int& value)
		{
			x += value;
			y += value;

			return *this;
		}
		Vector2& operator-=(const int& value)
		{
			x -= value;
			y -= value;

			return *this;
		}
		Vector2& operator*=(const int& value)
		{
			x *= value;
			y *= value;

			return *this;
		}
		Vector2& operator/=(const int& value)
		{
			x /= value;
			y /= value;

			return *this;
		}

		Vector2& operator+=(const Vector2& value)
		{
			x += value.x;
			y += value.y;

			return *this;
		}
		Vector2& operator-=(const Vector2& value)
		{
			x -= value.x;
			y -= value.y;

			return *this;
		}
		Vector2& operator*=(const Vector2& value)
		{
			x *= value.x;
			y *= value.y;

			return *this;
		}
		Vector2& operator/=(const Vector2& value)
		{
			x /= value.x;
			y /= value.y;

			return *this;
		}

		bool operator==(const Vector2& vec)const
		{
			return x == vec.x && y == vec.y;
		}
		bool operator!=(const Vector2& vec)const
		{
			return !operator==(vec);
		}

#pragma endregion
	};

	typedef Vector2 Size;
}