#pragma once

#include "BaseScene.h"

class GameScene : public BaseScene
{
private:
public:
	GameScene();
	void Update();
	int NextScene();
	int getScene();
};