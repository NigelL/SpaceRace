#include "GameScene.h"
#include "ReadMesh.h"


#define SIZE 5

Water* curWater = new Water();

bool cannonOut_01 = false;
bool cannonOut_02 = false;

GameObject* powerUp[15];
Mesh* curCube;

GameScene* GameScene::instance;




GameScene::GameScene()
{
	srand(time(0));
	instance = this;

	curCube = MeshBuilder::GenerateCube("Cube", Color(1, 1, 1), 1, 1, 1);

	//Manual Instantiation

	Mesh* waterMesh = MeshBuilder::GenerateQuad("Water", Color(0, 1, 0), 100, 100);
	waterMesh->textureID = LoadTGA("Image//Water.tga");
	Material *waterMat = new Material;
	waterMat->kAmbient.Set(0.4f, 0.4f, 0.4f);
	waterMat->kDiffuse.Set(0.1f, 0.1f, 0.1f);
	waterMat->kSpecular.Set(0.2f, 0.2f, 0.2f);
	waterMat->kShininess = 1.0f;
	waterMesh->material = *waterMat;
	sceneObjects["Water"] = new GameObject(waterMesh, Vector3(-50, -5.0f, 50), 180, Vector3(1, 0, 0), Vector3(1000, 1, 1000));
	
	curWater->waterMesh = waterMesh;
	meshList.push_back(sceneObjects["Water"]);

	///////////////////////////////////////////////

	//Read From Text File 
	std::vector<MeshInfo> readInfo = ReadFromMesh("text.txt");
	for (auto info : readInfo) {
		std::string infoName = info.meshName;
		Vector3 infoPos = Vector3(info.pos[0], info.pos[1], info.pos[2]);
		Vector3 infoRot = Vector3(info.rot[0], info.rot[1], info.rot[2]);
		Vector3 infoScale = Vector3(info.scale[0], info.scale[1], info.scale[2]);
		Vector3 infoBound = Vector3(info.bound[0], info.bound[1], info.bound[2]);
		float infoAmt = info.amt[0];
		int infoIndex = info.type[0];

		Transform t = Transform();
		t.position = infoPos;
		t.rotation = infoRot;
		t.scale = infoScale;
		t.amt = infoAmt;
		t.SetBounds(infoBound);
		t.type = infoIndex;

		Material* mat = new Material;
		mat->kAmbient.Set(info.ambient[0], info.ambient[1], info.ambient[2]);
		mat->kDiffuse.Set(info.diffuse[0], info.diffuse[1], info.diffuse[2]);
		mat->kSpecular.Set(info.specular[0], info.specular[1], info.specular[2]);
		mat->kShininess = info.shininess[0];

		sceneObjects[infoName] = GameObjectFactory::SpawnGameObject((GameObjectFactory::OBJECT_TYPE)(infoIndex), infoName, mat, t);
		 
		meshList.push_back(sceneObjects[infoName]);
	}

	

	Material *mat = new Material;
	mat->kAmbient.Set(0.4f, 0.4f, 0.4f);
	mat->kDiffuse.Set(0.1f, 0.1f, 0.1f);
	mat->kSpecular.Set(0.2f, 0.2f, 0.2f);
	mat->kShininess = 1.0f;

	Transform t;
	t.boxHalf = Vector3(1.5f,1.5f,1.5f);
	
	int islandx, islandz, islandposx, islandposz, islandposxA[18], islandposzA[18];


	for (int numberofislands = 0; numberofislands < 18; numberofislands++)
	{
		bool overlaps = false;
		islandposx = rand() % 500;
		islandposz = rand() % 500 - 500;
		islandposxA[numberofislands] = islandposx;
		islandposzA[numberofislands] = islandposz;

		// To prevent islands from overlapping
		for (int islands = 0; islands < numberofislands + 1; islands++)
		{
			// So that it won't check the same island as itself
			if (islands == numberofislands) { continue; }

			// Check all four directions to see if the island will land in them
			if ((islandposx <= islandposxA[islands] + 10 && islandposx >= islandposxA[islands] - 10 && islandposz <= islandposzA[islands] + 10 && islandposz >= islandposzA[islands] - 10) /* && not equal to the starting points of the ships*/)
			{
				// If overlaps, the boolean will become true, not printing out a an island
				overlaps = true;
				numberofislands--;
				break;
			}
		}


		// If it doesn't overlap, overlaps is still false, and will print out an island
		if (overlaps == false)
		{

			if (numberofislands <= 8)
			{
				
				GameObject* islands = GameObjectFactory::SpawnGameObject(GameObjectFactory::ISLAND, "someisland", mat, t);
				islands->GetTransform().GenerateBounds();
				islands->SetPosition(Vector3(islandposx, -3.0, islandposz));

				meshList.push_back(islands);

			}
			else
			{
				GameObject* parts = GameObjectFactory::SpawnGameObject(GameObjectFactory::HPCONSUMABLE, "Parts", mat, t);
				
				parts->SetPosition(Vector3(islandposx, 0, islandposz));
				
				meshList.push_back(parts);
			}

			//std::cout << "Island Overlapped, making a new one..." << std::endl;
		}
	}
}

