#include "HealthConsumable.h"



void HealthConsumable::SetHealth(int _health) {
	healthConsumed = _health;
}
int HealthConsumable::GetHealth() const {
	return healthConsumed;
}

void HealthConsumable::OnCollide(const GameObject& other) {
	CShipStats* tryShip = static_cast<CShipStats*>(&other);
	tryShip->SetHealth(tryShip->getHealth() + healthConsumed);
}

HealthConsumable::HealthConsumable()
{
	healthConsumed = 25;
}


HealthConsumable::~HealthConsumable()
{
}
