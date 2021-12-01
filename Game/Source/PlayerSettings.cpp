#include "PlayerSettings.h"

PlayerSettings* PlayerSettings::mInstance = nullptr;

PlayerSettings::PlayerSettings()
{

}

PlayerSettings::~PlayerSettings()
{
}

PlayerSettings* PlayerSettings::Instance()
{
	if (mInstance == nullptr)
	{
		mInstance = new PlayerSettings();
	}
	return mInstance;
}

void PlayerSettings::Release()
{
	if (mInstance != nullptr)
	{
		delete mInstance;
		mInstance = nullptr;
	}
}

void PlayerSettings::Reset()
{
	playerLifes = 3;
	playerScore = 0;
}
