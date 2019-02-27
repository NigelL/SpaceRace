#include "MainMenuScene.h"


bool MainMenuScene::play = false;
int MainMenuScene::mode = 1;
static double bouncetime;

MainMenuScene::MainMenuScene()
{
	//play = false;
	//mode = true;
}


MainMenuScene::MainMenuScene(MenuFunctionality _func)
{
	menuFunctions = _func;
	//play = false;
	//mode = true;
}

MainMenuScene::~MainMenuScene()
{
}

static inline float DegreeToRadian(float value)
{
	return value * 3.142f / 180.0f;
}

void MainMenuScene::Init()
{
	GameSound::instance()->MenuBGM->setDefaultVolume(0.1f);
	GameSound::instance()->engine->play2D(GameSound::instance()->MenuBGM, false);

	// Set background color to dark blue
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	// Enable Depth Test
	glEnable(GL_DEPTH_TEST);
	// Enable Back Face Culling
	glEnable(GL_CULL_FACE);
	// Enable blending
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	//Default to fill mode
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	//Generate a default VAO for now
	glGenVertexArrays(1, &m_vertexArrayID);
	glBindVertexArray(m_vertexArrayID);

	//Load vertex and fragment shaders
	m_programID = LoadShaders("Shader//Texture.vertexshader", "Shader//Text.fragmentshader");

	m_parameters[U_MVP] = glGetUniformLocation(m_programID, "MVP");
	m_parameters[U_MODELVIEW] = glGetUniformLocation(m_programID, "MV");
	m_parameters[U_MODELVIEW_INVERSE_TRANSPOSE] = glGetUniformLocation(m_programID, "MV_inverse_transpose");
	m_parameters[U_COLOR_TEXTURE_ENABLED] = glGetUniformLocation(m_programID, "colorTextureEnabled");
	m_parameters[U_COLOR_TEXTURE] = glGetUniformLocation(m_programID, "colorTexture");
	m_parameters[U_MATERIAL_AMBIENT] = glGetUniformLocation(m_programID, "material.kAmbient");
	m_parameters[U_MATERIAL_DIFFUSE] = glGetUniformLocation(m_programID, "material.kDiffuse");
	m_parameters[U_MATERIAL_SPECULAR] = glGetUniformLocation(m_programID, "material.kSpecular");
	m_parameters[U_MATERIAL_SHININESS] = glGetUniformLocation(m_programID, "material.kShininess");
	m_parameters[U_NUMLIGHTS] = glGetUniformLocation(m_programID, "numLights");

	m_parameters[U_LIGHTENABLED] = glGetUniformLocation(m_programID, "lightEnabled");
	m_parameters[U_TEXT_ENABLED] = glGetUniformLocation(m_programID, "textEnabled");
	m_parameters[U_TEXT_COLOR] = glGetUniformLocation(m_programID, "textColor");

	// Use our shader
	glUseProgram(m_programID);
	// Get a handle for our "MVP" uniform
	m_parameters[U_MVP] = glGetUniformLocation(m_programID, "MVP");

	//Initialize camera settings
	camera.Init(Vector3(0, 2, 15), Vector3(0, 2, 0), Vector3(0, 1, 0));

	//Initialize all meshes to NULL
	for (int i = 0; i < NUM_GEOMETRY; ++i)
	{
		meshList[i] = NULL;
	}
	meshList[GEO_AXES] = MeshBuilder::GenerateAxes("Reference", 500, 500, 500);
	meshList[MainMenu] = MeshBuilder::GenerateMenu("Main Menu", Color(0, 1, 0), 10);
	meshList[MainMenu]->textureID = LoadTGA("Image//start.tga");

	//Set projection to Perspective and load projection matrix
	Mtx44 projection;
	projection.SetToPerspective(45.f, 4.f / 3.f, 0.1f, 1000.f);
	projectionStack.LoadMatrix(projection);

	glfwSetCursorPos(Application::getGLFWWindow(), 1000, 700);
}


void MainMenuScene::RenderMainMenu()
{
	//RenderMesh(meshList[GEO_AXES], false);
	modelStack.PushMatrix();
	modelStack.Scale(1.7, 1.1, 0);
	RenderMesh(meshList[MainMenu], false);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(menuFunctions.GetHighLight()->GetPosition().x, menuFunctions.GetHighLight()->GetPosition().y, menuFunctions.GetHighLight()->GetPosition().z);
	modelStack.PushMatrix();
	modelStack.Scale(menuFunctions.GetHighLight()->GetScale().x, menuFunctions.GetHighLight()->GetScale().y, menuFunctions.GetHighLight()->GetScale().z);
	RenderMesh(menuFunctions.GetHighLight()->GetMesh(), false);

	modelStack.PopMatrix();
	modelStack.PopMatrix();

	for (auto buttons : menuFunctions.GetAllButtons()) {
		modelStack.PushMatrix();
		modelStack.Translate(buttons->GetPosition().x, buttons->GetPosition().y, buttons->GetPosition().z);
		modelStack.PushMatrix();
		modelStack.Scale(buttons->GetScale().x, buttons->GetScale().y, buttons->GetScale().z);
		RenderMesh(buttons->GetMesh(), false);

		modelStack.PopMatrix();
		modelStack.PopMatrix();
	}


}

