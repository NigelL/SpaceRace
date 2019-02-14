#ifndef UTILITY
#define UTILITY
#include "MatrixStack.h"
#include "Vertex.h"

class Vector4 {
public:
	int x, y, z, w;
	Vector4(int _x, int _y, int _z, int _w) :x(_x), y(_y), z(_z), w(_w) {

	}

};

Vector4 operator*(const Mtx44*lhs, const Position& rhs);
Position operator*(const Mtx44& lhs, const Position& rhs);
float Deg2Rad(float deg);

#endif
