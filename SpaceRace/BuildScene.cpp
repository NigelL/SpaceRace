#include "BuildScene.h"

#define SIZE 5


BuildScene::BuildScene()
{
	srand(time(0));

	//Factory f;
	//f.CreateGameObject(Name Of Obj ,Material ,Bounds)
	//ship pos amt rot scale

	Mesh* curShip = MeshBuilder::GenerateCube("Cube", Color(1, 1, 1), 1, 1, 1);
	//curShip->textureID = LoadTGA("Image//ship.tga");
	curShip->material.kAmbient.Set(0.4f, 0.4f, 0.4f);
	curShip->material.kDiffuse.Set(0.1f, 0.1f, 0.1f);
	curShip->material.kSpecular.Set(0.2f, 0.2f, 0.2f);
	curShip->material.kShininess = 1.0f;
	mainShip = new GameObject(curShip, Vector3(0, 0, 0), 0, Vector3(0, 1, 0), Vector3(0.1, 0.1, 0.1));
	mainShip->SetBounds(Vector3(1.0f, 1.0f, 1.0f));

	meshList.push_back(mainShip);


	gameText = MeshBuilder::GenerateText("text", 16, 16);
	gameText->textureID = LoadTGA("Image//calibri.tga");

	splashWater = MeshBuilder::GenerateQuad("Test", Color(0, 1, 0), 100, 100);
	splashWater->textureID = LoadTGA("Image//Water.tga");
	splashWater->material.kAmbient.Set(0.5f, 0.5f, 0.5f);
	splashWater->material.kDiffuse.Set(0.0f, 0.0f, 1.0f);
	splashWater->material.kShininess = 1.0f;
	splashWater->material.kSpecular.Set(0.0f, 0.0f, 1.0f);
	splashWaterMesh = new Water;
	splashWaterMesh->waterMesh = splashWater;

	meshList.push_back(new GameObject(splashWater, Vector3(-500, -2.5f, 500), 180, Vector3(1, 0, 0), Vector3(1000, 1, 1000)));


}

BuildScene::~BuildScene()
{

}

static inline float DegreeToRadian(float value)
{
	return value * 3.142f / 180.0f;
}

void BuildScene::RenderText(Mesh* mesh, std::string text, Color color)
{
	if (!mesh || mesh->textureID <= 0) //Proper error check
		return;
	glDisable(GL_DEPTH_TEST);
	glUniform1i(m_parameters[U_TEXT_ENABLED], 1);
	glUniform3fv(m_parameters[U_TEXT_COLOR], 1, &color.r);
	glUniform1i(m_parameters[U_LIGHTENABLED], 0);
	glUniform1i(m_parameters[U_COLOR_TEXTURE_ENABLED], 1);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, mesh->textureID);
	glUniform1i(m_parameters[U_COLOR_TEXTURE], 0);
	for (unsigned i = 0; i < text.length(); ++i)
	{
		Mtx44 characterSpacing;
		characterSpacing.SetToTranslation(i * 1.0f, 0, 0); //1.0f is the spacing of each character, you may change this value
		Mtx44 MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top() *
			characterSpacing;
		glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);
		//std::cout << "ASCII : " << (unsigned)text[i] << std::endl;

		unsigned actualOffset = (16 * 16 * 6) - ((((text[i] / 16)) * 6 * 16) + (16 * 6) - ((text[i] % 16) * 6));
		//std::cout << "Offset :" << actualOffset << std::endl;
		mesh->Render(actualOffset, 6);
	}
	glBindTexture(GL_TEXTURE_2D, 0);
	glUniform1i(m_parameters[U_TEXT_ENABLED], 0);
	glEnable(GL_DEPTH_TEST);
}

