#include "Collision.h"



Collision::Collision()
{
	
}

void Collision::CheckCollision(Vector3 origin, Vector3 dir, float t) {
	Transform thisObj = *(this->thisObj);
	thisObj.GenerateBounds();
	std::cout << "origin : " << origin << std::endl;
	std::cout << "Dir :" << dir << std::endl;

	//Top Face
	Vector3 topFaceR = Vector3(thisObj.maxX, thisObj.maxY, thisObj.minZ) - Vector3(thisObj.minX, thisObj.maxY, thisObj.minZ);
	Vector3 topFaceRU = Vector3(thisObj.minX, thisObj.maxY, thisObj.maxZ) - Vector3(thisObj.minX, thisObj.maxY, thisObj.minZ);
	Vector3 topFaceNormal = (topFaceR.Cross(topFaceRU)).Normalized();

	Vector3 botFaceNormal = -topFaceNormal;

	Vector3 leftFaceR = Vector3(thisObj.minX, thisObj.minY, thisObj.maxZ) - Vector3(thisObj.minX, thisObj.minY, thisObj.minZ);
	Vector3 leftFaceRU = Vector3(thisObj.minX, thisObj.maxY, thisObj.minZ) - Vector3(thisObj.minX, thisObj.minY, thisObj.minZ);
	Vector3 leftFaceNormal = (leftFaceR.Cross(leftFaceRU)).Normalized();

	Vector3 rightFaceNormal = -leftFaceNormal;

	Vector3 frontFaceR = Vector3(thisObj.maxX, thisObj.minY, thisObj.minZ) - Vector3(thisObj.minX, thisObj.minY, thisObj.minZ);
	Vector3 frontFaceRU = Vector3(thisObj.minX, thisObj.maxY, thisObj.minZ) - Vector3(thisObj.minX, thisObj.minY, thisObj.minZ);
	Vector3 frontFaceNormal = (frontFaceR.Cross(frontFaceRU)).Normalized();

	Vector3 backFaceNormal = -frontFaceNormal;

	int tIndex = 0;
	float minT = 1e9;
	float testT = (-origin.Dot(topFaceNormal)) / (dir.Dot(topFaceNormal));

	if (testT >= 0) { if (testT < minT) { minT = testT; tIndex = 0; } }

	testT = (-origin.Dot(botFaceNormal)) / (dir.Dot(botFaceNormal));

	if (testT >= 0) { if (testT < minT) { minT = testT; tIndex = 1; } }



	testT = (-origin.Dot(leftFaceNormal)) / (dir.Dot(leftFaceNormal));
	if (testT >= 0) {
		if (testT < minT) {
			minT = testT; tIndex = 2;
		}
	}

	testT = (-origin.Dot(rightFaceNormal)) / (dir.Dot(rightFaceNormal));
	std::cout << testT << std::endl;
	if (testT >= 0) { if (testT < minT) { minT = testT; tIndex = 3; } }

	testT = (-origin.Dot(frontFaceNormal)) / (dir.Dot(frontFaceNormal));
	if (testT >= 0) { if (testT < minT) { minT = testT; tIndex = 4; } }

	testT = -testT;
	if (testT >= 0) { if (testT < minT) { minT = testT; tIndex = 5; } }

	switch (tIndex) {
	case 0:
		std::cout << "Top face" << std::endl;
		break;
	case 1:
		std::cout << "Bot face" << std::endl;
		break;
	case 2:
		std::cout << "Left face" << std::endl;
		break;
	case 3:
		std::cout << "Right face" << std::endl;
		break;
	case 4:
		std::cout << "Front face" << std::endl;
		break;
	case 5:
		std::cout << "Back face" << std::endl;
		break;
	}

	system("cls");



}



