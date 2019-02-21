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

double CShipStats::getSpeed()
{
	return speed;
}

double CShipStats::getMaxSpeed()
{
	return maxSpeed;
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

void CShipStats::setStats(int speed, int health, int rotate, int fireRate, int parts, int fireDist)
{
	this->speed = speed;
	this->health = health;
	maxHP = health;
	this->rotate = rotate;
	this->fireRate = fireRate;
	this->fireDist = fireDist;
	this->parts = parts;
	
int CShipStats::getFireDist()
{
	return fireDist;
}

void CShipStats::SetHealth(int _health) 
{
	health = _health;
}

void CShipStats::SetSpeed(double _speed) 
{
	speed = _speed;
}

void CShipStats::SetParts(int _parts) {
	parts = _parts;
}