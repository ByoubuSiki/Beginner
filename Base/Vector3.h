#pragma once
#include"Vector2.h"

namespace Beginner
{
	struct Vector3 : Vector2
	{
		Vector3(const float vecX, const float vecY, const float vecZ): Vector2(vecX, vecY), z(vecZ) {}
		Vector3(const Vector2& vec2, const float vecZ) :Vector2(vec2), z(vecZ) {}
		Vector3() : Vector3(NULL, NULL, NULL) {}

		~Vector3() {}

		float z;

#pragma region operator
		const Vector3 operator+(const int& value) const
		{
			Vector3 vec;
			vec.x = x + value;
			vec.y = y + value;
			vec.z = z + value;

			return vec;
		}
		const Vector3 operator-(const int& value) const
		{
			Vector3 vec;
			vec.x = x - value;
			vec.y = y - value;
			vec.z = z - value;

			return vec;
		}
		const Vector3 operator*(const int& value) const
		{
			Vector3 vec;
			vec.x = x * value;
			vec.y = y * value;
			vec.z = z * value;

			return vec;
		}
		const Vector3 operator/(const int& value) const
		{
			Vector3 vec;
			vec.x = x / value;
			vec.y = y / value;
			vec.z = z / value;

			return vec;
		}

		const Vector3 operator+(const Vector3& value) const
		{
			Vector3 vec;
			vec.x = x + value.x;
			vec.y = y + value.y;
			vec.z = z + value.z;

			return vec;
		}
		const Vector3 operator-(const Vector3& value) const
		{
			Vector3 vec;
			vec.x = x - value.x;
			vec.y = y - value.y;
			vec.z = z - value.z;

			return vec;
		}
		const Vector3 operator*(const Vector3& value) const
		{
			Vector3 vec;
			vec.x = x * value.x;
			vec.y = y * value.y;
			vec.z = z * value.z;

			return vec;
		}
		const Vector3 operator/(const Vector3& value) const
		{
			Vector3 vec;
			vec.x = x / value.x;
			vec.y = y / value.y;
			vec.z = z / value.z;

			return vec;
		}

		Vector3& operator+=(const int& value)
		{
			x += value;
			y += value;
			z += value;

			return *this;
		}
		Vector3& operator-=(const int& value)
		{
			x -= value;
			y -= value;
			z -= value;

			return *this;
		}
		Vector3& operator*=(const int& value)
		{
			x *= value;
			y *= value;
			z *= value;

			return *this;
		}
		Vector3& operator/=(const int& value)
		{
			x /= value;
			y /= value;
			z /= value;

			return *this;
		}

		Vector3& operator+=(const Vector3& value)
		{
			x += value.x;
			y += value.y;
			z += value.z;

			return *this;
		}
		Vector3& operator-=(const Vector3& value)
		{
			x -= value.x;
			y -= value.y;
			z -= value.z;

			return *this;
		}
		Vector3& operator*=(const Vector3& value)
		{
			x *= value.x;
			y *= value.y;
			z *= value.z;

			return *this;
		}
		Vector3& operator/=(const Vector3& value)
		{
			x /= value.x;
			y /= value.y;
			z /= value.z;

			return *this;
		}

		bool operator==(const Vector3& vec)const
		{
			return x == vec.x && y == vec.y && z == vec.z;
		}
		bool operator!=(const Vector3& vec)const
		{
			return !operator==(vec);
		}

#pragma endregion
	};
}