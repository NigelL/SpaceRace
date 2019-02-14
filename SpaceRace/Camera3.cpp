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

void Camera3::Init2(const Vector3& pos, const Vector3& target, const Vector3& up)
{
	this->position = defaultPosition = pos;
	this->target = defaultTarget = target;
	this->up = defaultUp = up;
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
