#pragma once
#include "Consumable.h"
#include "HealthConsumable.h"
#include "SpeedConsumable.h"
#include "CShipStats.h"

class GameObjectFactory
{
public:
	GameObjectFactory();
	~GameObjectFactory();

	enum OBJECT_TYPE
	{
		SHIP,
		CONSUMABLES,
		ENVIRONMENT,
		OBJECT_TOTAL
	};

	Mesh* objectList[OBJECT_TOTAL];
	std::vector<GameObject*> meshList;
	GameObject* SpawnGameObject(OBJECT_TYPE type, string name, Material _material, Vector3 bounds);
	void SpawnShip(string name, Vector3 bounds);
	void SpawnConsumable();
	void SpawnEnvironment();
};

