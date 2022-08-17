#pragma once
#include "Vector3.h"
#include "Vector4.h"
#include "Matrix4.h"

namespace Calc {
	Vector3& DotVecMat(const Vector3 vec3, const Matrix4 worldTransform);
}