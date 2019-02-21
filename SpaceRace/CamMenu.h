#ifndef CAMMENU_H
#define CAMMENU_H

#include "Camera.h"

class CamMenu : public Camera
{
public:
	Vector3 defaultPosition;
	Vector3 defaultTarget;
	Vector3 defaultUp;

	CamMenu();
	~CamMenu();
	virtual void Init(const Vector3& pos, const Vector3& target, const Vector3& up);
	virtual void Update(double dt);
	void SetPosition(float _x, float _y, float _z);
};

#endif