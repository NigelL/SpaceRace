#ifndef CAMERA_3_H
#define CAMERA_3_H

#include "Camera.h"
#include "GameObject.h"


class Camera3 : public Camera
{
private:
	GameObject* cameraSkyBox;
	Vector3* cameraSkiesOffset;
	int skyBoxSize;

public:
	Vector3 defaultPosition;
	Vector3 defaultTarget;
	Vector3 defaultUp;


	Camera3();
	~Camera3();
	virtual void Init(const Vector3& pos, const Vector3& target, const Vector3& up);
	//void SetTarget(Vector3 _target);
	void SetTarget(float _x, float _y, float _z);
	void SetCameraSkyBox(GameObject*, int);
	void UpdateCameraSkyBox();
	void SetPosition(float _x, float _y, float _z);
	int GetSkyBoxSize() const;
	GameObject* GetAllSkyBox();
	virtual void Update(double dt);
};

#endif