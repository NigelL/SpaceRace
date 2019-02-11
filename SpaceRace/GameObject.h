#ifndef GAMEOBJECT
#define GAMEOBJECT

#include "Mesh.h"


class GameObject
{
private:
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



	GameObject(Mesh* _curObject);
	GameObject(Mesh* _curObject,Vector3 _pos,Vector3 _rotation ,Vector3 _scale);
	~GameObject();
};
#endif;
