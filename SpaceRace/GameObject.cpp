#include "GameObject.h"
#include "GameScene.h"

GameObject::GameObject() {
	curObject = nullptr;
	collision.thisObj = &transform;

}

GameObject::GameObject(Mesh* _curObject)
{
	curObject = _curObject;
	collision.thisObj = &transform;
}

GameObject::GameObject(Mesh* _curObject, Vector3 _position, float deg, Vector3 _rotation, Vector3 _scale)
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
float GameObject::GetAmt() const {
	return transform.amt;
}
float GameObject::GetTranslateX() {
	return transform.translateX;
}
float GameObject::GetTranslateY() {
	return transform.translateY;
}
float GameObject::GetTranslateZ() {
	return transform.translateZ;
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
void GameObject::SetAmt(float _amt) {
	transform.amt = _amt;
}
void GameObject::SetTranslateX(float _translateX) {
	transform.translateX = _translateX;
}
void GameObject::SetTranslateY(float _translateY) {
	transform.translateY = _translateY;
}
void GameObject::SetTranslateZ(float _translateZ) {
	transform.translateZ = _translateZ;
}
Transform& GameObject::GetTransform() {
	return transform;
}
void GameObject::SetTransform(Transform _trans) {
	transform = _trans;
	collision.thisObj = &transform;
}

void GameObject::translateObj(int speed, double dt)
{
	float direction = transform.amt * 3.142f / 180.0f;
	objectDir = Vector3(sin(direction), 0, cos(direction));
	SetPosition(Vector3(GetPosition().x + (sin(direction) * ((float)(speed * dt))), GetPosition().y, GetPosition().z + (cos(direction) * ((float)(speed * dt)))));
}

void GameObject::translateWater(float water)
{
	if (water > 0.6 && transform.translateY < 0.25)
	{
		/*if (translateY > 0)
		{
			translateY += water / 50;
		}
		else
		{
			translateY += water / 100;
		}*/

		transform.translateY += 0.005;
	}
	else if (water < 0.6  && transform.translateY > -0.25)
	{
		/*if (translateY < 0)
		{
			translateY += water / 50;
		}
		else
		{
			translateY += water / 100;
		}*/

		transform.translateY -= 0.005;
	}

	std::cout << " water : " << water << std::endl;

	std::cout << " Y : " << transform.translateY << std::endl;

	if (water > 0.5)
	{
		SetPosition(Vector3(GetPosition().x, -(water / 20), GetPosition().z));
	}
	else if (water < 0.5)
	{
		SetPosition(Vector3(GetPosition().x, water / 20, GetPosition().z));
	}
}

void GameObject::translateCannon(int speed, double dt)
{
	transform.translateX += (float)(speed * dt);
	transform.translateZ += (float)(speed * dt);
}

void GameObject::rotateObj(int speed)
{
	transform.amt += (int)(speed); // int because rotation need exact integer number

	if (transform.amt >= 360)
	{
		transform.amt = 0;
	}
	else if (transform.amt <= 0)
	{
		transform.amt = 360;
	}

	SetRotation(Vector3(0, 1, 0), transform.amt);
}

GameObject::~GameObject()
{
	delete curObject;
}