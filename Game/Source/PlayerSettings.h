#pragma once
class PlayerSettings
{
private:
	static PlayerSettings* mInstance;

	PlayerSettings();
	~PlayerSettings();

public:

	static PlayerSettings* Instance();

	static void Release();

	void Reset();

public:

	int playerLifes;

	int playerScore;

	bool isLevelCompleted[2];

	bool isInit = false;
};