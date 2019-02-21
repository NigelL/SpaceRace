#include "GameScene.h"
#include "ReadMesh.h"

#define SIZE 5

Water* curWater = new Water();

bool cannonOut_01 = false;
bool cannonOut_02 = false;

float timer = 0;

CShipStats* ship01Stats;
CShipStats* ship02Stats;

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

	gameText = MeshBuilder::GenerateText("text", 16, 16);
	gameText->textureID = LoadTGA("Image//calibri.tga");

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
	mat->kAmbient.Set(0.3f, 0.3f, 0.3f);
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

			if (numberofislands <= 40)
			{
				GameObject* islands = GameObjectFactory::SpawnGameObject(GameObjectFactory::ISLAND, "someisland", mat, t);
				islands->GetTransform().GenerateBounds();
				islands->SetPosition(Vector3(islandposx, -1.0, islandposz));
				islands->SetScale(Vector3(1, 1, 1));
				islands->GetTransform().SetBounds(Vector3(5.5f, 5.f, 5.5f));
				meshList.push_back(islands);

			}
			else
			{
				GameObject* parts = GameObjectFactory::SpawnGameObject(GameObjectFactory::HPCONSUMABLE, "Parts", mat, t);
				parts->SetPosition(Vector3(islandposx, 0, islandposz));
				parts->SetScale(Vector3(1, 1, 1));
				parts->GetTransform().SetBounds(Vector3(1.4f, 0.8f, 2.2f));
				meshList.push_back(parts);
			}

			//std::cout << "Island Overlapped, making a new one..." << std::endl;
		}
	}

	gameText = MeshBuilder::GenerateText("text", 16, 16);
	gameText->textureID = LoadTGA("Image//calibri.tga");
	healthBar = MeshBuilder::GenerateMenu("hpbar", Color(1, 0, 0), 10);
	healthBar->textureID = LoadTGA("Image//hpbar.tga");
	partsCount = MeshBuilder::GenerateMenu("partsUI", Color(1, 0, 0), 10);
	partsCount->textureID = LoadTGA("Image//partsUI.tga");
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

