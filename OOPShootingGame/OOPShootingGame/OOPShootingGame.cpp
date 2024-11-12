#include "HeaderSet.h"
#include "SceneManager.h"
#include <iostream>

int wmain()
{
	srand(time(NULL));
	timeBeginPeriod(1);

	while (!SceneManager::GetInstance()->_isExit)
	{
		SceneManager::GetInstance()->run();
	}
	SceneManager::DestroyInstance();
}