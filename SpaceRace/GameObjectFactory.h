#pragma once
#include "Consumable.h"
#include "HealthConsumable.h"
#include "SpeedConsumable.h"
#include "CShipStats.h"
#include "PartConsumable.h"
#include "IslandEnvironment.h"
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
		ISLAND,
		OBJECT_TOTAL
	};

	GameObjectFactory* objectList[OBJECT_TOTAL];

	std::vector<GameObject*> meshList;
	static GameObject* SpawnGameObject(OBJECT_TYPE type, std::string _name, Material *material, Transform);
	static CShipStats* SpawnShip(std::string name, Material *material, Transform);
	static HealthConsumable* SpawnHealthConsumable(std::string name, Material *material, Transform);
	static SpeedConsumable* SpawnSpeedConsumable(std::string name, Material *material, Transform);
	static PartConsumable* SpawnPartConsumable(std::string name, Material *material, Transform);
	static IslandEnvironment* SpawnIsland(std::string name, Material *material, Transform);
	//void SpawnEnvironment();
};

