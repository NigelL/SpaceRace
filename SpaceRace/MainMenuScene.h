#pragma once
class MainMenuScene
{
private:
	bool play, singleplayer, multiplayer;
public:
	MainMenuScene();
	~MainMenuScene();

	void RenderMainMenu();
	bool getPlay();
};