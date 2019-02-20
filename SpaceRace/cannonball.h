#ifndef CANNONBALL_H
#define CANNONBALL_H

#include "GameObject.h"


class cannonball : public GameObject
{
public:
	cannonball();
	~cannonball();
	cannonball::cannonball(Mesh* _curMesh, Vector3 _pos, float amt, Vector3 _rot, Vector3 _sca);
	virtual void OnCollide(GameObject& other);
};

#endif