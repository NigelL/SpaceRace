#include "Consumable.h"



void Consumable::OnCollide(const GameObject& other) {
	auto i = std::find(GameScene::instance->meshList.begin(), GameScene::instance->meshList.end(), this);
	if (i != GameScene::instance->meshList.end()) {
		int dist = std::distance(GameScene::instance->meshList.begin(), i);
		GameScene::instance->meshList.erase(GameScene::instance->meshList.begin() + dist);
	}
}

Consumable::Consumable() : GameObject()
{
	
}


Consumable::~Consumable()
{
}
