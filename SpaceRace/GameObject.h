#ifndef GAMEOBJECT
#define GAMEOBJECT

#include "Mesh.h"
#include "Utility.h"
#include <cmath>

class GameObject
{
private:
	float amt;
	float translateX, translateY, translateZ;
	float minX = 1e9, maxX = -1e9, minY = 1e9, maxY = -1e9, minZ = 1e9, maxZ = -1e9;
	Vector3 position, rotation, scale;
	Vector3 boxHalf;
	Mesh* curObject;
public:
	std::vector<GameObject*> collidedList;
	std::vector<GameObject*> collideCannon;
	Vector3 allBounds[8];

	GameObject();
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
	inline float GetTranslateX() const {
		return translateX;
	}
	void SetTranslateX(float _translateX) {
		translateX = _translateX;
	}
	inline float GetTranslateZ() const {
		return translateZ;
	}
	void SetTranslateZ(float _translateZ) {
		translateZ = _translateZ;
	}
	inline Vector3 GetBounds() const {
		return boxHalf;
	}
	GameObject(Mesh* _curObject);
	GameObject(Mesh* _curObject,Vector3 _pos,float deg,Vector3 _rotation ,Vector3 _scale);
	~GameObject();

	//void CheckCollision(GameObject& other);
	void CheckCollision(GameObject& other, std::vector<GameObject*>& list);
	void GenerateBounds();
	void SetBounds(Vector3 _half);

	void translateObj(int speed, double dt);
	void translateWater(float water);
	void translateCannon(int speed, double dt);
	void rotateObj(int speed);

	int health = 0;
};
#endif;