void BuildScene::RenderTextOnScreen(Mesh* mesh, std::string text, Color color, float size, float x, float y) {

	if (!mesh || mesh->textureID <= 0) //Proper error check
		return;
	glDisable(GL_DEPTH_TEST);
	Mtx44 ortho;
	ortho.SetToOrtho(0, 1920, 0, 1440, -10, 10); //size of screen UI
	projectionStack.PushMatrix();
	projectionStack.LoadMatrix(ortho);
	viewStack.PushMatrix();
	viewStack.LoadIdentity(); //No need camera for ortho mode
	modelStack.PushMatrix();
	modelStack.LoadIdentity(); //Reset modelStack
	modelStack.Scale(size, size, size);
	modelStack.Translate(x, y, 0);
	glUniform1i(m_parameters[U_TEXT_ENABLED], 1);
	glUniform3fv(m_parameters[U_TEXT_COLOR], 1, &color.r);
	glUniform1i(m_parameters[U_LIGHTENABLED], 0);
	glUniform1i(m_parameters[U_COLOR_TEXTURE_ENABLED], 1);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, mesh->textureID);
	glUniform1i(m_parameters[U_COLOR_TEXTURE], 0);
	for (unsigned i = 0; i < text.length(); ++i)
	{
		Mtx44 characterSpacing;
		characterSpacing.SetToTranslation(i * 1.0f, 0, 0);
		Mtx44 MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top() *
			characterSpacing;
		glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);
		unsigned actualOffset = (16 * 16 * 6) - ((((text[i] / 16)) * 6 * 16) + (16 * 6) - ((text[i] % 16) * 6));
		mesh->Render(actualOffset, 6);
	}
	glBindTexture(GL_TEXTURE_2D, 0);
	glUniform1i(m_parameters[U_TEXT_ENABLED], 0);
	projectionStack.PopMatrix();
	viewStack.PopMatrix();
	modelStack.PopMatrix();

	glEnable(GL_DEPTH_TEST);
}

void BuildScene::RenderUI(Mesh* mesh, float size, float x, float y)
{
	if (!mesh || mesh->textureID <= 0) //Proper error check
		return;
	glDisable(GL_DEPTH_TEST);
	Mtx44 ortho;
	ortho.SetToOrtho(0, 1920, 0, 1440, -10, 10); //size of screen UI

	projectionStack.PushMatrix();
	projectionStack.LoadMatrix(ortho);
	viewStack.PushMatrix();
	viewStack.LoadIdentity(); //No need camera for ortho mode
	modelStack.PushMatrix();
	modelStack.LoadIdentity(); //Reset modelStack
	modelStack.Scale(size, size, size);
	modelStack.Translate(x, y, 0);
	glUniform1i(m_parameters[U_LIGHTENABLED], 0);
	glUniform1i(m_parameters[U_COLOR_TEXTURE_ENABLED], 1);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, mesh->textureID);
	glUniform1i(m_parameters[U_COLOR_TEXTURE], 0);

	Mtx44 MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top();
	glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);
	mesh->Render();

	glBindTexture(GL_TEXTURE_2D, 0);

	projectionStack.PopMatrix();
	viewStack.PopMatrix();
	modelStack.PopMatrix();

	glEnable(GL_DEPTH_TEST);
}

inline void BuildScene::SetGameText(Mesh* curtext)
{
	gameText = curtext;
}