void MainMenuScene::RenderMesh(Mesh *mesh, bool enableLight)
{
	Mtx44 MVP, modelView, modelView_inverse_transpose;
	MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top();
	glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);
	modelView = viewStack.Top() * modelStack.Top();
	glUniformMatrix4fv(m_parameters[U_MODELVIEW], 1, GL_FALSE, &modelView.a[0]);

	if (enableLight)
	{
		glUniform1i(m_parameters[U_LIGHTENABLED], 1);
		modelView_inverse_transpose = modelView.GetInverse().GetTranspose();
		glUniformMatrix4fv(m_parameters[U_MODELVIEW_INVERSE_TRANSPOSE], 1, GL_FALSE,
			&modelView_inverse_transpose.a[0]);
		//load material
		glUniform3fv(m_parameters[U_MATERIAL_AMBIENT], 1, &mesh->material.kAmbient.r);
		glUniform3fv(m_parameters[U_MATERIAL_DIFFUSE], 1, &mesh->material.kDiffuse.r);
		glUniform3fv(m_parameters[U_MATERIAL_SPECULAR], 1, &mesh->material.kSpecular.r);
		glUniform1f(m_parameters[U_MATERIAL_SHININESS], mesh->material.kShininess);
	}
	else
	{
		glUniform1i(m_parameters[U_LIGHTENABLED], 0);
	}

	if (mesh->textureID > 0)
	{
		glUniform1i(m_parameters[U_COLOR_TEXTURE_ENABLED], 1);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture
		(GL_TEXTURE_2D, mesh->textureID);
		glUniform1i(m_parameters[U_COLOR_TEXTURE], 0);
	}
	else
	{
		glUniform1i(m_parameters[U_COLOR_TEXTURE_ENABLED], 0);
	}

	mesh->Render();

	if (mesh->textureID > 0)
	{
		glBindTexture(GL_TEXTURE_2D, 0);
	}
}

void MainMenuScene::setPlay(bool _play)
{
	play = _play;
}

void MainMenuScene::setMode(int _mode)
{
	mode = _mode;
}


bool MainMenuScene::getPlay()
{
	return play;
}

int MainMenuScene::getMode()
{
	return mode;
}

void MainMenuScene::Update(double dt)
{
	float translateZ = 15 + dt;

	camera.Update((float)dt);
	if (Application::IsKeyPressed(VK_NUMPAD1))
	{
		glEnable(GL_CULL_FACE);
	}
	else if (Application::IsKeyPressed(VK_NUMPAD2)) 
	{
		glDisable(GL_CULL_FACE);
	}
	else if (Application::IsKeyPressed(VK_NUMPAD3)) 
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}
	else if (Application::IsKeyPressed(VK_NUMPAD4))
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	}

	if (Application::IsKeyPressed(VK_DOWN)) {
		if (bouncetime <= 0)
		{
			GameSound::instance()->engine->play2D(GameSound::instance()->MenuMove);
			bouncetime = 0.04;
		}
		else
		{
			bouncetime -= dt;
		}
		menuFunctions.ScrollDown();
	}

	if (Application::IsKeyPressed(VK_UP)) {
		if (bouncetime <= 0)
		{
			GameSound::instance()->engine->play2D(GameSound::instance()->MenuMove);
			bouncetime = 0.04;
		}
		else
		{
			bouncetime -= dt;
		}
		menuFunctions.ScrollUp();
	}


	if (MainMenuScene::play)
	{
		if (translateZ < 15)
		{
			camera.SetPosition(0, 0, translateZ);
		}
		//if (MainMenuScene::mode == 1)
		//{
		//	meshList[MainMenu]->textureID = LoadTGA("Image//singleplayer.tga");
		//}
		//if (!MainMenuScene::mode == 2)
		//{
		//	meshList[MainMenu]->textureID = LoadTGA("Image//multiplayer.tga");
		//}
	}
}

void MainMenuScene::Render()
{
	// Render VBO here
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0, 0, 1, 1);

	modelStack.LoadIdentity();

	viewStack.LoadIdentity();

	//glEnable(GL_SCISSOR_TEST);
	viewStack.LookAt(camera.position.x, camera.position.y, camera.position.z, camera.target.x, camera.target.y, camera.target.z, camera.up.x, camera.up.y, camera.up.z);


	glViewport(0, 0, 1920, 1440);
	//glScissor(0, 0, 1920 / 2, 1440);

	
	RenderMainMenu();
	//glDisable(GL_SCISSOR_TEST);
}

void MainMenuScene::Init2() 
{
}

void MainMenuScene::Update2(double dt)
{
}

void MainMenuScene::Render2()
{
}

void MainMenuScene::Exit()
{
	// Cleanup here
	for (int i = 0; i < NUM_GEOMETRY; ++i)
	{
		if (meshList[i] != NULL)
			delete meshList[i];
	}
	glDeleteVertexArrays(1, &m_vertexArrayID);
	glDeleteProgram(m_programID);
}