#pragma once

/**
 * @file
 * @brief Defines Float2, Float3, and Int2.
 */

#include <array>
#include <cmath>
#include <cstddef>

namespace csc {
	class Int2;

	/**
	 * @brief Used to represent a float vector of size 2.
	 */
	class Float2 {
	public:
		Float2() : x{ 0.0f }, y{ 0.0f } {}
		Float2(float x, float y) : x{ x }, y{ y } {}

		explicit Float2(Int2 int2);

		Float2 floor() const { return Float2{ floorf(x), floorf(y) }; }

		float distance(const Float2& other) const;
		bool similar(const Float2& other, float margin) const;

		Float2 operator+(const Float2& other) const;
		Float2 operator-(const Float2& other) const;

		Float2 operator/(const float& other) const;
		Float2 operator/(const size_t& other) const;

		bool operator==(const Float2& other) const { return x == other.x && y == other.y; }
		bool operator!=(const Float2& other) const { return operator==(other) == false; }

		bool operator<(const Float2& other) const;

		float x;
		float y;
	};

	/**
	 * @brief Used to represent a float vector of size 3.
	 */
	class Float3 {
	public:
		Float3() : x{ 0.0f }, y{ 0.0f }, z{ 0.0f } {}
		Float3(float x, float y, float z) : x{ x }, y{ y }, z{ z } {}
		Float3(std::array<float, 3> in) : x{ in[0] }, y{ in[1] }, z{ in[2] } {}

		std::array<float, 3> as_array() const { return std::array<float, 3>{ x, y, z }; }

		Float3 operator+(const Float3& other) const;
		Float3 operator-(const Float3& other) const;

		bool similar(const Float3& other, const float margin) const;

		bool operator==(const Float3& other) const { return x == other.x && y == other.y && z == other.z; }
		bool operator!=(const Float3& other) const { return operator==(other) == false; }

		float x;
		float y;
		float z;
	};

	/**
	 * @brief Used to represent an int vector of size 2.
	 */
	class Int2 {
	public:
		Int2() : x{ 0 }, y{ 0 } {}
		Int2(int x, int y) : x{ x }, y{ y } {}

		explicit Int2(Float2 float2);

		Int2 operator+(const Int2& other) const;
		Int2 operator-(const Int2& other) const;

		bool operator==(const Int2& other) const { return x == other.x && y == other.y; }
		bool operator!=(const Int2& other) const { return operator==(other) == false; }

		bool operator<(const Int2& other) const;

		int x;
		int y;
	};
}
