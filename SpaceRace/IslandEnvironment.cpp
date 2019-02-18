#include "IslandEnvironment.h"



IslandEnvironment::IslandEnvironment()
{
}

IslandEnvironment::IslandEnvironment(Mesh* _curMesh, Vector3 _pos, float amt, Vector3 _rot, Vector3 _sca) : GameObject(_curMesh, _pos, amt, _rot, _sca)
{
}

IslandEnvironment::~IslandEnvironment()
{
}

void IslandEnvironment::OnCollide(GameObject& other)
{

}

