#include "Utility.h"
Position operator*(const Mtx44& lhs, const Position& rhs)
{
	float b[4];
	for (int i = 0; i < 4; i++)
		b[i] = lhs.a[0 * 4 + i] * rhs.x + lhs.a[1 * 4 + i] * rhs.y + lhs.a[2 * 4 + i] * rhs.z + lhs.a[3 *
		4 + i] * 1;
	return Position(b[0], b[1], b[2]);
}

Vector3 operator*(const Mtx44& lhs, const Vector3& rhs)
{
	float b[4];
	for (int i = 0; i < 4; i++)
		b[i] = lhs.a[0 * 4 + i] * rhs.x + lhs.a[1 * 4 + i] * rhs.y + lhs.a[2 * 4 + i] * rhs.z + lhs.a[3 *
		4 + i] * 1;
	return Vector3(b[0], b[1], b[2]);
}


Vector4 operator*(const Mtx44*lhs, const Position& rhs) {

	return Vector4(0, 0, 0, 0);
}

float Deg2Rad(float deg) {
	return (3.14159f / 180.0f) * deg;
}
