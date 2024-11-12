#pragma once

#include "BaseScene.h"
#include "Singleton.h"

#define TITLE_SCENE 0x1
#define GAME_SCENE 0x2
#define OVER_SCENE 0x3

class SceneManager : public Singleton<SceneManager>
{
private:
	BaseScene* _scene;
	void ChangeScene();
public:
	bool _isExit;
	SceneManager();
	virtual ~SceneManager();
	void run();
};