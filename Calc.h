#pragma once
#include "Vector3.h"
#include "Vector4.h"
#include "Matrix4.h"
#include "MathUtility.h"
#include <stdint.h>
#include <cmath>

namespace Calc {
	const float PI = 3.141592654f;

	Vector3& DotVecMat(const Vector3 vec3, const Matrix4 worldTransform);

	float ConvertToRadian(const int32_t angle);
	float ConvertToAngle(const int32_t rad);

	int32_t TransAngle(const int32_t angle);
	int32_t Trans360(const int32_t angle);

	double EaseOutSine(double num);
}