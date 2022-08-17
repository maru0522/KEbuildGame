#include "Vector3.h"
#include <cmath>

// 引数なしコンストラクタ
// 零ベクトルに初期化する
Vector3::Vector3()
	: x(0), y(0), z(0)
{
}

// 引数3つのコンストラクタ
Vector3::Vector3(float x, float y, float z)
	: x(x), y(y), z(z)
{
}

// ベクトルの長さを求める
float Vector3::length() const
{
	return sqrt(x * x + y * y + z * z);
}

// ベクトルを正規化する(単位ベクトルに)する
Vector3& Vector3::normalize()
{
	float len = length();
	if (len != 0) {
		return *this /= len;
	}
	return *this;
}

// 内積を求める
float Vector3::dot(const Vector3& v) const
{
	return x * v.x + y * v.y + z * v.z;
}

// 外積を求める
Vector3 Vector3::cross(const Vector3& v)const
{
	return Vector3(y * v.z - z * v.y,
				   z * v.x - x * v.z,
				   x * v.y - y * v.x);
}

// 単項演算子 +
Vector3 Vector3::operator+() const
{
	return *this;
}
// 単項演算子 -
Vector3 Vector3::operator-() const
{
	return Vector3(-x, -y, -z);
}

// 代入演算子 +=
Vector3& Vector3::operator+=(const Vector3& v)
{
	x += v.x;
	y += v.y;
	z += v.z;
	return *this;
}
// 代入演算子 -=
Vector3& Vector3::operator-=(const Vector3& v)
{
	x -= v.x;
	y -= v.y;
	z -= v.z;
	return *this;
}
// 代入演算子 *=
Vector3& Vector3::operator*=(float s)
{
	x *= s;
	y *= s;
	z *= s;
	return *this;
}
// 代入演算子 /=
Vector3& Vector3::operator/=(float s)
{
	x /= s;
	y /= s;
	z /= s;
	return *this;
}

// 2項演算子 +
const Vector3 operator+(const Vector3& v1, const Vector3& v2)
{
	Vector3 temp(v1);
	return temp += v2;
}
// 2項演算子 -
const Vector3 operator-(const Vector3& v1, const Vector3& v2)
{
	Vector3 temp(v1);
	return temp -= v2;
}
const Vector3 operator*(const Vector3& v, float s)
{
	Vector3 temp(v);
	return temp *= s;
}
const Vector3 operator/(const Vector3& v, float s)
{
	Vector3 temp(v);
	return temp /= s;
}