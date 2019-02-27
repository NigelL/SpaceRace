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

GameObject* GameObjectFactory::SpawnGameObject(OBJECT_TYPE type, std::string _name, Material *material, Transform _transform)
{
	if (type == SHIP)
	{
		CShipStats* cShip = SpawnShip(_name, material, _transform);
		return cShip;
	}
	if (type == SPDCONSUMABLE)
	{
		SpeedConsumable* cSPD = SpawnSpeedConsumable(_name, material, _transform);
		return cSPD;
	}
	if (type == HPCONSUMABLE)
	{
		HealthConsumable* cHP = SpawnHealthConsumable(_name, material, _transform);
		return cHP;
	}
	if (type == PARTSCONSUMABLE)
	{
		PartConsumable* cPart = SpawnPartConsumable(_name, material, _transform);
		cPart->SetPart(20);
		return cPart;
	}
	if (type == ISLAND)
	{
		IslandEnvironment* cIE = SpawnIsland(_name, material, _transform);
		return cIE;
	}
	if (type == CANNON) {
		cannonball* can = SpawnCannonBall(_name, material, _transform);
		return can;
	}
	if (type == BOUNDARY) {
		Boundary* cB = SpawnBoundary(_name, material, _transform);
		return cB;
	}
}

CShipStats* GameObjectFactory::SpawnShip(std::string name, Material *material, Transform transform)
{
	std::string obj = "OBJ//" + name + ".obj";
	std::string tga = "Image//" + name + ".tga";
	Mesh* name1 = MeshBuilder::GenerateOBJ(name, obj);
	name1->textureID = LoadTGA(tga.c_str());
	name1->material.kAmbient.Set(material->kAmbient.r, material->kAmbient.g, material->kAmbient.b);
	name1->material.kDiffuse.Set(material->kDiffuse.r, material->kDiffuse.g, material->kDiffuse.b);
	name1->material.kSpecular.Set(material->kSpecular.r, material->kSpecular.g, material->kSpecular.b);
	name1->material.kShininess = material->kShininess;
	CShipStats* retThis = new CShipStats(name1,transform.position, transform.amt, transform.rotation,transform.scale);
	transform.name = name;
	
	retThis->SetTransform(transform);
	return retThis;
}

cannonball * GameObjectFactory::SpawnCannonBall(std::string name, Material *material, Transform transform)
{
	std::string obj = "OBJ//" + name + ".obj";
	std::string tga = "Image//" + name + ".tga";
	Mesh* name1 = MeshBuilder::GenerateOBJ(name, obj);
	name1->textureID = LoadTGA(tga.c_str());
	name1->material.kAmbient.Set(material->kAmbient.r, material->kAmbient.g, material->kAmbient.b);
	name1->material.kDiffuse.Set(material->kDiffuse.r, material->kDiffuse.g, material->kDiffuse.b);
	name1->material.kSpecular.Set(material->kSpecular.r, material->kSpecular.g, material->kSpecular.b);
	name1->material.kShininess = material->kShininess;
	cannonball* retThis = new cannonball(name1, transform.position, transform.amt, transform.rotation, transform.scale);
	transform.name = name;

	retThis->SetTransform(transform);
	return retThis;
}


HealthConsumable* GameObjectFactory::SpawnHealthConsumable(std::string name, Material *material, Transform transform)
{
	std::string obj = "OBJ//" + name + ".obj";
	std::string tga = "Image//" + name + ".tga";
	Mesh* name1 = MeshBuilder::GenerateOBJ(name, obj);
	name1->textureID = LoadTGA(tga.c_str());
	name1->material.kAmbient.Set(material->kAmbient.r, material->kAmbient.g, material->kAmbient.b);
	name1->material.kDiffuse.Set(material->kDiffuse.r, material->kDiffuse.g, material->kDiffuse.b);
	name1->material.kSpecular.Set(material->kSpecular.r, material->kSpecular.g, material->kSpecular.b);
	name1->material.kShininess = material->kShininess;
	HealthConsumable* retThis = new HealthConsumable(name1, transform.position, transform.amt, transform.rotation, transform.scale);
	retThis->SetTransform(transform);
	retThis->SetHealth(20);
	return retThis;
}

