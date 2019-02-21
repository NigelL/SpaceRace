#include "Camera3.h"
#include "Application.h"
#include "Mtx44.h"

Camera3::Camera3()
{
}

Camera3::~Camera3()
{
}

void Camera3::Init(const Vector3& pos, const Vector3& target, const Vector3& up)
{
	this->position = defaultPosition = pos;
	this->target = defaultTarget = target;
	this->up = defaultUp = up;
}

int Camera3::GetSkyBoxSize() const {
	return skyBoxSize;
}

void Camera3::SetTarget(float _x, float _y, float _z)
{
	target.x = _x;
	target.y = _y;
	target.z = _z;
}

void Camera3::SetPosition(float _x, float _y, float _z)
{
	position.x = _x;
	position.y = _y;
	position.z = _z;
}


void Camera3::SetCameraSkyBox(GameObject* skies, int size) {
	cameraSkiesOffset = new Vector3[size];
	cameraSkyBox = skies;
	skyBoxSize = size;

	for (int i = 0; i < size; i++) {
		if (skies[i].GetPosition().Length() == abs(skies[i].GetPosition().x) || skies[i].GetPosition().Length() == abs(skies[i].GetPosition().y) || skies[i].GetPosition().Length() == abs(skies[i].GetPosition().z)) {
			cameraSkiesOffset[i] = skies[i].GetPosition();
		}
	}
}
void Camera3::UpdateCameraSkyBox(GameObject* curObj,GameObject* otherObj) {

	//Vector3 avgCamPos = (curObj->GetPosition() + otherObj->GetPosition()) * 0.5f;

	for (int i = 0; i < skyBoxSize; i++) {
		cameraSkyBox[i].SetPosition(position + cameraSkiesOffset[i]);
	
	}
}

GameObject* Camera3::GetAllSkyBox() {
	return cameraSkyBox;
}

void Camera3::Update(double dt)
{

	static const float CAMERA_SPEED = 50.f;
	Vector3 view = (target - position).Normalized();
	Vector3 right = view.Cross(up);

	if (Application::IsKeyPressed('A'))
	{
		position = position - 2 * right;
		target = position + view;
	}
	if (Application::IsKeyPressed('D'))
	{
		position = position + 2 * right;
		target = position + view;
	}
	if (Application::IsKeyPressed('Q'))
	{
		position = position + 2 * up;
		target = position + view;
	}
	if (Application::IsKeyPressed('E'))
	{
		position = position - 2 * up;
		target = position + view;
	}
	if (Application::IsKeyPressed('W'))
	{
		position = position + view;
		target = target + view;
	}
	if (Application::IsKeyPressed('S'))
	{
		position = position - view;
		target = target - view;
	}
}
