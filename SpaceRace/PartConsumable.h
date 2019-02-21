#ifndef PARTCONSUMABLE
#define PARTCONSUMABLE
#include "Consumable.h"



class PartConsumable : public Consumable
{
private:
	int partsPicked;
public:
	virtual void OnCollide(GameObject& other);
	void SetPart(int _part);
	int GetPart() const;
	PartConsumable();
	PartConsumable(Mesh* _curMesh, Vector3 _pos, float amt, Vector3 _rot, Vector3 _sca);
	~PartConsumable();
};
#endif

