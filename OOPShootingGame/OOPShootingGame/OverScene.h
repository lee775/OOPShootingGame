#pragma once

#include "BaseScene.h"
#include "Over.h"

class OverScene : public BaseScene
{
private:
	Over _over;
public:
	OverScene();
	void Update();
	int NextScene();
	int getScene();
};