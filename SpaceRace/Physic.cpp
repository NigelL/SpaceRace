#include "Physic.h"

double Physic::gravity = -9.81;

Physic::Physic()
{
}

Physic::Physic(double _mass)
{
	mass = _mass;
}

Physic::~Physic()
{
}

double Physic::acceleration(double speed, double dt)
{
	speed += 100 * dt;

	// thrust = velocity * (mass / time)

	return speed;
}

void Physic::bounceBack(double v)
{
	//v += at
	//a = ? t = dt
	double moment = v * mass;
}