void BuildScene::Init()
{
	//Camera Init
	camera.Init(Vector3(0, 0, 0.1f), Vector3(0, 0, 0), Vector3(0, 1, 0));

	//GameSound::instance()->GameBGM->setDefaultVolume(0.1f);
	//GameSound::instance()->engine->play2D(GameSound::instance()->GameBGM, true);

	Mtx44 projection;
	projection.SetToPerspective(45.f, 4.f / 3.f, 0.1f, 1000.f);
	projectionStack.LoadMatrix(projection);


	glGenVertexArrays(1, &m_vertexArrayID);
	glBindVertexArray(m_vertexArrayID);
	glEnable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);


	// Init VBO here
	m_programID =
		LoadShaders("Shader//Texture.vertexshader", "Shader//Blending.fragmentshader");
	m_parameters[U_MVP] = glGetUniformLocation(m_programID, "MVP");
	m_parameters[U_MODELVIEW] = glGetUniformLocation(m_programID, "MV");
	m_parameters[U_MODELVIEW_INVERSE_TRANSPOSE] =
		glGetUniformLocation(m_programID, "MV_inverse_transpose");
	m_parameters[U_MATERIAL_AMBIENT] = glGetUniformLocation(m_programID,
		"material.kAmbient");
	m_parameters[U_MATERIAL_DIFFUSE] = glGetUniformLocation(m_programID,
		"material.kDiffuse");
	m_parameters[U_MATERIAL_SPECULAR] = glGetUniformLocation(m_programID,
		"material.kSpecular");
	m_parameters[U_MATERIAL_SHININESS] = glGetUniformLocation(m_programID,
		"material.kShininess");
	m_parameters[U_LIGHT0_POSITION] = glGetUniformLocation(m_programID,
		"lights[0].position_cameraspace");
	m_parameters[U_LIGHT0_COLOR] = glGetUniformLocation(m_programID, "lights[0].color");
	m_parameters[U_LIGHT0_POWER] = glGetUniformLocation(m_programID,
		"lights[0].power");
	m_parameters[U_LIGHT0_KC] = glGetUniformLocation(m_programID, "lights[0].kC");
	m_parameters[U_LIGHT0_KL] = glGetUniformLocation(m_programID, "lights[0].kL");
	m_parameters[U_LIGHT0_KQ] = glGetUniformLocation(m_programID, "lights[0].kQ");
	m_parameters[U_LIGHTENABLED] = glGetUniformLocation(m_programID, "lightEnabled");
	m_parameters[U_NUMLIGHTS] = glGetUniformLocation(m_programID, "numLights");
	m_parameters[U_LIGHT0_TYPE] = glGetUniformLocation(m_programID, "lights[0].type");
	m_parameters[U_LIGHT0_SPOTDIRECTION] = glGetUniformLocation(m_programID,
		"lights[0].spotDirection");
	m_parameters[U_LIGHT0_COSCUTOFF] = glGetUniformLocation(m_programID,
		"lights[0].cosCutoff");
	m_parameters[U_LIGHT0_COSINNER] = glGetUniformLocation(m_programID,
		"lights[0].cosInner");
	m_parameters[U_LIGHT0_EXPONENT] = glGetUniformLocation(m_programID,
		"lights[0].exponent");

	m_parameters[U_LIGHT1_POSITION] = glGetUniformLocation(m_programID,
		"lights[1].position_cameraspace");
	m_parameters[U_LIGHT1_COLOR] = glGetUniformLocation(m_programID, "lights[1].color");
	m_parameters[U_LIGHT1_POWER] = glGetUniformLocation(m_programID,
		"lights[1].power");
	m_parameters[U_LIGHT1_KC] = glGetUniformLocation(m_programID, "lights[1].kC");
	m_parameters[U_LIGHT1_KL] = glGetUniformLocation(m_programID, "lights[1].kL");
	m_parameters[U_LIGHT1_KQ] = glGetUniformLocation(m_programID, "lights[1].kQ");

	m_parameters[U_LIGHT1_TYPE] = glGetUniformLocation(m_programID, "lights[1].type");
	m_parameters[U_LIGHT1_SPOTDIRECTION] = glGetUniformLocation(m_programID,
		"lights[0].spotDirection");
	m_parameters[U_LIGHT1_COSCUTOFF] = glGetUniformLocation(m_programID,
		"lights[0].cosCutoff");
	m_parameters[U_LIGHT1_COSINNER] = glGetUniformLocation(m_programID,
		"lights[0].cosInner");
	m_parameters[U_LIGHT1_EXPONENT] = glGetUniformLocation(m_programID,
		"lights[0].exponent");

	m_parameters[U_LIGHT2_POSITION] = glGetUniformLocation(m_programID,
		"lights[2].position_cameraspace");
	m_parameters[U_LIGHT2_COLOR] = glGetUniformLocation(m_programID, "lights[2].color");
	m_parameters[U_LIGHT2_POWER] = glGetUniformLocation(m_programID,
		"lights[2].power");
	m_parameters[U_LIGHT2_KC] = glGetUniformLocation(m_programID, "lights[2].kC");
	m_parameters[U_LIGHT2_KL] = glGetUniformLocation(m_programID, "lights[2].kL");
	m_parameters[U_LIGHT2_KQ] = glGetUniformLocation(m_programID, "lights[2].kQ");

	m_parameters[U_LIGHT2_TYPE] = glGetUniformLocation(m_programID, "lights[2].type");
	m_parameters[U_LIGHT2_SPOTDIRECTION] = glGetUniformLocation(m_programID,
		"lights[2].spotDirection");
	m_parameters[U_LIGHT2_COSCUTOFF] = glGetUniformLocation(m_programID,
		"lights[2].cosCutoff");
	m_parameters[U_LIGHT2_COSINNER] = glGetUniformLocation(m_programID,
		"lights[2].cosInner");
	m_parameters[U_LIGHT2_EXPONENT] = glGetUniformLocation(m_programID,
		"lights[2].exponent");

	m_parameters[U_COLOR_TEXTURE_ENABLED] = glGetUniformLocation(m_programID,
		"colorTextureEnabled");
	m_parameters[U_COLOR_TEXTURE] = glGetUniformLocation(m_programID, "colorTexture");


	// Use our shader
	glUseProgram(m_programID);

	light[0].type = Light::LIGHT_POINT;
	light[0].position.Set(-16, 2, 0.5);
	light[0].color.Set(1, 1, 1);
	light[0].power = 0.5f;
	light[0].kC = 1.0f;
	light[0].kL = 0.01f;
	light[0].kQ = 0.001f;
	light[0].cosCutoff = cos(Math::DegreeToRadian(45));
	light[0].cosInner = cos(Math::DegreeToRadian(30));
	light[0].exponent = 3.0f;
	light[0].spotDirection.Set(0.f, 0.0f, 0.f);

	light[1].type = Light::LIGHT_DIRECTIONAL;
	light[1].position.Set(8, 20, 0.5);
	light[1].color.Set(1, 1, 1);
	light[1].power = 0.1f;
	light[1].kC = 1.0f;
	light[1].kL = 0.01f;
	light[1].kQ = 0.001f;
	light[1].cosCutoff = cos(Math::DegreeToRadian(45));
	light[1].cosInner = cos(Math::DegreeToRadian(30));
	light[1].exponent = 3.0f;
	light[1].spotDirection.Set(0.f, 0.0f, 0.f);

	light[2].type = Light::LIGHT_SPOT;
	light[2].position.Set(8, 20, 0.5);
	light[2].color.Set(0, 1, 0);
	light[2].power = 1.0f;
	light[2].kC = 0.1f;
	light[2].kL = 0.01f;
	light[2].kQ = 0.001f;
	light[2].cosCutoff = cos(Math::DegreeToRadian(10));
	light[2].cosInner = cos(Math::DegreeToRadian(30));
	light[2].exponent = 1.0f;
	light[2].spotDirection.Set(0.f, 1.0f, 0.f);



	//Make sure you pass uniform parameters after glUseProgram()

	glUniform1i(m_parameters[U_NUMLIGHTS], 3);
	glUniform1i(m_parameters[U_LIGHT0_TYPE], light[0].type);
	glUniform3fv(m_parameters[U_LIGHT0_COLOR], 1, &light[0].color.r);
	glUniform1f(m_parameters[U_LIGHT0_POWER], light[0].power);
	glUniform1f(m_parameters[U_LIGHT0_KC], light[0].kC);
	glUniform1f(m_parameters[U_LIGHT0_KL], light[0].kL);
	glUniform1f(m_parameters[U_LIGHT0_KQ], light[0].kQ);
	glUniform1f(m_parameters[U_LIGHT0_COSCUTOFF], light[0].cosCutoff);
	glUniform1f(m_parameters[U_LIGHT0_COSINNER], light[0].cosInner);
	glUniform1f(m_parameters[U_LIGHT0_EXPONENT], light[0].exponent);

	glUniform1i(m_parameters[U_LIGHT1_TYPE], light[1].type);
	glUniform3fv(m_parameters[U_LIGHT1_COLOR], 1, &light[1].color.r);
	glUniform1f(m_parameters[U_LIGHT1_POWER], light[1].power);
	glUniform1f(m_parameters[U_LIGHT1_KC], light[1].kC);
	glUniform1f(m_parameters[U_LIGHT1_KL], light[1].kL);
	glUniform1f(m_parameters[U_LIGHT1_KQ], light[1].kQ);
	glUniform1f(m_parameters[U_LIGHT1_COSCUTOFF], light[1].cosCutoff);
	glUniform1f(m_parameters[U_LIGHT1_COSINNER], light[1].cosInner);
	glUniform1f(m_parameters[U_LIGHT1_EXPONENT], light[1].exponent);

	glUniform1i(m_parameters[U_LIGHT2_TYPE], light[2].type);
	glUniform3fv(m_parameters[U_LIGHT2_COLOR], 1, &light[2].color.r);
	glUniform1f(m_parameters[U_LIGHT2_POWER], light[2].power);
	glUniform1f(m_parameters[U_LIGHT2_KC], light[2].kC);
	glUniform1f(m_parameters[U_LIGHT2_KL], light[2].kL);
	glUniform1f(m_parameters[U_LIGHT2_KQ], light[2].kQ);
	glUniform1f(m_parameters[U_LIGHT2_COSCUTOFF], light[2].cosCutoff);
	glUniform1f(m_parameters[U_LIGHT2_COSINNER], light[2].cosInner);
	glUniform1f(m_parameters[U_LIGHT2_EXPONENT], light[2].exponent);



	m_programID = LoadShaders("Shader//Texture.vertexshader",
		"Shader//Text.fragmentshader");
	m_parameters[U_TEXT_ENABLED] = glGetUniformLocation(m_programID,
		"textEnabled");
	m_parameters[U_TEXT_COLOR] = glGetUniformLocation(m_programID,
		"textColor");

	 
	SpawnPowerUp();
}

