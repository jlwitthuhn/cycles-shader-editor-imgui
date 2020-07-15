#include "vector.h"

#include <cmath>

csc::Float2::Float2(const Int2 int2) :
	x{ static_cast<float>(int2.x) },
	y{ static_cast<float>(int2.y) }
{

}

float csc::Float2::distance(const Float2& other) const
{
	const csc::Float2 delta = *this - other;
	const float distance_squared{ delta.x * delta.x + delta.y * delta.y };
	return sqrt(distance_squared);
}

bool csc::Float2::similar(const Float2& other, const float margin) const
{
	const float diff_x{ fabs(x - other.x) };
	const float diff_y{ fabs(y - other.y) };
	return diff_x < margin && diff_y < margin;
}

csc::Float2 csc::Float2::operator+(const Float2& other) const
{
	return Float2{ x + other.x, y + other.y };
}

csc::Float2 csc::Float2::operator-(const Float2& other) const
{
	return Float2{ x - other.x, y - other.y };
}

csc::Float2 csc::Float2::operator/(const float& other) const
{
	return Float2{ x / other, y / other };
}

csc::Float2 csc::Float2::operator/(const size_t& other) const
{
	return Float2{ x / other, y / other };
}

bool csc::Float2::operator<(const Float2& other) const
{
	if (x < other.x) return true;
	if (other.x < x) return false;

	if (y < other.y) return true;
	if (other.y < y) return false;

	return false;
}

csc::Float3 csc::Float3::operator+(const Float3& other) const
{
	return Float3{ x + other.x, y + other.y, z + other.z };
}

csc::Float3 csc::Float3::operator-(const Float3& other) const
{
	return Float3{ x - other.x, y - other.y, z - other.z };
}

bool csc::Float3::similar(const Float3& other, const float margin) const
{
	const float diff_x{ fabs(x - other.x) };
	const float diff_y{ fabs(y - other.y) };
	const float diff_z{ fabs(z - other.z) };
	return diff_x < margin && diff_y < margin && diff_z < margin;
}

csc::Int2::Int2(const csc::Float2 float2) :
	x{ static_cast<int>(float2.x) },
	y{ static_cast<int>(float2.y) }
{

}

csc::Int2 csc::Int2::operator+(const Int2& other) const
{
	return Int2{ x + other.x, y + other.y };
}

csc::Int2 csc::Int2::operator-(const Int2& other) const
{
	return Int2{ x - other.x, y - other.y };
}

bool csc::Int2::operator<(const Int2& other) const
{
	if (x < other.x) return true;
	if (other.x < x) return false;

	if (y < other.y) return true;
	if (other.y < y) return false;

	return false;
}