GameScene::~GameScene()
{

}

static inline float DegreeToRadian(float value)
{
	return value * 3.142f / 180.0f;
}

void GameScene::RenderText(Mesh* mesh, std::string text, Color color)
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

void GameScene::RenderTextOnScreen(Mesh* mesh, std::string text, Color color, float size, float x, float y) {

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
			
void GameScene::RenderUI(Mesh* mesh ,float size, float x, float y) 
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

inline void GameScene::SetGameText(Mesh* curtext)
{
	gameText = curtext;
}

void GameScene::Init()
{	
	//Camera Init
	camera.Init(Vector3(10, 20, 0),Vector3(0, 0, 0), Vector3(0, 1, 0));

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
	light[1].position.Set(8,20, 0.5);
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
	light[2].color.Set(0,1,0);
	light[2].power = 1.0f;
	light[2].kC = 0.1f;
	light[2].kL = 0.01f;
	light[2].kQ = 0.001f;
	light[2].cosCutoff = cos(Math::DegreeToRadian(10));
	light[2].cosInner = cos(Math::DegreeToRadian(30));
	light[2].exponent = 1.0f;
	light[2].spotDirection.Set(0.f, 1.0f, 0.f);

	

	//Make sure you pass uniform parameters after glUseProgram()
	
	glUniform1i(m_parameters[U_NUMLIGHTS],3);
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

	SpawnPowerUp();
	
	m_programID = LoadShaders("Shader//Texture.vertexshader",
		"Shader//Text.fragmentshader");
	m_parameters[U_TEXT_ENABLED] = glGetUniformLocation(m_programID,
		"textEnabled");
	m_parameters[U_TEXT_COLOR] = glGetUniformLocation(m_programID,
		"textColor");

	glfwSetCursorPos(Application::getGLFWWindow(), 1000, 700);
}

static double bounceTime = 0.0;

void GameScene::SpawnPowerUp()
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

	for (int numberOfPowerUps = 0; numberOfPowerUps <= 15; numberOfPowerUps++)
	{
		powerUpX = rand() % 100 + (-50);
		powerUpZ = rand() % 100 + (-50);
		powerUpSpawn = rand() % 2;
		
		if (powerUpSpawn == 0)
		{
			powerUp[numberOfPowerUps] = new GameObject(SpeedUp, Vector3(powerUpX, 0, powerUpZ), 90, Vector3(0, 1, 0), Vector3(0.25, 0.25, 0.25));
			meshList.push_back(powerUp[numberOfPowerUps]);
		}
		if (powerUpSpawn == 1)
		{
			powerUp[numberOfPowerUps] = new GameObject(RestoreHP, Vector3(powerUpX, 0, powerUpZ), 90, Vector3(0, 1, 0), Vector3(0.5, 0.5, 0.5));
			meshList.push_back(powerUp[numberOfPowerUps]);
		}
	}
}

