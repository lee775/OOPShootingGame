#pragma once
#include "Selector.h"

class Over
{
private:
	//�޴� ���� ��ü
	Selector* _selector;
	//��¹��ۿ� ������ ũ�⸦ ���� Ÿ��Ʋ ����
	WCHAR** _overScreenBuffer;
	int _nextScene;
public:
	Over();
	~Over();
	void Update();
	void Render();
	int getNextScene();
};