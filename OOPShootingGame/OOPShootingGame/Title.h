#pragma once
#include "Selector.h"

class Title
{
private:
	//�޴� ���� ��ü
	Selector* _selector;
	//��¹��ۿ� ������ ũ�⸦ ���� Ÿ��Ʋ ����
	WCHAR** _titleScreenBuffer;
	int _nextScene;
	//����ȭ�� -> Ÿ��Ʋȭ������ �Ѿ��� �����̽��� �ν��� ������ �� �� �־ ó���� ���� ������ �߰�
	int _firstSelectFrame;
public:
	Title();
	~Title();
	void Update();
	void Render();
	int getNextScene();
};