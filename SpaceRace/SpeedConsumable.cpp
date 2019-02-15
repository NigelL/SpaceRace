#include "SpeedConsumable.h"



void SpeedConsumable::SetSpeed(float _speed) {
	speedIncreased = _speed;
}

float SpeedConsumable::GetSpeed() const {
	return speedIncreased;
}

void SpeedConsumable::OnCollide(const GameObject& other) {
	CShipStats* curShip = static_cast<CShipStats*>(&other);
	curShip->SetSpeed(curShip->getSpeed() + speedIncreased);
}


SpeedConsumable::SpeedConsumable()
{
}


SpeedConsumable::~SpeedConsumable()
{
}
