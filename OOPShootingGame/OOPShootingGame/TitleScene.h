#pragma once

#include "BaseScene.h"
#include "Title.h"

class TitleScene : public BaseScene
{
private:
	Title _title;
public:
	TitleScene();
	void Update();
	int NextScene();
	int getScene();
};