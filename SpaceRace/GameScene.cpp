#include "GameScene.h"

Water* curWater = new Water();
GameObject* ship_01;
GameObject* ship_02;

GameScene::GameScene()
{
	srand(time(0));

	gameText = MeshBuilder::GenerateText("text", 16, 16);
	gameText->textureID = LoadTGA("Image//calibri.tga");

	Mesh* curMesh = MeshBuilder::GenerateQuad("Test", Color(0, 1, 0), 100, 100);
	curMesh->textureID = LoadTGA("Image//Water.tga");

	curMesh->material.kAmbient.Set(0.5f, 0.5f, 0.5f);
	curMesh->material.kDiffuse.Set(0.0f, 0.0f, 1.0f);
	curMesh->material.kShininess = 1.0f;
	curMesh->material.kSpecular.Set(0.0f, 0.0f, 1.0f);
	
	curWater->waterMesh = curMesh;
	meshList.push_back(new GameObject(curMesh, Vector3(-50, -5.0f, 50), 180, Vector3(1, 0, 0), Vector3(100, 1, 100)));
	
	Mesh* Ship_01 = MeshBuilder::GenerateOBJ("ship 01", "OBJ//ship.obj");
	Ship_01->textureID = LoadTGA("Image//ship.tga");
	Ship_01->material.kAmbient.Set(0.4f, 0.4f, 0.4f);
	Ship_01->material.kDiffuse.Set(0.1f, 0.1f, 0.1f);
	Ship_01->material.kSpecular.Set(0.2f, 0.2f, 0.2f);
	Ship_01->material.kShininess = 1.0f;
	ship_01 = new GameObject(Ship_01, Vector3(2, 0, -2), 90, Vector3(0, 1, 0), Vector3(0.1, 0.1, 0.1));
	meshList.push_back(ship_01);

	Mesh* Ship_02 = MeshBuilder::GenerateOBJ("ship 02", "OBJ//ship2.obj");
	Ship_02->textureID = LoadTGA("Image//ship2.tga");
	Ship_02->material.kAmbient.Set(0.4f, 0.4f, 0.4f);
	Ship_02->material.kDiffuse.Set(0.1f, 0.1f, 0.1f);
	Ship_02->material.kSpecular.Set(0.2f, 0.2f, 0.2f);
	Ship_02->material.kShininess = 1.0f;
	ship_02 = new GameObject(Ship_02, Vector3(-2, 0, -2), 90, Vector3(0, 1, 0), Vector3(0.25, 0.25, 0.25));
	meshList.push_back(ship_02);
	
	//meshList.push_back(new GameObject(MeshBuilder::GenerateQuad("Floor", Color(0, 1, 0), 2, 2), Vector3(2, 2, 2), Vector3(0, 90, 0), Vector3(10, 5, 5)));

	//for (int numberofislands = 0; numberofislands < 5; numberofislands++)
	//{
	//	int islandx = rand() % 5 + 1;
	//	int islandy = rand() % 5 + 1;
	//	int islandposx = rand() % 5 + 1;
	//	int islandposy = rand() % 5 + 1;
	//	meshList.push_back(new GameObject(MeshBuilder::GenerateCube("Islands", Color(0, 1, 0), islandx, islandy, 2), Vector3(islandposx, islandposy, 0), Vector3(90, 0, 0), Vector3(50, 1, 1)));
	//}
	//meshList.push_back(new GameObject(MeshBuilder::GenerateCube("Islands", Color(COLOR), STARTING POSITION OF OBJ), Vector3(POSITION), Vector3(ROTATION), Vector3(SCALE)));
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

void GameScene::RenderTextOnScreen(Mesh* mesh, std::string text, Color color, float size, float
	x, float y) {

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
			
void GameScene::RenderUI(Mesh* mesh ,float size, float
	x, float y) {
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

inline void GameScene::SetGameText(Mesh* curtext)  {
	gameText = curtext;
}

void GameScene::Init()
{	
	//Camera Init
	camera.Init(Vector3(10, 20, 0),Vector3(0, 0, 0), Vector3(0, 1, 0));


	// GameSound::instance()->GameBGM->setDefaultVolume(0.8f);
	// GameSound::instance()->engine->play2D(GameSound::instance()->GameBGM, true);

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


	
	m_programID = LoadShaders("Shader//Texture.vertexshader",
		"Shader//Text.fragmentshader");
	m_parameters[U_TEXT_ENABLED] = glGetUniformLocation(m_programID,
		"textEnabled");
	m_parameters[U_TEXT_COLOR] = glGetUniformLocation(m_programID,
		"textColor");

	glfwSetCursorPos(Application::getGLFWWindow(), 1000, 700);
}

static double LSPEED = 10.0;
static short int lightType = 0;
static bool openShop = false;
int shopItem;

Vector3 ray_wor;
Vector3 ray_Clip;
Vector3 curHitPoint;

static double bounceTime = 0.0;

void GameScene::Update(double dt)
{
	curWater->UpdateWater(10, dt);

	short int multipler = 1;
	//Camera Logic
	camera.Update((float)dt);
	
	//Game Logic
	sceneFPS = 1.0f / (float)dt;
	double mouseX, mouseY;
	Application::GetMousePos(mouseX, mouseY);

	// ship stuff
	Vector3 pos, rot, scale;

	ship_01->translateX = (float)(5 * dt);
	ship_01->translateY = (float)(5 * dt);
	ship_01->translateZ = (float)(5 * dt);

	ship_02->translateX = (float)(15 * dt);
	ship_02->translateY = (float)(15 * dt);
	ship_02->translateZ = (float)(15 * dt);

	// control the ship
	if (Application::IsKeyPressed(VK_UP)) // 270
	{
		if (!(ship_01->GetPosition().x < -45) && !(ship_01->GetPosition().x > 45) && !(ship_01->GetPosition().z < -45) && !(ship_01->GetPosition().z > 45))
		{
			float direction = DegreeToRadian(ship_01->rotate);

			pos.Set(ship_01->GetPosition().x + (sin(direction) * ship_01->translateX), ship_01->GetPosition().y, ship_01->GetPosition().z + (cos(direction) * ship_01->translateZ));
			ship_01->SetPosition(pos);
		}

		if (Application::IsKeyPressed(VK_LEFT)) // 0
		{
			ship_01->rotate += (int)(2);

			rot.Set(0, 1, 0);
			ship_01->SetRotation(rot, ship_01->rotate);
		}
		if (Application::IsKeyPressed(VK_RIGHT)) // 180
		{
			ship_01->rotate -= (int)(2);
			rot.Set(0, 1, 0);
			ship_01->SetRotation(rot, ship_01->rotate);
		}
	}
	if (Application::IsKeyPressed(VK_SPACE)) // temporary scale the second ship
	{
		//ship_01->scaleObject += (float)(1.5 * dt);
		ship_02->scaleObject = 1;
		scale.Set(ship_02->scaleObject, ship_02->scaleObject, ship_02->scaleObject);
		ship_02->SetScale(scale);
	}

	//if (Application::IsKeyPressed('F')) // 270
	//{
	//	if (!(ship_02->GetPosition().x < -45))
	//	{
	//		if (ship_02->rotate > 90 && ship_02->rotate < 270)
	//		{
	//			pos.Set(ship_02->GetPosition().x - ship_02->translateX, ship_02->GetPosition().y, ship_02->GetPosition().z + (ship_02->translateZ / 2));
	//		}
	//		else if ((ship_02->rotate < 90 && ship_02->rotate > 0) || (ship_02->rotate > 270 && ship_02->rotate < 360))
	//		{
	//			pos.Set(ship_02->GetPosition().x - ship_02->translateX, ship_02->GetPosition().y, ship_02->GetPosition().z - (ship_02->translateZ / 2));
	//		}
	//		else
	//		{
	//			pos.Set(ship_02->GetPosition().x - ship_02->translateX, ship_02->GetPosition().y, ship_02->GetPosition().z);
	//		}

	//		ship_02->SetPosition(pos);
	//	}

	//	cout << endl;
	//	cout << " Angle : " << ship_02->rotate << endl;

	//	if (ship_02->rotate >= 90 && ship_02->rotate < 270)
	//	{
	//		ship_02->rotate += (int)(15);
	//	}
	//	else if (ship_02->rotate == 0 || ship_02->rotate > 270 || (ship_02->rotate < 90 && ship_02->rotate > 0))
	//	{
	//		if (ship_02->rotate == 0)
	//		{
	//			ship_02->rotate = 360;
	//		}

	//		ship_02->rotate -= (int)(15);
	//	}
	//	rot.Set(0, 1, 0);
	//	ship_02->SetRotation(rot, ship_02->rotate);
	//}
	//if (Application::IsKeyPressed('V')) // 90
	//{
	//	if (!(ship_02->GetPosition().x > 45))
	//	{
	//		if ((ship_02->rotate > 270 && ship_02->rotate < 360) || (ship_02->rotate >= 0 && ship_02->rotate < 90))
	//		{
	//			pos.Set(ship_02->translateX + ship_02->GetPosition().x, ship_02->GetPosition().y, ship_02->GetPosition().z + (ship_02->translateZ / 2));
	//		}
	//		else if (ship_02->rotate < 270 && ship_02->rotate > 90)
	//		{
	//			pos.Set(ship_02->translateX + ship_02->GetPosition().x, ship_02->GetPosition().y, ship_02->GetPosition().z - (ship_02->translateZ / 2));
	//		}
	//		else
	//		{
	//			pos.Set(ship_02->translateX + ship_02->GetPosition().x, ship_02->GetPosition().y, ship_02->GetPosition().z);
	//		}

	//		ship_02->SetPosition(pos);
	//	}

	//	cout << endl;
	//	cout << " Angle : " << ship_02->rotate << endl;

	//	if (ship_02->rotate > 90 && ship_02->rotate <= 270)
	//	{
	//		ship_02->rotate -= (int)(15);
	//	}
	//	else if (ship_02->rotate < 90 || (ship_02->rotate > 270 && ship_02->rotate <= 360))
	//	{
	//		if (ship_02->rotate == 360)
	//		{
	//			ship_02->rotate = 0;
	//		}
	//		ship_02->rotate += (int)(15);
	//	}
	//	rot.Set(0, 1, 0);
	//	ship_02->SetRotation(rot, ship_02->rotate);
	//}
	//if (Application::IsKeyPressed('C')) // 0
	//{
	//	if (!(ship_02->GetPosition().z > 45))
	//	{
	//		if (ship_02->rotate > 180 && ship_02->rotate < 360)
	//		{
	//			pos.Set(ship_02->GetPosition().x + (ship_02->translateX / 2), ship_02->GetPosition().y, ship_02->translateZ + ship_02->GetPosition().z);
	//		}
	//		else if (ship_02->rotate < 180 && ship_02->rotate > 0)
	//		{
	//			pos.Set(ship_02->GetPosition().x - (ship_02->translateX / 2), ship_02->GetPosition().y, ship_02->translateZ + ship_02->GetPosition().z);
	//		}
	//		else
	//		{
	//			pos.Set(ship_02->GetPosition().x, ship_02->GetPosition().y, ship_02->translateZ + ship_02->GetPosition().z);
	//		}

	//		ship_02->SetPosition(pos);
	//	}

	//	cout << endl;
	//	cout << " Angle : " << ship_02->rotate << endl;

	//	if (ship_02->rotate > 180 && ship_02->rotate < 360)
	//	{
	//		ship_02->rotate += (int)(15);
	//	}
	//	else if (ship_02->rotate > 0 && ship_02->rotate <= 180)
	//	{
	//		ship_02->rotate -= (int)(15);
	//	}
	//	rot.Set(0, 1, 0);
	//	ship_02->SetRotation(rot, ship_02->rotate);
	//}
	//if (Application::IsKeyPressed('B')) // 180
	//{
	//	if (!(ship_02->GetPosition().z < -45))
	//	{
	//		if (ship_02->rotate > 0 && ship_02->rotate < 180)
	//		{
	//			pos.Set(ship_02->GetPosition().x + (ship_02->translateX / 2), ship_02->GetPosition().y, ship_02->GetPosition().z - ship_02->translateZ);
	//		}
	//		else if (ship_02->rotate > 180 && ship_02->rotate < 360)
	//		{
	//			pos.Set(ship_02->GetPosition().x - (ship_02->translateX / 2), ship_02->GetPosition().y, ship_02->GetPosition().z - ship_02->translateZ);
	//		}
	//		else
	//		{
	//			pos.Set(ship_02->GetPosition().x, ship_02->GetPosition().y, ship_02->GetPosition().z - ship_02->translateZ);
	//		}

	//		ship_02->SetPosition(pos);
	//	}

	//	cout << endl;
	//	cout << " Angle : " << ship_02->rotate << endl;

	//	if (ship_02->rotate < 180 && ship_02->rotate >= 0)
	//	{
	//		ship_02->rotate += (int)(15);
	//	}
	//	else if (ship_02->rotate <= 360 && ship_02->rotate > 180)
	//	{
	//		ship_02->rotate -= (int)(15);
	//	}
	//	rot.Set(0, 1, 0);
	//	ship_02->SetRotation(rot, ship_02->rotate);
	//}

	if (Application::IsKeyPressed('R'))
	{
		Vector3 initial;
		// ship_01
		initial.Set(2, 0, -2);
		ship_01->SetPosition(initial);
		initial.Set(0, 1, 0);
		ship_01->SetRotation(initial, 90);
		initial.Set(0.1, 0.1, 0.1);
		ship_01->SetScale(initial);

		ship_01->rotate = 90;
		ship_01->scaleObject = 0;
		ship_01->translateX = 0;
		ship_01->translateY = 0;
		ship_01->translateZ = 0;
		// ship_02
		initial.Set(-2, 0, -2);
		ship_02->SetPosition(initial);
		initial.Set(0, 1, 0);
		ship_02->SetRotation(initial, 90);
		initial.Set(0.25, 0.25, 0.25);
		ship_02->SetScale(initial);

		ship_02->rotate = 90;
		ship_02->scaleObject = 0;
		ship_02->translateX = 0;
		ship_02->translateY = 0;
		ship_02->translateZ = 0;
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
			
	if (bounceTime <= 0.0) {
		if (Application::IsKeyPressed('G')) {
			openShop = !openShop;
			bounceTime = 0.5;
		}
		if (Application::IsKeyPressed('1')) {
			shopItem = 1;
			bounceTime = 0.5;
		}
		if (Application::IsKeyPressed('2')) {
			shopItem = 2;
			bounceTime = 0.5;
		}
		if (Application::IsKeyPressed('3')) {
			shopItem = 3;
			bounceTime = 0.5;
		}
	}
	else {
		bounceTime -= dt;
	}
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



	modelStack.LoadIdentity();

	viewStack.LoadIdentity();

	viewStack.LookAt(camera.position.x, camera.position.y, camera.position.z,
		camera.target.x, camera.target.y, camera.target.z, camera.up.x, camera.up.y,
		camera.up.z);

	
	

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

	

	

	//Skybox
	/*
	modelStack.PushMatrix();
	modelStack.Translate(0, 0, -600);
	modelStack.PushMatrix();
	modelStack.Rotate(180,1,0,0);
	modelStack.PushMatrix();
	modelStack.Rotate(180, 0, 1, 0);
	modelStack.PushMatrix();
	modelStack.Scale(1000, 1000, 1000);
	
	RenderMesh(GEO_FRONT, false);
	modelStack.PopMatrix();
	modelStack.PopMatrix();
	modelStack.PopMatrix();
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(0, 0, 600);
	modelStack.PushMatrix();
	modelStack.Rotate(180, 1, 0, 0);
	modelStack.PushMatrix();
	modelStack.Scale(1000, 1000, 1000);

	RenderMesh(GEO_BACK, false);
	modelStack.PopMatrix();
	modelStack.PopMatrix();
	modelStack.PopMatrix();
	
	modelStack.PushMatrix();
	modelStack.Translate(-700, 0, 0);
	modelStack.PushMatrix();
	modelStack.Rotate(180, 1, 0, 0);
	modelStack.PushMatrix();
	modelStack.Rotate(90, 0, 1, 0);
	modelStack.PushMatrix();
	modelStack.Scale(1000, 1000, 1000);

	RenderMesh(GEO_LEFT, false);
	modelStack.PopMatrix();
	modelStack.PopMatrix();
	modelStack.PopMatrix();
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(700, 0, 0);
	modelStack.PushMatrix();
	modelStack.Rotate(180, 1, 0, 0);
	modelStack.PushMatrix();
	modelStack.Rotate(-90, 0, 1, 0);
	modelStack.PushMatrix();
	modelStack.Scale(1000, 1000, 1000);

	RenderMesh(GEO_RIGHT, false);
	modelStack.PopMatrix();
	modelStack.PopMatrix();
	modelStack.PopMatrix();
	modelStack.PopMatrix();
	
	modelStack.PushMatrix();
	modelStack.Translate(200, 700, 0);
	modelStack.PushMatrix();
	modelStack.Rotate(90,1, 0, 0);
	modelStack.PushMatrix();
	modelStack.Scale(1000, 1000, 1000);

	RenderMesh(GEO_TOP, false);
	modelStack.PopMatrix();
	modelStack.PopMatrix();
	modelStack.PopMatrix();
	

	modelStack.PushMatrix();
	modelStack.Translate(-200, -700, 0);
	modelStack.PushMatrix();
	modelStack.Rotate(90, -1, 0, 0);
	modelStack.PushMatrix();
	modelStack.Scale(2000, 2000, 2000);

	RenderMesh(GEO_BOTTOM, false);
	modelStack.PopMatrix();
	modelStack.PopMatrix();
	modelStack.PopMatrix();

	*/


	//Render Floor
	/*
	modelStack.PushMatrix();
	modelStack.Translate(0, -1, 0);
	modelStack.PushMatrix();
	modelStack.Rotate(-90, 1, 0, 0);
	modelStack.PushMatrix();
	modelStack.Scale(50, 50, 50);
		
	RenderMesh(GEO_QUAD, true);
	modelStack.PopMatrix();
	modelStack.PopMatrix();
	modelStack.PopMatrix();
	*/

	for (int i = 0; i < (int)meshList.size(); i++) {
		
		modelStack.PushMatrix();
		modelStack.Translate(meshList[i]->GetPosition().x, meshList[i]->GetPosition().y, meshList[i]->GetPosition().z);		

		modelStack.PushMatrix();
		modelStack.Rotate(meshList[i]->GetAmt(),meshList[i]->GetRotation().x / meshList[i]->GetRotation().Length(), meshList[i]->GetRotation().y / meshList[i]->GetRotation().Length(), meshList[i]->GetRotation().z / meshList[i]->GetRotation().Length());
		

		modelStack.PushMatrix();
		modelStack.Scale(meshList[i]->GetScale().x, meshList[i]->GetScale().y, meshList[i]->GetScale().z);

		RenderMesh(meshList[i], true);
		
		modelStack.PopMatrix();
		modelStack.PopMatrix();
		modelStack.PopMatrix();
	}

	RenderTextOnScreen(gameText, "FPS : " + std::to_string(sceneFPS)  , Color(0, 1, 0), 30, 0, 28);


	//modelStack.PushMatrix();
	//modelStack.Translate(0, 5, 0);
	//meshList[GEO_PIKACHUEARS]->Render();


	//modelStack.PopMatrix();


	glDisableVertexAttribArray(2);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(0);
}

void GameScene::Exit()
{
	glDeleteVertexArrays(1, &m_vertexArrayID);
	glDeleteProgram(m_programID);
}
