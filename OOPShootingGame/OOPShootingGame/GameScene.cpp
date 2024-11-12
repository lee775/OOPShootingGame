#include "GameScene.h"
#include <Windows.h>
#include "ScreenBuffer.h"
#include "GameManager.h"
#include "SceneManager.h"
 
//�����Ǹ鼭 �������� �и����� üũ
DWORD gameSleepTime = -1;

GameScene::GameScene()
{
	gameSleepTime = -1;
}

void GameScene::Update()
{
	GameManager::GetInstance()->Initialize();
	//DWORD firstTime = startTime;
	//DWORD logic = 0;
	//DWORD render = 0;
	
	//������ ������ �ð� üũ
	DWORD startTime = timeGetTime();
	GameManager::GetInstance()->Update();
	//logic++;
	if (!GameManager::GetInstance()->_isEnd)
	{
		if (FrameSkip(gameSleepTime, startTime))
		{
			GameManager::GetInstance()->Render();

			ScreenBuffer::GetInstance()->FilpHp();
			ScreenBuffer::GetInstance()->Flip();
			//render++;

			//�������� ���� �� �޽��� �ð� ����
			gameSleepTime = timeGetTime() - startTime;
			//����� �� �������� 1�������̶� ������쿡�� sleep  
			if (gameSleepTime < 20)
				//50���������� ���߱� ���� �۾�
				Sleep(20 - gameSleepTime);
		}
		startTime += 20;
		//if (firstTime + 1000 <= timeGetTime())
		//{
		//	wprintf(L"\nLogic : %d / Render : %d \n", logic, render);
		//	logic = 0;
		//	render = 0;
		//	firstTime += 1000;
		//}
	}
}

int GameScene::NextScene()
{
	if (!GameManager::GetInstance()->_isEnd)
		return GAME_SCENE;
	else
	{
		return OVER_SCENE;
	}
}

int GameScene::getScene()
{
	return GAME_SCENE;
}
