#include "CShipStats.h"



CShipStats::CShipStats() : GameObject()
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

int CShipStats::getRotate()
{
	return rotate;
}

int CShipStats::getFireRate()
{
	return fireRate;
}

void CShipStats::setStats(int speed, int health, int rotate, int fireRate)
{
	this->speed = speed;
	this->health = health;
	this->rotate = rotate;
	this->fireRate = fireRate;
}

void CShipStats::SetHealth(int _health) {
	health = _health;
}

void CShipStats::SetSpeed(int _speed) {
	speed = _speed;
}