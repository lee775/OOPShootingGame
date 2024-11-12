#include "SceneManager.h"
#include "TitleScene.h"
#include "GameScene.h"
#include "OverScene.h"
#include "ScreenBuffer.h"
#include "GameManager.h"

//������ ������ ���Ŵ����� �����ϰ� ������ ó���� Ÿ��Ʋ ȭ����� ������
SceneManager::SceneManager():_scene(new TitleScene), _isExit(false)
{
}

SceneManager::~SceneManager()
{
	//�� �Ŵ����� ����� �� ���� ��ü�� �����̹Ƿ� ��� �Ŵ��� ��ü���� ������
	GameManager::DestroyInstance();
	ScreenBuffer::DestroyInstance();

}

void SceneManager::run()
{
	if (!this->_isExit)
	{
		//�� ��ü
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
				//�������� ���� �� �÷��̾� ��� or ���� Ŭ���� �ÿ��� �Ѿ� �� �� ����
				//���� ����� �Ŵ����� �Բ� �����Ű��, ������ ������Ҷ� �ٽ� ���������� �������
				GameManager::DestroyInstance();
				break;
		}
	}
}