#pragma once
#include <cmath>
#include <iostream>
#include "CShipStats.h"
#include <ctime>


class Physic
{
private:
	static double gravity;
public:
	Physic();
	Physic(double _mass);
	~Physic();

	double acceleration(double speed, double dt);
	double deceleration(double speed, double dt);
	void bounceBack(CShipStats& sth, double dt, int rate);
	void floating();

	double mass = 5000;
	int diameter = 10;
	int prop_pitch = 10;
	int prop_speed = 200;
	int knots = 20;
	float trf = 0.12f;
	float wake = 0.18f;
	float eff = 1.0f;
	float kT = 0.12f;
	float kQ = 0.23f;
	double a;
	double voa;
	double n;
	double advRatio;
	double Thrust;
	double moment;

	double VoA(int _knot, float _wake);
	double AdvRatio(int _prop_speed, int _diameter);
	double thrust(float _kT, int prop_pitch, int _diameter);
};

