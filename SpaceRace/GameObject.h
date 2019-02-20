#ifndef GAMEOBJECT
#define GAMEOBJECT

#include "Mesh.h"
#include "Collision.h"
#include "Utility.h"
#include <cmath>

class GameObject
{
private:
	Transform transform;
public:
	Mesh* curObject;

	Collision collision;


	Mesh* GetMesh() const;
	Vector3 GetPosition() const;
	Vector3 GetRotation() const;
	Vector3 GetScale() const;
	float GetAmt() const;
	Transform& GetTransform();

	
	 void SetPosition(const Vector3& pos);
	void SetRotation(Vector3 rot, float _amt);
	void SetScale(Vector3 scal);
	void SetAmt(float _amt);
	void SetTransform(Transform _trans);


	float translateX = 0;
	float translateY = 0;
	float translateZ = 0;
	float scaleObject = 0;
	float rotate = 90;


	GameObject();
	float x = 0;
	float y = 0;
	float z = 0;

	GameObject(Mesh* _curObject);
	GameObject(Mesh* _curObject,Vector3 _pos,float deg,Vector3 _rotation ,Vector3 _scale);
	virtual ~GameObject();
};
#endif;