static double LSPEED = 10.0;
static short int lightType = 0;
static bool openShop = false;


static double bounceTime = 0.0;

void BuildScene::SpawnPowerUp()
{
	srand(time(NULL));

	int powerUpSpawn;
	float powerUpX, powerUpZ;

	Mesh* SpeedUp = MeshBuilder::GenerateOBJ("Speed Up", "OBJ//SpeedUp.obj");
	SpeedUp->textureID = LoadTGA("Image//SpeedUpTexture.tga");
	SpeedUp->material.kDiffuse.Set(0.99f, 0.99f, 0.99f);
	SpeedUp->material.kSpecular.Set(0.1f, 0.1f, 0.1f);
	SpeedUp->material.kAmbient.Set(0.1f, 0.1f, 0.1f);
	SpeedUp->material.kShininess = 1.0f;

	Mesh* RestoreHP = MeshBuilder::GenerateOBJ("HP Restore", "OBJ//RestoreHP.obj");
	RestoreHP->textureID = LoadTGA("Image//RestoreHPTexture.tga");
	RestoreHP->material.kDiffuse.Set(0.99f, 0.99f, 0.99f);
	RestoreHP->material.kSpecular.Set(0.1f, 0.1f, 0.1f);
	RestoreHP->material.kAmbient.Set(0.1f, 0.1f, 0.1f);
	RestoreHP->material.kShininess = 1.0f;

}


