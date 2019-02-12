#ifndef GAMEOBJECT
#define GAMEOBJECT

#include "Mesh.h"


class GameObject
{
private:
	float amt;
	Vector3 position,rotation,scale;

	Mesh* curObject;
public:
	inline Mesh* GetMesh() const {
		return curObject;
	}
	inline Vector3 GetPosition() const {
		return position;
	}
	inline Vector3 GetRotation() const {
		return rotation;
	}
	inline Vector3 GetScale() const {
		return scale;
	}
	inline void SetPosition(Vector3 pos) {
		position = pos;
	}
	inline void SetRotation(Vector3 rot) {
		rotation = rot;
	}
	inline void SetScale(Vector3 scal)  {
		scale = scal;
	}
	inline float GetAmt() const {
		return amt;
	}
	inline void SetAmt(float _amt) {
		amt = _amt;
	}


	GameObject(Mesh* _curObject);
	GameObject(Mesh* _curObject,Vector3 _pos,float deg,Vector3 _rotation ,Vector3 _scale);
	~GameObject();
};
#endif;
