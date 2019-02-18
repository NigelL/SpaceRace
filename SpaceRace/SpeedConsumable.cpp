#include "SpeedConsumable.h"



void SpeedConsumable::SetSpeed(float _speed) {
	speedIncreased = _speed;
}

float SpeedConsumable::GetSpeed() const {
	return speedIncreased;
}

void SpeedConsumable::OnCollide(GameObject& other) {
	CShipStats* curShip = static_cast<CShipStats*>(&other);
	curShip->SetSpeed(curShip->getSpeed() + speedIncreased);
}


SpeedConsumable::SpeedConsumable()
{
}

SpeedConsumable::SpeedConsumable(Mesh* _curMesh, Vector3 _pos, float amt, Vector3 _rot, Vector3 _sca) : Consumable(_curMesh, _pos, amt, _rot, _sca)
{
}

SpeedConsumable::~SpeedConsumable()
{
}