void GameScene::RenderUI(Mesh* mesh ,float sizeX, float sizeY, float x, float y) 
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
	modelStack.Scale(sizeX, sizeY, 0);
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
	m_programID = LoadShaders("Shader//Texture.vertexshader", "Shader//Blending.fragmentshader");

	m_parameters[U_MVP] = glGetUniformLocation(m_programID, "MVP");
	m_parameters[U_MODELVIEW] = glGetUniformLocation(m_programID, "MV");
	m_parameters[U_MODELVIEW_INVERSE_TRANSPOSE] = glGetUniformLocation(m_programID, "MV_inverse_transpose");
	m_parameters[U_MATERIAL_AMBIENT] = glGetUniformLocation(m_programID, "material.kAmbient");
	m_parameters[U_MATERIAL_DIFFUSE] = glGetUniformLocation(m_programID, "material.kDiffuse");
	m_parameters[U_MATERIAL_SPECULAR] = glGetUniformLocation(m_programID, "material.kSpecular");
	m_parameters[U_MATERIAL_SHININESS] = glGetUniformLocation(m_programID, "material.kShininess");

	m_parameters[U_NUMLIGHTS] = glGetUniformLocation(m_programID, "numLights");

	m_parameters[U_LIGHT0_POSITION] = glGetUniformLocation(m_programID, "lights[0].position_cameraspace");
	m_parameters[U_LIGHT0_COLOR] = glGetUniformLocation(m_programID, "lights[0].color");
	m_parameters[U_LIGHT0_POWER] = glGetUniformLocation(m_programID, "lights[0].power");
	m_parameters[U_LIGHT0_KC] = glGetUniformLocation(m_programID, "lights[0].kC");
	m_parameters[U_LIGHT0_KL] = glGetUniformLocation(m_programID, "lights[0].kL");
	m_parameters[U_LIGHT0_KQ] = glGetUniformLocation(m_programID, "lights[0].kQ");
	m_parameters[U_LIGHTENABLED] = glGetUniformLocation(m_programID, "lightEnabled");
	m_parameters[U_LIGHT0_TYPE] = glGetUniformLocation(m_programID, "lights[0].type");
	m_parameters[U_LIGHT0_SPOTDIRECTION] = glGetUniformLocation(m_programID, "lights[0].spotDirection");
	m_parameters[U_LIGHT0_COSCUTOFF] = glGetUniformLocation(m_programID, "lights[0].cosCutoff");
	m_parameters[U_LIGHT0_COSINNER] = glGetUniformLocation(m_programID, "lights[0].cosInner");
	m_parameters[U_LIGHT0_EXPONENT] = glGetUniformLocation(m_programID, "lights[0].exponent");

	m_parameters[U_LIGHT1_POSITION] = glGetUniformLocation(m_programID, "lights[1].position_cameraspace");
	m_parameters[U_LIGHT1_COLOR] = glGetUniformLocation(m_programID, "lights[1].color");
	m_parameters[U_LIGHT1_POWER] = glGetUniformLocation(m_programID, "lights[1].power");
	m_parameters[U_LIGHT1_KC] = glGetUniformLocation(m_programID, "lights[1].kC");
	m_parameters[U_LIGHT1_KL] = glGetUniformLocation(m_programID, "lights[1].kL");
	m_parameters[U_LIGHT1_KQ] = glGetUniformLocation(m_programID, "lights[1].kQ");
	m_parameters[U_LIGHT1_TYPE] = glGetUniformLocation(m_programID, "lights[1].type");
	m_parameters[U_LIGHT1_SPOTDIRECTION] = glGetUniformLocation(m_programID, "lights[1].spotDirection");
	m_parameters[U_LIGHT1_COSCUTOFF] = glGetUniformLocation(m_programID, "lights[1].cosCutoff");
	m_parameters[U_LIGHT1_COSINNER] = glGetUniformLocation(m_programID, "lights[1].cosInner");
	m_parameters[U_LIGHT1_EXPONENT] = glGetUniformLocation(m_programID, "lights[1].exponent");

	m_parameters[U_LIGHT2_POSITION] = glGetUniformLocation(m_programID, "lights[2].position_cameraspace");
	m_parameters[U_LIGHT2_COLOR] = glGetUniformLocation(m_programID, "lights[2].color");
	m_parameters[U_LIGHT2_POWER] = glGetUniformLocation(m_programID, "lights[2].power");
	m_parameters[U_LIGHT2_KC] = glGetUniformLocation(m_programID, "lights[2].kC");
	m_parameters[U_LIGHT2_KL] = glGetUniformLocation(m_programID, "lights[2].kL");
	m_parameters[U_LIGHT2_KQ] = glGetUniformLocation(m_programID, "lights[2].kQ");
	m_parameters[U_LIGHT2_TYPE] = glGetUniformLocation(m_programID, "lights[2].type");
	m_parameters[U_LIGHT2_SPOTDIRECTION] = glGetUniformLocation(m_programID, "lights[2].spotDirection");
	m_parameters[U_LIGHT2_COSCUTOFF] = glGetUniformLocation(m_programID, "lights[2].cosCutoff");
	m_parameters[U_LIGHT2_COSINNER] = glGetUniformLocation(m_programID, "lights[2].cosInner");
	m_parameters[U_LIGHT2_EXPONENT] = glGetUniformLocation(m_programID, "lights[2].exponent");

	m_parameters[U_COLOR_TEXTURE_ENABLED] = glGetUniformLocation(m_programID, "colorTextureEnabled");
	m_parameters[U_COLOR_TEXTURE] = glGetUniformLocation(m_programID, "colorTexture");

	// Use our shader
	glUseProgram(m_programID);

	light[0].type = Light::LIGHT_DIRECTIONAL;
	light[0].position.Set(0, 1000, 0);
	light[0].color.Set(1, 1, 1);
	light[0].power = 1.f;
	light[0].kC = 1.0f;
	light[0].kL = 0.01f;
	light[0].kQ = 0.001f;
	light[0].cosCutoff = cos(Math::DegreeToRadian(45));
	light[0].cosInner = cos(Math::DegreeToRadian(30));
	light[0].exponent = 3.0f;
	light[0].spotDirection.Set(0.0f, 0.0f, 0.0f);

	light[1].type = Light::LIGHT_POINT;
	light[1].position.Set(sceneObjects["ship01"]->GetPosition().x + (sin(DegreeToRadian(sceneObjects["ship01"]->GetAmt())) * 1.5), sceneObjects["ship01"]->GetPosition().y + 0.25, sceneObjects["ship01"]->GetPosition().z + (cos(DegreeToRadian(sceneObjects["ship01"]->GetAmt())) * 1.5));
	light[1].color.Set(1, 1, 1);
	light[1].power = 1.f;
	light[1].kC = 1.0f;
	light[1].kL = 0.01f;
	light[1].kQ = 0.001f;
	light[1].cosCutoff = cos(Math::DegreeToRadian(45));
	light[1].cosInner = cos(Math::DegreeToRadian(30));
	light[1].exponent = 3.0f;
	light[1].spotDirection.Set(0.0f, 0.0f, 0.0f);

	light[2].type = Light::LIGHT_POINT;
	light[2].position.Set(sceneObjects["ship02"]->GetPosition().x + (sin(DegreeToRadian(sceneObjects["ship02"]->GetAmt())) * 1.5), sceneObjects["ship02"]->GetPosition().y + 0.25, sceneObjects["ship02"]->GetPosition().z + (cos(DegreeToRadian(sceneObjects["ship02"]->GetAmt())) * 1.5));
	light[2].color.Set(1, 1, 1);
	light[2].power = 1.f;
	light[2].kC = 1.0f;
	light[2].kL = 0.01f;
	light[2].kQ = 0.001f;
	light[2].cosCutoff = cos(Math::DegreeToRadian(45));
	light[2].cosInner = cos(Math::DegreeToRadian(30));
	light[2].exponent = 3.0f;
	light[2].spotDirection.Set(0.0f, 0.0f, 0.0f);
	
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
	
	m_programID = LoadShaders("Shader//Texture.vertexshader", "Shader//Text.fragmentshader");
	m_parameters[U_TEXT_ENABLED] = glGetUniformLocation(m_programID, "textEnabled");
	m_parameters[U_TEXT_COLOR] = glGetUniformLocation(m_programID, "textColor");

	glfwSetCursorPos(Application::getGLFWWindow(), 1000, 700);

	SpawnPowerUp();
	renderSkybox();

	ship01Stats = dynamic_cast<CShipStats*>(sceneObjects["ship01"]);
	ship01Stats->setStats(0, 300, 10, 5, 30, 25);
	ship02Stats = dynamic_cast<CShipStats*>(sceneObjects["ship02"]);
	ship02Stats->setStats(50, 500, 15, 5, 50, 20);
}

