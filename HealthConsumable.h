#ifndef HEALTHCONSUMABLE
#define HEALTHCONSUMABLE
#include "Consumable.h"
//#include "GameSound.h"



class HealthConsumable : public Consumable
{
private:
	int healthConsumed;
public:
	virtual void OnCollide(GameObject& other);
	void SetHealth(int _health);
	int GetHealth() const;
	HealthConsumable();
	HealthConsumable(Mesh* _curMesh, Vector3 _pos, float amt, Vector3 _rot, Vector3 _sca);
	~HealthConsumable();
};
#endif

