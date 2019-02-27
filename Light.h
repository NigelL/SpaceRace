#pragma once
#ifndef LIGHT
#define LIGHT

struct Light
{
	Position position;
	Color color;
	float power;
	float kC, kL, kQ;

	enum LIGHT_TYPE
	{
		LIGHT_POINT = 0,
		LIGHT_DIRECTIONAL,
		LIGHT_SPOT,
	};
	LIGHT_TYPE type;
	Vector3 spotDirection;
	float cosCutoff;
	float cosInner;
	float exponent;

	Light() {
		
	}

	Light(Position _pos, Color _color, float _power, float _kC, float _kL, float _kQ) {
		position = _pos;
		color = _color;
		power = _power;
		kC = _kC;
		kL = _kL;
		kQ = _kQ;
	}

	
};
#endif