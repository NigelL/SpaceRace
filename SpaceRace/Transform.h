#ifndef TRANSFORM
#define TRANSFORM

#include "Vector3.h"
#include <string>
#include "MyMath.h"
#include "Mtx44.h"

class Transform
{
private:
public:
	std::string name;
	Vector3 boxHalf;

	float minX = 1e9, maxX = -1e9, minY = 1e9, maxY = -1e9, minZ = 1e9, maxZ = -1e9;

	enum TYPE {
		SHIP,
		CONSUMABLESHIP
	};

	int type = 0;

	float amt;
	Vector3 allBounds[8];
	Vector3 position, rotation, scale;

	void GenerateBounds();
	Vector3 GetBounds() const;
	void SetBounds(Vector3 _half);

	Transform& operator=(Transform& other);

	Transform();
	~Transform();
};
#endif

