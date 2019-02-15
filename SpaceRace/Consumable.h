#ifndef CONSUMABLE
#define CONSUMABLE

#include "GameObject.h"
#include "GameScene.h"


class Consumable : public GameObject
{
private:
	
public:
	virtual void OnCollide(const GameObject& other) = 0;


	Consumable();
	~Consumable();
};

#endif