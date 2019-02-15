#ifndef SPEEDCONSUMABLE
#define SPEEDCONSUMABLE

#include "CShipStats.h"
#include "Consumable.h"
class SpeedConsumable : public Consumable
{
private:
	float speedIncreased;
public:
	void SetSpeed(float _speed);
	float GetSpeed() const;

	virtual void OnCollide(const GameObject& other);
	SpeedConsumable();
	~SpeedConsumable();
};
#endif

