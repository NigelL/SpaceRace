#ifndef MAINMENUSCENE_H
#define MAINMENUSCENE_H
#include "Scene.h"
#include "Camera2.h"
#include "CamMenu.h"
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

#include <iostream>

class MainMenuScene : public Scene
{
private:
	MS modelStack, viewStack, projectionStack;
	Mesh* gameText;
	float sceneFPS;
	static bool play;
	static int mode;

public:
	MainMenuScene();
	~MainMenuScene();
	void setPlay(bool _play);
	void setMode(int _mode);
	bool getPlay();
	int getMode();


	Mtx44 MVP;

	enum GEOMETRY_TYPE
	{
		MainMenu,
		GEO_AXES,
		NUM_GEOMETRY
	};
	enum UNIFORM_TYPE
	{
		U_MVP = 0,
		U_MODELVIEW,
		U_MODELVIEW_INVERSE_TRANSPOSE,
		U_LIGHTENABLED,
		U_MATERIAL_AMBIENT,
		U_MATERIAL_DIFFUSE,
		U_MATERIAL_SPECULAR,
		U_MATERIAL_SHININESS,
		U_NUMLIGHTS,
		U_COLOR_TEXTURE_ENABLED,
		U_COLOR_TEXTURE,
		U_TEXT_ENABLED,
		U_TEXT_COLOR,
		U_TOTAL,
	};

private:
	unsigned m_vertexArrayID;
	//unsigned m_vertexBuffer[MAX_GEOMETRY];
	//unsigned m_colorBuffer[MAX_GEOMETRY];
	unsigned m_parameters[U_TOTAL];
	unsigned int m_indexBuffer;
	unsigned m_programID;

	Camera2 camera;
	//CamMenu camera;

	Mesh* meshList[NUM_GEOMETRY];

	void RenderMesh(Mesh *mesh, bool enableLight);
	//void RenderMesh(GameObject* curMesh, bool enableLight);
	void RenderMainMenu();


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