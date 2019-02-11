#include "GameObject.h"



GameObject::GameObject(Mesh* _curObject)
{
	curObject = _curObject;
}

GameObject::GameObject(Mesh* _curObject, Vector3 _position, Vector3 _rotation, Vector3 _scale)
{
	curObject = _curObject;
	position = _position;
	rotation = _rotation;
	scale = _scale;
}



GameObject::~GameObject()
{
	delete curObject;
}
