#ifndef SCENE_H
#define SCENE_H
#include "Mesh.h"
#include "Camera.h"


class Scene
{
public:
	Scene() {}
	~Scene() {}

	virtual void Init() = 0;
	virtual void Update(double dt) = 0;
	virtual void Render() = 0;
	virtual void Exit() = 0;
};

#endif