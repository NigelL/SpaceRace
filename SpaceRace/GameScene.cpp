#include "GameScene.h"
#include "ReadMesh.h"

#define SIZE 5

Water* curWater = new Water();

CShipStats* ship01Stats;
CShipStats* ship02Stats;
std::vector<Vector3> partsmarkervector;
Physic physic;

Mesh* curCube;
GameObject* damage[5];
GameObject* damage2[5];

int islandx, islandz, islandposx, islandposz, islandposxA[38], islandposzA[38];

GameScene* GameScene::instance;

GameScene::GameScene()
{
	srand(time(0));
	instance = this;

	curCube = MeshBuilder::GenerateCube("Cube", Color(1, 1, 1), 1, 1, 1);

	physic.VoA(20, 0.18);
	physic.AdvRatio(200, 10);
	physic.thrust(0.12, 10, 10);
	//Manual Instantiation

	Mesh* waterMesh = MeshBuilder::GenerateQuad("Water", Color(0, 1, 0), 100, 100);
	waterMesh->textureID = LoadTGA("Image//Water.tga");
	Material *waterMat = new Material;
	waterMat->kAmbient.Set(0.4f, 0.4f, 0.4f);
	waterMat->kDiffuse.Set(0.1f, 0.1f, 0.1f);
	waterMat->kSpecular.Set(0.2f, 0.2f, 0.2f);
	waterMat->kShininess = 1.0f;
	waterMesh->material = *waterMat;
	sceneObjects["Water"] = new GameObject(waterMesh, Vector3(-1000, -5.0f, 1000), 180, Vector3(1, 0, 0), Vector3(2000, 1, 2000));

	curWater->waterMesh = waterMesh;
	meshList.push_back(sceneObjects["Water"]);

	///////////////////////////////////////////////

	Material *mat1 = new Material;
	mat1->kAmbient.Set(0.4f, 0.4f, 0.4f);
	mat1->kDiffuse.Set(0.1f, 0.1f, 0.1f);
	mat1->kSpecular.Set(0.2f, 0.2f, 0.2f);
	mat1->kShininess = 1.0f;

	Transform t1;
	Transform t2;
	Transform t3;
	Transform t4;

	//t1.position = Vector3(0, 0, -985);
	t1.position = Vector3(0, 0, 0);
	t1.rotation = Vector3(0, 1, 0);
	t1.amt = 0;
	t1.scale = Vector3(2000, 10, 10);
	t1.SetBounds(Vector3(2500, 1000, 100));
	meshList.push_back(GameObjectFactory::SpawnGameObject(GameObjectFactory::BOUNDARY, "BB1", mat1, t1));
	t1.position = Vector3(985, 0, 30);
	t1.rotation = Vector3(0, 1, 0);
	t1.amt = 0;
	t1.scale = Vector3(10, 10, 1990);
	t1.SetBounds(Vector3(10, 1000, 2500));
	GameObjectFactory::SpawnGameObject(GameObjectFactory::BOUNDARY, "BB2", mat1, t2);
	t1.position = Vector3(0, 0, 995);
	t1.rotation = Vector3(0, 1, 0);
	t1.amt = 0;
	t1.scale = Vector3(2000, 10, 10);
	t1.SetBounds(Vector3(2500, 1000, 10));
	GameObjectFactory::SpawnGameObject(GameObjectFactory::BOUNDARY, "BB3", mat1, t3);
	//t1.position = Vector3(0, 0, -985);
	//t1.rotation = Vector3(0, 1, 0);
	//t1.amt = 0;
	//t1.scale = Vector3(2000, 10, 10);
	//GameObjectFactory::SpawnGameObject(GameObjectFactory::BOUNDARY, "BB4", mat1, t4);


	//Mesh* boundBox4 = MeshBuilder::GenerateBound("BB4", Color(1, 1, 1), 10, 10, 10);
	//boundBox4->material = *mat1;
	//GameObject* BoundBox4 = new GameObject(boundBox1, Vector3(0, 0, -985), 0, Vector3(0, 1, 0), Vector3(2000, 10, 10));
	//meshList.push_back(BoundBox4);

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
	t.boxHalf = Vector3(1.5f, 1.5f, 1.5f);

	for (int numberofislands = 0; numberofislands < 38; numberofislands++)
	{
		bool overlaps = false;
		islandposx = rand() % 1700 + (-900);
		islandposz = rand() % 1800 + (-800);
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
				GameObject* islands = GameObjectFactory::SpawnGameObject(GameObjectFactory::ISLAND, "Island", mat, t);
				islands->SetPosition(Vector3(islandposx, 0, islandposz));
				islands->SetScale(Vector3(1, 1,1));
				islands->GetTransform().SetBounds(Vector3(10.0f, 8.0f, 10.0f));
				meshList.push_back(islands);

			}
			else
			{
				GameObject* parts = GameObjectFactory::SpawnGameObject(GameObjectFactory::PARTSCONSUMABLE, "Parts", mat, t);
				parts->SetPosition(Vector3(islandposx, 0, islandposz));
				parts->SetScale(Vector3(1, 1, 1));
				parts->GetTransform().SetBounds(Vector3(1.4f, 0.8f, 2.2f));
				meshList.push_back(parts);
				partsmarkervector.push_back(Vector3(islandposx, 0, islandposz));

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
	MapWater = MeshBuilder::GenerateMenu("mapwater", Color(1, 0, 0), 40);
	MapWater->textureID = LoadTGA("Image//NavyBlue.tga");
	ShipMarker1 = MeshBuilder::GenerateMenu("shipmarker1", Color(1, 0, 0), 40);
	ShipMarker1->textureID = LoadTGA("Image//White.tga");
	ShipMarker2 = MeshBuilder::GenerateMenu("shipmarker2", Color(1, 0, 0), 40);
	ShipMarker2->textureID = LoadTGA("Image//Red.tga");
	IslandMarker = MeshBuilder::GenerateMenu("islandmarker", Color(1, 0, 0), 40);
	IslandMarker->textureID = LoadTGA("Image//Green.tga");
	PartsMarker = MeshBuilder::GenerateMenu("partsmarker", Color(1, 0, 0), 40);
	PartsMarker->textureID = LoadTGA("Image//Yellow.tga");
	HealthMarker = MeshBuilder::GenerateMenu("healthmarker", Color(1, 0, 0), 40);
	HealthMarker->textureID = LoadTGA("Image//LightGreen.tga");
	SpeedUpMarker = MeshBuilder::GenerateMenu("speedupmarker", Color(1, 0, 0), 40);
	SpeedUpMarker->textureID = LoadTGA("Image//Purple.tga");

	Win = MeshBuilder::GenerateMenu("win", Color(1, 0, 0), 40);
	Win->textureID = LoadTGA("Image//win.tga");
	Lose = MeshBuilder::GenerateMenu("lose", Color(1, 0, 0), 40);
	Lose->textureID = LoadTGA("Image//lose.tga");
	Select = MeshBuilder::GenerateMenu("select", Color(1, 0, 0), 10);
	Select->textureID = LoadTGA("Image//select.tga");

	speedometer = MeshBuilder::GenerateMenu("speedometer", Color(1, 0, 0), 10);
	speedometer->textureID = LoadTGA("Image//speedometer.tga");
	needle = MeshBuilder::GenerateMenu("needle", Color(1, 0, 0), 10);
	needle->textureID = LoadTGA("Image//needle.tga");
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
		characterSpacing.SetToTranslation(i * 0.75f, 0, 0);
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

void GameScene::RenderUI(Mesh* mesh, float sizeX, float sizeY, float x, float y, float amt, int Rx, int Ry, int Rz)
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
	modelStack.Rotate(amt, Rx, Ry, Rz);
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

double GameScene::getTimer()
{
	return timer;
}

void GameScene::setTimer(double _timer)
{
	timer = _timer;
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

inline void GameScene::SetGameText(Mesh* curtext)
{
	gameText = curtext;
}

float Lerp(float position, float target, float amt)
{
	float lerp = (target - position) * amt;
	return position + lerp;
}

GameObject* ship01_2;
GameObject* ship01_3;
GameObject* ship01_4;
GameObject* spaceship1;
cannonball* CannonBall[8];
cannonball* CannonBall2[8];

void GameScene::Init()
{
	//Camera Init
	camera.Init(Vector3(10, 20, 0), Vector3(0, 0, 0), Vector3(0, 1, 0));

	GameSound::instance()->engine->stopAllSounds();
	GameSound::instance()->GameBGM->setDefaultVolume(0.1f);
	GameSound::instance()->engine->play2D(GameSound::instance()->GameBGM, true);

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
	light[0].power = 0.5f;
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

	m_programID = LoadShaders("Shader//Texture.vertexshader", "Shader//Text.fragmentshader");
	m_parameters[U_TEXT_ENABLED] = glGetUniformLocation(m_programID, "textEnabled");
	m_parameters[U_TEXT_COLOR] = glGetUniformLocation(m_programID, "textColor");

	glfwSetCursorPos(Application::getGLFWWindow(), 1000, 700);

	SpawnPowerUp();
	renderSkybox();

	ship01Stats = dynamic_cast<CShipStats*>(sceneObjects["ship01"]);
	ship01Stats->setStats(0, 500, 15, 0, 15, 1000, 15);
	
	Material *mat = new Material;
	mat->kAmbient.Set(0.3f, 0.3f, 0.3f);
	mat->kDiffuse.Set(0.1f, 0.1f, 0.1f);
	mat->kSpecular.Set(0.2f, 0.2f, 0.2f);
	mat->kShininess = 1.0f;

	Transform t;

	ship01_2 = GameObjectFactory::SpawnGameObject(GameObjectFactory::SHIP, "ship01-2", mat, t);
	ship01_2->SetScale(Vector3(0.7, 0.7, 0.7));
	ship01_3 = GameObjectFactory::SpawnGameObject(GameObjectFactory::SHIP, "ship01-3", mat, t);
	ship01_4 = GameObjectFactory::SpawnGameObject(GameObjectFactory::SHIP, "ship01-4", mat, t);
	spaceship1 = GameObjectFactory::SpawnGameObject(GameObjectFactory::SHIP, "spaceship1", mat, t);

	translateY = sceneObjects["ship01"]->GetPosition().y;
	rotateAmt = 0;
	shipUpgrade = false;
	ship01Type = 0;
	winCondition1 = false;
	winCondition2 = false;

	for (int i = 0; i < 8; i++)
	{
		CannonBall[i] = new cannonball(sceneObjects["cannon01"]->curObject, Vector3(0, 0, 0), 0, Vector3(0, 1, 0), Vector3(1.5f, 1.5f, 1.5f));
		meshList.push_back(CannonBall[i]);
		CannonBall2[i] = new cannonball(sceneObjects["cannon01"]->curObject, Vector3(0, 0, 0), 0, Vector3(0, 1, 0), Vector3(1.5f, 1.5f, 1.5f));
		meshList.push_back(CannonBall2[i]);
	}
} 

static double LSPEED = 10.0;
static short int lightType = 0;
//static bool openShop = false;
//int shopItem;

Vector3 ray_wor;
Vector3 ray_Clip;
Vector3 curHitPoint;

static double bounceTime = 0.0;
static double bounceTime2 = 0.0;
static double cannonHit_01 = 0.0f;
static double cannonHit_02 = 0.0f;

bool floating_01 = false;
bool floating_02 = false;
bool cannonForce_01 = false;

int cannonCount = 0;
int cannonCount2 = 0;

void GameScene::Update(double dt)
{
	setTimer(dt);
	if (Application::IsKeyPressed('B'))
	{
		if (ship01Type == 0 && ship01Stats->getParts() >= 5)
		{
			ship01Stats->SetSpeed(0);

			shipUpgrade = true;
		}
		if (ship01Type == 1 && ship01Stats->getParts() >= 7)
		{
			ship01Stats->SetSpeed(0);

			shipUpgrade = true;
		}
		if (ship01Type == 2 && ship01Stats->getParts() >= 10)
		{
			ship01Stats->SetSpeed(0);

			shipUpgrade = true;
		}
		if (ship01Type == 3 && ship01Stats->getParts() >= 12)
		{
			ship01Stats->SetSpeed(0);

			shipUpgrade = true;
		}
	}
	if (shipUpgrade)
	{
		Transform t;
		t.SetBounds(Vector3(1.3f, 1.3f, 1.3f));
		t.position.x = sceneObjects["ship01"]->GetPosition().x;
		t.position.z = sceneObjects["ship01"]->GetPosition().z;

		if (ship01Type == 0)
		{
			for (int j = 0; j < meshList.size(); j++)
			{
				if (meshList[j] == sceneObjects["ship01"])
				{
					camera.SetTarget(sceneObjects["ship01"]->GetPosition().x, sceneObjects["ship01"]->GetPosition().y, sceneObjects["ship01"]->GetPosition().z);
					camera.SetPosition(sceneObjects["ship01"]->GetPosition().x + 50, sceneObjects["ship01"]->GetPosition().y + 15, sceneObjects["ship01"]->GetPosition().z + 50);

					sceneObjects["ship01"]->TranslateShip(Vector3(0, 30 * dt, 0));
					sceneObjects["ship01"]->SetRotation(Vector3(0, 1, 0), sceneObjects["ship01"]->GetAmt() + 500 * dt);

					if (sceneObjects["ship01"]->GetAmt() >= 1000 && sceneObjects["ship01"]->GetPosition().y >= 20)
					{
						sceneObjects["ship01"] = ship01_2;
						sceneObjects["ship01"]->SetPosition(t.position);
						sceneObjects["ship01"]->GetTransform().SetBounds(Vector3(5.0f, 10.0f, 10.0f));
						meshList[j] = ship01_2;
						ship01Stats->SetParts(ship01Stats->getParts() - 5);
						shipUpgrade = false;
						ship01Type = 1;
					}
				}
			}
		}
		else if (ship01Type == 1)
		{
			for (int j = 0; j < meshList.size(); j++)
			{
				if (meshList[j] == sceneObjects["ship01"])
				{
					camera.SetTarget(sceneObjects["ship01"]->GetPosition().x, sceneObjects["ship01"]->GetPosition().y, sceneObjects["ship01"]->GetPosition().z);
					camera.SetPosition(sceneObjects["ship01"]->GetPosition().x + 50, sceneObjects["ship01"]->GetPosition().y + 15, sceneObjects["ship01"]->GetPosition().z + 50);
					sceneObjects["ship01"]->TranslateShip(Vector3(0, 30 * dt, 0));

					sceneObjects["ship01"]->SetRotation(Vector3(0, 1, 0), sceneObjects["ship01"]->GetAmt() + 500 * dt);

					if (sceneObjects["ship01"]->GetAmt() >= 1000 && sceneObjects["ship01"]->GetPosition().y >= 20)
					{
						sceneObjects["ship01"] = ship01_3;
						sceneObjects["ship01"]->SetPosition(t.position);
						sceneObjects["ship01"]->GetTransform().SetBounds(Vector3(5.0f, 10.0f, 15.0f));
						meshList[j] = ship01_3;
						ship01Stats->SetParts(ship01Stats->getParts() - 7);
						shipUpgrade = false;
						ship01Type = 2;
					}
				}
			}
		}
		else if (ship01Type == 2)
		{
			for (int j = 0; j < meshList.size(); j++)
			{
				if (meshList[j] == sceneObjects["ship01"])
				{
					camera.SetTarget(sceneObjects["ship01"]->GetPosition().x, sceneObjects["ship01"]->GetPosition().y, sceneObjects["ship01"]->GetPosition().z);
					camera.SetPosition(sceneObjects["ship01"]->GetPosition().x + 50, sceneObjects["ship01"]->GetPosition().y + 15, sceneObjects["ship01"]->GetPosition().z + 50);

					sceneObjects["ship01"]->TranslateShip(Vector3(0, 30 * dt, 0));
					sceneObjects["ship01"]->SetRotation(Vector3(0, 1, 0), sceneObjects["ship01"]->GetAmt() + 500 * dt);

					if (sceneObjects["ship01"]->GetAmt() >= 1000 && sceneObjects["ship01"]->GetPosition().y >= 20)
					{
						sceneObjects["ship01"] = ship01_4;
						sceneObjects["ship01"]->GetTransform().SetBounds(Vector3(5.0f, 10.0f, 18.0f));
						sceneObjects["ship01"]->SetPosition(t.position);
						meshList[j] = ship01_4;
						ship01Stats->SetParts(ship01Stats->getParts() - 10);
						shipUpgrade = false;
						ship01Type = 3;
					}
				}
			}
		}
		else if (ship01Type == 3)
		{
			for (int j = 0; j < meshList.size(); j++)
			{
				if (meshList[j] == sceneObjects["ship01"])
				{
					camera.SetTarget(sceneObjects["ship01"]->GetPosition().x, sceneObjects["ship01"]->GetPosition().y, sceneObjects["ship01"]->GetPosition().z);
					camera.SetPosition(sceneObjects["ship01"]->GetPosition().x + 50, sceneObjects["ship01"]->GetPosition().y + 15, sceneObjects["ship01"]->GetPosition().z + 50);

					sceneObjects["ship01"]->TranslateShip(Vector3(0, 30 * dt, 0));
					sceneObjects["ship01"]->SetRotation(Vector3(0, 1, 0), sceneObjects["ship01"]->GetAmt() + 500 * dt);

					if (sceneObjects["ship01"]->GetAmt() >= 1000 && sceneObjects["ship01"]->GetPosition().y >= 20)
					{
						sceneObjects["ship01"] = spaceship1;
						sceneObjects["ship01"]->SetPosition(t.position);
						meshList[j] = spaceship1;
						ship01Stats->SetParts(ship01Stats->getParts() - 15);
						shipUpgrade = false;
						ship01Type = 4;
					}
				}
			}
		}
	}
	else {

		if (!shipUpgrade && ship01Type != 4)
		{
			if (sceneObjects["ship01"]->GetPosition().y >= -2)
			{
				sceneObjects["ship01"]->TranslateShip(Vector3(0, -30 * dt, 0));

				sceneObjects["ship01"]->SetRotation(Vector3(0, 1, 0), sceneObjects["ship01"]->GetAmt() + 500 * dt);
			}
		}
		/*
		if (SpeedConsumable::spdup)
		{
			timer += dt;

			if (timer >= 5)
			{
				//ship01Stats->SetSpeed(ship01Stats->getSpeed() - 100);
				timer = 0;
				SpeedConsumable::spdup = false;
			}
		}
		*/
		if (ship01Type == 4)
		{
			camera.SetTarget(sceneObjects["ship01"]->GetPosition().x, sceneObjects["ship01"]->GetPosition().y, sceneObjects["ship01"]->GetPosition().z);
			camera.SetPosition(sceneObjects["ship01"]->GetPosition().x + 50, sceneObjects["ship01"]->GetPosition().y - 15, sceneObjects["ship01"]->GetPosition().z + 50);

			if (sceneObjects["ship01"]->GetPosition().y > 1000)
			{
				winCondition1 = true;
			}
			else if (!winCondition2)
			{
				sceneObjects["ship01"]->TranslateShip(Vector3(0, 300 * dt, 0));
				sceneObjects["ship01"]->SetRotation(Vector3(0, 1, 0), sceneObjects["ship01"]->GetAmt() + 500 * dt);
			}
		}

		if (ship02Stats->getHealth() <= 0)
		{
			winCondition1 = true;
		}

		curWater->UpdateWater(10, dt / 2); 

		std::vector<Consumable*> toBeErased;
		for (int j = 0; j < meshList.size(); j++)
		{
			if (meshList[j] == sceneObjects["ship01"]) { continue; }

			if (sceneObjects["ship01"]->collision.CheckCollision(meshList[j]->GetTransform())) {
				Consumable* cPtr = dynamic_cast<Consumable*>(meshList[j]);
				IslandEnvironment* isPtr = dynamic_cast<IslandEnvironment*>(meshList[j]);
				cannonball* cannonPtr = dynamic_cast<cannonball*>(meshList[j]);

				if (isPtr != nullptr) {

					/*
					float dirDot = ship01Stats->objectDir.Dot(ship01Stats->GetPosition() - meshList[j]->GetPosition());
					if (dirDot >= 0) {
						if (!isPtr->collidedOnce) {
							isPtr->OnCollide(*ship01Stats); //Ship Collision with island
							isPtr->collidedOnce = true;
						}
						physic.bounceBack(*ship01Stats, dt, 100);
						sceneObjects["ship01"]->translateObj(-ship01Stats->getSpeed() * 6.0f, dt);
					}
					*/

					if (!isPtr->collidedOnce) {
						isPtr->OnCollide(*ship01Stats); //Ship Collision with island
						isPtr->collidedOnce = true;
					}

				}

				if (cPtr != nullptr) {
					toBeErased.push_back(cPtr);
					auto i = std::find(partsmarkervector.begin(), partsmarkervector.end(), cPtr->GetPosition());
					if (i != partsmarkervector.end()) {
						int dist = std::distance(partsmarkervector.begin(), i);
						partsmarkervector.erase(partsmarkervector.begin() + dist);
					}
				}

				if (cannonPtr != nullptr && cannonPtr->playerType == 1) { // other ship cannon hit ship01
					cannonPtr->OnCollide(*ship01Stats);
					if (meshList[j] != nullptr) {
						//	meshList.erase(meshList.begin() + j);
					}
				}
			}
			else {
				IslandEnvironment* isPtr = dynamic_cast<IslandEnvironment*>(meshList[j]);
				if (isPtr != nullptr && !sceneObjects["ship02"]->collision.CheckCollision(meshList[j]->GetTransform())) {
					isPtr->collidedOnce = false;
				}
			}

			/*cannonball* checkCannon = dynamic_cast<cannonball*>(meshList[j]);
			if (checkCannon != nullptr) {
				checkCannon->Update();
			}*/
		}
		for (auto eraseThis : toBeErased) {
			eraseThis->OnCollide(*ship01Stats);
		}

		short int multipler = 1;

		//Game Logic
		sceneFPS = 1.0f / (float)dt;
		double mouseX, mouseY;
		Application::GetMousePos(mouseX, mouseY);

		if (sceneObjects["ship01"]->GetPosition().y < -6)
		{
			// renderSplash(*sceneObjects["ship01"]);

			if (sceneObjects["ship01"]->GetPosition().y < -6.25)
			{
				floating_01 = true;
			}
		}

		if (sceneObjects["ship01"]->GetPosition().y > -5.75)
		{
			floating_01 = false;
		}

		physic.floating(*sceneObjects["ship01"], 7.5, dt, floating_01);

		if (ship01Stats->getHealth() < 10 && ship01Type != 4)
		{
			renderDamage(*sceneObjects["ship01"], damage);
		}

		light[1].position.x = sceneObjects["ship01"]->GetPosition().x + (sin(DegreeToRadian(sceneObjects["ship01"]->GetAmt())) * 1.5);
		light[1].position.y = sceneObjects["ship01"]->GetPosition().y + 0.25;
		light[1].position.z = sceneObjects["ship01"]->GetPosition().z + (cos(DegreeToRadian(sceneObjects["ship01"]->GetAmt())) * 1.5);

		// control the sceneObjects["ship01"]
		if (Application::IsKeyPressed('W')) // 270
		{

			if (ship01Stats->getSpeed() < ship01Stats->getMaxSpeed()) // speed limit
			{
				ship01Stats->SetSpeed(physic.acceleration(ship01Stats->getSpeed(), dt));
			}
			else {
				ship01Stats->SetSpeed(physic.acceleration(ship01Stats->getMaxSpeed(), dt));
			}

			sceneObjects["ship01"]->translateObj(ship01Stats->getSpeed(), dt);
		}
		else if (ship01Stats->getSpeed() > 0)
		{
			ship01Stats->SetSpeed(physic.deceleration(ship01Stats->getSpeed(), dt));
			sceneObjects["ship01"]->translateObj(ship01Stats->getSpeed(), dt);
		}

		if (Application::IsKeyPressed('A') && ship01Stats->getSpeed() != 0) // 0
		{
			sceneObjects["ship01"]->rotateObj(ship01Stats->getSpeed() * 0.025);
		}
		if (Application::IsKeyPressed('D') && ship01Stats->getSpeed() != 0) // 180
		{
			sceneObjects["ship01"]->rotateObj(-(ship01Stats->getSpeed() * 0.025));
		}

		if (bounceTime <= 0)
		{
			if (Application::IsKeyPressed('S')) // shoot cannonball
			{
				float direction = sceneObjects["ship01"]->GetAmt() * 3.142f / 180.0f;

				if (ship01Type == 0)
				{
					//cannonball* CannonBall1 = new cannonball(sceneObjects["cannon01"]->curObject, sceneObjects["ship01"]->GetPosition(), 0, Vector3(0, 1, 0), Vector3(1.5f, 1.5f, 1.5f));
					CannonBall[cannonCount]->GetTransform().SetBounds(Vector3(5.0f, 5.0f, 5.0f));
					CannonBall[cannonCount]->dir = Vector3(sin(direction), 0, cos(direction));
					CannonBall[cannonCount]->speed = ship01Stats->getFireRate();
					CannonBall[cannonCount]->playerType = 0;
					CannonBall[cannonCount]->SetPosition(sceneObjects["ship01"]->GetPosition());
					//meshList.push_back(CannonBall[cannonCount]);
					cannonCount++;
					if (cannonCount > 7)
					{
						cannonCount = 0;
					}
				}
				if (ship01Type == 1)
				{
					// cannonball* CannonBall1 = new cannonball(sceneObjects["cannon01"]->curObject, sceneObjects["ship01"]->GetPosition(), 0, Vector3(0, 1, 0), Vector3(1.5f, 1.5f, 1.5f));
					CannonBall[cannonCount]->GetTransform().SetBounds(Vector3(5.0f, 5.0f, 5.0f));
					CannonBall[cannonCount]->dir = Vector3(sin(direction + (30 * 3.142f / 180.0f)), 0, cos(direction + (30 * 3.142f / 180.0f)));
					CannonBall[cannonCount]->speed = ship01Stats->getFireRate();
					CannonBall[cannonCount]->playerType = 0;
					CannonBall[cannonCount]->SetPosition(sceneObjects["ship01"]->GetPosition());
					//meshList.push_back(CannonBall[cannonCount]);
					cannonCount++;
					if (cannonCount > 7)
					{
						cannonCount = 0;
					}
					// cannonball* CannonBall2 = new cannonball(sceneObjects["cannon01"]->curObject, sceneObjects["ship01"]->GetPosition(), 0, Vector3(0, 1, 0), Vector3(1.5f, 1.5f, 1.5f));
					CannonBall[cannonCount]->GetTransform().SetBounds(Vector3(5.0f, 5.0f, 5.0f));
					CannonBall[cannonCount]->dir = Vector3(sin(direction - (30 * 3.142f / 180.0f)), 0, cos(direction - (30 * 3.142f / 180.0f)));
					CannonBall[cannonCount]->speed = ship01Stats->getFireRate();
					CannonBall[cannonCount]->playerType = 0;
					CannonBall[cannonCount]->SetPosition(sceneObjects["ship01"]->GetPosition());
					//meshList.push_back(CannonBall[cannonCount]);
					cannonCount++;
					if (cannonCount > 7)
					{
						cannonCount = 0;
					}
				}
				if (ship01Type == 2)
				{
					//cannonball* CannonBall1 = new cannonball(sceneObjects["cannon01"]->curObject, sceneObjects["ship01"]->GetPosition(), 0, Vector3(0, 1, 0), Vector3(1.5f, 1.5f, 1.5f));
					CannonBall[cannonCount]->GetTransform().SetBounds(Vector3(5.0f, 5.0f, 5.0f));
					CannonBall[cannonCount]->dir = Vector3(sin(direction + (30 * 3.142f / 180.0f)), 0, cos(direction + (30 * 3.142f / 180.0f)));
					CannonBall[cannonCount]->speed = ship01Stats->getFireRate();
					CannonBall[cannonCount]->playerType = 0;
					CannonBall[cannonCount]->SetPosition(sceneObjects["ship01"]->GetPosition());
					//meshList.push_back(CannonBall[cannonCount]);
					cannonCount++;
					if (cannonCount > 7)
					{
						cannonCount = 0;
					}
					//cannonball* CannonBall2 = new cannonball(sceneObjects["cannon01"]->curObject, sceneObjects["ship01"]->GetPosition(), 0, Vector3(0, 1, 0), Vector3(1.5f, 1.5f, 1.5f));
					CannonBall[cannonCount]->GetTransform().SetBounds(Vector3(5.0f, 5.0f, 5.0f));
					CannonBall[cannonCount]->dir = Vector3(sin(direction - (30 * 3.142f / 180.0f)), 0, cos(direction - (30 * 3.142f / 180.0f)));
					CannonBall[cannonCount]->speed = ship01Stats->getFireRate();
					CannonBall[cannonCount]->playerType = 0;
					CannonBall[cannonCount]->SetPosition(sceneObjects["ship01"]->GetPosition());
					//meshList.push_back(CannonBall[cannonCount]);
					cannonCount++;
					if (cannonCount > 7)
					{
						cannonCount = 0;
					}
					//cannonball* CannonBall3 = new cannonball(sceneObjects["cannon01"]->curObject, Vector3(sceneObjects["ship01"]->GetPosition().x - 8, sceneObjects["ship01"]->GetPosition().y, sceneObjects["ship01"]->GetPosition().z - 8), 0, Vector3(0, 1, 0), Vector3(1.5f, 1.5f, 1.5f));
					CannonBall[cannonCount]->GetTransform().SetBounds(Vector3(5.0f, 5.0f, 5.0f));
					CannonBall[cannonCount]->dir = Vector3(sin(direction + (30 * 3.142f / 180.0f)), 0, cos(direction + (30 * 3.142f / 180.0f)));
					CannonBall[cannonCount]->speed = ship01Stats->getFireRate();
					CannonBall[cannonCount]->playerType = 0;
					CannonBall[cannonCount]->SetPosition(sceneObjects["ship01"]->GetPosition());
					//meshList.push_back(CannonBall[cannonCount]);
					cannonCount++;
					if (cannonCount > 7)
					{
						cannonCount = 0;
					}
					//cannonball* CannonBall4 = new cannonball(sceneObjects["cannon01"]->curObject, Vector3(sceneObjects["ship01"]->GetPosition().x - 8, sceneObjects["ship01"]->GetPosition().y, sceneObjects["ship01"]->GetPosition().z - 8), 0, Vector3(0, 1, 0), Vector3(1.5f, 1.5f, 1.5f));
					CannonBall[cannonCount]->GetTransform().SetBounds(Vector3(5.0f, 5.0f, 5.0f));
					CannonBall[cannonCount]->dir = Vector3(sin(direction - (30 * 3.142f / 180.0f)), 0, cos(direction - (30 * 3.142f / 180.0f)));
					CannonBall[cannonCount]->speed = ship01Stats->getFireRate();
					CannonBall[cannonCount]->playerType = 0;
					CannonBall[cannonCount]->SetPosition(sceneObjects["ship01"]->GetPosition());
					//meshList.push_back(CannonBall[cannonCount]);
					cannonCount++;
					if (cannonCount > 7)
					{
						cannonCount = 0;
					}
				}
				if (ship01Type == 3)
				{
					//cannonball* CannonBall1 = new cannonball(sceneObjects["cannon01"]->curObject, Vector3(sceneObjects["ship01"]->GetPosition().x, sceneObjects["ship01"]->GetPosition().y, sceneObjects["ship01"]->GetPosition().z - 8), 0, Vector3(0, 1, 0), Vector3(1.5f, 1.5f, 1.5f));
					CannonBall[cannonCount]->GetTransform().SetBounds(Vector3(5.0f, 5.0f, 5.0f));
					CannonBall[cannonCount]->dir = Vector3(sin(direction), 0, cos(direction));
					CannonBall[cannonCount]->speed = ship01Stats->getFireRate();
					CannonBall[cannonCount]->playerType = 0;
					CannonBall[cannonCount]->SetPosition(sceneObjects["ship01"]->GetPosition());
					//meshList.push_back(CannonBall[cannonCount]);
					cannonCount++;
					if (cannonCount > 7)
					{
						cannonCount = 0;
					}
					//cannonball* CannonBall2 = new cannonball(sceneObjects["cannon01"]->curObject, Vector3(sceneObjects["ship01"]->GetPosition().x, sceneObjects["ship01"]->GetPosition().y, sceneObjects["ship01"]->GetPosition().z + 8), 0, Vector3(0, 1, 0), Vector3(1.5f, 1.5f, 1.5f));
					CannonBall[cannonCount]->GetTransform().SetBounds(Vector3(5.0f, 5.0f, 5.0f));
					CannonBall[cannonCount]->dir = Vector3(sin(direction), 0, cos(direction));
					CannonBall[cannonCount]->speed = ship01Stats->getFireRate();
					CannonBall[cannonCount]->playerType = 0;
					CannonBall[cannonCount]->SetPosition(sceneObjects["ship01"]->GetPosition());
					//meshList.push_back(CannonBall[cannonCount]);
					cannonCount++;
					if (cannonCount > 7)
					{
						cannonCount = 0;
					}
					//cannonball* CannonBall3 = new cannonball(sceneObjects["cannon01"]->curObject, Vector3(sceneObjects["ship01"]->GetPosition().x - 8, sceneObjects["ship01"]->GetPosition().y, sceneObjects["ship01"]->GetPosition().z - 8), 0, Vector3(0, 1, 0), Vector3(1.5f, 1.5f, 1.5f));
					CannonBall[cannonCount]->GetTransform().SetBounds(Vector3(5.0f, 5.0f, 5.0f));
					CannonBall[cannonCount]->dir = Vector3(sin(direction + (30 * 3.142f / 180.0f)), 0, cos(direction + (30 * 3.142f / 180.0f)));
					CannonBall[cannonCount]->speed = ship01Stats->getFireRate();
					CannonBall[cannonCount]->playerType = 0;
					CannonBall[cannonCount]->SetPosition(sceneObjects["ship01"]->GetPosition());
					//meshList.push_back(CannonBall[cannonCount]);
					cannonCount++;
					if (cannonCount > 7)
					{
						cannonCount = 0;
					}
					//cannonball* CannonBall4 = new cannonball(sceneObjects["cannon01"]->curObject, Vector3(sceneObjects["ship01"]->GetPosition().x - 8, sceneObjects["ship01"]->GetPosition().y, sceneObjects["ship01"]->GetPosition().z - 8), 0, Vector3(0, 1, 0), Vector3(1.5f, 1.5f, 1.5f));
					CannonBall[cannonCount]->GetTransform().SetBounds(Vector3(5.0f, 5.0f, 5.0f));
					CannonBall[cannonCount]->dir = Vector3(sin(direction - (30 * 3.142f / 180.0f)), 0, cos(direction - (30 * 3.142f / 180.0f)));
					CannonBall[cannonCount]->speed = ship01Stats->getFireRate();
					CannonBall[cannonCount]->playerType = 0;
					CannonBall[cannonCount]->SetPosition(sceneObjects["ship01"]->GetPosition());
					//meshList.push_back(CannonBall[cannonCount]);
					cannonCount++;
					if (cannonCount > 7)
					{
						cannonCount = 0;
					}
				}

				bounceTime = 0.5;
			}
		}
		else
		{
			bounceTime -= dt;
		}

		for (int i = 0; i < 8; i++)
		{
			if (CannonBall[i] != nullptr)
			{
				CannonBall[i]->Update();
			}
		}

		if (Application::IsKeyPressed('R'))
		{
			sceneObjects["ship01"]->SetPosition(Vector3(0, 0, 0));
			sceneObjects["ship01"]->SetAmt(90);
			sceneObjects["ship01"]->SetRotation(Vector3(0, 1, 0), sceneObjects["ship01"]->GetAmt());
			sceneObjects["ship01"]->SetScale(Vector3(0.5, 0.5, 0.5));
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

		camera.UpdateCameraSkyBox();

		//Camera Logic
		if (!shipUpgrade && ship01Type != 4)
		{
			float yaw = DegreeToRadian(sceneObjects["ship01"]->GetAmt());
			Vector3 direction = Vector3(sin(yaw), 0, cos(yaw));
			Vector3 position = sceneObjects["ship01"]->GetPosition() - direction * 50;
			camera.SetTarget(sceneObjects["ship01"]->GetPosition().x, sceneObjects["ship01"]->GetPosition().y + 15, sceneObjects["ship01"]->GetPosition().z);
			camera.SetPosition(position.x, position.y + 15, position.z);
		}
		// splashTime(*sceneObjects["ship01"]);
	}

	damageTime(*sceneObjects["ship01"], damage);
}

void GameScene::Render()
{
	// Render VBO here
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0, 0, 1, 1);

	//viewStack.LookAt(camera.position.x, camera.position.y, camera.position.z, camera.target.x, camera.target.y, camera.target.z, camera.up.x, camera.up.y, camera.up.z);

	modelStack.LoadIdentity();

	viewStack.LoadIdentity();


	viewStack.LookAt(camera.position.x, camera.position.y, camera.position.z, camera.target.x, camera.target.y, camera.target.z, camera.up.x, camera.up.y, camera.up.z);

	glViewport(0, 0, 1920 / 2, 1440);


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

	GameObject* allSkyBox = camera.GetAllSkyBox();
	if (allSkyBox != nullptr) {
		for (int i = 0; i < camera.GetSkyBoxSize(); i++) {
			modelStack.PushMatrix();
			modelStack.Translate(allSkyBox[i].GetPosition().x, 0, allSkyBox[i].GetPosition().z);
			modelStack.PushMatrix();
			modelStack.Rotate(allSkyBox[i].GetAmt(), allSkyBox[i].GetRotation().x, allSkyBox[i].GetRotation().y, allSkyBox[i].GetRotation().z);
			modelStack.PushMatrix();
			modelStack.Scale(allSkyBox[i].GetScale().x, allSkyBox[i].GetScale().y, allSkyBox[i].GetScale().z);

			RenderMesh(&allSkyBox[i], false);
			modelStack.PopMatrix();
			modelStack.PopMatrix();
			modelStack.PopMatrix();
		}
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
		modelStack.Rotate(meshList[i]->GetAmt(), meshList[i]->GetRotation().x, meshList[i]->GetRotation().y, meshList[i]->GetRotation().z);

		modelStack.PushMatrix();
		modelStack.Scale(meshList[i]->GetScale().x, meshList[i]->GetScale().y, meshList[i]->GetScale().z);

		RenderMesh(meshList[i], true);

		modelStack.PopMatrix();
		modelStack.PopMatrix();

		modelStack.PopMatrix();
	}

	//RenderTextOnScreen(gameText, "Speed : " + std::to_string(ship01Stats->getSpeed()), Color(0, 1, 0), 50, 2, 20);
	//RenderTextOnScreen(gameText, "Pray for fps : " + std::to_string(sceneFPS), Color(0, 1, 0), 50, 2, 17.5);
	RenderUI(partsCount, 20, 7, 7, 26, 0, 0, 1, 0);
	RenderTextOnScreen(gameText, " :" + std::to_string(ship01Stats->getParts()), Color(0, 1, 0), 50, 5, 3);
	RenderTextOnScreen(gameText, "Health : " + std::to_string(ship01Stats->getHealth()), Color(0, 1, 0), 50, 1, 2);
	RenderUI(healthBar, ship01Stats->getHealth(), 3.5, 5, 20, 0, 0, 1, 0);
	RenderUI(speedometer, 45, 25, 38, 7, 0, 0, 1, 0);
	RenderUI(needle, 30, 28, 57, 6, (-(ship01Stats->getSpeed() / ship01Stats->getMaxSpeed()) * 250), 0, 0, 1);

	//RenderTextOnScreen(gameText, "Collision : ", Color(0, 1, 0), 10, 2, 15);

	
	MiniMap();
	if (winCondition1 && !winCondition2)
	{
		// std::cout << "Player 1 Wins" << std::endl;
		renderWin();
	}
	else if (!winCondition1 && winCondition2)
	{
		renderLose();
	}

	glDisableVertexAttribArray(2);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(0);
}

GameObject* ship02_2;
GameObject* ship02_3;
GameObject* ship02_4;
GameObject* spaceship2;

void GameScene::Init2() {
	//Camera Init
	secondCamera.Init(Vector3(20, 20, 0), Vector3(0, 0, 0), Vector3(0, 1, 0));

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

	Material *mat = new Material;
	mat->kAmbient.Set(0.3f, 0.3f, 0.3f);
	mat->kDiffuse.Set(0.1f, 0.1f, 0.1f);
	mat->kSpecular.Set(0.2f, 0.2f, 0.2f);
	mat->kShininess = 1.0f;

	Transform t;

	ship02Stats = dynamic_cast<CShipStats*>(sceneObjects["ship02"]);
	ship02Stats->setStats(0, 500, 15, 5, 15, 1000, 20);
	ship02_2 = GameObjectFactory::SpawnGameObject(GameObjectFactory::SHIP, "ship02-2", mat, t);
	ship02_2->SetScale(Vector3(0.7, 0.7, 0.7));
	ship02_3 = GameObjectFactory::SpawnGameObject(GameObjectFactory::SHIP, "ship02-3", mat, t);
	ship02_4 = GameObjectFactory::SpawnGameObject(GameObjectFactory::SHIP, "ship02-4", mat, t);
	spaceship2 = GameObjectFactory::SpawnGameObject(GameObjectFactory::SHIP, "spaceship2", mat, t);
}

void GameScene::Update2(double dt)
{
	//secondCamera.SetTarget(sceneObjects["ship02"]->GetPosition().x, sceneObjects["ship02"]->GetPosition().y, sceneObjects["ship02"]->GetPosition().z);
	//secondCamera.SetPosition(sceneObjects["ship02"]->GetPosition().x + 100, sceneObjects["ship02"]->GetPosition().y + 15, sceneObjects["ship02"]->GetPosition().z + 50);

	if (Application::IsKeyPressed('M'))
	{
		if (ship02Type == 0 && ship02Stats->getParts() >= 5)
		{
			ship02Stats->SetSpeed(0);

			shipUpgrade2 = true;
		}
		if (ship02Type == 1 && ship02Stats->getParts() >= 7)
		{
			ship02Stats->SetSpeed(0);

			shipUpgrade2 = true;
		}
		if (ship02Type == 2 && ship02Stats->getParts() >= 10)
		{
			ship02Stats->SetSpeed(0);

			shipUpgrade2 = true;
		}
		if (ship02Type == 3 && ship02Stats->getParts() >= 12)
		{
			ship02Stats->SetSpeed(0);

			shipUpgrade2 = true;
		}
	}
	if (shipUpgrade2)
	{
		
		Transform t;
		t.SetBounds(Vector3(1.3f, 1.3f, 1.3f));
		t.position.x = sceneObjects["ship02"]->GetPosition().x;
		t.position.z = sceneObjects["ship02"]->GetPosition().z;
		
		if (ship02Type == 0)
		{
			for (int j = 0; j < meshList.size(); j++)
			{
				if (meshList[j] == sceneObjects["ship02"])
				{
					secondCamera.SetTarget(sceneObjects["ship02"]->GetPosition().x, sceneObjects["ship02"]->GetPosition().y, sceneObjects["ship02"]->GetPosition().z);
					secondCamera.SetPosition(sceneObjects["ship02"]->GetPosition().x + 50, sceneObjects["ship02"]->GetPosition().y + 15, sceneObjects["ship02"]->GetPosition().z + 50);

					
					sceneObjects["ship02"]->TranslateShip(Vector3(0, 30 * dt, 0));
					sceneObjects["ship02"]->SetRotation(Vector3(0, 1, 0), sceneObjects["ship02"]->GetAmt() + 500 * dt);

					if (sceneObjects["ship02"]->GetAmt() >= 1000 && sceneObjects["ship02"]->GetPosition().y >= 20)
					{
						sceneObjects["ship02"] = ship02_2;
						sceneObjects["ship02"]->GetTransform().SetBounds(Vector3(5.0f, 10.0f, 10.0f));
						sceneObjects["ship02"]->SetPosition(t.position);
						meshList[j] = ship02_2;
						ship02Stats->SetParts(ship02Stats->getParts() - 5);
						shipUpgrade2 = false;
						ship02Type = 1;
					}
				}
			}
		}
		else if (ship02Type == 1)
		{
			for (int j = 0; j < meshList.size(); j++)
			{
				if (meshList[j] == sceneObjects["ship02"])
				{
					secondCamera.SetTarget(sceneObjects["ship02"]->GetPosition().x, sceneObjects["ship02"]->GetPosition().y, sceneObjects["ship02"]->GetPosition().z);
					secondCamera.SetPosition(sceneObjects["ship02"]->GetPosition().x + 50, sceneObjects["ship02"]->GetPosition().y + 15, sceneObjects["ship02"]->GetPosition().z + 50);

				

					sceneObjects["ship02"]->SetRotation(Vector3(0, 1, 0), sceneObjects["ship02"]->GetAmt() + 500 * dt);
					sceneObjects["ship02"]->TranslateShip(Vector3(0, 30 * dt, 0));
					if (sceneObjects["ship02"]->GetAmt() >= 1000 && sceneObjects["ship02"]->GetPosition().y >= 20)
					{
						sceneObjects["ship02"] = ship02_3;
						sceneObjects["ship02"]->GetTransform().SetBounds(Vector3(5.0f, 10.0f, 15.0f));
						sceneObjects["ship02"]->SetPosition(t.position);
						meshList[j] = ship02_3;
						ship02Stats->SetParts(ship02Stats->getParts() - 7);
						shipUpgrade2 = false;					
						ship02Type = 2;
					}
				}
			}
		}
		else if (ship02Type == 2)
		{
			for (int j = 0; j < meshList.size(); j++)
			{
				if (meshList[j] == sceneObjects["ship02"])
				{
					secondCamera.SetTarget(sceneObjects["ship02"]->GetPosition().x, sceneObjects["ship02"]->GetPosition().y, sceneObjects["ship02"]->GetPosition().z);
					secondCamera.SetPosition(sceneObjects["ship02"]->GetPosition().x + 50, sceneObjects["ship02"]->GetPosition().y + 15, sceneObjects["ship02"]->GetPosition().z + 50);

				
					sceneObjects["ship02"]->SetRotation(Vector3(0, 1, 0), sceneObjects["ship02"]->GetAmt() + 500 * dt);
					sceneObjects["ship02"]->TranslateShip(Vector3(0, 30 * dt, 0));
					if (sceneObjects["ship02"]->GetAmt() >= 1000 && sceneObjects["ship02"]->GetPosition().y >= 20)
					{
						sceneObjects["ship02"] = ship02_4;
						sceneObjects["ship02"]->GetTransform().SetBounds(Vector3(5.0f, 10.0f, 18.0f));
						sceneObjects["ship02"]->SetPosition(t.position);
						meshList[j] = ship02_4;
						ship02Stats->SetParts(ship01Stats->getParts() - 10);
						shipUpgrade2 = false;
						ship02Type = 3;
					}
				}
			}
		}
		else if (ship02Type == 3)
		{
			for (int j = 0; j < meshList.size(); j++)
			{
				if (meshList[j] == sceneObjects["ship02"])
				{
					secondCamera.SetTarget(sceneObjects["ship02"]->GetPosition().x, sceneObjects["ship02"]->GetPosition().y, sceneObjects["ship02"]->GetPosition().z);
					secondCamera.SetPosition(sceneObjects["ship02"]->GetPosition().x + 50, sceneObjects["ship02"]->GetPosition().y + 15, sceneObjects["ship02"]->GetPosition().z + 50);

					sceneObjects["ship02"]->TranslateShip(Vector3(0, 30 * dt, 0));
					sceneObjects["ship02"]->SetRotation(Vector3(0, 1, 0), sceneObjects["ship02"]->GetAmt() + 500 * dt);

					if (sceneObjects["ship02"]->GetAmt() >= 1000 && sceneObjects["ship02"]->GetPosition().y >= 20)
					{
						sceneObjects["ship02"] = spaceship2;
						sceneObjects["ship02"]->SetPosition(t.position);
						meshList[j] = spaceship2;
						ship02Stats->SetParts(ship02Stats->getParts() - 15);
						shipUpgrade2 = false;
						ship02Type = 4;
					}
				}
			}
		}
	}
	else {

		if (!shipUpgrade2 && ship02Type != 4)
		{
			if (sceneObjects["ship02"]->GetPosition().y >= -2)
			{
				sceneObjects["ship02"]->TranslateShip(Vector3(0, -30 * dt, 0));
				//sceneObjects["ship02"]->SetPosition(Vector3(sceneObjects["ship02"]->GetPosition().x, translateY, sceneObjects["ship02"]->GetPosition().z));
				sceneObjects["ship02"]->SetRotation(Vector3(0, 1, 0), sceneObjects["ship02"]->GetAmt() + 500 * dt);
			}
		}

		/*
		if (SpeedConsumable::spdup)
		{
			timer += dt;

			if (timer >= 5)
			{
			//	ship02Stats->SetSpeed(ship02Stats->getSpeed() - 100);
				timer = 0;
				SpeedConsumable::spdup = false;
			}
		}
		*/
		//dt = getTimer(); // get same dt as update()

		if (ship02Type == 4)
		{
			secondCamera.SetTarget(sceneObjects["ship02"]->GetPosition().x, sceneObjects["ship02"]->GetPosition().y, sceneObjects["ship02"]->GetPosition().z);
			secondCamera.SetPosition(sceneObjects["ship02"]->GetPosition().x + 50, sceneObjects["ship02"]->GetPosition().y - 15, sceneObjects["ship02"]->GetPosition().z + 50);

			if (sceneObjects["ship02"]->GetPosition().y > 1000)
			{
				winCondition2 = true;
			}
			else if (!winCondition1)
			{
				sceneObjects["ship02"]->TranslateShip(Vector3(0, 300 * dt, 0));
				sceneObjects["ship02"]->SetRotation(Vector3(0, 1, 0), sceneObjects["ship02"]->GetAmt() + 500 * dt);
			}
		}

		if (ship01Stats->getHealth() <= 0)
		{
			winCondition2 = true;
		}

		curWater->UpdateWater(10, dt / 2);

		std::vector<Consumable*> toBeErased;

		for (int j = 0; j < meshList.size(); j++) {
			if (meshList[j] == sceneObjects["ship02"]) { continue; }

			if (sceneObjects["ship02"]->collision.CheckCollision(meshList[j]->GetTransform())) {
				Consumable* cPtr = dynamic_cast<Consumable*>(meshList[j]);
				IslandEnvironment* isPtr = dynamic_cast<IslandEnvironment*>(meshList[j]);
				cannonball* cannonPtr = dynamic_cast<cannonball*>(meshList[j]);

				//std::cout << "Consumable : " << cPtr << "," << "Island : " << isPtr << std::endl;

				if (isPtr != nullptr) {
					/*
					float dirDot = ship02Stats->objectDir.Dot(ship02Stats->GetPosition() - meshList[j]->GetPosition());
					if (dirDot >= 0) {
						if (!isPtr->collidedOnce) {
							isPtr->OnCollide(*ship02Stats); //Ship Collision with island
							isPtr->collidedOnce = true;
						}
						physic.bounceBack(*ship02Stats, dt, 100);
						sceneObjects["ship02"]->translateObj(-ship02Stats->getSpeed() * 6.0f, dt);
					}
					*/
					if (!isPtr->collidedOnce) {
						isPtr->OnCollide(*ship02Stats); //Ship Collision with island
						isPtr->collidedOnce = true;
					}
					//sceneObjects["ship02"]->translateObj(-ship02Stats->getSpeed()* 10.0f * 10.0f, dt);

				}


				if (cPtr != nullptr) {

					toBeErased.push_back(cPtr);
					auto i = std::find(partsmarkervector.begin(), partsmarkervector.end(), cPtr->GetPosition());
					if (i != partsmarkervector.end()) {
						int dist = std::distance(partsmarkervector.begin(), i);
						partsmarkervector.erase(partsmarkervector.begin() + dist);
					}
				}

				if (cannonPtr != nullptr && cannonPtr->playerType == 0) { // other ship cannon hit ship02
					cannonPtr->OnCollide(*ship02Stats);
					//	meshList.erase(meshList.begin() + j);
				}
			}
			else {
				IslandEnvironment* isPtr = dynamic_cast<IslandEnvironment*>(meshList[j]);
				if (isPtr != nullptr && !sceneObjects["ship01"]->collision.CheckCollision(meshList[j]->GetTransform())) {
					isPtr->collidedOnce = false;
				}
			}
		}

		for (auto eraseThis : toBeErased) {
			eraseThis->OnCollide(*ship02Stats);
		}

		short int multipler = 1;

		//Game Logic
		sceneFPS = 1.0f / (float)dt;
		double mouseX, mouseY;
		Application::GetMousePos(mouseX, mouseY);

		//ship...physics
		if (sceneObjects["ship02"]->GetPosition().y < -6)
		{
			// renderSplash(*sceneObjects["ship02"]);

			if (sceneObjects["ship02"]->GetPosition().y < -6.25)
			{
				floating_02 = true;
			}
		}

		if (sceneObjects["ship02"]->GetPosition().y > -5.75)
		{
			floating_02 = false;
		}

		physic.floating(*sceneObjects["ship02"], 7.5, dt, floating_02);

		if (ship02Stats->getHealth() < 10 && ship02Type != 4)
		{
			renderDamage(*sceneObjects["ship02"], damage2);
		}

		light[2].position.x = sceneObjects["ship02"]->GetPosition().x + (sin(DegreeToRadian(sceneObjects["ship02"]->GetAmt())) * 1.5);
		light[2].position.y = sceneObjects["ship02"]->GetPosition().y + 0.25;
		light[2].position.z = sceneObjects["ship02"]->GetPosition().z + (cos(DegreeToRadian(sceneObjects["ship02"]->GetAmt())) * 1.5);

		// control the sceneObjects["ship02"]
		if (Application::IsKeyPressed(VK_UP)) // 270
		{

			if (ship02Stats->getSpeed() < ship02Stats->getMaxSpeed()) // speed limit
			{
				ship02Stats->SetSpeed(physic.acceleration(ship02Stats->getSpeed(), dt));
			}
			else {
				ship02Stats->SetSpeed(physic.acceleration(ship02Stats->getMaxSpeed(), dt));
			}

			sceneObjects["ship02"]->translateObj(ship02Stats->getSpeed(), dt);
		}
		else if (ship02Stats->getSpeed() > 0)
		{
			ship02Stats->SetSpeed(physic.deceleration(ship02Stats->getSpeed(), dt));
			sceneObjects["ship02"]->translateObj(ship02Stats->getSpeed(), dt);
		}

		if (Application::IsKeyPressed(VK_LEFT) && ship02Stats->getSpeed() != 0) // 0
		{
			sceneObjects["ship02"]->rotateObj(ship02Stats->getSpeed() * 0.025);
		}
		if (Application::IsKeyPressed(VK_RIGHT) && ship02Stats->getSpeed() != 0) // 180
		{
			sceneObjects["ship02"]->rotateObj(-(ship02Stats->getSpeed() * 0.025));
		}

		if (bounceTime2 <= 0)
		{
			if (Application::IsKeyPressed(VK_DOWN)) // shoot cannonball
			{
				float direction = sceneObjects["ship02"]->GetAmt() * 3.142f / 180.0f;

				if (ship02Type == 0)
				{
					//cannonball* CannonBall1 = new cannonball(sceneObjects["cannon01"]->curObject, sceneObjects["ship02"]->GetPosition(), 0, Vector3(0, 1, 0), Vector3(1.5f, 1.5f, 1.5f));
					CannonBall2[cannonCount2]->GetTransform().SetBounds(Vector3(5.0f, 5.0f, 5.0f));
					CannonBall2[cannonCount2]->dir = Vector3(sin(direction), 0, cos(direction));
					CannonBall2[cannonCount2]->speed = ship02Stats->getFireRate();
					CannonBall2[cannonCount2]->playerType = 1;
					CannonBall2[cannonCount2]->SetPosition(sceneObjects["ship02"]->GetPosition());
					//meshList.push_back(CannonBall2[cannonCount2]);
					cannonCount2++;
					if (cannonCount2 > 7)
					{
						cannonCount2 = 0;
					}
				}
				if (ship02Type == 1)
				{
					// cannonball* CannonBall1 = new cannonball(sceneObjects["cannon01"]->curObject, sceneObjects["ship02"]->GetPosition(), 0, Vector3(0, 1, 0), Vector3(1.5f, 1.5f, 1.5f));
					CannonBall2[cannonCount2]->GetTransform().SetBounds(Vector3(5.0f, 5.0f, 5.0f));
					CannonBall2[cannonCount2]->dir = Vector3(sin(direction + (30 * 3.142f / 180.0f)), 0, cos(direction + (30 * 3.142f / 180.0f)));
					CannonBall2[cannonCount2]->speed = ship02Stats->getFireRate();
					CannonBall2[cannonCount2]->playerType = 1;
					CannonBall2[cannonCount2]->SetPosition(sceneObjects["ship02"]->GetPosition());
					//meshList.push_back(CannonBall2[cannonCount2]);
					cannonCount2++;
					if (cannonCount2 > 7)
					{
						cannonCount2 = 0;
					}
					// cannonball* CannonBall2 = new cannonball(sceneObjects["cannon01"]->curObject, sceneObjects["ship02"]->GetPosition(), 0, Vector3(0, 1, 0), Vector3(1.5f, 1.5f, 1.5f));
					CannonBall2[cannonCount2]->GetTransform().SetBounds(Vector3(5.0f, 5.0f, 5.0f));
					CannonBall2[cannonCount2]->dir = Vector3(sin(direction - (30 * 3.142f / 180.0f)), 0, cos(direction - (30 * 3.142f / 180.0f)));
					CannonBall2[cannonCount2]->speed = ship02Stats->getFireRate();
					CannonBall2[cannonCount2]->playerType = 1;
					CannonBall2[cannonCount2]->SetPosition(sceneObjects["ship02"]->GetPosition());
					//meshList.push_back(CannonBall2[cannonCount2]);
					cannonCount2++;
					if (cannonCount2 > 7)
					{
						cannonCount2 = 0;
					}
				}
				if (ship02Type == 2)
				{
					//cannonball* CannonBall1 = new cannonball(sceneObjects["cannon01"]->curObject, sceneObjects["ship02"]->GetPosition(), 0, Vector3(0, 1, 0), Vector3(1.5f, 1.5f, 1.5f));
					CannonBall2[cannonCount2]->GetTransform().SetBounds(Vector3(5.0f, 5.0f, 5.0f));
					CannonBall2[cannonCount2]->dir = Vector3(sin(direction + (30 * 3.142f / 180.0f)), 0, cos(direction + (30 * 3.142f / 180.0f)));
					CannonBall2[cannonCount2]->speed = ship02Stats->getFireRate();
					CannonBall2[cannonCount2]->playerType = 1;
					CannonBall2[cannonCount2]->SetPosition(sceneObjects["ship02"]->GetPosition());
					//meshList.push_back(CannonBall2[cannonCount2]);
					cannonCount2++;
					if (cannonCount2 > 7)
					{
						cannonCount2 = 0;
					}
					//cannonball* CannonBall2 = new cannonball(sceneObjects["cannon01"]->curObject, sceneObjects["ship02"]->GetPosition(), 0, Vector3(0, 1, 0), Vector3(1.5f, 1.5f, 1.5f));
					CannonBall2[cannonCount2]->GetTransform().SetBounds(Vector3(5.0f, 5.0f, 5.0f));
					CannonBall2[cannonCount2]->dir = Vector3(sin(direction - (30 * 3.142f / 180.0f)), 0, cos(direction - (30 * 3.142f / 180.0f)));
					CannonBall2[cannonCount2]->speed = ship02Stats->getFireRate();
					CannonBall2[cannonCount2]->playerType = 1;
					CannonBall2[cannonCount2]->SetPosition(sceneObjects["ship02"]->GetPosition());
					//meshList.push_back(CannonBall2[cannonCount2]);
					cannonCount2++;
					if (cannonCount2 > 7)
					{
						cannonCount2 = 0;
					}
					//cannonball* CannonBall3 = new cannonball(sceneObjects["cannon01"]->curObject, Vector3(sceneObjects["ship02"]->GetPosition().x - 8, sceneObjects["ship02"]->GetPosition().y, sceneObjects["ship02"]->GetPosition().z - 8), 0, Vector3(0, 1, 0), Vector3(1.5f, 1.5f, 1.5f));
					CannonBall2[cannonCount2]->GetTransform().SetBounds(Vector3(5.0f, 5.0f, 5.0f));
					CannonBall2[cannonCount2]->dir = Vector3(sin(direction + (30 * 3.142f / 180.0f)), 0, cos(direction + (30 * 3.142f / 180.0f)));
					CannonBall2[cannonCount2]->speed = ship02Stats->getFireRate();
					CannonBall2[cannonCount2]->playerType = 1;
					CannonBall2[cannonCount2]->SetPosition(sceneObjects["ship02"]->GetPosition());
					//meshList.push_back(CannonBall2[cannonCount2]);
					cannonCount2++;
					if (cannonCount2 > 7)
					{
						cannonCount2 = 0;
					}
					//cannonball* CannonBall4 = new cannonball(sceneObjects["cannon01"]->curObject, Vector3(sceneObjects["ship02"]->GetPosition().x - 8, sceneObjects["ship02"]->GetPosition().y, sceneObjects["ship02"]->GetPosition().z - 8), 0, Vector3(0, 1, 0), Vector3(1.5f, 1.5f, 1.5f));
					CannonBall2[cannonCount2]->GetTransform().SetBounds(Vector3(5.0f, 5.0f, 5.0f));
					CannonBall2[cannonCount2]->dir = Vector3(sin(direction - (30 * 3.142f / 180.0f)), 0, cos(direction - (30 * 3.142f / 180.0f)));
					CannonBall2[cannonCount2]->speed = ship02Stats->getFireRate();
					CannonBall2[cannonCount2]->playerType = 1;
					CannonBall2[cannonCount2]->SetPosition(sceneObjects["ship02"]->GetPosition());
					//meshList.push_back(CannonBall2[cannonCount2]);
					cannonCount2++;
					if (cannonCount2 > 7)
					{
						cannonCount2 = 0;
					}
				}
				if (ship02Type == 3)
				{
					//cannonball* CannonBall1 = new cannonball(sceneObjects["cannon01"]->curObject, Vector3(sceneObjects["ship02"]->GetPosition().x, sceneObjects["ship02"]->GetPosition().y, sceneObjects["ship02"]->GetPosition().z - 8), 0, Vector3(0, 1, 0), Vector3(1.5f, 1.5f, 1.5f));
					CannonBall2[cannonCount2]->GetTransform().SetBounds(Vector3(5.0f, 5.0f, 5.0f));
					CannonBall2[cannonCount2]->dir = Vector3(sin(direction), 0, cos(direction));
					CannonBall2[cannonCount2]->speed = ship02Stats->getFireRate();
					CannonBall2[cannonCount2]->playerType = 1;
					CannonBall2[cannonCount2]->SetPosition(sceneObjects["ship02"]->GetPosition());
					//meshList.push_back(CannonBall2[cannonCount2]);
					cannonCount2++;
					if (cannonCount2 > 7)
					{
						cannonCount2 = 0;
					}
					//cannonball* CannonBall2 = new cannonball(sceneObjects["cannon01"]->curObject, Vector3(sceneObjects["ship02"]->GetPosition().x, sceneObjects["ship02"]->GetPosition().y, sceneObjects["ship02"]->GetPosition().z + 8), 0, Vector3(0, 1, 0), Vector3(1.5f, 1.5f, 1.5f));
					CannonBall2[cannonCount2]->GetTransform().SetBounds(Vector3(5.0f, 5.0f, 5.0f));
					CannonBall2[cannonCount2]->dir = Vector3(sin(direction), 0, cos(direction));
					CannonBall2[cannonCount2]->speed = ship02Stats->getFireRate();
					CannonBall2[cannonCount2]->playerType = 1;
					CannonBall2[cannonCount2]->SetPosition(sceneObjects["ship02"]->GetPosition());
					//meshList.push_back(CannonBall2[cannonCount2]);
					cannonCount2++;
					if (cannonCount2 > 7)
					{
						cannonCount2 = 0;
					}
					//cannonball* CannonBall3 = new cannonball(sceneObjects["cannon01"]->curObject, Vector3(sceneObjects["ship02"]->GetPosition().x - 8, sceneObjects["ship02"]->GetPosition().y, sceneObjects["ship02"]->GetPosition().z - 8), 0, Vector3(0, 1, 0), Vector3(1.5f, 1.5f, 1.5f));
					CannonBall2[cannonCount2]->GetTransform().SetBounds(Vector3(5.0f, 5.0f, 5.0f));
					CannonBall2[cannonCount2]->dir = Vector3(sin(direction + (30 * 3.142f / 180.0f)), 0, cos(direction + (30 * 3.142f / 180.0f)));
					CannonBall2[cannonCount2]->speed = ship02Stats->getFireRate();
					CannonBall2[cannonCount2]->playerType = 1;
					CannonBall2[cannonCount2]->SetPosition(sceneObjects["ship02"]->GetPosition());
					//meshList.push_back(CannonBall2[cannonCount2]);
					cannonCount2++;
					if (cannonCount2 > 7)
					{
						cannonCount2 = 0;
					}
					//cannonball* CannonBall4 = new cannonball(sceneObjects["cannon01"]->curObject, Vector3(sceneObjects["ship02"]->GetPosition().x - 8, sceneObjects["ship02"]->GetPosition().y, sceneObjects["ship02"]->GetPosition().z - 8), 0, Vector3(0, 1, 0), Vector3(1.5f, 1.5f, 1.5f));
					CannonBall2[cannonCount2]->GetTransform().SetBounds(Vector3(5.0f, 5.0f, 5.0f));
					CannonBall2[cannonCount2]->dir = Vector3(sin(direction - (30 * 3.142f / 180.0f)), 0, cos(direction - (30 * 3.142f / 180.0f)));
					CannonBall2[cannonCount2]->speed = ship02Stats->getFireRate();
					CannonBall2[cannonCount2]->playerType = 1;
					CannonBall2[cannonCount2]->SetPosition(sceneObjects["ship02"]->GetPosition());
					//meshList.push_back(CannonBall2[cannonCount2]);
					cannonCount2++;
					if (cannonCount2 > 7)
					{
						cannonCount2 = 0;
					}
				}

				bounceTime2 = 0.5;
			}
		}
		else
		{
			bounceTime2 -= dt;
		}

		for (int i = 0; i < 8; i++)
		{
			if (CannonBall2[i] != nullptr)
			{
				CannonBall2[i]->Update();
			}
		}

		secondCamera.UpdateCameraSkyBox();
		//Camera Logic
		if (!shipUpgrade2 && ship02Type != 4)
		{
			float yaw = DegreeToRadian(sceneObjects["ship02"]->GetAmt());
			Vector3 direction = Vector3(sin(yaw), 0, cos(yaw));
			Vector3 position = sceneObjects["ship02"]->GetPosition() - direction * 50;
			secondCamera.SetTarget(sceneObjects["ship02"]->GetPosition().x, sceneObjects["ship02"]->GetPosition().y + 15, sceneObjects["ship02"]->GetPosition().z);
			secondCamera.SetPosition(position.x, position.y + 15, position.z);
		}
	}

	damageTime(*sceneObjects["ship02"], damage2);
	// splashTime(*sceneObjects["ship02"]);
}

void GameScene::Render2()
{
	//viewStack.LookAt(camera.position.x, camera.position.y, camera.position.z, camera.target.x, camera.target.y, camera.target.z, camera.up.x, camera.up.y, camera.up.z);

	modelStack.LoadIdentity();
	viewStack.LoadIdentity();


	viewStack.LookAt(secondCamera.position.x, secondCamera.position.y, secondCamera.position.z, secondCamera.target.x, secondCamera.target.y, secondCamera.target.z, secondCamera.up.x, secondCamera.up.y, secondCamera.up.z);

	glViewport(1920 / 2, 0, 1920 / 2, 1440);


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

	GameObject* allSkyBox = secondCamera.GetAllSkyBox();
	if (allSkyBox != nullptr) {
		for (int i = 0; i < secondCamera.GetSkyBoxSize(); i++) {
			modelStack.PushMatrix();
			modelStack.Translate(allSkyBox[i].GetPosition().x, 0, allSkyBox[i].GetPosition().z);


			modelStack.PushMatrix();
			modelStack.Rotate(allSkyBox[i].GetAmt(), allSkyBox[i].GetRotation().x, allSkyBox[i].GetRotation().y, allSkyBox[i].GetRotation().z);
			modelStack.PushMatrix();
			modelStack.Scale(allSkyBox[i].GetScale().x, allSkyBox[i].GetScale().y, allSkyBox[i].GetScale().z);


			RenderMesh(&allSkyBox[i], false);
			modelStack.PopMatrix();
			modelStack.PopMatrix();
			modelStack.PopMatrix();
		}
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

	MiniMap2();

	// RenderTextOnScreen(gameText, "Speed : " + std::to_string(ship02Stats->getSpeed()), Color(0, 1, 0), 50, 2, 20);
	RenderUI(partsCount, 20, 7, 7, 26, 0, 0, 1, 0);
	RenderTextOnScreen(gameText, " :" + std::to_string(ship02Stats->getParts()), Color(0, 1, 0), 50, 5, 3);
	RenderTextOnScreen(gameText, "Health : " + std::to_string(ship02Stats->getHealth()), Color(0, 1, 0), 50, 1, 2);
	RenderUI(healthBar, ship02Stats->getHealth(), 3.5, 5, 20, 0, 0, 1, 0);
	RenderUI(speedometer, 45, 25, 38, 7, 0, 0, 1, 0);
	RenderUI(needle, 30, 28, 57, 6, (-(ship02Stats->getSpeed() / ship02Stats->getMaxSpeed()) * 250), 0, 0, 1);

	if (!winCondition1 && winCondition2)
	{
		// std::cout << "Player 1 Wins" << std::endl;
		renderWin();
	}
	else if (winCondition1 && !winCondition2)
	{
		renderLose();
	}

	glDisableVertexAttribArray(2);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(0);
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
			powerUp->GetTransform().SetBounds(Vector3(3.f, 4.f, 3.f));
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

	GameObject* allSkyBox = new GameObject[4];

	Left->textureID = LoadTGA("Image//LeftBox.tga");
	box = new GameObject(Left, Vector3(0, 0, -500), 90, Vector3(1, 0, 0), Vector3(2100, 2100, 2100));
	allSkyBox[0] = *box;
	//meshList.push_back(box);
	Right->textureID = LoadTGA("Image//RightBox.tga");
	box = new GameObject(Right, Vector3(0, 0, 500), -90, Vector3(1, 0, 0), Vector3(2100, 2100, 2100));
	allSkyBox[1] = *box;
	//meshList.push_back(box);
	Back->textureID = LoadTGA("Image//BackBox.tga");
	box = new GameObject(Back, Vector3(-500, 0, 0), -90, Vector3(0, 0, 1), Vector3(2100, 2100, 2100));
	allSkyBox[2] = *box;

	//meshList.push_back(box);
	Front->textureID = LoadTGA("Image//FrontBox.tga");
	box = new GameObject(Front, Vector3(500, 0, 0), 90, Vector3(0, 0, 1), Vector3(2100, 2100, 2100));
	allSkyBox[3] = *box;

	//meshList.push_back(box);

	camera.SetCameraSkyBox(allSkyBox, 4);

	GameObject* allSecondSkyBox = new GameObject[4];


	Left->textureID = LoadTGA("Image//LeftBox.tga");
	box = new GameObject(Left, Vector3(0, 0, -500), 90, Vector3(1, 0, 0), Vector3(2100, 2100, 2100));
	allSecondSkyBox[0] = *box;
	//meshList.push_back(box);
	Right->textureID = LoadTGA("Image//RightBox.tga");
	box = new GameObject(Right, Vector3(0, 0, 500), -90, Vector3(1, 0, 0), Vector3(2100, 2100, 2100));
	allSecondSkyBox[1] = *box;
	//meshList.push_back(box);
	Back->textureID = LoadTGA("Image//BackBox.tga");
	box = new GameObject(Back, Vector3(-500, 0, 0), -90, Vector3(0, 0, 1), Vector3(2100, 2100, 2100));
	allSecondSkyBox[2] = *box;

	//meshList.push_back(box);
	Front->textureID = LoadTGA("Image//FrontBox.tga");
	box = new GameObject(Front, Vector3(500, 0, 0), 90, Vector3(0, 0, 1), Vector3(2100, 2100, 2100));
	allSecondSkyBox[3] = *box;

	secondCamera.SetCameraSkyBox(allSecondSkyBox, 4);

}

void GameScene::renderDamage(GameObject& ship, GameObject** smoke)
{
	// Mesh* Smoke = MeshBuilder::GenerateCube("smoke particles", Color(1, 1, 1), 1, 1, 1);
	Mesh* Smoke = MeshBuilder::GenerateOBJ("smoke damage", "OBJ//damage.obj");
	Smoke->material.kAmbient.Set(0.1f, 0.1f, 0.1f);
	Smoke->material.kDiffuse.Set(0.1f, 0.1f, 0.1f);
	Smoke->material.kSpecular.Set(0.2f, 0.2f, 0.2f);
	Smoke->material.kShininess = 1.0f;
	double timing = 0;

	for (int num = 0; num < 5; num++)
	{
		if (timing <= 0)
		{
			if (smoke[num] == nullptr)
			{
				smoke[num] = new GameObject(Smoke, Vector3(ship.GetPosition().x, ship.GetPosition().y + 1.0, ship.GetPosition().z), 90, Vector3(0, 1, 0), Vector3(0.1, 0.1, 0.1));
				meshList.push_back(smoke[num]);
				timing = 1.0;
			}
		}
		else
		{
			timing -= 0.017;
		}
	}
}

void GameScene::damageTime(GameObject& ship, GameObject** smoke)
{
	for (int num = 0; num < 5; num++)
	{
		if (smoke[num] != nullptr)
		{
			if (smoke[num]->lifespan <= 8)
			{
				smoke[num]->lifespan += 0.3;

				if (num % 2 == 0)
				{
					smoke[num]->SetScale(Vector3(smoke[num]->lifespan / 2, smoke[num]->lifespan / 2, smoke[num]->lifespan / 2));
					smoke[num]->SetRotation(Vector3(ship.GetRotation()), ship.GetAmt());
					smoke[num]->SetPosition(Vector3(Vector3(ship.GetPosition().x + smoke[num]->lifespan, ship.GetPosition().y + smoke[num]->lifespan, ship.GetPosition().z + smoke[num]->lifespan)));

				}
				else
				{
					smoke[num]->SetScale(Vector3(smoke[num]->lifespan / 2, smoke[num]->lifespan / 2, smoke[num]->lifespan / 2));
					smoke[num]->SetRotation(Vector3(ship.GetRotation()), ship.GetAmt());
					smoke[num]->SetPosition(Vector3(Vector3(ship.GetPosition().x - smoke[num]->lifespan, ship.GetPosition().y + smoke[num]->lifespan, ship.GetPosition().z - smoke[num]->lifespan)));

				}
			}
			else
			{
				auto i = std::find(GameScene::instance->meshList.begin(), GameScene::instance->meshList.end(), smoke[num]);
				if (i != GameScene::instance->meshList.end())
				{
					int dist = std::distance(GameScene::instance->meshList.begin(), i);
					GameScene::instance->meshList.erase(GameScene::instance->meshList.begin() + dist);
				}

				smoke[num] = nullptr;
			}
		}
	}
}

GameObject* splash[4];

void GameScene::renderSplash(GameObject& other)
{
	Mesh* Splash = MeshBuilder::GenerateCube("splash", Color(0, 0, 1), 1, 1, 1);
	Splash->textureID = LoadTGA("Image//Water.tga");
	Splash->material.kAmbient.Set(0.4f, 0.4f, 0.4f);
	Splash->material.kDiffuse.Set(0.1f, 0.1f, 0.1f);
	Splash->material.kSpecular.Set(0.2f, 0.2f, 0.2f);
	Splash->material.kShininess = 1.0f;

	double timing = 0;

	for (int num = 0; num < 4; num++)
	{
		if (num == 0)
		{
			//GameSound::instance()->WaterSplash->setDefaultVolume(0.25f);
			//GameSound::instance()->engine->play2D(GameSound::instance()->WaterSplash, false);
		}

		if (timing <= 0)
		{
			if (splash[num] == nullptr)
			{
				splash[num] = new GameObject(Splash, Vector3(0, 0, 0), 90, Vector3(0, 1, 0), Vector3(1, 1, 1));

				//splash[num]->SetRotation(splash[num]->objectDir, 0);
				splash[num]->SetPosition(Vector3(other.GetPosition().x + 5, other.GetPosition().y - 2, other.GetPosition().z + 5) + (other.objectDir.Cross(Vector3(0, 1, 0))));
				std::cout << " x : " << splash[num]->GetPosition().x << std::endl;
				std::cout << " y : " << splash[num]->GetPosition().y << std::endl;
				std::cout << " z : " << splash[num]->GetPosition().z << std::endl;
				meshList.push_back(splash[num]);
				timing = 0.5;

				//GameSound::instance()->WaterSplash->setDefaultVolume(1.f);
				//GameSound::instance()->engine->play2D(GameSound::instance()->WaterSplash, false);
			}
		}
		else
		{
			timing -= 0.017;
		}

		num++;

		if (timing <= 0)
		{
			if (splash[num] == nullptr)
			{
				splash[num] = new GameObject(Splash, Vector3(0, 0, 0), 90, Vector3(0, 1, 0), Vector3(1, 1, 1));
				splash[num]->objectDir = other.objectDir.Cross(Vector3(0, 1, 0));
				splash[num]->SetPosition(Vector3(other.GetPosition().x - 5, other.GetPosition().y - 2, other.GetPosition().z - 5) - (other.objectDir.Cross(Vector3(0, 1, 0))));



				meshList.push_back(splash[num]);
				timing = 0.5;

				
			}
		}
		else
		{
			timing -= 0.017;
		}
	}
}

void GameScene::splashTime(GameObject& other)
{
	for (int num = 0; num < 4; num++)
	{
		float direction = DegreeToRadian(other.GetAmt());

		if (splash[num] != nullptr)
		{
			if (splash[num]->lifespan <= 6)
			{
				splash[num]->lifespan += 0.3;

				splash[num]->SetScale(Vector3(splash[num]->lifespan / 3, splash[num]->lifespan / 3, splash[num]->lifespan / 3));
				splash[num]->SetPosition(Vector3(
					splash[num]->GetPosition().x - (sin(direction) * ((float)(splash[num]->lifespan * 5 * 0.17))),
					splash[num]->GetPosition().y + splash[num]->lifespan / 4,
					splash[num]->GetPosition().z - (cos(direction) * ((float)(splash[num]->lifespan * 5 * 0.17))))
				);
			}
			else
			{
				auto i = std::find(GameScene::instance->meshList.begin(), GameScene::instance->meshList.end(), splash[num]);
				if (i != GameScene::instance->meshList.end())
				{
					int dist = std::distance(GameScene::instance->meshList.begin(), i);
					GameScene::instance->meshList.erase(GameScene::instance->meshList.begin() + dist);
				}

				splash[num] = nullptr;
			}
		}
	}
}

void GameScene::MiniMap()
{
	int ship1x = sceneObjects["ship01"]->GetPosition().z;
	int ship1y = sceneObjects["ship01"]->GetPosition().x;
	int ship2x = sceneObjects["ship02"]->GetPosition().z;
	int ship2y = sceneObjects["ship02"]->GetPosition().x;

	RenderUI(MapWater, 13, 13, 140, 58, 0, 0, 1, 0); // 22
	RenderUI(ShipMarker1, 0.2, 0.2, (ship1x) * 1.3 + 9600, (ship1y) * 1.3 + 3800, 0, 0, 1, 0); // 1400
	RenderUI(ShipMarker2, 0.2, 0.2, (ship2x) * 1.3 + 9600, (ship2y) * 1.3 + 3800, 0, 0, 1, 0);

	for (int i = 0; i <= 8; i++)
	{
		RenderUI(IslandMarker, 0.2, 0.2, (islandposzA[i]) * 1.3 + 9600, (islandposxA[i]) * 1.3 + 3800, 0, 0, 1, 0);
	}

	for (int i = 0; i < partsmarkervector.size(); i++)
	{
		RenderUI(PartsMarker, 0.2, 0.2, partsmarkervector[i].z * 1.3 + 9600, partsmarkervector[i].x * 1.3 + 3800, 0, 0, 1, 0);
	}

}

void GameScene::MiniMap2()
{
	int ship1x = sceneObjects["ship01"]->GetPosition().z;
	int ship1y = sceneObjects["ship01"]->GetPosition().x;
	int ship2x = sceneObjects["ship02"]->GetPosition().z;
	int ship2y = sceneObjects["ship02"]->GetPosition().x;

	RenderUI(MapWater, 13, 13, 10, 58, 0, 0, 1, 0);
	RenderUI(ShipMarker1, 0.2, 0.2, (ship1x) * 1.3 + 0, (ship1y) * 1.3 + 3800, 0, 0, 1, 0);
	RenderUI(ShipMarker2, 0.2, 0.2, (ship2x) * 1.3 + 0, (ship2y) * 1.3 + 3800, 0, 0, 1, 0);

	for (int i = 0; i <= 8; i++)
	{
		RenderUI(IslandMarker, 0.2, 0.2, (islandposzA[i]) * 1.3 + 0, (islandposxA[i]) * 1.3 + 3800, 0, 0, 1, 0);
	}

	for (int i = 0; i < partsmarkervector.size(); i++)
	{
		RenderUI(PartsMarker, 0.2, 0.2, partsmarkervector[i].z * 1.3 + 0, partsmarkervector[i].x * 1.3 + 3800, 0, 0, 1, 0);
	}

}

int UIy = 29;
int UIy2 = 29;

void GameScene::renderWin()
{
	RenderUI(Win, 50, 27, 20, 20, 0, 0, 1, 0);

	if (winCondition1)
	{
		RenderUI(Select, 20, 7, 30, UIy, 0, 0, 1, 0); // x 30 y 29 11

		if (Application::IsKeyPressed('W'))
		{
			GameSound::instance()->Select->setDefaultVolume(1.0f);
			GameSound::instance()->engine->play2D(GameSound::instance()->Select, false);
			UIy = 29;
		}
			
		if (Application::IsKeyPressed('S'))
		{
			GameSound::instance()->Select->setDefaultVolume(1.0f);
			GameSound::instance()->engine->play2D(GameSound::instance()->Select, false);
			UIy = 11;
		}

		if (Application::IsKeyPressed(VK_SPACE))
		{
			GameSound::instance()->WaterSplash->setDefaultVolume(1.0f);
			GameSound::instance()->engine->play2D(GameSound::instance()->WaterSplash, false);

			if (UIy == 29)
				returnMainMenu = true;
			else
				exit(0);
		}
	}

	if (winCondition2)
	{
		RenderUI(Select, 20, 7, 30, UIy2, 0, 0, 1, 0); // x 30 y 29 11

		if (Application::IsKeyPressed(VK_UP))
		{
			GameSound::instance()->Select->setDefaultVolume(1.0f);
			GameSound::instance()->engine->play2D(GameSound::instance()->Select, false);
			UIy2 = 29;
		}

		if (Application::IsKeyPressed(VK_DOWN))
		{
			GameSound::instance()->Select->setDefaultVolume(1.0f);
			GameSound::instance()->engine->play2D(GameSound::instance()->Select, false);
			UIy2 = 11;
		}

		if (Application::IsKeyPressed(VK_SPACE))
		{
			GameSound::instance()->WaterSplash->setDefaultVolume(1.0f);
			GameSound::instance()->engine->play2D(GameSound::instance()->WaterSplash, false);

			if (UIy2 == 29)
				returnMainMenu = true;
			else
				exit(0);
		}
	}
}

void GameScene::renderLose()
{
	RenderUI(Lose, 50, 27, 20, 20, 0, 0, 1, 0);

	if (!winCondition1)
	{
		RenderUI(Select, 20, 7, 30, UIy, 0, 0, 1, 0); // x 30 y 29 11

		if (Application::IsKeyPressed('W'))
		{
			GameSound::instance()->Select->setDefaultVolume(1.0f);
			GameSound::instance()->engine->play2D(GameSound::instance()->Select, false);
			UIy = 29;
		}

		if (Application::IsKeyPressed('S'))
		{
			GameSound::instance()->Select->setDefaultVolume(1.0f);
			GameSound::instance()->engine->play2D(GameSound::instance()->Select, false);
			UIy = 11;
		}

		if (Application::IsKeyPressed(VK_SPACE))
		{
			GameSound::instance()->WaterSplash->setDefaultVolume(1.0f);
			GameSound::instance()->engine->play2D(GameSound::instance()->WaterSplash, false);

			if (UIy == 29)
				returnMainMenu = true;
			else
				exit(0);
		}
	}

	if (!winCondition2)
	{
		RenderUI(Select, 20, 7, 30, UIy2, 0, 0, 1, 0); // x 30 y 29 11

		if (Application::IsKeyPressed(VK_UP))
		{
			GameSound::instance()->Select->setDefaultVolume(1.0f);
			GameSound::instance()->engine->play2D(GameSound::instance()->Select, false);
			UIy2 = 29;
		}

		if (Application::IsKeyPressed(VK_DOWN))
		{
			GameSound::instance()->Select->setDefaultVolume(1.0f);
			GameSound::instance()->engine->play2D(GameSound::instance()->Select, false);
			UIy2 = 11;
		}

		if (Application::IsKeyPressed(VK_SPACE))
		{
			GameSound::instance()->WaterSplash->setDefaultVolume(1.0f);
			GameSound::instance()->engine->play2D(GameSound::instance()->WaterSplash, false);

			if (UIy2 == 29)
				returnMainMenu = true;
			else
				exit(0);
		}
	}

	/*if (Application::IsKeyPressed('Z'))
	{
		UIx++;
		std::cout << "X : " << UIx << std::endl;
	}
	if (Application::IsKeyPressed('X'))
	{
		UIx--;
		std::cout << "X : " << UIx << std::endl;
	}

	if (Application::IsKeyPressed('C'))
	{
		UIy++;
		std::cout << "Y : " << UIy << std::endl;
	}
	if (Application::IsKeyPressed('V'))
	{
		UIy--;
		std::cout << "Y : " << UIy << std::endl;
	}*/
}

void GameScene::Exit()
{
	delete curCube;
	glDeleteVertexArrays(1, &m_vertexArrayID);
	glDeleteProgram(m_programID);
}