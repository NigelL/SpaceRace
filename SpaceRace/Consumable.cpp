#include "Consumable.h"
#include "GameScene.h"

Consumable::Consumable() : GameObject()
{

}

Consumable::Consumable(Mesh* _curMesh, Vector3 _pos, float amt, Vector3 _rot, Vector3 _sca) : GameObject(_curMesh, _pos, amt, _rot, _sca)
{
	
}

void Consumable::OnCollide(GameObject& other) {
	auto i = std::find(GameScene::instance->meshList.begin(), GameScene::instance->meshList.end(), this);
	if (i != GameScene::instance->meshList.end()) {
		int dist = std::distance(GameScene::instance->meshList.begin(), i);
		GameScene::instance->meshList.erase(GameScene::instance->meshList.begin() + dist);
	}
}



Consumable::~Consumable()
{
}
