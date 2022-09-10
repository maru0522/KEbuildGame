#include "Vector3.h"
#include <cmath>

// �����Ȃ��R���X�g���N�^
// ��x�N�g���ɏ���������
Vector3::Vector3()
	: x(0), y(0), z(0)
{
}

// ����3�̃R���X�g���N�^
Vector3::Vector3(float x, float y, float z)
	: x(x), y(y), z(z)
{
}

// �x�N�g���̒��������߂�
float Vector3::length() const
{
	return sqrt(x * x + y * y + z * z);
}

// �x�N�g���𐳋K������(�P�ʃx�N�g����)����
Vector3& Vector3::normalize()
{
	float len = length();
	if (len != 0) {
		return *this /= len;
	}
	return *this;
}

// ���ς����߂�
float Vector3::dot(const Vector3& v) const
{
	return x * v.x + y * v.y + z * v.z;
}

// �O�ς����߂�
Vector3 Vector3::cross(const Vector3& v)const
{
	return Vector3(y * v.z - z * v.y,
				   z * v.x - x * v.z,
				   x * v.y - y * v.x);
}

// �P�����Z�q +
Vector3 Vector3::operator+() const
{
	return *this;
}
// �P�����Z�q -
Vector3 Vector3::operator-() const
{
	return Vector3(-x, -y, -z);
}

// ������Z�q +=
Vector3& Vector3::operator+=(const Vector3& v)
{
	x += v.x;
	y += v.y;
	z += v.z;
	return *this;
}
// ������Z�q -=
Vector3& Vector3::operator-=(const Vector3& v)
{
	x -= v.x;
	y -= v.y;
	z -= v.z;
	return *this;
}
// ������Z�q *=
Vector3& Vector3::operator*=(float s)
{
	x *= s;
	y *= s;
	z *= s;
	return *this;
}
// ������Z�q /=
Vector3& Vector3::operator/=(float s)
{
	x /= s;
	y /= s;
	z /= s;
	return *this;
}

const bool& Vector3::operator==(const Vector3& v1)
{
	return x == v1.x && y == v1.y && z == v1.z;
}

// 2�����Z�q +
const Vector3 operator+(const Vector3& v1, const Vector3& v2)
{
	Vector3 temp(v1);
	return temp += v2;
}
// 2�����Z�q -
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