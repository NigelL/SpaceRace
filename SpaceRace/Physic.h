#pragma once
#include <cmath>


class Physic
{
private:
	static double gravity;
	double mass;
public:
	Physic();
	Physic(double _mass);
	~Physic();

	double acceleration(double speed, double dt);
	void bounceBack(double v);

	int diameter = 10;
	int prop_pitch = 10;
	int prop_speed = 200;
	int knots = 20;
	float trf = 0.12f;
	float wake = 0.18f;
	float eff = 1.0f;
	float kT = 0.12f;
	float kQ = 0.23f;
	double voa;
	double n;
	double advRatio;
	double thrust;

	double VoA(int _knot, float _wake)
	{
		voa = _knot * (1 - _wake);

		return voa;
	}

	double AdvRatio(int _prop_speed, int _diameter)
	{
		n = _prop_speed / 60;

		advRatio = voa / (n * _diameter);

		return advRatio;
	}

	double thrust(float _kT, int prop_pitch, int _diameter)
	{
		thrust = _kT * prop_pitch * pow(n, 2) * pow(_diameter, 4);

		return thrust;
	}

};

