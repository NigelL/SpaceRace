#ifndef HEALTHCONSUMABLE
#define HEALTHCONSUMABLE

#include "CShipStats.h"
#include "Consumable.h"


class HealthConsumable : public Consumable
{
private:
	int healthConsumed;
public:
	virtual void OnCollide(const GameObject& other);
	void SetHealth(int _health);
	int GetHealth() const;
	HealthConsumable();

	~HealthConsumable();
};
#endif

