#include "TitleScene.h"
#include <Windows.h>
#include "ScreenBuffer.h"
#include "SceneManager.h"

//유지되면서 프레임이 밀리는지 체크
DWORD titleSleepTime = -1;

TitleScene::TitleScene()
{
	titleSleepTime - 1;
}

void TitleScene::Update()
{
	//로직이 돌때만 시간 체크
	DWORD startTime = timeGetTime();
	this->_title.Update();
	if (FrameSkip(titleSleepTime, startTime))
	{
		this->_title.Render();

		ScreenBuffer::GetInstance()->FirstLineClear();
		ScreenBuffer::GetInstance()->Flip();

		//렌더까지 종료 후 휴식할 시간 측정
		titleSleepTime = timeGetTime() - startTime;
		//쉬어야 할 프레임이 1프레임이라도 남은경우에만 sleep  
		if (titleSleepTime < 20)
			//50프레임으로 맞추기 위한 작업
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
