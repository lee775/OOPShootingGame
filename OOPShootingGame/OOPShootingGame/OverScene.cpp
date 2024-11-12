#include "OverScene.h"
#include "SceneManager.h"
#include <Windows.h>
#include "ScreenBuffer.h"
#include "SceneManager.h"

//�����Ǹ鼭 �������� �и����� üũ
DWORD overSleepTime = -1;

OverScene::OverScene()
{
	overSleepTime = -1;
}

void OverScene::Update()
{
	//������ ������ �ð� üũ
	DWORD startTime = timeGetTime();
	this->_over.Update();
	if (FrameSkip(overSleepTime, startTime))
	{
		this->_over.Render();

		ScreenBuffer::GetInstance()->FirstLineClear();
		ScreenBuffer::GetInstance()->Flip();

		//�������� ���� �� �޽��� �ð� ����
		overSleepTime = timeGetTime() - startTime;
		//����� �� �������� 1�������̶� ������쿡�� sleep  
		if (overSleepTime < 20)
			//50���������� ���߱� ���� �۾�
			Sleep(20 - overSleepTime);
	}
	startTime += 20;
}

int OverScene::NextScene()
{
	return this->_over.getNextScene();
}

int OverScene::getScene()
{
    return OVER_SCENE;
}