SpeedConsumable* GameObjectFactory::SpawnSpeedConsumable(std::string name, Material *material, Transform transform)
{
	std::string obj = "OBJ//" + name + ".obj";
	std::string tga = "Image//" + name + ".tga";
	Mesh* name1 = MeshBuilder::GenerateOBJ(name, obj);
	name1->textureID = LoadTGA(tga.c_str());
	name1->material.kAmbient.Set(material->kAmbient.r, material->kAmbient.g, material->kAmbient.b);
	name1->material.kDiffuse.Set(material->kDiffuse.r, material->kDiffuse.g, material->kDiffuse.b);
	name1->material.kSpecular.Set(material->kSpecular.r, material->kSpecular.g, material->kSpecular.b);
	name1->material.kShininess = material->kShininess;
	SpeedConsumable* retThis = new SpeedConsumable(name1, transform.position, transform.amt, transform.rotation, transform.scale);
	retThis->SetTransform(transform);
	return retThis;
}

PartConsumable* GameObjectFactory::SpawnPartConsumable(std::string name, Material *material, Transform transform)
{
	std::string obj = "OBJ//" + name + ".obj";
	std::string tga = "Image//" + name + ".tga";
	Mesh* name1 = MeshBuilder::GenerateOBJ(name, obj);
	name1->textureID = LoadTGA(tga.c_str());
	name1->material.kAmbient.Set(material->kAmbient.r, material->kAmbient.g, material->kAmbient.b);
	name1->material.kDiffuse.Set(material->kDiffuse.r, material->kDiffuse.g, material->kDiffuse.b);
	name1->material.kSpecular.Set(material->kSpecular.r, material->kSpecular.g, material->kSpecular.b);
	name1->material.kShininess = material->kShininess;
	PartConsumable* retThis = new PartConsumable(name1, transform.position, transform.amt, transform.rotation, transform.scale);
	transform.type = 1;
	transform.name = name;
	retThis->SetTransform(transform);
	return retThis;
}

IslandEnvironment* GameObjectFactory::SpawnIsland(std::string name, Material *material, Transform transform)
{
	std::string obj = "OBJ//" + name + ".obj";
	std::string tga = "Image//" + name + ".tga";
	Mesh* name1 = MeshBuilder::GenerateOBJ(name, obj);
	name1->textureID = LoadTGA(tga.c_str());

	name1->material.kAmbient.Set(material->kAmbient.r, material->kAmbient.g, material->kAmbient.b);
	name1->material.kDiffuse.Set(material->kDiffuse.r, material->kDiffuse.g, material->kDiffuse.b);
	name1->material.kSpecular.Set(material->kSpecular.r, material->kSpecular.g, material->kSpecular.b);
	name1->material.kShininess = material->kShininess;
	IslandEnvironment* retThis = new IslandEnvironment(name1, transform.position, transform.amt, transform.rotation, transform.scale);
	retThis->SetTransform(transform);
	return retThis;
}

Boundary* GameObjectFactory::SpawnBoundary(std::string name, Material *material, Transform transform)
{
	Mesh* name1 = MeshBuilder::GenerateCube(name, Color(0, 1, 0), 1, 1, 1);

	name1->material.kAmbient.Set(material->kAmbient.r, material->kAmbient.g, material->kAmbient.b);
	name1->material.kDiffuse.Set(material->kDiffuse.r, material->kDiffuse.g, material->kDiffuse.b);
	name1->material.kSpecular.Set(material->kSpecular.r, material->kSpecular.g, material->kSpecular.b);
	name1->material.kShininess = material->kShininess;
	Boundary* retThis = new Boundary(name1, transform.position, transform.amt, transform.rotation, transform.scale);
	retThis->SetTransform(transform);
	return retThis;
}