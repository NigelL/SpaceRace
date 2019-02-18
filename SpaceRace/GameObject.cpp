#include "GameObject.h"



GameObject::GameObject() {
	boxHalf = Vector3(0.5f, 0.5f, 0.5f);
	for (int i = 0; i < 8; i++) {
		allBounds[i] = Vector3(0, 0, 0);
	}
}

GameObject::GameObject(Mesh* _curObject)
{
	curObject = _curObject;
	boxHalf = Vector3(0.5f, 0.5f, 0.5f);
	for (int i = 0; i < 8; i++) {
		allBounds[i] = Vector3(0, 0, 0);
	}
}

GameObject::GameObject(Mesh* _curObject, Vector3 _position,float deg, Vector3 _rotation, Vector3 _scale)
{
	curObject = _curObject;
	position = _position;
	rotation = _rotation;
	scale = _scale;
	boxHalf = Vector3(0.5f, 0.5f, 0.5f);
	amt = deg;
	for (int i = 0; i < 8; i++) {
		allBounds[i] = Vector3(0, 0, 0);
	}
}

void GameObject::CheckCollision(Vector3 origin,Vector3 dir,float t) {
	GenerateBounds();
	std::cout << "origin : " << origin << std::endl;
	std::cout << "Dir :" << dir << std::endl;
	
	//Top Face
	Vector3 topFaceR = Vector3(maxX,maxY,minZ) - Vector3(minX, maxY, minZ);
	Vector3 topFaceRU = Vector3(minX, maxY, maxZ) - Vector3(minX, maxY, minZ);
	Vector3 topFaceNormal = (topFaceR.Cross(topFaceRU)).Normalized();

	Vector3 botFaceNormal = -topFaceNormal;

	Vector3 leftFaceR = Vector3(minX, minY, maxZ) - Vector3(minX, minY, minZ);
	Vector3 leftFaceRU = Vector3(minX, maxY, minZ) - Vector3(minX, minY, minZ);
	Vector3 leftFaceNormal = (leftFaceR.Cross(leftFaceRU)).Normalized();

	Vector3 rightFaceNormal = -leftFaceNormal;

	Vector3 frontFaceR = Vector3(maxX, minY, minZ) - Vector3(minX, minY, minZ);
	Vector3 frontFaceRU = Vector3(minX, maxY, minZ) - Vector3(minX, minY, minZ);
	Vector3 frontFaceNormal = (frontFaceR.Cross(frontFaceRU)).Normalized();

	Vector3 backFaceNormal = -frontFaceNormal;

	int tIndex = 0;
	float minT = 1e9;
	float testT = (-origin.Dot(topFaceNormal))/ (dir.Dot(topFaceNormal));
	std::cout << testT << std::endl;

	if (testT >= 0) { if (testT < minT) { minT = testT; tIndex = 0; } }

	testT = -testT;
	std::cout << testT << std::endl;

	if (testT >= 0) { if (testT < minT) { minT = testT; tIndex = 1; }}



	testT = (-origin.Dot(leftFaceNormal) + origin.x ) / (dir.Dot(leftFaceNormal));
	std::cout << testT << std::endl;
	if (testT >= 0) { if (testT < minT) { minT = testT; tIndex = 2; 
	}}

	testT = -testT + origin.x;
	std::cout << testT << std::endl;
	if (testT >= 0) { if (testT < minT) { minT = testT; tIndex = 3; }   }

	testT = (-origin.Dot(frontFaceNormal)) / (dir.Dot(frontFaceNormal));
	if (testT >= 0) { if (testT < minT) { minT = testT; tIndex = 4; }}

	testT = -testT;
	if (testT >= 0) { if (testT < minT) { minT = testT; tIndex = 5; }}

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

void GameObject::CheckCollision( GameObject& other) {
	GenerateBounds();
	other.GenerateBounds();
	
	Vector3 T = other.position - position;
	Vector3 Ax = (Vector3(maxX, minY, minZ) - Vector3(minX, minY, minZ)).Normalize();
    Vector3 Ay = (Vector3(minX, maxY, minZ) - Vector3(minX, minY, minZ)).Normalize();
	Vector3 Az = (Vector3(minX, minY, maxZ) - Vector3(minX, minY, minZ)).Normalize();
	Vector3 Bx = (Vector3(other.maxX, other.minY, other.minZ) - Vector3(other.minX, other.minY, other.minZ)).Normalize();
	Vector3 By = (Vector3(other.minX, other.maxY, other.minZ) - Vector3(other.minX, other.minY, other.minZ)).Normalize();
	Vector3 Bz = (Vector3(other.minX, other.minY, other.maxZ) - Vector3(other.minX, other.minY, other.minZ)).Normalize();




	if (!(abs(T.Dot(Ax)) > boxHalf.x + abs(other.boxHalf.x * Ax.Dot(Bx)) + abs(other.boxHalf.y * Ax.Dot(By)) + abs(other.boxHalf.z * Ax.Dot(Bz))
		|| abs(T.Dot(Ay)) > boxHalf.y + abs(other.boxHalf.x * Ay.Dot(Bx)) + abs(other.boxHalf.y * Ay.Dot(By)) + abs(other.boxHalf.z * Ay.Dot(Bz))
		|| abs(T.Dot(Az)) > boxHalf.z + abs(other.boxHalf.x * Az.Dot(Bx)) + abs(other.boxHalf.y * Az.Dot(By)) + abs(other.boxHalf.z * Az.Dot(Bz))
		|| abs(T.Dot(Bx)) > other.boxHalf.x + abs(boxHalf.x * Ax.Dot(Bx)) + abs(boxHalf.y * Ay.Dot(Bx)) + abs(boxHalf.z * Az.Dot(Bx))
		|| abs(T.Dot(By)) > other.boxHalf.y + abs(boxHalf.x * Ax.Dot(By)) + abs(boxHalf.y * Ay.Dot(By)) + abs(boxHalf.z * Az.Dot(By))
		|| abs(T.Dot(Bz)) > other.boxHalf.z + abs(boxHalf.x * Ax.Dot(Bz)) + abs(boxHalf.y * Ay.Dot(Bz)) + abs(boxHalf.z * Az.Dot(Bz))

		|| abs(T.Dot(Ax.Cross(Bx))) > abs(boxHalf.y * Az.Dot(Bx)) + abs(boxHalf.z * Ay.Dot(Bx)) + abs(other.boxHalf.y * Ax.Dot(Bz)) + abs(other.boxHalf.z * Ax.Dot(By))
		|| abs(T.Dot(Ax.Cross(By))) > abs((boxHalf.y * Az.Dot(By))) + abs((boxHalf.z * Ay.Dot(By))) + abs(other.boxHalf.x * Ax.Dot(Bz)) + abs(other.boxHalf.z * Ax.Dot(Bx))
		|| abs(T.Dot(Ax.Cross(Bz))) > abs((boxHalf.y * Az.Dot(Bz))) + abs((boxHalf.z * Ay.Dot(Bz))) + abs(other.boxHalf.x * Ax.Dot(By)) + abs(other.boxHalf.y * Ax.Dot(Bx))
		|| abs(T.Dot(Ay.Cross(Bx))) > abs(boxHalf.x * Az.Dot(Bx)) + abs(boxHalf.z * Ax.Dot(Bx)) + abs(other.boxHalf.y * Ay.Dot(Bz)) + abs(other.boxHalf.z * Ay.Dot(By))
		|| abs(T.Dot(Ay.Cross(By))) > abs(boxHalf.x * Az.Dot(By)) + abs(boxHalf.z * Ax.Dot(By)) + abs(other.boxHalf.x * Ay.Dot(Bz)) + abs(other.boxHalf.z * Ay.Dot(Bx))
		|| abs(T.Dot(Ay.Cross(Bz))) > abs(boxHalf.x * Az.Dot(Bz)) + abs(boxHalf.z * Ax.Dot(Bz)) + abs(other.boxHalf.x * Ay.Dot(By)) + abs(other.boxHalf.y * Ay.Dot(Bx))
		|| abs(T.Dot(Az.Cross(Bx))) > abs(boxHalf.x * Ay.Dot(Bx)) + abs(boxHalf.y * Ax.Dot(Bx)) + abs(other.boxHalf.y * Az.Dot(Bz)) + abs(other.boxHalf.z * Az.Dot(By))
		|| abs(T.Dot(Az.Cross(By))) > abs(boxHalf.x * Ay.Dot(By)) + abs(boxHalf.y * Ax.Dot(By)) + abs(other.boxHalf.x * Az.Dot(Bz)) + abs(other.boxHalf.z * Az.Dot(Bx))
		|| abs(T.Dot(Az.Cross(Bz))) > abs(boxHalf.x * Ay.Dot(Bz)) + abs(boxHalf.y * Ax.Dot(Bz)) + abs(other.boxHalf.x * Az.Dot(By)) + abs(other.boxHalf.y * Az.Dot(Bx))
		)) {
		if (std::find(collidedList.begin(), collidedList.end(), &other) == collidedList.end()) {
			collidedList.push_back(&other);
		}
	}
	else {
		auto index = std::find(collidedList.begin(), collidedList.end(), &other);
		if ( index != collidedList.end()) {
			int ind = std::distance(collidedList.begin(), index);
			collidedList.erase(collidedList.begin() + ind);
		}
	}
	


}


void GameObject::SetBounds(Vector3 _half) {
	boxHalf = _half;
}

void GameObject::GenerateBounds() {



	allBounds[0] = Vector3(- boxHalf.x ,  + boxHalf.y, + boxHalf.z ); //Top Left
	allBounds[1] = Vector3(+ boxHalf.x ,  + boxHalf.y,  + boxHalf.z ); // TR
	allBounds[2] = Vector3(- boxHalf.x ,  - boxHalf.y,  + boxHalf.z ); // BL
	allBounds[3] = Vector3(+ boxHalf.x ,  - boxHalf.y, + boxHalf.z ); // BR


	allBounds[4] = Vector3(- boxHalf.x ,  + boxHalf.y,  - boxHalf.z);
	allBounds[5] = Vector3(+ boxHalf.x ,  + boxHalf.y, - boxHalf.z);
	allBounds[6] = Vector3(- boxHalf.x ,  - boxHalf.y,  - boxHalf.z);
	allBounds[7] = Vector3(+ boxHalf.x ,  - boxHalf.y,  - boxHalf.z );

	//Mtx44 curMat = Mtx44(cos(angleB) * cos(angleC), (sin(angleA) *sin(angleB) * cos(angleC)) + (cos(angleA) * sin(angleC), -cos(angleA) * sin(angleB) * cos(angleC) + sin(angleA) * sin(angleC), 0, -cos(angleB) *sin(angleC), -sin(angleA) * sin(angleB) * sin(angleC) + cos(angleA) * cos(angleC),
	//	cos(angleA) * sin(angleB) * sin(angleC) + sin(angleA) * cos(angleC), 0, sin(angleB), -sin(angleA) * cos(angleB), cos(angleA) * cos(angleB), 1));
	
	float angleA = Deg2Rad(rotation.x * amt);
	float angleB = Deg2Rad(rotation.y * -amt);
	float angleC = Deg2Rad(rotation.z * amt);
	

	Mtx44 nxtMarix,backMatrix;
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


GameObject::~GameObject()
{
	delete curObject;
}