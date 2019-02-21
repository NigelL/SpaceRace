#include "CShipStats.h"
#include <string>

using namespace std;

CShipStats::CShipStats() {

}

CShipStats::CShipStats(Mesh* _curMesh,Vector3 _pos, float amt, Vector3 _rot, Vector3 _sca) : GameObject(_curMesh, _pos, amt, _rot, _sca)
{
}


CShipStats::~CShipStats()
{
}

int CShipStats::getSpeed()
{
	return speed;
}

int CShipStats::getHealth()
{
	return health;
}

int CShipStats::getMaxHP()
{
	return maxHP;
}


int CShipStats::getRotate()
{
	return rotate;
}

int CShipStats::getParts()
{
	return parts;
}

int CShipStats::getFireRate()
{
	return fireRate;
}

void CShipStats::setStats(int speed, int health, int rotate, int fireRate, int parts)
{
	this->speed = speed;
	this->health = health;
	maxHP = health;
	this->rotate = rotate;
	this->fireRate = fireRate;
	this->parts = parts;
}

void CShipStats::SetHealth(int _health) {
	health = _health;
}

void CShipStats::SetSpeed(int _speed) {
	speed = _speed;
}

void CShipStats::SetParts(int _parts) {
	parts = _parts;
}

