#ifndef GAMESCENE_H
#define GAMESCENE_H

#define MAX_GEOMETRY 1000


#include "Scene.h"
#include "Camera3.h"
#include "GameObjectFactory.h"
#include "Consumable.h"
#include "Light.h"
#include "MatrixStack.h"
#include "MeshBuilder.h"
#include "GL\glew.h"
#include "GLFW\glfw3.h"
#include <vector>
#include "shader.hpp"
#include "Utility.h"
#include "Mtx44.h"
#include "Water.h"
#include "GameSound.h"
#include "LoadTGA.h"
#include "Application.h"
#include <map>
#include "Physic.h"

class GameScene : public Scene
{
private:
	MS modelStack, viewStack, projectionStack;
	Mesh* gameText;
	Mesh* healthBar;
	Mesh* partsCount;
	float sceneFPS;
	std::map<std::string, GameObject*> sceneObjects;
public:
	GameScene();
	~GameScene();
	inline void SetGameText(Mesh* curText);
	static GameScene* instance;
	std::vector<GameObject*> meshList;

	Mtx44 MVP;

	enum GEOMETRY_TYPE
	{
		GEO_SHIP,
		GEO_ENVIRONMENT,
		GEO_CONSUMABLES,
		NUM_GEOMETRY
	};
	enum UNIFORM_TYPE
	{
		U_MVP = 0,
		U_MODELVIEW,
		U_MODELVIEW_INVERSE_TRANSPOSE,
		U_MATERIAL_AMBIENT,
		U_MATERIAL_DIFFUSE,
		U_MATERIAL_SPECULAR,
		U_MATERIAL_SHININESS,
		U_LIGHT0_POSITION,
		U_LIGHT0_COLOR,
		U_LIGHT0_POWER,
		U_LIGHT0_KC,
		U_LIGHT0_KL,
		U_LIGHT0_KQ,
		U_LIGHTENABLED,
		U_LIGHT0_TYPE,
		U_LIGHT0_SPOTDIRECTION,
		U_LIGHT0_COSCUTOFF,
		U_LIGHT0_COSINNER,
		U_LIGHT0_EXPONENT,
		U_LIGHT1_POSITION,
		U_LIGHT1_COLOR,
		U_LIGHT1_POWER,
		U_LIGHT1_KC,
		U_LIGHT1_KL,
		U_LIGHT1_KQ,
		U_LIGHT1ENABLED,
		U_LIGHT1_TYPE,
		U_LIGHT1_SPOTDIRECTION,
		U_LIGHT1_COSCUTOFF,
		U_LIGHT1_COSINNER,
		U_LIGHT1_EXPONENT,
		U_LIGHT2_POSITION,
		U_LIGHT2_COLOR,
		U_LIGHT2_POWER,
		U_LIGHT2_KC,
		U_LIGHT2_KL,
		U_LIGHT2_KQ,
		U_LIGHT2ENABLED,
		U_LIGHT2_TYPE,
		U_LIGHT2_SPOTDIRECTION,
		U_LIGHT2_COSCUTOFF,
		U_LIGHT2_COSINNER,
		U_LIGHT2_EXPONENT,
		U_NUMLIGHTS,
		U_COLOR_TEXTURE_ENABLED,
		U_COLOR_TEXTURE,
		U_TEXT_ENABLED,
		U_TEXT_COLOR,
		U_TOTAL,
	};

private:
	unsigned m_vertexArrayID;
	unsigned m_vertexBuffer[MAX_GEOMETRY];
	unsigned m_colorBuffer[MAX_GEOMETRY];
	unsigned m_parameters[U_TOTAL];
	unsigned int m_indexBuffer;
	unsigned m_programID;

	Light light[3];
	Camera3 camera;
	double timer;

	void SpawnPowerUp();
	void renderSkybox();
	void renderSmoke();
	void RenderMesh(GameObject* curMesh, bool enableLight);
	void RenderMesh(Mesh* curMesh, bool enableLight);
	void RenderMesh(GEOMETRY_TYPE curType, bool enableLight);
	void RenderUI(Mesh* mesh, float sizeX, float sizeY, float x, float y);
	void RenderText(Mesh* mesh, std::string text, Color color);
	void RenderTextOnScreen(Mesh* mesh, std::string text, Color color, float size, float x, float y);
	double getTimer();
	void setTimer(double timer);
public:
	virtual void Init();
	virtual void Init2();
	virtual void Update(double dt);
	virtual void Update2(double dt);
	virtual void Render();
	virtual void Render2();
	virtual void Exit();

};

#endif