void GameScene::Init2() {
	//Camera Init
	camera.Init(Vector3(10, 20, 0), Vector3(0, 0, 0), Vector3(0, 1, 0));

	Mtx44 projection;
	projection.SetToPerspective(45.f, 4.f / 3.f, 0.1f, 1000.f);
	projectionStack.LoadMatrix(projection);

	glGenVertexArrays(1, &m_vertexArrayID);
	glBindVertexArray(m_vertexArrayID);
	glEnable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	curCube = MeshBuilder::GenerateCube("Cube", Color(1, 1, 1), 1, 1, 1);

}

void GameScene::Update2(double dt)
{
	//Camera Logic
	camera.Update((float)dt);
	float yaw = DegreeToRadian(sceneObjects["ship02"]->rotate);
	Vector3 direction = Vector3(sin(yaw), 0, cos(yaw));
	Vector3 position = sceneObjects["ship02"]->GetPosition() - direction * 3;
	camera.SetTarget(sceneObjects["ship02"]->GetPosition().x, sceneObjects["ship02"]->GetPosition().y + 1, sceneObjects["ship02"]->GetPosition().z);
	camera.SetPosition(position.x, position.y + 1, position.z);

	//Game Logic
	sceneFPS = 1.0f / (float)dt;
	double mouseX, mouseY;
	Application::GetMousePos(mouseX, mouseY);

	// ship stuff
	Vector3 pos, rot, scale;

	sceneObjects["ship02"]->translateX = (float)(2 * dt);
	sceneObjects["ship02"]->translateY = (float)(2 * dt);
	sceneObjects["ship02"]->translateZ = (float)(2 * dt);

	// control the sceneObjects["ship02"]

	position = sceneObjects["ship02"]->GetPosition() - direction * 3;
	camera.SetTarget(sceneObjects["ship02"]->GetPosition().x, sceneObjects["ship02"]->GetPosition().y + 1, sceneObjects["ship02"]->GetPosition().z);
	camera.SetPosition(position.x, position.y + 1, position.z);

	//if (Application::IsKeyPressed(VK_NUMPAD5)) // 270
	//{
	//	if (!(sceneObjects["ship02"]->GetPosition().x < -45) && !(sceneObjects["ship02"]->GetPosition().x > 45) && !(sceneObjects["ship02"]->GetPosition().z < -45) && !(sceneObjects["ship02"]->GetPosition().z > 45))
	//	{

	//	}

	//	float direction = DegreeToRadian(sceneObjects["ship02"]->rotate);
	//	pos.Set(sceneObjects["ship02"]->GetPosition().x + (sin(direction) * sceneObjects["ship02"]->translateX), sceneObjects["ship02"]->GetPosition().y, sceneObjects["ship02"]->GetPosition().z + (cos(direction) * sceneObjects["ship02"]->translateZ));
	//	sceneObjects["ship02"]->SetPosition(pos);

	//	if (Application::IsKeyPressed(VK_NUMPAD1)) // 0
	//	{
	//		sceneObjects["ship02"]->rotate += (int)(5);

	//		if (sceneObjects["ship02"]->rotate >= 360)
	//		{
	//			sceneObjects["ship02"]->rotate = 0;
	//		}

	//		rot.Set(0, 1, 0);
	//		sceneObjects["ship02"]->SetRotation(rot, sceneObjects["ship02"]->rotate);
	//	}
	//	if (Application::IsKeyPressed(VK_NUMPAD3)) // 180
	//	{
	//		sceneObjects["ship02"]->rotate -= (int)(5);

	//		if (sceneObjects["ship02"]->rotate <= 0)
	//		{
	//			sceneObjects["ship02"]->rotate = 360;
	//		}

	//		rot.Set(0, 1, 0);
	//		sceneObjects["ship02"]->SetRotation(rot, sceneObjects["ship02"]->rotate);
	//	}
	//}

	if (Application::IsKeyPressed('W')) // 270
	{
		if (!(sceneObjects["ship02"]->GetPosition().x - 1 < -45) && !(sceneObjects["ship02"]->GetPosition().x + 1 > 45) && !(sceneObjects["ship02"]->GetPosition().z - 1 < -45) && !(sceneObjects["ship02"]->GetPosition().z + 1 > 45))
		{

		}

		float direction = DegreeToRadian(sceneObjects["ship02"]->rotate);
		pos.Set(sceneObjects["ship02"]->GetPosition().x + (sin(direction) * sceneObjects["ship02"]->translateX), sceneObjects["ship02"]->GetPosition().y, sceneObjects["ship02"]->GetPosition().z + (cos(direction) * sceneObjects["ship02"]->translateZ));
		sceneObjects["ship02"]->SetPosition(pos);

		if (Application::IsKeyPressed('A')) // 0
		{
			sceneObjects["ship02"]->rotate += (int)(2);

			if (sceneObjects["ship02"]->rotate >= 360)
			{
				sceneObjects["ship02"]->rotate = 0;
			}

			rot.Set(0, 1, 0);
			sceneObjects["ship02"]->SetRotation(rot, sceneObjects["ship02"]->rotate);
		}
	}

	if (Application::IsKeyPressed('D')) // 180
	{
		sceneObjects["ship02"]->rotate -= (int)(2);

		if (sceneObjects["ship02"]->rotate <= 0)
		{
			sceneObjects["ship02"]->rotate = 360;
		}

		rot.Set(0, 1, 0);
		sceneObjects["ship02"]->SetRotation(rot, sceneObjects["ship02"]->rotate);
	}

	if (bounceTime <= 0)
	{
		if (Application::IsKeyPressed('S') && !cannonOut_02) // shoot cannonball
		{
			//cannonOut_02 = true;
			//cannon_02->rotate = sceneObjects["ship02"]->rotate;
			//sceneObjects["ship02"]->x = sceneObjects["ship02"]->GetPosition().x; // get position of ship
			//sceneObjects["ship02"]->z = sceneObjects["ship02"]->GetPosition().z;
			//bounceTime = 0.5;
		}
	}
	else
	{
		bounceTime -= dt;
	}

	//if (cannonOut_02)
	//{
	//	if (cannon_02->translateX == 0 && cannon_02->translateZ == 0) // && cannon_02->translateY == 0
	//	{
	//		GameSound::instance()->PaintSplat->setDefaultVolume(1.f);
	//		GameSound::instance()->engine->play2D(GameSound::instance()->PaintSplat, false);
	//	}
	//	else
	//	{
	//		bounceTime -= dt;
	//	}

	//	cannon_02->translateX += (float)(15 * dt);
	//	/*
	//	if (cannon_02->translateY > -0.25)
	//	{
	//		cannon_02->translateY -= (float)(0.5 * dt);
	//	}
	//	*/
	//	cannon_02->translateZ += (float)(15 * dt);

	//	if (cannon_02->translateX > 5 && cannon_02->translateZ > 5)
	//	{
	//		std::cout << " X : " << cannon_02->translateX << std::endl;
	//		std::cout << " Z : " << cannon_02->translateZ << std::endl;
	//		cannonOut_02 = false;
	//	}

	//	float direction = DegreeToRadian(cannon_02->rotate);

	//	pos.Set(sceneObjects["ship02"]->x + (sin(direction) * cannon_02->translateX), cannon_02->translateY, sceneObjects["ship02"]->z + (cos(direction) * cannon_02->translateZ));
	//	cannon_02->SetPosition(pos);
	//}
	//else
	//{
	//	cannon_02->translateX = 0;
	//	cannon_02->translateZ = 0;
	//	pos.Set(sceneObjects["ship02"]->GetPosition().x, sceneObjects["ship02"]->GetPosition().y + 0.25, sceneObjects["ship02"]->GetPosition().z);
	//	cannon_02->SetPosition(pos);
	//}

	if (Application::IsKeyPressed('R'))
	{
		Vector3 initial;

		sceneObjects["ship02"]->rotate = 90;
		sceneObjects["ship02"]->scaleObject = 0;
		sceneObjects["ship02"]->translateX = 0;
		sceneObjects["ship02"]->translateY = 0;
		sceneObjects["ship02"]->translateZ = 0;

		initial.Set(-2, 0, -2);
		sceneObjects["ship02"]->SetPosition(initial);
		initial.Set(0, 1, 0);
		sceneObjects["ship02"]->SetRotation(initial, sceneObjects["ship02"]->rotate);
		initial.Set(0.1, 0.1, 0.1);
		sceneObjects["ship02"]->SetScale(initial);
	}

	if (Application::IsKeyPressed(VK_SPACE)) // temporary scale the second ship
	{
		//sceneObjects["ship01"]->scaleObject += (float)(1.5 * dt);
		sceneObjects["ship02"]->scaleObject = 0.25;
		scale.Set(sceneObjects["ship02"]->scaleObject, sceneObjects["ship02"]->scaleObject, sceneObjects["ship02"]->scaleObject);
		sceneObjects["ship02"]->SetScale(scale);
	}
}

