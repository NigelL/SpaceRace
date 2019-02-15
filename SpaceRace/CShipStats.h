#ifndef CSHIPSTATS_H
#define CSHIPSTATS_H
#include "GameObject.h"

class CShipStats : public GameObject
{
private:
	int speed;
	int health;
	int rotate;
	int fireRate;
public:
	CShipStats();
	~CShipStats();
	int getSpeed();
	int getHealth();
	int getRotate();
	int getFireRate();
	void SetHealth(int _health) ;
	void SetSpeed(int _speed);
	void setStats(int speed, int health, int rotate, int fireRate);
};

#endif