#include "GameObject.h"



GameObject::GameObject() {
	curObject = nullptr;
	collision.thisObj = &transform;
	
}

GameObject::GameObject(Mesh* _curObject)
{
	curObject = _curObject;
	collision.thisObj = &transform;
}



GameObject::GameObject(Mesh* _curObject, Vector3 _position,float deg, Vector3 _rotation, Vector3 _scale)
{
	curObject = _curObject;
	collision.thisObj = &transform;
	transform.position = _position;
	transform.rotation = _rotation;
	transform.scale = _scale;
	transform.amt = deg;
	transform.name = curObject->name;
	

}




 Mesh* GameObject::GetMesh() const {
	return curObject;
}
Vector3 GameObject::GetPosition() const {
	return transform.position;
}
Vector3 GameObject::GetRotation() const {
	return transform.rotation;
}
Vector3 GameObject::GetScale() const {
	return transform.scale;
}
void GameObject::SetPosition(const Vector3& pos) {
	transform.position = pos;
}
void GameObject::SetRotation(Vector3 rot, float _amt) {
	transform.rotation = rot;
	transform.amt = _amt;
}
void GameObject::SetScale(Vector3 scal) {
	transform.scale = scal;
}
Transform& GameObject::GetTransform() {
	return transform;
}

void GameObject::SetTransform(Transform _trans) {
	transform = _trans;
	collision.thisObj = &transform;

}

float GameObject::GetAmt() const {
	return transform.amt;
}
void GameObject::SetAmt(float _amt) {
	transform.amt = _amt;
}


GameObject::~GameObject()
{
	delete curObject;
}