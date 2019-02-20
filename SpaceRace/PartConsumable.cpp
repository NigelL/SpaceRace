#include "PartConsumable.h"

void PartConsumable::SetPart(int _part) 
{
	partsPicked = _part;
}
int PartConsumable::GetPart() const 
{
	return partsPicked;
}

void PartConsumable::OnCollide(GameObject& other)
{
	Consumable::OnCollide(other);
	CShipStats* tryShip = static_cast<CShipStats*>(&other);
	//tryShip->Set(tryShip-() + healthConsumed);
}

PartConsumable::PartConsumable()
{
}

PartConsumable::PartConsumable(Mesh* _curMesh, Vector3 _pos, float amt, Vector3 _rot, Vector3 _sca) : Consumable(_curMesh, _pos, amt, _rot, _sca)
{
}

PartConsumable::~PartConsumable()
{
}