void BuildScene::Update(double dt)
{
	splashWaterMesh->UpdateWater(10, dt);

	mainShip->CheckCollision(camera.position, camera.view, 1);
	for (int j = 0; j < meshList.size(); j++) {
		/*
		for (int i = 0; i < meshList.size(); i++) {
			if (i == j) { continue; }
			meshList[i]->CheckCollision(*meshList[j]);
		}
		*/
		if (j == 0) { continue; }
		meshList[0]->CheckCollision(*meshList[j]);

	}

	short int multipler = 1;

	//Camera Logic
	camera.Update((float)dt);
	float yaw = DegreeToRadian(mainShip->rotate);
	Vector3 direction = Vector3(sin(yaw), 0, cos(yaw));
	Vector3 position = mainShip->GetPosition() - direction * 3;
	

	//Game Logic
	sceneFPS = 1.0f / (float)dt;
	double mouseX, mouseY;
	Application::GetMousePos(mouseX, mouseY);

	// ship stuff
	Vector3 pos, rot, scale;

	mainShip->translateX = (float)(25 * dt);
	mainShip->translateY = (float)(25 * dt);
	mainShip->translateZ = (float)(25 * dt);

	// control the mainShip
	if (Application::IsKeyPressed(VK_UP)) // 270
	{
		if (!(mainShip->GetPosition().x - 1 < -45) && !(mainShip->GetPosition().x + 1 > 45) && !(mainShip->GetPosition().z - 1 < -45) && !(mainShip->GetPosition().z + 1 > 45))
		{

		}

		float direction = DegreeToRadian(mainShip->rotate);
		pos.Set(mainShip->GetPosition().x + (sin(direction) * mainShip->translateX), mainShip->GetPosition().y, mainShip->GetPosition().z + (cos(direction) * mainShip->translateZ));
		mainShip->SetPosition(pos);

		if (Application::IsKeyPressed(VK_LEFT)) // 0
		{
			mainShip->rotate += (int)(2);

			if (mainShip->rotate >= 360)
			{
				mainShip->rotate = 0;
			}

			rot.Set(0, 1, 0);
			mainShip->SetRotation(rot, mainShip->rotate);
		}
		if (Application::IsKeyPressed(VK_RIGHT)) // 180
		{
			mainShip->rotate -= (int)(2);

			if (mainShip->rotate <= 0)
			{
				mainShip->rotate = 360;
			}

			rot.Set(0, 1, 0);
			mainShip->SetRotation(rot, mainShip->rotate);
		}
	}

	if (Application::IsKeyPressed('R'))
	{
		Vector3 initial;
		// mainShip
		mainShip->rotate = 90;
		mainShip->scaleObject = 0;
		mainShip->translateX = 0;
		mainShip->translateY = 0;
		mainShip->translateZ = 0;

		initial.Set(2, 0, -2);
		mainShip->SetPosition(initial);
		initial.Set(0, 1, 0);
		mainShip->SetRotation(initial, mainShip->rotate);
		initial.Set(0.1, 0.1, 0.1);
		mainShip->SetScale(initial);

	}


	/*if (Application::IsKeyPressed(VK_NUMPAD1)) {
		glEnable(GL_CULL_FACE);
	}
	else if (Application::IsKeyPressed(VK_NUMPAD2)) {
		glDisable(GL_CULL_FACE);
	}
	else if (Application::IsKeyPressed(VK_NUMPAD3)) {
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}
	else if (Application::IsKeyPressed(VK_NUMPAD4)) {
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	}*/

	if (Application::IsKeyPressed('I'))
		light[0].position.z -= (float)(LSPEED * dt);
	if (Application::IsKeyPressed('K'))
		light[0].position.z += (float)(LSPEED * dt);
	if (Application::IsKeyPressed('J'))
		light[0].position.x -= (float)(LSPEED * dt);
	if (Application::IsKeyPressed('L'))
		light[0].position.x += (float)(LSPEED * dt);
	if (Application::IsKeyPressed('O'))
		light[0].position.y -= (float)(LSPEED * dt);
	if (Application::IsKeyPressed('P'))
		light[0].position.y += (float)(LSPEED * dt);


}

