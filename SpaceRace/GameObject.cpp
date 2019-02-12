#include "GameObject.h"



GameObject::GameObject(Mesh* _curObject)
{
	curObject = _curObject;
}

GameObject::GameObject(Mesh* _curObject, Vector3 _position,float deg, Vector3 _rotation, Vector3 _scale)
{
	curObject = _curObject;
	position = _position;
	rotation = _rotation;
	scale = _scale;
	amt = deg;
}



GameObject::~GameObject()
{
	delete curObject;
}
