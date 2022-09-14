#include "Calc.h"

Vector3& Calc::DotVecMat(const Vector3 vec3, const Matrix4 worldTransform) {
	Vector4 vec4 = { vec3.x, vec3.y, vec3.z, 0 };
	Vector3 result;
	result.x = vec4.x * worldTransform.m[0][0] + vec4.y * worldTransform.m[1][0] + vec4.z * worldTransform.m[2][0] + vec4.w * worldTransform.m[3][0];
	result.y = vec4.x * worldTransform.m[0][1] + vec4.y * worldTransform.m[1][1] + vec4.z * worldTransform.m[2][1] + vec4.w * worldTransform.m[3][1];
	result.z = vec4.x * worldTransform.m[0][2] + vec4.y * worldTransform.m[1][2] + vec4.z * worldTransform.m[2][2] + vec4.w * worldTransform.m[3][2];
	return result;
}

float Calc::ConvertToRadian(const int32_t angle)
{
	return angle * PI / 180 ;
}

float Calc::ConvertToAngle(const int32_t rad)
{ 
	return 180 * rad;
}

int32_t Calc::TransAngle(const int32_t angle)
{
	return angle + 360;
}

int32_t Calc::Trans360(const int32_t angle)
{
	if (angle == 360)
		return 0;
}

double Calc::EaseOutSine(double num)
{
	return sin((num * PI) / 2);
}
