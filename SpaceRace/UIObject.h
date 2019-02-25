#ifndef UIOBJECTH
#define UIOBJECTH
#include "Mesh.h"

struct UIObject {
	Mesh* curMesh;
	float size, posX, posY;
	UIObject();
	UIObject(Mesh* _curMesh, float _size, float _posX, float _posY);
	~UIObject();
		
	
};
#endif