#include "MainMenuScene.h"
#include "GameObject.h"
#include "Mesh.h"
#include "MeshBuilder.h"

MainMenuScene::MainMenuScene()
{
	play = singleplayer = multiplayer = false;
}


MainMenuScene::~MainMenuScene()
{

}

void MainMenuScene::RenderMainMenu()
{
	Mesh* MainMenu = MeshBuilder::GenerateQuad("Main Menu", Color(0, 1, 0), 10, 10);
}

bool MainMenuScene::getPlay()
{
	return play;
}