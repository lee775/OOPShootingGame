#include "GameScene.h"
#include <Windows.h>
#include "ScreenBuffer.h"
#include "GameManager.h"
#include "SceneManager.h"
 
//유지되면서 프레임이 밀리는지 체크
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
	
	//로직이 돌때만 시간 체크
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

			//렌더까지 종료 후 휴식할 시간 측정
			gameSleepTime = timeGetTime() - startTime;
			//쉬어야 할 프레임이 1프레임이라도 남은경우에만 sleep  
			if (gameSleepTime < 20)
				//50프레임으로 맞추기 위한 작업
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
