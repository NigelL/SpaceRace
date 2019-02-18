#pragma once
#include "Consumable.h"
#include "HealthConsumable.h"
#include "SpeedConsumable.h"
#include "CShipStats.h"
#include "PartConsumable.h"
#include <string>

class GameObjectFactory
{
public:
	GameObjectFactory();
	~GameObjectFactory();

	enum OBJECT_TYPE
	{
		SHIP,
		SPDCONSUMABLE,
		HPCONSUMABLE,
		PARTSCONSUMABLE,
		ENVIRONMENT,
		OBJECT_TOTAL
	};

	GameObjectFactory* objectList[OBJECT_TOTAL];

	std::vector<GameObject*> meshList;
	static GameObject* SpawnGameObject(OBJECT_TYPE type, std::string _name, Material *material, Vector3 _bounds);
	static CShipStats* SpawnShip(std::string name, Material *material, Vector3 bounds);
	static HealthConsumable* SpawnHealthConsumable(std::string name, Material *material, Vector3 bounds);
	static SpeedConsumable* SpawnSpeedConsumable(std::string name, Material *material, Vector3 bounds);
	static PartConsumable* SpawnPartConsumable(std::string name, Material *material, Vector3 bounds);
	//void SpawnEnvironment();
};