static double LSPEED = 10.0;
static short int lightType = 0;
static bool openShop = false;
int shopItem;

Vector3 ray_wor;
Vector3 ray_Clip;
Vector3 curHitPoint;

static double bounceTime = 0.0;
static double cannonHit_01 = 0.0f;
static double cannonHit_02 = 0.0f;

float Lerp(float position, float target, float amt) 
{
	float lerp = (target - position) * amt;
	return position + lerp;
}

void GameScene::Update(double dt)
{
	std::cout << ship01Stats->getParts() << std::endl;

	curWater->UpdateWater(10, dt / 2);

	for (int j = 0; j < meshList.size(); j++) {
		if (meshList[j] == sceneObjects["ship01"]) { continue; }
	
		if (sceneObjects["ship01"]->collision.CheckCollision(meshList[j]->GetTransform())) {
			Consumable* cPtr = dynamic_cast<Consumable*>(meshList[j]);
			IslandEnvironment* isPtr = dynamic_cast<IslandEnvironment*>(meshList[j]);
			std::cout << "Consumable : " << cPtr << "," << "Island : " << isPtr << std::endl;
			if (isPtr != nullptr) {
				//Ship Collision with island 
			}
			if (cPtr != nullptr) {
				cPtr->OnCollide(*sceneObjects["ship01"]);
			}
		}
	}

	if (ship01Stats->getSpeed() > 50)
	{
		timer += dt;

		if (timer >= 5)
		{
			ship01Stats->SetSpeed(50);
			timer = 0;
		}
	}

	/*for (int j = 0; j < meshList.size(); j++)
	{
		if (j == 0)
		{
			continue;
		}

		sceneObjects["ship01"]->CheckCollision(*meshList[j], sceneObjects["ship01"]->collidedList); // every collision
		sceneObjects["ship01"]->CheckCollision(*cannon_02, sceneObjects["ship01"]->collideCannon); // cannon collision
		// pirateShip->CheckCollision(*cannon_01, pirateShip->collideCannon);
	}

	if (sceneObjects["ship01"]->collideCannon.size() > 0 && cannonHit_01 <= 0)
	{
		sceneObjects["ship01"]->health--;
		cannonHit_01 = 0.25;
	}
	else
	{
		cannonHit_01 -= dt;
	}*/

	/*if (pirateShip->collideCannon.size() > 0) // game end when shoot pirate ship
	{
		exit(0);
	}*/

	short int multipler = 1;

	//Camera Logic
	float output;
	float input1;
	float input2;

	

	float yaw = DegreeToRadian(sceneObjects["ship01"]->GetAmt());
	Vector3 direction = Vector3(sin(yaw), 0, cos(yaw));
	Vector3 position = sceneObjects["ship01"]->GetPosition() - direction * 3;
	Vector3 smoothDamp;
	//if (smoothDamp.x != position.x)
	//{
		smoothDamp.x += position.x * 0.99;
	//}
	//if (smoothDamp.z != position.z)
	//{
		smoothDamp.z += position.z * 0.99;
	//}
	camera.SetTarget(sceneObjects["ship01"]->GetPosition().x, sceneObjects["ship01"]->GetPosition().y + 1, sceneObjects["ship01"]->GetPosition().z);
	camera.SetPosition(smoothDamp.x, position.y + 1, smoothDamp.z);


bool bouncing = false;

float floating = 0.0f; // maybe use

void GameScene::Update(double dt)
{
	curWater->UpdateWater(10, dt / 2);

	/////////

	Physic physic;

	physic.VoA(20, 0.18);
	physic.AdvRatio(200, 10);
	physic.thrust(0.12, 10, 10);

	/////////

	for (int j = 0; j < meshList.size(); j++) {
		if (meshList[j] == sceneObjects["ship01"]) { continue; }
		
		if (sceneObjects["ship01"]->collision.CheckCollision(meshList[j]->GetTransform())) {
			Consumable* cPtr = dynamic_cast<Consumable*>(meshList[j]);
			IslandEnvironment* isPtr = dynamic_cast<IslandEnvironment*>(meshList[j]);
			cannonball* cannonPtr = dynamic_cast<cannonball*>(meshList[j]);

			std::cout << "Consumable : " << cPtr << "," << "Island : " << isPtr << std::endl;

			if (isPtr != nullptr) {
				isPtr->OnCollide(*ship01Stats); //Ship Collision with island
				bouncing = true;
			}

			if (cPtr != nullptr) {
				cPtr->OnCollide(*sceneObjects["ship01"]); // consumable collision
			}

			if (cannonPtr != nullptr && meshList[j] == sceneObjects["cannon02"]) { // other ship cannon hit ship01
				cannonPtr->OnCollide(*ship01Stats);
			}
		}
	}

	if (bouncing && ship01Stats->getSpeed() > 0)
	{
		physic.bounceBack(*ship01Stats, dt, 10);
		sceneObjects["ship01"]->translateObj(-ship01Stats->getSpeed() * 1.5, dt);
	}
	else
	{
		bouncing = false;
	}

	short int multipler = 1;

	//Camera Logic
	float yaw = DegreeToRadian(sceneObjects["ship01"]->GetAmt());
	Vector3 direction = Vector3(sin(yaw), 0, cos(yaw));
	Vector3 position = sceneObjects["ship01"]->GetPosition() - direction * 20;
	camera.SetTarget(sceneObjects["ship01"]->GetPosition().x, sceneObjects["ship01"]->GetPosition().y + 6, sceneObjects["ship01"]->GetPosition().z);
	camera.SetPosition(position.x, position.y + 5, position.z);

	//Game Logic
	sceneFPS = 1.0f / (float)dt;
	double mouseX, mouseY;
	Application::GetMousePos(mouseX, mouseY);

	// ship ...physics
	/*if (curWater->getWater() < 0.5)
	{
		sceneObjects["ship01"]->SetPosition(Vector3(sceneObjects["ship01"]->GetPosition().x, -curWater->getWater() / 20, sceneObjects["ship01"]->GetPosition().z));
	}
	else if (curWater->getWater() > 0.5)
	{
		sceneObjects["ship01"]->SetPosition(Vector3(sceneObjects["ship01"]->GetPosition().x, curWater->getWater() / 20, sceneObjects["ship01"]->GetPosition().z));
	}*/

	sceneObjects["ship01"]->SetPosition(Vector3(sceneObjects["ship01"]->GetPosition().x, floating, sceneObjects["ship01"]->GetPosition().z));

	std::cout << "float : " << floating << std::endl;

	if (Application::IsKeyPressed(VK_SPACE))
	{
		std::cout << curWater->getWater() << std::endl;
	}

	// control the sceneObjects["ship01"]
	if (Application::IsKeyPressed(VK_UP)) // 270
	{
		sceneObjects["ship01"]->translateObj(ship01Stats->getSpeed(), dt);

		if (Application::IsKeyPressed(VK_LEFT)) // 0
		{
			sceneObjects["ship01"]->rotateObj(10);
		}
		if (Application::IsKeyPressed(VK_RIGHT)) // 180
		{
			sceneObjects["ship01"]->rotateObj(-10);
		}

		light[1].position.x = sceneObjects["ship01"]->GetPosition().x + (sin(DegreeToRadian(sceneObjects["ship01"]->GetAmt())) * 1.5);
		light[1].position.y = sceneObjects["ship01"]->GetPosition().y + 0.25;
		light[1].position.z = sceneObjects["ship01"]->GetPosition().z + (cos(DegreeToRadian(sceneObjects["ship01"]->GetAmt())) * 1.5);
	}
	if (bounceTime <= 0)
	{
		if (Application::IsKeyPressed(VK_DOWN) && !cannonOut_01) // shoot cannonball
		{
			cannonOut_01 = true;
			sceneObjects["cannon01"]->SetAmt(sceneObjects["ship01"]->GetAmt());
			bounceTime = 0.5;
		}
	}
	else
	{
		bounceTime -= dt;
	}

	if (cannonOut_01)
	{
		if (sceneObjects["cannon01"]->GetTranslateX() == 0.0f && sceneObjects["cannon01"]->GetTranslateZ() == 0.0f) // && sceneObjects["cannon01"]->translateY == 0
		{
			GameSound::instance()->CannonFire->setDefaultVolume(1.f);
			GameSound::instance()->engine->play2D(GameSound::instance()->CannonFire, false);
		}

		sceneObjects["cannon01"]->translateObj(10, dt);
		sceneObjects["cannon01"]->translateCannon(10, dt);

		if (sceneObjects["cannon01"]->GetTranslateX() > 5 && sceneObjects["cannon01"]->GetTranslateZ() > 5)
		{
			cannonOut_01 = false;

	light[1].position.x = sceneObjects["ship01"]->GetPosition().x + (sin(DegreeToRadian(sceneObjects["ship01"]->GetAmt())) * 1.5);
	light[1].position.y = sceneObjects["ship01"]->GetPosition().y + 0.25;
	light[1].position.z = sceneObjects["ship01"]->GetPosition().z + (cos(DegreeToRadian(sceneObjects["ship01"]->GetAmt())) * 1.5);

	// control the sceneObjects["ship01"]
	if (Application::IsKeyPressed(VK_UP) && !bouncing) // 270
	{
		if (ship01Stats->getSpeed() < ship01Stats->getMaxSpeed()) // speed limit
		{
			//std::cout << " thrust : " << physic.Thrust << std::endl;

			ship01Stats->SetSpeed(physic.acceleration(ship01Stats->getSpeed(), dt));
		}

		sceneObjects["ship01"]->translateObj(ship01Stats->getSpeed(), dt);

		if (Application::IsKeyPressed(VK_LEFT)) // 0
		{
			sceneObjects["ship01"]->rotateObj(ship01Stats->getRotate());
		}
		if (Application::IsKeyPressed(VK_RIGHT)) // 180
		{
			sceneObjects["ship01"]->rotateObj(-ship01Stats->getRotate());
		}
	}
	else if (ship01Stats->getSpeed() > 0)
	{
		ship01Stats->SetSpeed(physic.deceleration(ship01Stats->getSpeed(), dt));
		sceneObjects["ship01"]->translateObj(ship01Stats->getSpeed(), dt);
	}

	if (bounceTime <= 0)
	{
		if (Application::IsKeyPressed(VK_DOWN) && !cannonOut_01) // shoot cannonball
		{
			cannonOut_01 = true;
			sceneObjects["cannon01"]->SetAmt(sceneObjects["ship01"]->GetAmt());
			bounceTime = 0.5;
		}
	}
	else
	{
		sceneObjects["cannon01"]->SetTranslateX(0);
		sceneObjects["cannon01"]->SetTranslateZ(0);
		sceneObjects["cannon01"]->SetPosition(Vector3(sceneObjects["ship01"]->GetPosition().x, sceneObjects["ship01"]->GetPosition().y, sceneObjects["ship01"]->GetPosition().z));
	}

	if (cannonOut_01)
	{
		if (sceneObjects["cannon01"]->GetTranslateX() == 0.0f && sceneObjects["cannon01"]->GetTranslateZ() == 0.0f) // && sceneObjects["cannon01"]->translateY == 0
		{
			GameSound::instance()->CannonFire->setDefaultVolume(1.f);
			GameSound::instance()->engine->play2D(GameSound::instance()->CannonFire, false);
		}

		sceneObjects["cannon01"]->translateObj(ship01Stats->getFireRate(), dt);
		sceneObjects["cannon01"]->translateCannon(ship01Stats->getFireRate(), dt);

		if (ship01Stats->getSpeed() > 0)
		{
			physic.bounceBack(*ship01Stats, dt, 2);
			sceneObjects["ship01"]->translateObj(-ship01Stats->getSpeed() * 1.5, dt);
		}

		if (sceneObjects["cannon01"]->GetTranslateX() > ship01Stats->getFireDist() && sceneObjects["cannon01"]->GetTranslateZ() > ship01Stats->getFireDist())
		{
			cannonOut_01 = false;
		}
	}
	else
	{
		sceneObjects["cannon01"]->SetTranslateX(0);
		sceneObjects["cannon01"]->SetTranslateZ(0);
		sceneObjects["cannon01"]->SetPosition(Vector3(sceneObjects["ship01"]->GetPosition().x, sceneObjects["ship01"]->GetPosition().y, sceneObjects["ship01"]->GetPosition().z));
	}

	if (Application::IsKeyPressed('R'))
	{
		sceneObjects["ship01"]->SetPosition(Vector3(0, 0, 0));
		sceneObjects["ship01"]->SetAmt(90);
		sceneObjects["ship01"]->SetRotation(Vector3(0, 1, 0), sceneObjects["ship01"]->GetAmt());
		sceneObjects["ship01"]->SetScale(Vector3(0.1, 0.1, 0.1));
	}

	if (Application::IsKeyPressed(VK_NUMPAD1)) {
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

	RenderTextOnScreen(gameText, "Speed : " + std::to_string(ship01Stats->getSpeed()), Color(0, 1, 0), 50, 2, 20);
	//RenderTextOnScreen(gameText, "Rotate : " + std::to_string(ship01Stats->getRotate()), Color(0, 1, 0), 50, 2, 15);
	//RenderTextOnScreen(gameText, "FireRate : " + std::to_string(ship01Stats->getFireRate()), Color(0, 1, 0), 50, 2, 12.5);
	RenderTextOnScreen(gameText, " :" + std::to_string(ship01Stats->getParts()), Color(0, 1, 0), 45, 4, 3);
	RenderUI(partsCount, 20, 7, 5, 22);
	RenderTextOnScreen(gameText, "Health : " + std::to_string(ship01Stats->getHealth()), Color(0, 1, 0), 40, 1, 2);
	RenderUI(healthBar, ship01Stats->getHealth() * 5, 3.5, 5, 20);

	//RenderTextOnScreen(gameText, "Collision : ", Color(0, 1, 0), 10, 2, 15);

	glDisable(GL_SCISSOR_TEST);
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
	float yaw = DegreeToRadian(sceneObjects["ship02"]->GetAmt());
	Vector3 direction = Vector3(sin(yaw), 0, cos(yaw));
	Vector3 position = sceneObjects["ship02"]->GetPosition() - direction * 20;
	camera.SetTarget(sceneObjects["ship02"]->GetPosition().x, sceneObjects["ship02"]->GetPosition().y + 6, sceneObjects["ship02"]->GetPosition().z);
	camera.SetPosition(position.x, position.y + 5, position.z);

	//Game Logic
	sceneFPS = 1.0f / (float)dt;
	double mouseX, mouseY;
	Application::GetMousePos(mouseX, mouseY);

	// control the sceneObjects["ship02"]
	position = sceneObjects["ship02"]->GetPosition() - direction * 3;
	camera.SetTarget(sceneObjects["ship02"]->GetPosition().x, sceneObjects["ship02"]->GetPosition().y + 1, sceneObjects["ship02"]->GetPosition().z);
	camera.SetPosition(position.x, position.y + 1, position.z);

	// collision for ship and cannon
	for (int j = 0; j < meshList.size(); j++)
	{
		if (j == 0)
		{
			continue;
	// collision for ship and cannon
	for (int j = 0; j < meshList.size(); j++) {
		if (meshList[j] == sceneObjects["ship02"]) { continue; }

		if (sceneObjects["ship02"]->collision.CheckCollision(meshList[j]->GetTransform())) {
			Consumable* cPtr = dynamic_cast<Consumable*>(meshList[j]);
			IslandEnvironment* isPtr = dynamic_cast<IslandEnvironment*>(meshList[j]);
			cannonball* cannonPtr = dynamic_cast<cannonball*>(meshList[j]);
			std::cout << "Consumable : " << cPtr << "," << "Island : " << isPtr << std::endl;
			if (isPtr != nullptr) {
				//Ship Collision with island
			}
			if (cPtr != nullptr) {
				cPtr->OnCollide(*sceneObjects["ship02"]);
			}
			if (cannonPtr != nullptr && meshList[j] == sceneObjects["cannon01"])
			{
				cannonPtr->OnCollide(*ship02Stats);
			}
		}
	}

		// sceneObjects["ship02"]->CheckCollision(*meshList[j], sceneObjects["ship02"]->collidedList);
		// sceneObjects["ship02"]->CheckCollision(*cannon_01, sceneObjects["ship02"]>collideCannon);
	}

	/*if (sceneObjects["ship02"]->collideCannon.size() > 0 && cannonHit_02 <= 0)
	{
		sceneObjects["ship02"]->health--;
		cannonHit_02 = 0.5;
	}
	else
	{
		cannonHit_02 -= dt;
	}*/

	//Ship 2 Bobbing
	if (curWater->getWater() < 0.5)
	{
		sceneObjects["ship02"]->SetPosition(Vector3(sceneObjects["ship02"]->GetPosition().x, (-curWater->getWater() - 40) / 10, sceneObjects["ship02"]->GetPosition().z));
	}
	else if (curWater->getWater() > 0.5)
	{
		sceneObjects["ship02"]->SetPosition(Vector3(sceneObjects["ship02"]->GetPosition().x, (curWater->getWater() - 40) / 10, sceneObjects["ship02"]->GetPosition().z));
	}

	// control the sceneObjects["ship02"]
	if (Application::IsKeyPressed('W')) // 270
	{
		sceneObjects["ship02"]->translateObj(20, dt);

		if (Application::IsKeyPressed('A')) // 0
		{
			sceneObjects["ship02"]->rotateObj(5);
		}
		if (Application::IsKeyPressed('D')) // 180
		{
			sceneObjects["ship02"]->rotateObj(-5);
	/*if (curWater->getWater() < 0.5)
	{
		sceneObjects["ship02"]->SetPosition(Vector3(sceneObjects["ship02"]->GetPosition().x, -curWater->getWater() / 20, sceneObjects["ship02"]->GetPosition().z));
	}
	else if (curWater->getWater() > 0.5)
	{
		sceneObjects["ship02"]->SetPosition(Vector3(sceneObjects["ship02"]->GetPosition().x, curWater->getWater() / 20, sceneObjects["ship02"]->GetPosition().z));
	}*/

	light[2].position.x = sceneObjects["ship02"]->GetPosition().x + (sin(DegreeToRadian(sceneObjects["ship02"]->GetAmt())) * 1.5);
	light[2].position.y = sceneObjects["ship02"]->GetPosition().y + 0.25;
	light[2].position.z = sceneObjects["ship02"]->GetPosition().z + (cos(DegreeToRadian(sceneObjects["ship02"]->GetAmt())) * 1.5);

	// control the sceneObjects["ship02"]
	if (Application::IsKeyPressed('W')) // 270
	{
		sceneObjects["ship02"]->translateObj(ship02Stats->getSpeed(), dt);

		if (Application::IsKeyPressed('A')) // 0
		{
			sceneObjects["ship02"]->rotateObj(ship02Stats->getRotate());
		}
		if (Application::IsKeyPressed('D')) // 180
		{
			sceneObjects["ship02"]->rotateObj(-ship02Stats->getRotate());
		}

		light[2].position.x = sceneObjects["ship02"]->GetPosition().x + (sin(DegreeToRadian(sceneObjects["ship02"]->GetAmt())) * 1.5);
		light[2].position.y = sceneObjects["ship02"]->GetPosition().y + 0.25;
		light[2].position.z = sceneObjects["ship02"]->GetPosition().z + (cos(DegreeToRadian(sceneObjects["ship02"]->GetAmt())) * 1.5);
	}

	if (bounceTime <= 0)
	{
		if (Application::IsKeyPressed('S') && !cannonOut_02) // shoot cannonball
		{
			cannonOut_02 = true;
			sceneObjects["cannon02"]->SetAmt(sceneObjects["ship02"]->GetAmt());
			bounceTime = 0.5;
		}
	}
	else
	{
		bounceTime -= dt;
	}

	if (cannonOut_02)
	{
		if (sceneObjects["cannon02"]->GetTranslateX() == 0.0f && sceneObjects["cannon02"]->GetTranslateZ() == 0.0f) // && cannon_02->translateY == 0
		{
			GameSound::instance()->PaintSplat->setDefaultVolume(1.f);
			GameSound::instance()->engine->play2D(GameSound::instance()->PaintSplat, false);
		}

		sceneObjects["cannon02"]->translateObj(ship02Stats->getFireRate(), dt);
		sceneObjects["cannon02"]->translateCannon(ship02Stats->getFireRate(), dt);

		if (sceneObjects["cannon02"]->GetTranslateX() > 20 && sceneObjects["cannon02"]->GetTranslateZ() > 20)
		{
			cannonOut_02 = false;
		}
	}
	else
	{
		sceneObjects["cannon02"]->SetTranslateX(0);
		sceneObjects["cannon02"]->SetTranslateZ(0);
		sceneObjects["cannon02"]->SetPosition(Vector3(sceneObjects["ship02"]->GetPosition().x, sceneObjects["ship02"]->GetPosition().y, sceneObjects["ship02"]->GetPosition().z));
	}

	if (Application::IsKeyPressed('R'))
	{
		sceneObjects["ship02"]->SetPosition(Vector3(10, 0, 0));
		sceneObjects["ship02"]->SetAmt(90);
		sceneObjects["ship02"]->SetRotation(Vector3(0, 1, 0), sceneObjects["ship02"]->GetAmt());
		sceneObjects["ship02"]->SetScale(Vector3(0.5, 0.5, 0.5));
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
		modelStack.Rotate(meshList[i]->GetAmt(), meshList[i]->GetRotation().x, meshList[i]->GetRotation().y, meshList[i]->GetRotation().z);

		modelStack.PushMatrix();
		modelStack.Scale(meshList[i]->GetScale().x, meshList[i]->GetScale().y, meshList[i]->GetScale().z);

		RenderMesh(meshList[i], true);

		modelStack.PopMatrix();
		modelStack.PopMatrix();
		modelStack.PopMatrix();
	}

	RenderTextOnScreen(gameText, "Speed : " + std::to_string(ship02Stats->getSpeed()), Color(0, 1, 0), 50, 2, 20);
	RenderTextOnScreen(gameText, "Health : " + std::to_string(ship02Stats->getHealth()), Color(0, 1, 0), 50, 2, 17.5);
	RenderTextOnScreen(gameText, "Rotate : " + std::to_string(ship02Stats->getRotate()), Color(0, 1, 0), 50, 2, 15);
	RenderTextOnScreen(gameText, "FireRate : " + std::to_string(ship02Stats->getFireRate()), Color(0, 1, 0), 50, 2, 12.5);
	RenderTextOnScreen(gameText, "Parts : " + std::to_string(ship02Stats->getParts()), Color(0, 1, 0), 50, 2, 10);
	RenderUI(healthBar, ship02Stats->getHealth() * 5, 3.5, 5, 20);

	glDisable(GL_SCISSOR_TEST);
	glDisableVertexAttribArray(2);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(0);
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

void GameScene::SpawnPowerUp()
{
	srand(time(NULL));

	GameObject* powerUp;
	int powerUpSpawn;
	float powerUpX, powerUpZ;

	Transform t;

	Material *mat = new Material;
	mat->kAmbient.Set(0.4f, 0.4f, 0.4f);
	mat->kDiffuse.Set(0.1f, 0.1f, 0.1f);
	mat->kSpecular.Set(0.2f, 0.2f, 0.2f);
	mat->kShininess = 1.0f;

	for (int numberOfPowerUps = 0; numberOfPowerUps <= 20; numberOfPowerUps++)
	{
		powerUpX = rand() % 500;
		powerUpZ = rand() % 500 - 500;
		powerUpSpawn = rand() % 2;
		
		if (powerUpSpawn == 0)
		{
			powerUp = GameObjectFactory::SpawnGameObject(GameObjectFactory::SPDCONSUMABLE, "speedup", mat, t);
			powerUp->GetTransform().GenerateBounds();
			powerUp->SetPosition(Vector3(powerUpX, -3.0, powerUpZ));
			powerUp->SetScale(Vector3(4, 4, 4));
			powerUp->GetTransform().SetBounds(Vector3(3.f, 4.f,3.f));
			meshList.push_back(powerUp);
		}
		if (powerUpSpawn == 1)
		{
			powerUp = GameObjectFactory::SpawnGameObject(GameObjectFactory::HPCONSUMABLE, "restorehp", mat, t);
			powerUp->GetTransform().GenerateBounds();
			powerUp->SetPosition(Vector3(powerUpX, -3.0, powerUpZ));
			powerUp->SetScale(Vector3(4, 4, 4));
			powerUp->GetTransform().SetBounds(Vector3(3.f, 4.f, 3.f));
			meshList.push_back(powerUp);
		}
	}
}

void GameScene::renderSkybox()
{
	GameObject* box;

	// no top and bottom
#pragma region skybox
	Mesh* Left = MeshBuilder::GenerateOBJ("left", "OBJ//side.obj");
	Left->material.kAmbient.Set(0.2f, 0.2f, 0.2f);
	Left->material.kDiffuse.Set(0.1f, 0.1f, 0.1f);
	Left->material.kSpecular.Set(0.1f, 0.1f, 0.1f);
	Left->material.kShininess = 1.0f;

	Mesh* Right = MeshBuilder::GenerateOBJ("right", "OBJ//side.obj");
	Right->material.kAmbient.Set(0.2f, 0.2f, 0.2f);
	Right->material.kDiffuse.Set(0.1f, 0.1f, 0.1f);
	Right->material.kSpecular.Set(0.1f, 0.1f, 0.1f);
	Right->material.kShininess = 1.0f;

	Mesh* Back = MeshBuilder::GenerateOBJ("back", "OBJ//side.obj");
	Back->material.kAmbient.Set(0.2f, 0.2f, 0.2f);
	Back->material.kDiffuse.Set(0.1f, 0.1f, 0.1f);
	Back->material.kSpecular.Set(0.1f, 0.1f, 0.1f);
	Back->material.kShininess = 1.0f;

	Mesh* Front = MeshBuilder::GenerateOBJ("front", "OBJ//side.obj");
	Front->material.kAmbient.Set(0.2f, 0.2f, 0.2f);
	Front->material.kDiffuse.Set(0.1f, 0.1f, 0.1f);
	Front->material.kSpecular.Set(0.1f, 0.1f, 0.1f);
	Front->material.kShininess = 1.0f;

	Left->textureID = LoadTGA("Image//left.tga");
	box = new GameObject(Left, Vector3(0, 0, -1050), 90, Vector3(1, 0, 0), Vector3(2100, 2100, 2100));
	meshList.push_back(box);
	Right->textureID = LoadTGA("Image//right.tga");
	box = new GameObject(Right, Vector3(0, 0, 1050), -90, Vector3(1, 0, 0), Vector3(2100, 2100, 2100));
	meshList.push_back(box);
	Back->textureID = LoadTGA("Image//back.tga");
	box = new GameObject(Back, Vector3(-1050, 0, 0), -90, Vector3(0, 0, 1), Vector3(2100, 2100, 2100));
	meshList.push_back(box);
	Front->textureID = LoadTGA("Image//front.tga");
	box = new GameObject(Front, Vector3(1050, 0, 0), 90, Vector3(0, 0, 1), Vector3(2100, 2100, 2100));
	meshList.push_back(box);
#pragma endregion
}

void GameScene::Exit()
{
	delete curCube;
	glDeleteVertexArrays(1, &m_vertexArrayID);
	glDeleteProgram(m_programID);
}
