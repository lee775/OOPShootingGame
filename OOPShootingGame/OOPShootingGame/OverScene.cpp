#include "OverScene.h"
#include "SceneManager.h"
#include <Windows.h>
#include "ScreenBuffer.h"
#include "SceneManager.h"

//유지되면서 프레임이 밀리는지 체크
DWORD overSleepTime = -1;

OverScene::OverScene()
{
	overSleepTime = -1;
}

void OverScene::Update()
{
	//로직이 돌때만 시간 체크
	DWORD startTime = timeGetTime();
	this->_over.Update();
	if (FrameSkip(overSleepTime, startTime))
	{
		this->_over.Render();

		ScreenBuffer::GetInstance()->FirstLineClear();
		ScreenBuffer::GetInstance()->Flip();

		//렌더까지 종료 후 휴식할 시간 측정
		overSleepTime = timeGetTime() - startTime;
		//쉬어야 할 프레임이 1프레임이라도 남은경우에만 sleep  
		if (overSleepTime < 20)
			//50프레임으로 맞추기 위한 작업
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
