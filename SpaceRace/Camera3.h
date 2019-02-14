#ifndef CAMERA_3_H
#define CAMERA_3_H

#include "Camera.h"

class Camera3 : public Camera
{
public:
	Vector3 defaultPosition;
	Vector3 defaultTarget;
	Vector3 defaultUp;


	Camera3();
	~Camera3();
	virtual void Init(const Vector3& pos, const Vector3& target, const Vector3& up);
	virtual void Init2(const Vector3& pos, const Vector3& target, const Vector3& up);
	//void SetTarget(Vector3 _target);
	void SetTarget(float _x, float _y, float _z);
	void SetPosition(float _x, float _y, float _z);
	virtual void Update(double dt);
};

#endif