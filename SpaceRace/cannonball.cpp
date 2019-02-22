#include "cannonball.h"
#include "CShipStats.h"



cannonball::cannonball()
{
}

cannonball::cannonball(Mesh* _curMesh, Vector3 _pos, float amt, Vector3 _rot, Vector3 _sca) : GameObject(_curMesh, _pos, amt, _rot, _sca)
{
}


cannonball::~cannonball()
{
}

void cannonball::Update() {
	SetPosition(GetPosition() + (dir * speed));
}

void cannonball::OnCollide(GameObject& other)
{
	CShipStats* tryShip = dynamic_cast<CShipStats*>(&other);
	if (tryShip != nullptr) {

		tryShip->SetHealth(tryShip->getHealth() - 1);
	}
}
