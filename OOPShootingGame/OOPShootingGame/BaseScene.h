#pragma once

class BaseScene
{
public:
	virtual void Update() = 0;
	virtual int NextScene() = 0;
	virtual int getScene() = 0;
};