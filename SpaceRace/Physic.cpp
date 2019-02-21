#include "Physic.h"

double Physic::gravity = -9.81;

Physic::Physic()
{
	srand(time(NULL));
}

Physic::Physic(double _mass)
{
	mass = _mass;
}

Physic::~Physic()
{
}

double Physic::acceleration(double v, double dt)
{
	a = Thrust / mass;

	v += a * dt;

	// thrust = velocity * (mass / time)

	return v;
}

double Physic::deceleration(double v, double dt)
{
	a = Thrust / mass;

	v -= a * dt;

	if (v < 0)
		v = 0;

	return v;
}

void Physic::bounceBack(CShipStats& sth, double dt, int rate)
{
	//v += at
	//a = ? t = dt

	sth.SetSpeed(deceleration(sth.getSpeed(), dt * rate));

	/*int a = Thrust / mass;

	v += a * dt;

	moment = v * mass;*/
}

void Physic::floating()
{

}

double Physic::VoA(int _knot, float _wake)
{
	voa = _knot * (1 - _wake);

	return voa;
}

double Physic::AdvRatio(int _prop_speed, int _diameter)
{
	n = _prop_speed / 60;

	advRatio = voa / (n * _diameter);

	return advRatio;
}

double Physic::thrust(float _kT, int prop_pitch, int _diameter)
{
	Thrust = _kT * prop_pitch * pow(n, 2) * pow(_diameter, 4);

	return Thrust;
}
