#ifndef SPEEDCONSUMABLE
#define SPEEDCONSUMABLE
#include "Consumable.h"
class SpeedConsumable : public Consumable
{
private:
	float speedIncreased;
	float timer;
public:
	void SetSpeed(float _speed);
	float GetSpeed() const;

	virtual void OnCollide(GameObject& other);
	void Timer(double dt);
	SpeedConsumable();
	SpeedConsumable(Mesh* _curMesh, Vector3 _pos, float amt, Vector3 _rot, Vector3 _sca);
	~SpeedConsumable();
};
#endif

