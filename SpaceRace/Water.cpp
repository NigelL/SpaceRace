#include "Water.h"

static float curDeg = 0.0f;


Water::Water()
{
}



void Water::UpdateWater(int lengthX,float dt) {
	if (slowDelay <= 0) {
		for (int i = 0; i < waterMesh->allVertices.size(); i++) {
<<<<<<< HEAD
			curDeg += ((rand() % 5) * dt);
			if (curDeg >= 720.0f) { curDeg = 0.0f; }
			waterMesh->allVertices[i].pos.Set(waterMesh->allVertices[i].pos.x, waterMesh->allVertices[i].pos.y + sin(Deg2Rad(curDeg)) * 0.01f, waterMesh->allVertices[i].pos.z);
=======
			curDeg += (10 * dt);
			if (curDeg >= 720.0f) { curDeg = 0.0f; }
			waterMesh->allVertices[i].pos.Set(waterMesh->allVertices[i].pos.x, sin(Deg2Rad(curDeg)), waterMesh->allVertices[i].pos.z);
>>>>>>> b3cf0aee085bd0ad9328ade669ba24727a0cc3d5
		}
		
		waterMesh->UpdateVertices();
		slowDelay = 0.25f;
	}
	else {
		slowDelay -= 2.5f * dt;		
	}
}


Water::~Water()
{
	delete waterMesh;
}
