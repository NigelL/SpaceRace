#ifndef CSHIPSTATS_H
#define CSHIPSTATS_H
#include "GameObject.h"

class CShipStats : public GameObject
{
private:
	double speed;
	int health;
	int rotate;
	int fireRate;
	int parts;
public:
	CShipStats();
	CShipStats(Mesh* _curMesh, Vector3 _pos, float amt, Vector3 _rot, Vector3 _sca);
	~CShipStats();
	double getSpeed();
	int getHealth();
	int getParts();
	int getRotate();
	int getFireRate();
	void SetHealth(int _health) ;
	void SetSpeed(double _speed);
	void SetParts(int _parts);
	void setStats(int speed, int health, int rotate, int fireRate, int parts);
};

#endif