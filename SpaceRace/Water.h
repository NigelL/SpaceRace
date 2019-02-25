#ifndef WATER
#define WATER

#include "Utility.h"
#include "Mesh.h"

class Water 
{
public:
	Mesh* waterMesh;
	float slowDelay = 0.01f;
	void UpdateWater(int lengthX,float dt);
	float getWater();

	Water();
	~Water();
};

#endif
