#include "Transform.h"



Transform::Transform()
{
	boxHalf = Vector3(0.5f, 0.5f, 0.5f);
	for (int i = 0; i < 8; i++) {
		allBounds[i] = Vector3(0, 0, 0);
	}
	rotation = Vector3(0, 1, 0);
	scale = Vector3(1, 1, 1);
	amt = 0;
}


Transform::~Transform()
{
}

Transform& Transform::operator=(Transform& other) {
	if (&other != this) {
		position = other.position;
		rotation = other.rotation;
		scale = other.scale;
		amt = other.amt;
		boxHalf = other.boxHalf;
		type = other.type;
	}

	return *this;
}


void Transform::GenerateBounds() {

	allBounds[0] = Vector3(-boxHalf.x, +boxHalf.y, +boxHalf.z); //Top Left
	allBounds[1] = Vector3(+boxHalf.x, +boxHalf.y, +boxHalf.z); // TR
	allBounds[2] = Vector3(-boxHalf.x, -boxHalf.y, +boxHalf.z); // BL
	allBounds[3] = Vector3(+boxHalf.x, -boxHalf.y, +boxHalf.z); // BR


	allBounds[4] = Vector3(-boxHalf.x, +boxHalf.y, -boxHalf.z);
	allBounds[5] = Vector3(+boxHalf.x, +boxHalf.y, -boxHalf.z);
	allBounds[6] = Vector3(-boxHalf.x, -boxHalf.y, -boxHalf.z);
	allBounds[7] = Vector3(+boxHalf.x, -boxHalf.y, -boxHalf.z);

	//Mtx44 curMat = Mtx44(cos(angleB) * cos(angleC), (sin(angleA) *sin(angleB) * cos(angleC)) + (cos(angleA) * sin(angleC), -cos(angleA) * sin(angleB) * cos(angleC) + sin(angleA) * sin(angleC), 0, -cos(angleB) *sin(angleC), -sin(angleA) * sin(angleB) * sin(angleC) + cos(angleA) * cos(angleC),
	//	cos(angleA) * sin(angleB) * sin(angleC) + sin(angleA) * cos(angleC), 0, sin(angleB), -sin(angleA) * cos(angleB), cos(angleA) * cos(angleB), 1));

	float angleA = Math::DegreeToRadian(rotation.x * amt);
	float angleB = Math::DegreeToRadian(rotation.y * -amt);
	float angleC = Math::DegreeToRadian(rotation.z * amt);


	Mtx44 nxtMarix, backMatrix;
	nxtMarix.SetToIdentity();
	backMatrix.SetToIdentity();

	for (int i = 0; i < 8; i++) {
		Mtx44 curMat = Mtx44(cos(angleB), 0, sin(angleB), 0, 0, 1, 0, 0, -sin(angleB), 0, cos(angleB), 0, 0, 0, 0, 1);
		nxtMarix.a[12] = position.x;
		nxtMarix.a[13] = position.y;
		nxtMarix.a[14] = position.z;
		curMat = curMat * nxtMarix;

		allBounds[i] = curMat * allBounds[i];

	}



	for (int i = 0; i < 8; i++) {
		if (allBounds[i].x < minX) { minX = allBounds[i].x; }
		if (allBounds[i].x > maxX) { maxX = allBounds[i].x; }
		if (allBounds[i].y < minY) { minY = allBounds[i].y; }
		if (allBounds[i].y > maxY) { maxY = allBounds[i].y; }
		if (allBounds[i].z < minZ) { minZ = allBounds[i].z; }
		if (allBounds[i].z > maxZ) { maxZ = allBounds[i].z; }
	}

	
}


Vector3 Transform::GetBounds() const {
	return boxHalf;
}

/*
Transform& Transform::operator==(Transform& other) {
	if (&other == this) { return *this; }
	boxHalf = other.boxHalf;


}
*/

void Transform::SetBounds(Vector3 _half) {
	boxHalf = _half;
}
