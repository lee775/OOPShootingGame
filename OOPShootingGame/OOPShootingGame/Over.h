#pragma once
#include "Selector.h"

class Over
{
private:
	//메뉴 선택 객체
	Selector* _selector;
	//출력버퍼와 동일한 크기를 가진 타이틀 버퍼
	WCHAR** _overScreenBuffer;
	int _nextScene;
public:
	Over();
	~Over();
	void Update();
	void Render();
	int getNextScene();
};