void BuildScene::RenderMesh(GameObject* curType, bool enableLight)
{

	Mtx44 modelView, modelView_inverse_transpose;
	MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top();
	Mtx44 posPM = projectionStack.Top() * modelStack.Top();
	glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);
	modelView = viewStack.Top() * modelStack.Top();

	glUniformMatrix4fv(m_parameters[U_MODELVIEW], 1, GL_FALSE, &modelView.a[0]);
	if (enableLight)
	{
		glUniform1i(m_parameters[U_LIGHTENABLED], 1);
		glUniform1i(m_parameters[U_LIGHT1ENABLED], 1);
		modelView_inverse_transpose = modelView.GetInverse().GetTranspose();
		glUniformMatrix4fv(m_parameters[U_MODELVIEW_INVERSE_TRANSPOSE], 1, GL_FALSE,
			&modelView_inverse_transpose.a[0]);
		//load material
		glUniform3fv(m_parameters[U_MATERIAL_AMBIENT], 1, &curType->GetMesh()->material.kAmbient.r);
		glUniform3fv(m_parameters[U_MATERIAL_DIFFUSE], 1, &curType->GetMesh()->material.kDiffuse.r);
		glUniform3fv(m_parameters[U_MATERIAL_SPECULAR], 1, &curType->GetMesh()->material.kSpecular.r);
		glUniform1f(m_parameters[U_MATERIAL_SHININESS], curType->GetMesh()->material.kShininess);
	}
	else
	{
		glUniform1i(m_parameters[U_LIGHTENABLED], 0);
		glUniform1i(m_parameters[U_LIGHT1ENABLED], 0);
	}
	if (curType->GetMesh()->textureID > 0)
	{
		glUniform1i(m_parameters[U_COLOR_TEXTURE_ENABLED], 1);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, curType->GetMesh()->textureID);
		glUniform1i(m_parameters[U_COLOR_TEXTURE], 0);
	}
	else
	{
		glUniform1i(m_parameters[U_COLOR_TEXTURE_ENABLED], 0);
	}

	curType->GetMesh()->Render();

	if (curType->GetMesh()->textureID > 0)
	{
		glBindTexture(GL_TEXTURE_2D, 0);
	}
}


