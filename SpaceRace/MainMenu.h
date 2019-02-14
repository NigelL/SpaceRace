#pragma once

class MainMenu
{
private:
	MS modelStack, viewStack, projectionStack;
public:
	MainMenu();
	~MainMenu();

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
		U_NUMLIGHTS,
		U_COLOR_TEXTURE_ENABLED,
		U_COLOR_TEXTURE,
		U_TEXT_ENABLED,
		U_TEXT_COLOR,
		U_TOTAL,
	};

private:
	unsigned m_vertexArrayID;
	unsigned m_parameters[U_TOTAL];
	unsigned int m_indexBuffer;
	unsigned m_programID;

	Camera3 camera;

public:
	virtual void Init();
	virtual void Update(double dt);
	virtual void Render();
	virtual void Exit();
};

