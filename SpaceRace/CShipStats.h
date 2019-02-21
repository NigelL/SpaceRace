#ifndef CSHIPSTATS_H
#define CSHIPSTATS_H
#include "GameObject.h"

class CShipStats : public GameObject
{
private:
	double speed;
	double maxSpeed;
	int health;
	int maxHP;
	int rotate;
	int fireRate;
	int parts;
	int fireDist;
public:
	CShipStats();
	CShipStats(Mesh* _curMesh, Vector3 _pos, float amt, Vector3 _rot, Vector3 _sca);
	~CShipStats();
	double getSpeed();
	double getMaxSpeed();
	int getHealth();
	int getMaxHP();
	int getParts();
	int getRotate();
	int getFireRate();
	int getFireDist();
	void SetHealth(int _health) ;
	void SetSpeed(int _speed);
	void SetParts(int _parts);
	void setStats(int speed, int health, int rotate, int fireRate, int parts, int fireDist);
};

#endif