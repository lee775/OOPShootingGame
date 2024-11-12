#include "TitleScene.h"
#include <Windows.h>
#include "ScreenBuffer.h"
#include "SceneManager.h"

//�����Ǹ鼭 �������� �и����� üũ
DWORD titleSleepTime = -1;

TitleScene::TitleScene()
{
	titleSleepTime - 1;
}

void TitleScene::Update()
{
	//������ ������ �ð� üũ
	DWORD startTime = timeGetTime();
	this->_title.Update();
	if (FrameSkip(titleSleepTime, startTime))
	{
		this->_title.Render();

		ScreenBuffer::GetInstance()->FirstLineClear();
		ScreenBuffer::GetInstance()->Flip();

		//�������� ���� �� �޽��� �ð� ����
		titleSleepTime = timeGetTime() - startTime;
		//����� �� �������� 1�������̶� ������쿡�� sleep  
		if (titleSleepTime < 20)
			//50���������� ���߱� ���� �۾�
			Sleep(20 - titleSleepTime);
	}
	startTime += 20;
}

int TitleScene::NextScene()
{
	return this->_title.getNextScene();
}

int TitleScene::getScene()
{
	return TITLE_SCENE;
}
