#include "HealthConsumable.h"



void HealthConsumable::SetHealth(int _health) {
	healthConsumed = _health;
}
int HealthConsumable::GetHealth() const {
	return healthConsumed;
}

void HealthConsumable::OnCollide(GameObject& other) {
	Consumable::OnCollide(other);
	CShipStats* tryShip = dynamic_cast<CShipStats*>(&other);
	if (tryShip != nullptr) {
		
		tryShip->SetHealth(tryShip->getHealth() + healthConsumed);
		
	}
}

HealthConsumable::HealthConsumable()
{
	healthConsumed = 25;
}

HealthConsumable::HealthConsumable(Mesh* _curMesh, Vector3 _pos, float amt, Vector3 _rot, Vector3 _sca) : Consumable(_curMesh, _pos, amt, _rot, _sca)
{

}

HealthConsumable::~HealthConsumable()
{
}