bool Collision::CheckCollision(Transform& otherObj) {
	Transform& thisObj = *(this->thisObj);

	thisObj.GenerateBounds();
	otherObj.GenerateBounds();
	

	Vector3 T = otherObj.position - thisObj.position;
	Vector3 Ax = (Vector3(thisObj.maxX, thisObj.minY, thisObj.minZ) - Vector3(thisObj.minX, thisObj.minY, thisObj.minZ)).Normalize();
	Vector3 Ay = (Vector3(thisObj.minX, thisObj.maxY, thisObj.minZ) - Vector3(thisObj.minX, thisObj.minY, thisObj.minZ)).Normalize();
	Vector3 Az = (Vector3(thisObj.minX, thisObj.minY, thisObj.maxZ) - Vector3(thisObj.minX, thisObj.minY, thisObj.minZ)).Normalize();
	Vector3 Bx = (Vector3(otherObj.maxX, otherObj.minY, otherObj.minZ) - Vector3(otherObj.minX, otherObj.minY, otherObj.minZ)).Normalize();
	Vector3 By = (Vector3(otherObj.minX, otherObj.maxY, otherObj.minZ) - Vector3(otherObj.minX, otherObj.minY, otherObj.minZ)).Normalize();
	Vector3 Bz = (Vector3(otherObj.minX, otherObj.minY, otherObj.maxZ) - Vector3(otherObj.minX, otherObj.minY, otherObj.minZ)).Normalize();

	


	if (!(abs(T.Dot(Ax)) > thisObj.boxHalf.x + abs(otherObj.boxHalf.x * Ax.Dot(Bx)) + abs(otherObj.boxHalf.y * Ax.Dot(By)) + abs(otherObj.boxHalf.z * Ax.Dot(Bz))
		|| abs(T.Dot(Ay)) > thisObj.boxHalf.y + abs(otherObj.boxHalf.x * Ay.Dot(Bx)) + abs(otherObj.boxHalf.y * Ay.Dot(By)) + abs(otherObj.boxHalf.z * Ay.Dot(Bz))
		|| abs(T.Dot(Az)) > thisObj.boxHalf.z + abs(otherObj.boxHalf.x * Az.Dot(Bx)) + abs(otherObj.boxHalf.y * Az.Dot(By)) + abs(otherObj.boxHalf.z * Az.Dot(Bz))
		|| abs(T.Dot(Bx)) > otherObj.boxHalf.x + abs(thisObj.boxHalf.x * Ax.Dot(Bx)) + abs(thisObj.boxHalf.y * Ay.Dot(Bx)) + abs(thisObj.boxHalf.z * Az.Dot(Bx))
		|| abs(T.Dot(By)) > otherObj.boxHalf.y + abs(thisObj.boxHalf.x * Ax.Dot(By)) + abs(thisObj.boxHalf.y * Ay.Dot(By)) + abs(thisObj.boxHalf.z * Az.Dot(By))
		|| abs(T.Dot(Bz)) > otherObj.boxHalf.z + abs(thisObj.boxHalf.x * Ax.Dot(Bz)) + abs(thisObj.boxHalf.y * Ay.Dot(Bz)) + abs(thisObj.boxHalf.z * Az.Dot(Bz))

		|| abs(T.Dot(Ax.Cross(Bx))) > abs(thisObj.boxHalf.y * Az.Dot(Bx)) + abs(thisObj.boxHalf.z * Ay.Dot(Bx)) + abs(otherObj.boxHalf.y * Ax.Dot(Bz)) + abs(otherObj.boxHalf.z * Ax.Dot(By))
		|| abs(T.Dot(Ax.Cross(By))) > abs((thisObj.boxHalf.y * Az.Dot(By))) + abs((thisObj.boxHalf.z * Ay.Dot(By))) + abs(otherObj.boxHalf.x * Ax.Dot(Bz)) + abs(otherObj.boxHalf.z * Ax.Dot(Bx))
		|| abs(T.Dot(Ax.Cross(Bz))) > abs((thisObj.boxHalf.y * Az.Dot(Bz))) + abs((thisObj.boxHalf.z * Ay.Dot(Bz))) + abs(otherObj.boxHalf.x * Ax.Dot(By)) + abs(otherObj.boxHalf.y * Ax.Dot(Bx))
		|| abs(T.Dot(Ay.Cross(Bx))) > abs(thisObj.boxHalf.x * Az.Dot(Bx)) + abs(thisObj.boxHalf.z * Ax.Dot(Bx)) + abs(otherObj.boxHalf.y * Ay.Dot(Bz)) + abs(otherObj.boxHalf.z * Ay.Dot(By))
		|| abs(T.Dot(Ay.Cross(By))) > abs(thisObj.boxHalf.x * Az.Dot(By)) + abs(thisObj.boxHalf.z * Ax.Dot(By)) + abs(otherObj.boxHalf.x * Ay.Dot(Bz)) + abs(otherObj.boxHalf.z * Ay.Dot(Bx))
		|| abs(T.Dot(Ay.Cross(Bz))) > abs(thisObj.boxHalf.x * Az.Dot(Bz)) + abs(thisObj.boxHalf.z * Ax.Dot(Bz)) + abs(otherObj.boxHalf.x * Ay.Dot(By)) + abs(otherObj.boxHalf.y * Ay.Dot(Bx))
		|| abs(T.Dot(Az.Cross(Bx))) > abs(thisObj.boxHalf.x * Ay.Dot(Bx)) + abs(thisObj.boxHalf.y * Ax.Dot(Bx)) + abs(otherObj.boxHalf.y * Az.Dot(Bz)) + abs(otherObj.boxHalf.z * Az.Dot(By))
		|| abs(T.Dot(Az.Cross(By))) > abs(thisObj.boxHalf.x * Ay.Dot(By)) + abs(thisObj.boxHalf.y * Ax.Dot(By)) + abs(otherObj.boxHalf.x * Az.Dot(Bz)) + abs(otherObj.boxHalf.z * Az.Dot(Bx))
		|| abs(T.Dot(Az.Cross(Bz))) > abs(thisObj.boxHalf.x * Ay.Dot(Bz)) + abs(thisObj.boxHalf.y * Ax.Dot(Bz)) + abs(otherObj.boxHalf.x * Az.Dot(By)) + abs(otherObj.boxHalf.y * Az.Dot(Bx))
		)) {
		if (std::find(collidedList.begin(), collidedList.end(), &otherObj) == collidedList.end()) {
			collidedList.push_back(&otherObj);
			return true;
		}
	}
	else {
		auto index = std::find(collidedList.begin(), collidedList.end(), &otherObj);
		if (index != collidedList.end()) {
			int ind = std::distance(collidedList.begin(), index);
			collidedList.erase(collidedList.begin() + ind);
		}
	}
	return false;
}


Collision::~Collision()
{
}