void GameScene::Render2()
{

	//viewStack.LookAt(camera.position.x, camera.position.y, camera.position.z, camera.target.x, camera.target.y, camera.target.z, camera.up.x, camera.up.y, camera.up.z);


	modelStack.LoadIdentity();

	viewStack.LoadIdentity();


	glEnable(GL_SCISSOR_TEST);

	viewStack.LookAt(camera.position.x, camera.position.y, camera.position.z, camera.target.x, camera.target.y, camera.target.z, camera.up.x, camera.up.y, camera.up.z);

	glViewport(1920 / 2, 0, 1920 / 2, 1440);
	glScissor(1920 / 2, 0, 1920 / 2, 1440);

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

	glDisable(GL_SCISSOR_TEST);
	glDisableVertexAttribArray(2);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(0);
}


void GameScene::Update(double dt)
{
	curWater->UpdateWater(10, dt);


	for (int j = 0; j < meshList.size(); j++) {
		if (meshList[j] == sceneObjects["ship01"]) { continue; }
	
		if (sceneObjects["ship01"]->collision.CheckCollision(meshList[j]->GetTransform())) {
			Consumable* cPtr = dynamic_cast<Consumable*>(meshList[j]);
			IslandEnvironment* isPtr = dynamic_cast<IslandEnvironment*>(meshList[j]);
			std::cout << "Consumable : " << cPtr << "," << "Island : " << isPtr;
			if (isPtr != nullptr) {
				//Ship Collision with island 
			}
			if (cPtr != nullptr) {
				cPtr->OnCollide(*sceneObjects["ship01"]);
				//if (cPtr == HealthConsumable)
				//{

				//}
			}
		}
	}




	short int multipler = 1;

	//Camera Logic
	//camera.Update((float)dt);
	
	float yaw = DegreeToRadian(sceneObjects["ship01"]->rotate);
	Vector3 direction = Vector3(sin(yaw), 0, cos(yaw));
	Vector3 position = sceneObjects["ship01"]->GetPosition() - direction * 3;
	camera.SetTarget(sceneObjects["ship01"]->GetPosition().x, sceneObjects["ship01"]->GetPosition().y + 1, sceneObjects["ship01"]->GetPosition().z);
	camera.SetPosition(position.x, position.y + 1, position.z);

	//Game Logic
	sceneFPS = 1.0f / (float)dt;
	double mouseX, mouseY;
	Application::GetMousePos(mouseX, mouseY);

	// ship stuff
	Vector3 pos, rot, scale;

	sceneObjects["ship01"]->translateX = (float)(25 * dt);
	sceneObjects["ship01"]->translateY = (float)(25 * dt);
	sceneObjects["ship01"]->translateZ = (float)(25 * dt);

	sceneObjects["ship02"]->translateX = (float)(5 * dt);
	sceneObjects["ship02"]->translateY = (float)(5 * dt);
	sceneObjects["ship02"]->translateZ = (float)(5 * dt);

	// control the sceneObjects["ship01"]
	if (Application::IsKeyPressed(VK_UP)) // 270
	{
		if (!(sceneObjects["ship01"]->GetPosition().x - 1 < -45) && !(sceneObjects["ship01"]->GetPosition().x + 1 > 45) && !(sceneObjects["ship01"]->GetPosition().z - 1 < -45) && !(sceneObjects["ship01"]->GetPosition().z + 1 > 45))
		{
			
		}

		float direction = DegreeToRadian(sceneObjects["ship01"]->rotate);
		pos.Set(sceneObjects["ship01"]->GetPosition().x + (sin(direction) * sceneObjects["ship01"]->translateX), sceneObjects["ship01"]->GetPosition().y, sceneObjects["ship01"]->GetPosition().z + (cos(direction) * sceneObjects["ship01"]->translateZ));
		sceneObjects["ship01"]->SetPosition(pos);

		if (Application::IsKeyPressed(VK_LEFT)) // 0
		{
			sceneObjects["ship01"]->rotate += (int)(2);

			if (sceneObjects["ship01"]->rotate >= 360)
			{
				sceneObjects["ship01"]->rotate = 0;
			}

			rot.Set(0, 1, 0);
			sceneObjects["ship01"]->SetRotation(rot, sceneObjects["ship01"]->rotate);
		}
		if (Application::IsKeyPressed(VK_RIGHT)) // 180
		{
			sceneObjects["ship01"]->rotate -= (int)(2);

			if (sceneObjects["ship01"]->rotate <= 0)
			{
				sceneObjects["ship01"]->rotate = 360;
			}

			rot.Set(0, 1, 0);
			sceneObjects["ship01"]->SetRotation(rot, sceneObjects["ship01"]->rotate);
		}
	}
	if (bounceTime <= 0)
	{
		if (Application::IsKeyPressed(VK_DOWN) && !cannonOut_01) // shoot cannonball
		{
			//cannonOut_01 = true;
			//cannon_01->rotate = sceneObjects["ship01"]->rotate;
			//sceneObjects["ship01"]->x = sceneObjects["ship01"]->GetPosition().x; // get position of ship
			//sceneObjects["ship01"]->z = sceneObjects["ship01"]->GetPosition().z;
			//bounceTime = 0.5;
		}
	}
	else
	{
		bounceTime -= dt;
	}

	// control the sceneObjects["ship02"]
	if (Application::IsKeyPressed(VK_NUMPAD5)) // 270
	{
		if (!(sceneObjects["ship02"]->GetPosition().x < -45) && !(sceneObjects["ship02"]->GetPosition().x > 45) && !(sceneObjects["ship02"]->GetPosition().z < -45) && !(sceneObjects["ship02"]->GetPosition().z > 45))
		{
			
		}

		float direction = DegreeToRadian(sceneObjects["ship02"]->rotate);
		pos.Set(sceneObjects["ship02"]->GetPosition().x + (sin(direction) * sceneObjects["ship02"]->translateX), sceneObjects["ship02"]->GetPosition().y, sceneObjects["ship02"]->GetPosition().z + (cos(direction) * sceneObjects["ship02"]->translateZ));
		sceneObjects["ship02"]->SetPosition(pos);

		if (Application::IsKeyPressed(VK_NUMPAD1)) // 0
		{
			sceneObjects["ship02"]->rotate += (int)(5);

			if (sceneObjects["ship02"]->rotate >= 360)
			{
				sceneObjects["ship02"]->rotate = 0;
			}

			rot.Set(0, 1, 0);
			sceneObjects["ship02"]->SetRotation(rot, sceneObjects["ship02"]->rotate);
		}
		if (Application::IsKeyPressed(VK_NUMPAD3)) // 180
		{
			sceneObjects["ship02"]->rotate -= (int)(5);

			if (sceneObjects["ship02"]->rotate <= 0)
			{
				sceneObjects["ship02"]->rotate = 360;
			}

			rot.Set(0, 1, 0);
			sceneObjects["ship02"]->SetRotation(rot, sceneObjects["ship02"]->rotate);
		}
	}
	if (bounceTime <= 0)
	{
		if (Application::IsKeyPressed(VK_NUMPAD2) && !cannonOut_02) // shoot cannonball
		{
			cannonOut_02 = true;
		//	cannon_02->rotate = sceneObjects["ship02"]->rotate;
			sceneObjects["ship02"]->x = sceneObjects["ship02"]->GetPosition().x; // get position of ship
			sceneObjects["ship02"]->z = sceneObjects["ship02"]->GetPosition().z;
			bounceTime = 0.5;
		}
	}
	else
	{
		bounceTime -= dt;
	}

	//if (cannonOut_01)
	//{
	//	if (cannon_01->translateX == 0  && cannon_01->translateZ == 0) // && cannon_01->translateY == 0
	//	{
	//		GameSound::instance()->CannonFire->setDefaultVolume(1.f);
	//		GameSound::instance()->engine->play2D(GameSound::instance()->CannonFire, false);
	//	}

	//	cannon_01->translateX += (float)(10 * dt);
	//	/*
	//	if (cannon_01->translateY > -0.25)
	//	{
	//		cannon_01->translateY -= (float)(0.5 * dt);
	//	}
	//	*/
	//	cannon_01->translateZ += (float)(10 * dt);

	//	if (cannon_01->translateX > 5 && cannon_01->translateZ > 5)
	//	{
	//		cannonOut_01 = false;
	//	}

	//	float direction = DegreeToRadian(cannon_01->rotate);

	//	pos.Set(sceneObjects["ship01"]->x + (sin(direction) * cannon_01->translateX), cannon_01->translateY, sceneObjects["ship01"]->z + (cos(direction) * cannon_01->translateZ));
	//	cannon_01->SetPosition(pos);
	//}
	//else
	//{
	//	cannon_01->translateX = 0;
	//	cannon_01->translateZ = 0;
	//	pos.Set(sceneObjects["ship01"]->GetPosition().x, sceneObjects["ship01"]->GetPosition().y + 0.25, sceneObjects["ship01"]->GetPosition().z);
	//	cannon_01->SetPosition(pos);
	//}

	//if (cannonOut_02)
	//{
	//	if (cannon_02->translateX == 0 && cannon_02->translateZ == 0) // && cannon_02->translateY == 0
	//	{
	//		GameSound::instance()->PaintSplat->setDefaultVolume(1.f);
	//		GameSound::instance()->engine->play2D(GameSound::instance()->PaintSplat, false);
	//	}

	//	cannon_02->translateX += (float)(15 * dt);
	//	/*
	//	if (cannon_02->translateY > -0.25)
	//	{
	//		cannon_02->translateY -= (float)(0.5 * dt);
	//	}
	//	*/
	//	cannon_02->translateZ += (float)(15 * dt);

	//	if (cannon_02->translateX > 5 && cannon_02->translateZ > 5)
	//	{
	//		std::cout << " X : " << cannon_02->translateX << std::endl;
	//		std::cout << " Z : " << cannon_02->translateZ << std::endl;
	//		cannonOut_02 = false;
	//	}

	//	float direction = DegreeToRadian(cannon_02->rotate);

	//	pos.Set(sceneObjects["ship02"]->x + (sin(direction) * cannon_02->translateX),cannon_02->translateY, sceneObjects["ship02"]->z + (cos(direction) * cannon_02->translateZ));
	//	cannon_02->SetPosition(pos);
	//}
	//else
	//{
	//	cannon_02->translateX = 0;
	//	cannon_02->translateZ = 0;
	//	pos.Set(sceneObjects["ship02"]->GetPosition().x, sceneObjects["ship02"]->GetPosition().y + 0.25, sceneObjects["ship02"]->GetPosition().z);
	//	cannon_02->SetPosition(pos);
	//}

	//if (Application::IsKeyPressed(VK_SPACE)) // temporary scale the second ship
	//{
	//	//sceneObjects["ship01"]->scaleObject += (float)(1.5 * dt);
	//	sceneObjects["ship02"]->scaleObject = 0.25;
	//	scale.Set(sceneObjects["ship02"]->scaleObject, sceneObjects["ship02"]->scaleObject, sceneObjects["ship02"]->scaleObject);
	//	sceneObjects["ship02"]->SetScale(scale);
	//}

	//if (Application::IsKeyPressed('R'))
	//{
	//	Vector3 initial;
	//	// sceneObjects["ship01"]
	//	sceneObjects["ship01"]->rotate = 90;
	//	sceneObjects["ship01"]->scaleObject = 0;
	//	sceneObjects["ship01"]->translateX = 0;
	//	sceneObjects["ship01"]->translateY = 0;
	//	sceneObjects["ship01"]->translateZ = 0;

	//	initial.Set(2, 0, -2);
	//	sceneObjects["ship01"]->SetPosition(initial);
	//	initial.Set(0, 1, 0);
	//	sceneObjects["ship01"]->SetRotation(initial, sceneObjects["ship01"]->rotate);
	//	initial.Set(0.1, 0.1, 0.1);
	//	sceneObjects["ship01"]->SetScale(initial);

	//	// sceneObjects["ship02"]
	//	sceneObjects["ship02"]->rotate = 90;
	//	sceneObjects["ship02"]->scaleObject = 0;
	//	sceneObjects["ship02"]->translateX = 0;
	//	sceneObjects["ship02"]->translateY = 0;
	//	sceneObjects["ship02"]->translateZ = 0;

	//	initial.Set(-2, 0, -2);
	//	sceneObjects["ship02"]->SetPosition(initial);
	//	initial.Set(0, 1, 0);
	//	sceneObjects["ship02"]->SetRotation(initial, sceneObjects["ship02"]->rotate);
	//	initial.Set(0.1, 0.1, 0.1);
	//	sceneObjects["ship02"]->SetScale(initial);
	//}


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


}

