#ifndef CONSUMABLE
#define CONSUMABLE

#include "CShipStats.h"


class Consumable : public GameObject
{
private:
	
public:
	virtual void OnCollide(GameObject& other);

	Consumable();
	Consumable(Mesh* _curMesh, Vector3 _pos, float amt, Vector3 _rot, Vector3 _sca);
	~Consumable();
};

#endif