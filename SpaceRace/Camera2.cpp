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
	position = -view * 100.0f;
	Vector3 right = view.Cross(up);
	right.y = 0;
	right.Normalize();
	this->up = defaultUp = right.Cross(view).Normalized();
}

void Camera2::Update(double dt)
{
	static const float CAMERA_SPEED = 50.f;

	view = target - position;
	right = view.Cross(up);
	
	
	
		if (Application::IsKeyPressed('A') && position.z <= 100.0f) {
			
			position = position - right * (float)dt;
			target = position + view;
		}
		if (Application::IsKeyPressed('D') && position.z >= -100.0f) {
			position = position + right * (float)dt;
			target = position + view;
			
		}
		if (Application::IsKeyPressed('W') && position.x >= -100.0f) {
			position = position - Vector3(20.0f, 0, 0) * (float)dt;
			target = position + view;
		}
		if (Application::IsKeyPressed('S') && position.x <= 100.0f) {
			position = position + Vector3(20.0f, 0, 0) * (float)dt;
			target = position + view;			
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