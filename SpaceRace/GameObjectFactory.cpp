#include "GameObjectFactory.h"
#include <string>
#include "Material.h"
#include "Vector3.h"
#include "GameScene.h"
GameObjectFactory::GameObjectFactory()
{
	
}


GameObjectFactory::~GameObjectFactory()
{
}

GameObject* GameObjectFactory::SpawnGameObject(OBJECT_TYPE type, std::string _name, Material *material, Vector3 _bounds)
{
	if (type == SHIP)
	{
		CShipStats* cShip = SpawnShip(_name, material, _bounds);
		return cShip;
	}
	if (type == SPDCONSUMABLE)
	{
		SpeedConsumable* cSPD = SpawnSpeedConsumable(_name, material, _bounds);
		return cSPD;
	}
	if (type == HPCONSUMABLE)
	{
		HealthConsumable* cHP = SpawnHealthConsumable(_name, material, _bounds);
		return cHP;
	}
	if (type == PARTSCONSUMABLE)
	{
		PartConsumable* cPart = SpawnPartConsumable(_name, material, _bounds);
		return cPart;
	}
	if (type == CANNONBALL)
	{
		IslandEnvironment* cC = SpawnCannonball(_name, material, _bounds);
		return cC;
	}
	if (type == ISLAND)
	{
		IslandEnvironment* cIE = SpawnIsland(_name, material, _bounds);
		return cIE;
	}
}

CShipStats* GameObjectFactory::SpawnShip(std::string name, Material *material, Vector3 bounds)
{
	std::string obj = "OBJ//" + name + ".obj";
	std::string tga = "Image//" + name + ".tga";
	Mesh* name1 = MeshBuilder::GenerateOBJ(name, obj);
	name1->textureID = LoadTGA(tga.c_str());
	name1->material.kAmbient.Set(material->kAmbient.r, material->kAmbient.g, material->kAmbient.b);
	name1->material.kDiffuse.Set(material->kDiffuse.r, material->kDiffuse.g, material->kDiffuse.b);
	name1->material.kSpecular.Set(material->kSpecular.r, material->kSpecular.g, material->kSpecular.b);
	name1->material.kShininess = material->kShininess;
	CShipStats* retThis = new CShipStats(name1, Vector3 (0, 0, 0), 90, Vector3 (0, 1, 0), Vector3 (0.1, 0.1, 0.1));
	retThis->SetBounds(bounds);
	return retThis;
}

HealthConsumable* GameObjectFactory::SpawnHealthConsumable(std::string name, Material *material, Vector3 bounds)
{
	std::string obj = "OBJ//" + name + ".obj";
	std::string tga = "Image//" + name + ".tga";
	Mesh* name1 = MeshBuilder::GenerateOBJ(name, obj);
	name1->textureID = LoadTGA(tga.c_str());
	name1->material.kAmbient.Set(material->kAmbient.r, material->kAmbient.g, material->kAmbient.b);
	name1->material.kDiffuse.Set(material->kDiffuse.r, material->kDiffuse.g, material->kDiffuse.b);
	name1->material.kSpecular.Set(material->kSpecular.r, material->kSpecular.g, material->kSpecular.b);
	name1->material.kShininess = material->kShininess;
	HealthConsumable* retThis = new HealthConsumable(name1, Vector3 (0, 0, 0), 0, Vector3 (0, 1, 0), Vector3 (0.5, 0.5, 0.5));
	retThis->SetBounds(bounds);
	return retThis;
}

SpeedConsumable* GameObjectFactory::SpawnSpeedConsumable(std::string name, Material *material, Vector3 bounds)
{
	std::string obj = "OBJ//" + name + ".obj";
	std::string tga = "Image//" + name + ".tga";
	Mesh* name1 = MeshBuilder::GenerateOBJ(name, obj);
	name1->textureID = LoadTGA(tga.c_str());
	name1->material.kAmbient.Set(material->kAmbient.r, material->kAmbient.g, material->kAmbient.b);
	name1->material.kDiffuse.Set(material->kDiffuse.r, material->kDiffuse.g, material->kDiffuse.b);
	name1->material.kSpecular.Set(material->kSpecular.r, material->kSpecular.g, material->kSpecular.b);
	name1->material.kShininess = material->kShininess;
	SpeedConsumable* retThis = new SpeedConsumable(name1, Vector3 (5, 0, 10), 90, Vector3 (0, 1, 0), Vector3 (0.5, 0.5, 0.5));
	retThis->SetBounds(bounds);
	return retThis;
}

PartConsumable* GameObjectFactory::SpawnPartConsumable(std::string name, Material *material, Vector3 bounds)
{
	std::string obj = "OBJ//" + name + ".obj";
	std::string tga = "Image//" + name + ".tga";
	Mesh* name1 = MeshBuilder::GenerateOBJ(name, obj);
	name1->textureID = LoadTGA(tga.c_str());
	name1->material.kAmbient.Set(material->kAmbient.r, material->kAmbient.g, material->kAmbient.b);
	name1->material.kDiffuse.Set(material->kDiffuse.r, material->kDiffuse.g, material->kDiffuse.b);
	name1->material.kSpecular.Set(material->kSpecular.r, material->kSpecular.g, material->kSpecular.b);
	name1->material.kShininess = material->kShininess;
	PartConsumable* retThis = new PartConsumable(name1, Vector3(5, 0, 10), 90, Vector3(0, 1, 0), Vector3(0.5, 0.5, 0.5));
	retThis->SetBounds(bounds);
	return retThis;
}

IslandEnvironment* GameObjectFactory::SpawnCannonball(std::string name, Material *material, Vector3 bounds)
{
	std::string obj = "OBJ//" + name + ".obj";
	std::string tga = "Image//" + name + ".tga";
	Mesh* name1 = MeshBuilder::GenerateOBJ(name, obj);
	name1->textureID = LoadTGA(tga.c_str());
	name1->material.kAmbient.Set(material->kAmbient.r, material->kAmbient.g, material->kAmbient.b);
	name1->material.kDiffuse.Set(material->kDiffuse.r, material->kDiffuse.g, material->kDiffuse.b);
	name1->material.kSpecular.Set(material->kSpecular.r, material->kSpecular.g, material->kSpecular.b);
	name1->material.kShininess = material->kShininess;
	IslandEnvironment* retThis = new IslandEnvironment(name1, Vector3(5, 0, 10), 90, Vector3(0, 1, 0), Vector3(0.5, 0.5, 0.5));
	retThis->SetBounds(bounds);
	return retThis;
}

IslandEnvironment* GameObjectFactory::SpawnIsland(std::string name, Material *material, Vector3 bounds)
{
	std::string obj = "OBJ//" + name + ".obj";
	std::string tga = "Image//" + name + ".tga";
	Mesh* name1 = MeshBuilder::GenerateOBJ(name, obj);
	name1->textureID = LoadTGA(tga.c_str());
	name1->material.kAmbient.Set(material->kAmbient.r, material->kAmbient.g, material->kAmbient.b);
	name1->material.kDiffuse.Set(material->kDiffuse.r, material->kDiffuse.g, material->kDiffuse.b);
	name1->material.kSpecular.Set(material->kSpecular.r, material->kSpecular.g, material->kSpecular.b);
	name1->material.kShininess = material->kShininess;
	IslandEnvironment* retThis = new IslandEnvironment(name1, Vector3(5, 0, 10), 90, Vector3(0, 1, 0), Vector3(0.5, 0.5, 0.5));
	retThis->SetBounds(bounds);
	return retThis;
}

