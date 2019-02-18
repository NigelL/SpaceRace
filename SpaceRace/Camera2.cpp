#include "Camera2.h"
#include "Application.h"
#include "Mtx44.h"

Camera2::Camera2()
{
}

Camera2::~Camera2()
{
}

void Camera2::Init(const Vector3& pos, const Vector3& target, const Vector3& up)
{
	this->position = defaultPosition = pos;
	this->target = defaultTarget = target;
	Vector3 view = (target - position).Normalized();
	position = -view * 2.0f;
	Vector3 right = view.Cross(up);
	right.y = 0;
	right.Normalize();
	this->up = defaultUp = right.Cross(view).Normalized();
}

void Camera2::Update(double dt)
{
	static const float CAMERA_SPEED = 2.5f;

	view = target - position;
	right = view.Cross(up);
	
	
	
		if (Application::IsKeyPressed('A')) {
			Mtx44 rot;
			rot.SetToRotation(1.0f * CAMERA_SPEED, up.x, up.y, up.z);
			position = rot * position;
			//target = position + view;
		}
		if (Application::IsKeyPressed('D')) {
			Mtx44 rot;
			rot.SetToRotation(-1.0f * CAMERA_SPEED, up.x, up.y, up.z);
			position = rot * position;
			//target = position + view;
			
		}
		if (Application::IsKeyPressed('W')) {
			Mtx44 rot;
			rot.SetToRotation(1.0f * CAMERA_SPEED, right.x, right.y, right.z);
			position = rot * position;
			//target = position + view;
		}
		if (Application::IsKeyPressed('S')) {
			Mtx44 rot;
			rot.SetToRotation(-1.0f * CAMERA_SPEED, right.x, right.y, right.z);
			position = rot * position;
		}

	
	if(Application::IsKeyPressed('N'))
	{
		Vector3 direction = target - position;
		if(direction.Length() > 5)
		{
			Vector3 view = (target - position).Normalized();
			position += view * (float)(10.f * dt);
		}
	}
	if(Application::IsKeyPressed('M'))
	{
		Vector3 view = (target - position).Normalized();
		position -= view * (float)(10.f * dt);
	}
	if(Application::IsKeyPressed('R'))
	{
		Reset();
	}
}

void Camera2::Reset()
{
	position = defaultPosition;
	target = defaultTarget;
	up = defaultUp;
}