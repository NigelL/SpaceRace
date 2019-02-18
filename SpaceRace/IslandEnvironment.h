#ifndef ISLANDENVIRONMENT_H
#define ISLANDENVIRONMENT_H
#include "GameObject.h"

class IslandEnvironment : public GameObject
{
public:
	IslandEnvironment();
	IslandEnvironment::IslandEnvironment(Mesh* _curMesh, Vector3 _pos, float amt, Vector3 _rot, Vector3 _sca);
	~IslandEnvironment();
	void IslandEnvironment::OnCollide(GameObject& other);
};

#endif