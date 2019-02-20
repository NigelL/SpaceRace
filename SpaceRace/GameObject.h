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
	float GetTranslateX();
	float GetTranslateY();
	float GetTranslateZ();
	Transform& GetTransform();
	
	void SetPosition(const Vector3& pos);
	void SetRotation(Vector3 rot, float _amt);
	void SetScale(Vector3 scal);
	void SetAmt(float _amt);
	void SetTranslateX(float _translateX);
	void SetTranslateY(float _translateY);
	void SetTranslateZ(float _translateZ);
	void SetTransform(Transform _trans);


	void translateObj(int speed, double dt);
	void translateWater(float water);
	void translateCannon(int speed, double dt);
	void rotateObj(int speed);

	GameObject();
	GameObject(Mesh* _curObject);
	GameObject(Mesh* _curObject,Vector3 _pos,float deg,Vector3 _rotation ,Vector3 _scale);
	virtual ~GameObject();

	int health = 0;
};
#endif;
