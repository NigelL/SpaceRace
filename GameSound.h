#pragma once
#include <irrKlang.h>

class GameSound
{
	int m_value;
	static GameSound *s_instance;
	GameSound(int v = 0)
	{
		m_value = v;
	}

public:

	~GameSound();

	irrklang::ISoundEngine* engine = irrklang::createIrrKlangDevice();

	irrklang::ISoundSource* MenuBGM = engine->addSoundSourceFromFile("OST/MenuBGM.wav");

	irrklang::ISoundSource* GameBGM = engine->addSoundSourceFromFile("OST/GameBGM.wav");

	irrklang::ISoundSource* CannonFire = engine->addSoundSourceFromFile("OST/CannonFire.wav");

	irrklang::ISoundSource* PaintSplat = engine->addSoundSourceFromFile("OST/PaintSplat.wav");

	irrklang::ISoundSource* WaterSplash = engine->addSoundSourceFromFile("OST/WaterSplash.wav");

	irrklang::ISoundSource* Select = engine->addSoundSourceFromFile("OST/Select.wav");

	irrklang::ISoundSource* MenuMove = engine->addSoundSourceFromFile("OST/MenuMove.wav");

	irrklang::ISoundSource* MenuSelect = engine->addSoundSourceFromFile("OST/MenuSelect.wav");

	irrklang::ISoundSource* SpeedUp = engine->addSoundSourceFromFile("OST/SpeedUp.wav");

	irrklang::ISoundSource* HealthUp = engine->addSoundSourceFromFile("OST/HealthUp.wav");

	irrklang::ISoundSource* PartsPickUp = engine->addSoundSourceFromFile("OST/PartsPickUp.wav");

	irrklang::ISoundSource* Thud = engine->addSoundSourceFromFile("OST/Thud.wav");

	static GameSound *instance()
	{
		if (!s_instance)
		{
			s_instance = new GameSound;
		}
		return s_instance;
	}
};

//			GameSound::instance()->engine->play2D(GameSound::instance()->Melee);
//			REFRENCE TO HWAT TO DO TO PLAY SOUND