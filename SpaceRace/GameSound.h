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
	
	irrklang::ISoundSource* Melee = engine->addSoundSourceFromFile("OST/MeleeAttack.wav");

	irrklang::ISoundSource* GameBGM = engine->addSoundSourceFromFile("OST/GameBGM.wav");

	irrklang::ISoundSource* CannonFire = engine->addSoundSourceFromFile("OST/CannonFire.wav");

	irrklang::ISoundSource* PaintSplat = engine->addSoundSourceFromFile("OST/PaintSplat.wav");


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