void BuildScene::RenderMesh(Mesh* curMesh, bool enableLight) {
	Mtx44 modelView, modelView_inverse_transpose;
	MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top();
	Mtx44 posPM = projectionStack.Top() * modelStack.Top();
	glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);
	modelView = viewStack.Top() * modelStack.Top();

	glUniformMatrix4fv(m_parameters[U_MODELVIEW], 1, GL_FALSE, &modelView.a[0]);
	if (enableLight)
	{
		glUniform1i(m_parameters[U_LIGHTENABLED], 1);
		glUniform1i(m_parameters[U_LIGHT1ENABLED], 1);
		modelView_inverse_transpose = modelView.GetInverse().GetTranspose();
		glUniformMatrix4fv(m_parameters[U_MODELVIEW_INVERSE_TRANSPOSE], 1, GL_FALSE,
			&modelView_inverse_transpose.a[0]);
		//load material
		glUniform3fv(m_parameters[U_MATERIAL_AMBIENT], 1, &curMesh->material.kAmbient.r);
		glUniform3fv(m_parameters[U_MATERIAL_DIFFUSE], 1, &curMesh->material.kDiffuse.r);
		glUniform3fv(m_parameters[U_MATERIAL_SPECULAR], 1, &curMesh->material.kSpecular.r);
		glUniform1f(m_parameters[U_MATERIAL_SHININESS], curMesh->material.kShininess);
	}
	else
	{
		glUniform1i(m_parameters[U_LIGHTENABLED], 0);
		glUniform1i(m_parameters[U_LIGHT1ENABLED], 0);
	}
	if (curMesh->textureID > 0)
	{
		glUniform1i(m_parameters[U_COLOR_TEXTURE_ENABLED], 1);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, curMesh->textureID);
		glUniform1i(m_parameters[U_COLOR_TEXTURE], 0);
	}
	else
	{
		glUniform1i(m_parameters[U_COLOR_TEXTURE_ENABLED], 0);
	}

	curMesh->Render();

	if (curMesh->textureID > 0)
	{
		glBindTexture(GL_TEXTURE_2D, 0);
	}
}

void BuildScene::RenderMesh(GEOMETRY_TYPE curType, bool enableLight)
{
	Mtx44 modelView, modelView_inverse_transpose;
	MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top();
	Mtx44 posPM = projectionStack.Top() * modelStack.Top();
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
		glUniform3fv(m_parameters[U_MATERIAL_AMBIENT], 1, &meshList[curType]->GetMesh()->material.kAmbient.r);
		glUniform3fv(m_parameters[U_MATERIAL_DIFFUSE], 1, &meshList[curType]->GetMesh()->material.kDiffuse.r);
		glUniform3fv(m_parameters[U_MATERIAL_SPECULAR], 1, &meshList[curType]->GetMesh()->material.kSpecular.r);
		glUniform1f(m_parameters[U_MATERIAL_SHININESS], meshList[curType]->GetMesh()->material.kShininess);
	}
	else
	{
		glUniform1i(m_parameters[U_LIGHTENABLED], 0);
	}
	if (meshList[curType]->GetMesh()->textureID > 0)
	{
		glUniform1i(m_parameters[U_COLOR_TEXTURE_ENABLED], 1);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, meshList[curType]->GetMesh()->textureID);
		glUniform1i(m_parameters[U_COLOR_TEXTURE], 0);
	}
	else
	{
		glUniform1i(m_parameters[U_COLOR_TEXTURE_ENABLED], 0);
	}

	meshList[curType]->GetMesh()->Render();

	if (meshList[curType]->GetMesh()->textureID > 0)
	{
		glBindTexture(GL_TEXTURE_2D, 0);
	}
}

