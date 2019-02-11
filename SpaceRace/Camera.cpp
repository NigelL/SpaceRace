#include "Camera.h"

/******************************************************************************/
/*!
\brief
Default constructor
*/
/******************************************************************************/
Camera::Camera()
{
}

/******************************************************************************/
/*!
\brief
Destructor
*/
/******************************************************************************/
Camera::~Camera()
{
}

/******************************************************************************/
/*!
\brief
Initialize camera

\param pos - position of camera
\param target - where the camera is looking at
\param up - up vector of camera
*/
/******************************************************************************/
void Camera::Init(const Vector3& pos, const Vector3& target, const Vector3& up)
{
	this->position = pos;
	this->target = target;
	this->up = up;
}

/******************************************************************************/
/*!
\brief
Reset the camera settings
*/
/******************************************************************************/
void Camera::Reset()
{
}

/******************************************************************************/
/*!
\brief
To be called every frame. Camera will get user inputs and update its position and orientation

\param dt - frame time
*/
/******************************************************************************/
double DegreeToRadian(double value)
{
	return value * 3.142 / 180.0f;
}


static const float CAMERA_SPEED = 100;
static double theta, phi,radius = 1;
float rotateAngle;

void Camera::Update(double dt) {
	
	Vector3 view = target - position;	
	Vector3 right = view.Cross(up);
	Vector3 forward = right.Cross(up);
	
	//std::cout << "Forward Vector : " << forward.Normalized() << std::endl;
	
	//rotateAngle += dt * 10.0;
	if (Application::IsKeyPressed('D'))
	{
		theta += CAMERA_SPEED * dt;			
		/*	
		Mtx44 rotation;		
		rotation.SetToRotation(rotateAngle, up.x, up.y, up.z);
		position = rotation * position;
		std::cout << position << std::endl;
		*/
	}
	else if (Application::IsKeyPressed('A'))
	{
		theta -= CAMERA_SPEED * dt;
		

		/*
		Mtx44 rotation;
		rotation.SetToRotation(-rotateAngle, up.x, up.y, up.z);
		position = rotation * position;
		*/
	}
	else if (Application::IsKeyPressed('W'))
	{
		phi += CAMERA_SPEED * dt;
	

		/*
		Mtx44 rotation;
		rotation.SetToRotation(-rotateAngle, right.x, right.y, right.z);
		position = rotation * position;
		*/
	}
	else if (Application::IsKeyPressed('S'))
	{
		phi -= CAMERA_SPEED * dt;
	

		/*
		Mtx44 rotation;
		rotation.SetToRotation(rotateAngle, right.x, right.y, right.z);
		position = rotation * position;
		*/
	}
	if (Application::IsKeyPressed('E'))
	{
		radius += CAMERA_SPEED * dt;
	}
	if (Application::IsKeyPressed('Q'))
	{
		radius -= CAMERA_SPEED * dt;
	}


	position.x = (float)(radius * cos(DegreeToRadian(phi)) * cos(DegreeToRadian(theta)));
	position.y = (float)(radius * sin(DegreeToRadian(phi)));
	position.z = (float)(radius * cos(DegreeToRadian(phi)) * sin(DegreeToRadian(theta)));
	
}

void Camera::Update(double dt,short int animType)
{
	
	
	if (animType == 0) {
		target = Vector3(0, 1.0f, 0);
		phi = 0;
		theta = 0;
		radius = 15;
	}
	else if (animType == 2) {



		if (Application::IsKeyPressed('D'))
		{
			theta += CAMERA_SPEED * dt;
		}
		if (Application::IsKeyPressed('A'))
		{
			theta -= CAMERA_SPEED * dt;
		}
		if (Application::IsKeyPressed('W'))
		{
			phi += CAMERA_SPEED * dt;
		}
		if (Application::IsKeyPressed('S'))
		{
			phi -= CAMERA_SPEED * dt;
		}
		if (Application::IsKeyPressed('E'))
		{
			radius += CAMERA_SPEED * dt;
		}
		if (Application::IsKeyPressed('Q'))
		{
			radius -= CAMERA_SPEED * dt;
		}
	

	}
	else if (animType == 1) {
		radius = 25;
		phi = 15.0;
		theta = 0;
	}

	position.x =(float) (radius * cos(DegreeToRadian(phi)) * cos(DegreeToRadian(theta)));
	position.y = (float)(radius * sin(DegreeToRadian(phi)));
	position.z = (float)(radius * cos(DegreeToRadian(phi)) * sin(DegreeToRadian(theta)));
}