void GameScene::RenderMesh(GameObject* curType, bool enableLight)
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


void GameScene::RenderMesh(Mesh* curMesh, bool enableLight) {
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

void GameScene::RenderMesh(GEOMETRY_TYPE curType, bool enableLight)
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

void GameScene::Render()
{
	// Render VBO here
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0, 0, 1, 1);

	//viewStack.LookAt(camera.position.x, camera.position.y, camera.position.z, camera.target.x, camera.target.y, camera.target.z, camera.up.x, camera.up.y, camera.up.z);


	modelStack.LoadIdentity();

	viewStack.LoadIdentity();

	glEnable(GL_SCISSOR_TEST);
	viewStack.LookAt(camera.position.x, camera.position.y, camera.position.z, camera.target.x, camera.target.y, camera.target.z, camera.up.x, camera.up.y, camera.up.z);


	glViewport(0, 0, 1920 / 2, 1440);
	glScissor(0, 0, 1920 / 2, 1440);

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

		
		
		for (int j = 0; j < 8; j++) {


			modelStack.PushMatrix();
			modelStack.Translate(meshList[i]->GetPosition().x + meshList[i]->GetTransform().allBounds[j].x, meshList[i]->GetPosition().y + meshList[i]->GetTransform().allBounds[j].y, meshList[i]->GetPosition().z + meshList[i]->GetTransform().allBounds[j].z);

			modelStack.PushMatrix();
			modelStack.Scale(0.1f, 0.1f, 0.1f);
			RenderMesh(curCube, false);
			modelStack.PopMatrix();
			modelStack.PopMatrix();
		}
		


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
	RenderTextOnScreen(gameText, "Collision : " + std::to_string(meshList[0]->collision.collidedList.size()), Color(0, 1, 0), 30, 0, 10);


	glDisable(GL_SCISSOR_TEST);
}

void GameScene::Exit()
{
	delete curCube;
	glDeleteVertexArrays(1, &m_vertexArrayID);
	glDeleteProgram(m_programID);
}
