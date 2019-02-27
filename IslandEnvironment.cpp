#include "IslandEnvironment.h"
#include "CShipStats.h"
#include "GameSound.h"


IslandEnvironment::IslandEnvironment()
{	
}

IslandEnvironment::IslandEnvironment(Mesh* _curMesh, Vector3 _pos, float amt, Vector3 _rot, Vector3 _sca) : GameObject(_curMesh, _pos, amt, _rot, _sca)
{

}

IslandEnvironment::~IslandEnvironment()
{
}

void IslandEnvironment::OnCollide(GameObject & other)
{
	CShipStats* tryShip = dynamic_cast<CShipStats*>(&other);
	if (tryShip != nullptr) {
		GameSound::instance()->engine->play2D(GameSound::instance()->Thud);
		tryShip->SetHealth(tryShip->getHealth() - 5);
		tryShip->SetRotation(other.GetRotation(), other.GetAmt() + 180);
		tryShip->SetSpeed(tryShip->getSpeed() / 3);
	}
}
