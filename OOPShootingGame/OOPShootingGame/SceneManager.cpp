#include "SceneManager.h"
#include "TitleScene.h"
#include "GameScene.h"
#include "OverScene.h"
#include "ScreenBuffer.h"
#include "GameManager.h"

//시작은 언제니 씬매니저로 시작하고 게임의 처음은 타이틀 화면부터 등장함
SceneManager::SceneManager():_scene(new TitleScene), _isExit(false)
{
}

SceneManager::~SceneManager()
{
	//씬 매니저의 종료는 곧 게임 자체의 종료이므로 모든 매니저 객체또한 삭제함
	GameManager::DestroyInstance();
	ScreenBuffer::DestroyInstance();

}

void SceneManager::run()
{
	if (!this->_isExit)
	{
		//씬 교체
		this->ChangeScene();

		this->_scene->Update();
	}
}

void SceneManager::ChangeScene()
{
 	if (this->_scene->getScene() != this->_scene->NextScene())
	{
		switch (this->_scene->NextScene())
		{
			case TITLE_SCENE:
				delete _scene;
				_scene = new TitleScene;
				break;
			case GAME_SCENE:
				delete _scene;
				_scene = new GameScene;
				break;
			case OVER_SCENE:
				delete _scene;
				_scene = new OverScene;
				//오버씬은 게임 중 플레이어 사망 or 게임 클리어 시에만 넘어 갈 수 있음
				//게임 종료시 매니저도 함께 종료시키며, 게임을 재시작할때 다시 내부적으로 재생성함
				GameManager::DestroyInstance();
				break;
		}
	}
}