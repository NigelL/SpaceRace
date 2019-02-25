#ifndef COLLISION
#define COLLISION

#include "Transform.h"
#include <vector>
#include "Vector3.h"
#include "Camera2.h"

#include "MyMath.h"


struct Collision {

	Transform* thisObj;
	std::vector<Transform*> collidedList;

	Vector3 boxHalf;


	enum COLLISIONTYPE {
		SHIP,
		CONSUMABLES
	};

	bool CheckCollision(Transform& other);
	Vector3 CheckCollision(Vector3 origin, Vector3 other, float t,Camera2 rot);

	





	Collision();




	~Collision();
};


#endif
