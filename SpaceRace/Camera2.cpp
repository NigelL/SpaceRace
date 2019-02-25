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



void Camera2::UpdateUI(double dt, UIObject& uiObj) {

	static const float CAMERA_SPEED = 2.5f;
	static const float CAMERA_SENSITIVITY = 500.0f;

	double newMouseX, newMouseY;
	Application::GetMousePos(newMouseX, newMouseY);
	Vector3 mouseAxis = Vector3(newMouseX - prevMouseX, newMouseY - prevMouseY, 0);
	//mouseAxis.x = Math::Clamp<float>(mouseAxis.x, -1.0f, 1.0f);
	//mouseAxis.y = Math::Clamp<float>(mouseAxis.y, -1.0f, 1.0f);

	mouseAxis.x *= 0.01f;
	mouseAxis.y *= 0.01f;

	std::cout << mouseAxis << std::endl;

	uiObj.posX += mouseAxis.x * 10.0f;
	uiObj.posY -= mouseAxis.y * 10.0f;

	Application::GetMousePos(prevMouseX, prevMouseY);

}

void Camera2::Update(double dt)
{

	view = target - position;
	right = view.Cross(up);

	static const float CAMERA_SPEED = 2.5f;
	static const float CAMERA_SENSITIVITY = 500.0f;

	double newMouseX, newMouseY;
	Application::GetMousePos(newMouseX, newMouseY);
	Vector3 mouseAxis = Vector3(newMouseX - prevMouseX, newMouseY - prevMouseY,0);
	//mouseAxis.x = Math::Clamp<float>(mouseAxis.x, -1.0f, 1.0f);
	//mouseAxis.y = Math::Clamp<float>(mouseAxis.y, -1.0f, 1.0f);

	mouseAxis.x *= 0.01f;
	mouseAxis.y *= 0.01f;
	if (mouseAxis.x != 0 && mouseAxis.y != 0) {
		Vector3 normaliseThis = Vector3(mouseAxis.x, mouseAxis.y, 0).Normalized();
		//mouseAxis.x = normaliseThis.x;
		//mouseAxis.y = normaliseThis.y;
	}


	if (Application::IsKeyPressed('A')) {

		position = position - (right * CAMERA_SPEED * dt);
		target = position + view;
	}
	if (Application::IsKeyPressed('D')) {
		position = position + (right * CAMERA_SPEED * dt);

		target = position + view;

	}
	if (Application::IsKeyPressed('W')) {

		position = position + (view * CAMERA_SPEED * dt);
		target = position + view;
	}
	if (Application::IsKeyPressed('S')) {
		position = position - (view * CAMERA_SPEED * dt);
		target = position + view;

	}


	if (mouseAxis.x != 0){
		Mtx44 rotY;
		rotY.SetToRotation(-mouseAxis.x * CAMERA_SENSITIVITY * dt, up.x, up.y, up.z);
		view = rotY * view;
		target = position + view;

	}if (mouseAxis.y != 0) {
		Mtx44 rotY;
		rotY.SetToRotation(-mouseAxis.y * CAMERA_SENSITIVITY * dt,right.x, right.y, right.z);
		view = rotY * view;
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
	glfwSetCursorPos(Application::getGLFWWindow(), 1920 / 2, 1440 / 2);
	Application::GetMousePos(prevMouseX, prevMouseY);

}

void Camera2::Reset()
{
	position = defaultPosition;
	target = defaultTarget;
	up = defaultUp;
}