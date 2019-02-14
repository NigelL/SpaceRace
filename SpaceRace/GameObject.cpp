#include "GameObject.h"



GameObject::GameObject(Mesh* _curObject)
{
	curObject = _curObject;
}

GameObject::GameObject(Mesh* _curObject, Vector3 _position,float deg, Vector3 _rotation, Vector3 _scale)
{
	curObject = _curObject;
	position = _position;
	rotation = _rotation;
	scale = _scale;
	amt = deg;
}

void GameObject::CheckCollision(GameObject& other) {
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
		std::cout << "Collided " << std::endl;
	}



}

void GameObject::GenerateBounds() {
	allBounds[0] = Vector3(position.x - boxHalf.x, position.y + boxHalf.y, position.z + boxHalf.z); //Top Left
	allBounds[1] = Vector3(position.x + boxHalf.x, position.y + boxHalf.y, position.z + boxHalf.z); // TR
	allBounds[2] = Vector3(position.x - boxHalf.x, position.y - boxHalf.y, position.z + boxHalf.z); // BL
	allBounds[3] = Vector3(position.x + boxHalf.x, position.y - boxHalf.y, position.z + boxHalf.z); // BR


	allBounds[4] = Vector3(position.x - boxHalf.x, position.y + boxHalf.y, position.z - boxHalf.z);
	allBounds[5] = Vector3(position.x + boxHalf.x, position.y + boxHalf.y, position.z - boxHalf.z);
	allBounds[6] = Vector3(position.x - boxHalf.x, position.y - boxHalf.y, position.z - boxHalf.z);
	allBounds[7] = Vector3(position.x + boxHalf.x, position.y - boxHalf.y, position.z - boxHalf.z);

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
