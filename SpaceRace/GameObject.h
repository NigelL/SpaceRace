#ifndef GAMEOBJECT
#define GAMEOBJECT

#include "Mesh.h"


class GameObject
{
private:
	float amt;
	Vector3 position,rotation,scale;
	Vector3 allBounds[8];
	Vector3 boxHalf;
	float minX = 1e9, maxX = -1e9, minY = 1e9, maxY = -1e9, minZ = 1e9, maxZ = -1e9;
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
	inline void SetRotation(Vector3 rot, float _amt) {
		rotation = rot;
		amt = _amt;
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
	void SetBounds(Vector3 _half) {
		boxHalf = _half;
	}

	void CheckCollision(GameObject& other);
	void GenerateBounds();

	float translateX = 0;
	float translateY = 0;
	float translateZ = 0;
	float scaleObject = 0;
	float rotate = 90;

	GameObject(Mesh* _curObject);
	GameObject(Mesh* _curObject,Vector3 _pos,float deg,Vector3 _rotation ,Vector3 _scale);
	~GameObject();
};
#endif;
