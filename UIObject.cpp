#include "UIObject.h"


UIObject::UIObject() {

}
UIObject::~UIObject() {

}

UIObject::UIObject(Mesh* _curMesh, float _size, float _posX, float _posY) {
	curMesh = _curMesh;
	size = _size;
	posX = _posX;
	posY = _posY;

}

