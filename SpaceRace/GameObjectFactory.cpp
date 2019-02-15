#include "GameObjectFactory.h"
#include <string>
#include "Material.h"
#include "Vector3.h"

GameObjectFactory::GameObjectFactory()
{
	
}


GameObjectFactory::~GameObjectFactory()
{
}

GameObject* GameObjectFactory::SpawnGameObject(OBJECT_TYPE type, string name, Material _material, Vector3 bounds)
{
	if (type == SHIP)
	{
		objectList[type] = new SpawnShip(name, bounds);
	}
	if (type == CONSUMABLES)
	{

	}
	if (type == ENVIRONMENT)
	{

	}
	GameObject* type1;
	string obj = "OBJ//" + name + ".obj";
	string tga = "Image//" + name + ".tga";
	
	Mesh* name1 = MeshBuilder::GenerateOBJ(name, obj);
	name1->textureID = LoadTGA(tga.c_str());
	name1->material.kAmbient.Set(0.4f, 0.4f, 0.4f);
	name1->material.kDiffuse.Set(0.1f, 0.1f, 0.1f);
	name1->material.kSpecular.Set(0.2f, 0.2f, 0.2f);
	name1->material.kShininess = 1.0f;
	type1 = (new GameObject(name1, Vector3(-50, -2.5f, 50), 180, Vector3(1, 0, 0), Vector3(100, 1, 100)));
	type1->SetBounds(bounds);
	//Get the .obj file 
	//
}

void GameObjectFactory::SpawnShip(string name, Vector3 bounds)
{
	GameObject* name;
	string obj = "OBJ//" + name + ".obj";
	string tga = "Image//" + name + ".tga";
}

void GameObjectFactory::SpawnConsumable()
{

}

void GameObjectFactory::SpawnEnvironment()
{

}