void BuildScene::Render()
{
	// Render VBO here
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0, 0, 1, 1);

	//viewStack.LookAt(camera.position.x, camera.position.y, camera.position.z, camera.target.x, camera.target.y, camera.target.z, camera.up.x, camera.up.y, camera.up.z);


	modelStack.LoadIdentity();

	viewStack.LoadIdentity();

	viewStack.LookAt(camera.position.x, camera.position.y, camera.position.z, camera.target.x, camera.target.y, camera.target.z, camera.up.x, camera.up.y, camera.up.z);



	if (light[0].type == Light::LIGHT_DIRECTIONAL)
	{
		Vector3 lightDir(light[0].position.x, light[0].position.y, light[0].position.z);
		Vector3 lightDirection_cameraspace = viewStack.Top() * lightDir;
		glUniform3fv(m_parameters[U_LIGHT0_POSITION], 1,
			&lightDirection_cameraspace.x);
	}
	else if (light[0].type == Light::LIGHT_SPOT)
	{
		Position lightPosition_cameraspace = viewStack.Top() * light[0].position;
		glUniform3fv(m_parameters[U_LIGHT0_POSITION], 1,
			&lightPosition_cameraspace.x);
		Vector3 spotDirection_cameraspace = viewStack.Top() *
			light[0].spotDirection;
		glUniform3fv(m_parameters[U_LIGHT0_SPOTDIRECTION], 1,
			&spotDirection_cameraspace.x);
	}
	else
	{
		Position lightPosition_cameraspace = viewStack.Top() * light[0].position;
		glUniform3fv(m_parameters[U_LIGHT0_POSITION], 1,
			&lightPosition_cameraspace.x);
	}

	if (light[1].type == Light::LIGHT_DIRECTIONAL)
	{
		Vector3 lightDir(light[1].position.x, light[1].position.y, light[1].position.z);
		Vector3 lightDirection_cameraspace = viewStack.Top() * lightDir;
		glUniform3fv(m_parameters[U_LIGHT1_POSITION], 1,
			&lightDirection_cameraspace.x);
	}
	else if (light[1].type == Light::LIGHT_SPOT)
	{
		Position lightPosition_cameraspace = viewStack.Top() * light[1].position;
		glUniform3fv(m_parameters[U_LIGHT1_POSITION], 1,
			&lightPosition_cameraspace.x);
		Vector3 spotDirection_cameraspace = viewStack.Top() *
			light[1].spotDirection;
		glUniform3fv(m_parameters[U_LIGHT1_SPOTDIRECTION], 1,
			&spotDirection_cameraspace.x);
	}
	else
	{
		Position lightPosition_cameraspace = viewStack.Top() * light[1].position;
		glUniform3fv(m_parameters[U_LIGHT1_POSITION], 1,
			&lightPosition_cameraspace.x);
	}

	if (light[2].type == Light::LIGHT_DIRECTIONAL)
	{
		Vector3 lightDir(light[2].position.x, light[2].position.y, light[2].position.z);
		Vector3 lightDirection_cameraspace = viewStack.Top() * lightDir;
		glUniform3fv(m_parameters[U_LIGHT2_POSITION], 1,
			&lightDirection_cameraspace.x);
	}
	else if (light[2].type == Light::LIGHT_SPOT)
	{
		Position lightPosition_cameraspace = viewStack.Top() * light[2].position;
		glUniform3fv(m_parameters[U_LIGHT2_POSITION], 1,
			&lightPosition_cameraspace.x);
		Vector3 spotDirection_cameraspace = viewStack.Top() *
			light[2].spotDirection;
		glUniform3fv(m_parameters[U_LIGHT2_SPOTDIRECTION], 1,
			&spotDirection_cameraspace.x);
	}
	else
	{
		Position lightPosition_cameraspace = viewStack.Top() * light[2].position;
		glUniform3fv(m_parameters[U_LIGHT2_POSITION], 1,
			&lightPosition_cameraspace.x);
	}



	for (int i = 0; i < (int)meshList.size(); i++) {


		//OBB
		/*
		for (int j = 0; j < 8; j++) {


			modelStack.PushMatrix();
			modelStack.Translate(meshList[i]->GetPosition().x + meshList[i]->allBounds[j].x, meshList[i]->GetPosition().y + meshList[i]->allBounds[j].y, meshList[i]->GetPosition().z + meshList[i]->allBounds[j].z);

			modelStack.PushMatrix();
			modelStack.Scale(0.1f, 0.1f, 0.1f);
			RenderMesh(curCube, false);
			modelStack.PopMatrix();
			modelStack.PopMatrix();
		}
		*/


		modelStack.PushMatrix();
		modelStack.Translate(meshList[i]->GetPosition().x, meshList[i]->GetPosition().y, meshList[i]->GetPosition().z);

		modelStack.PushMatrix();
		modelStack.Rotate(meshList[i]->GetAmt(), meshList[i]->GetRotation().x / meshList[i]->GetRotation().Length(), meshList[i]->GetRotation().y / meshList[i]->GetRotation().Length(), meshList[i]->GetRotation().z / meshList[i]->GetRotation().Length());


		modelStack.PushMatrix();
		modelStack.Scale(meshList[i]->GetScale().x, meshList[i]->GetScale().y, meshList[i]->GetScale().z);

		RenderMesh(meshList[i], true);

		modelStack.PopMatrix();
		modelStack.PopMatrix();
		modelStack.PopMatrix();
	}

	RenderTextOnScreen(gameText, "FPS : " + std::to_string(sceneFPS), Color(0, 1, 0), 30, 0, 28);
	RenderTextOnScreen(gameText, "Collision : " + std::to_string(meshList[0]->collidedList.size()), Color(0, 1, 0), 30, 0, 10);


}

void BuildScene::Init2() {

}
void BuildScene::Update2(double dt) {

}
void BuildScene::Render2() {

}

void BuildScene::Exit()
{
	glDeleteVertexArrays(1, &m_vertexArrayID);
	glDeleteProgram(m_programID);
}
