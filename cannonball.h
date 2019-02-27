#ifndef CANNONBALL_H
#define CANNONBALL_H
#include "GameObject.h"

class cannonball : public GameObject
{
public:
	Vector3 dir;
	float speed;
	int playerType = 0;

	cannonball();
	~cannonball();
	cannonball(Mesh* _curMesh, Vector3 _pos, float amt, Vector3 _rot, Vector3 _sca);
	void Update();
	virtual void OnCollide(GameObject& other);
};

#endif