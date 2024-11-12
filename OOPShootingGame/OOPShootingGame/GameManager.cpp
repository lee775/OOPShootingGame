#include "GameManager.h"
#include "HeaderSet.h"
#include "ScreenBuffer.h"
#include "Bullet.h"
#include "Player.h"
#include <iostream>

GameManager::GameManager() : _isEnd(false), _currentStage(1), _isClear(false), _monsterCount(0), _stageChange(false), _isInitialize(false), _endlessCount(0)
{
	//�о�� ���͵��� �̸��� ���迭 �ʱ�ȭ
	wmemset(this->_readMonsterList, L' ', 100);
	this->_readMonsterList[0] = L'\0';
}

GameManager::~GameManager()
{
	//�����Ҵ� �� ��� ���� ����
	List<BaseObject*>::iterator baseIter;
	for (baseIter = this->_objectList.begin(); baseIter != this->_objectList.end(); ++baseIter)
	{
		delete* baseIter;
	}
	this->_objectList.clear();

	List<MonsterInfo*>::iterator monsterInfoIter;
	for (monsterInfoIter = this->_monsterInfoList.begin(); monsterInfoIter != this->_monsterInfoList.end(); ++monsterInfoIter)
	{
		delete* monsterInfoIter;
	}
	this->_monsterInfoList.clear();

	List<Stage*>::iterator stageIter;
	for (stageIter = this->_stageMonsterList.begin(); stageIter != this->_stageMonsterList.end(); ++stageIter)
	{
		delete* stageIter;
	}
	this->_stageMonsterList.clear();
}

void GameManager::EndlessMonsterInfoSet()
{
	List<MonsterInfo*>::iterator iter;
	for (iter = this->_monsterInfoList.begin(); iter != this->_monsterInfoList.end(); iter++)
	{
		(*iter)->EndlessInfoSet(this->_endlessCount);
	}
}

void GameManager::Initialize()
{
	if (!_isInitialize)
	{
		//�������� ���� �� ���� ������ �о� ��
		CreateObject(L"stageInfo.txt", MONSTER_OBJECT);

		//�÷��̾� ������ �о��
		CreateObject(L"player.txt", PLAYER_OBJECT);

		//�ʹ� �������� ����
		stageObjectSet();
		_isInitialize = true;
	}
}

bool GameManager::CreateObject(BaseObject* newObject)
{
	if (!newObject)
		return false;
	//BaseObject��ü�� ������ ���� �Ѿ� �ۿ� ����
	//������ ���� ���� �Ͼ�� �ϱ⶧���� ���ʿ� ����
	this->_objectList.push_front(newObject);
	return true;
}

bool GameManager::CreateObject(const WCHAR* fileName, int type)
{
	//�ܼ� ũ�������� �������� ���� ScreenBuffer�� �ҷ���
	ScreenBuffer* screenBuffer = ScreenBuffer::GetInstance();

	//������ �д� ������ ����
	HANDLE hHandle = CreateFileW(fileName, GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	WCHAR temp[1024];
	DWORD bytesRead;
	if (!ReadFile(hHandle, temp, sizeof(temp) - sizeof(WCHAR), &bytesRead, NULL))
	{
		CloseHandle(hHandle);
		return false;
	}
	int size = bytesRead / sizeof(WCHAR);
	temp[size] = L'\0';

	//BOM���ڰ� ������ ������ �����ִ� �۾��� �ʿ���
	WCHAR* buffer = new WCHAR[size];
	wcscpy_s(buffer, size, temp + 1);

	if (type == PLAYER_OBJECT)
	{
		Player* player = new Player(buffer, type);
		//���� ����ñ��� ������ ��ü�̱� ������ ���ʿ� ����
		this->_objectList.push_back(player);
	}
	else if (type == MONSTER_OBJECT)
	{
		WCHAR* stageToken;
		WCHAR* nextStageToken = nullptr;
		stageToken = wcstok_s(buffer, L"\r\n", &nextStageToken);
		int stageCount = _wtoi(stageToken);

		//����info ��ü�� ���� ��������
		for (int stageLv = 0; stageLv < stageCount; stageLv++)
		{
			//�������� ���� ������
			stageToken = wcstok_s(NULL, L"\r\n", &nextStageToken);

			HANDLE hStage = CreateFileW(stageToken, GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
			WCHAR stageTemp[1024];
			DWORD stageBytesRead;
			if (!ReadFile(hStage, stageTemp, sizeof(stageTemp) - sizeof(WCHAR), &stageBytesRead, NULL))
				return false;

			int stageSize = stageBytesRead / sizeof(WCHAR);
			stageTemp[stageSize] = L'\0';
			WCHAR* stageBuffer = new WCHAR[stageSize + 1];
			wcscpy_s(stageBuffer, stageSize + 1, stageTemp + 1);

			//�о�� �������� ������ �������� ���� ������ �о�� ����
			WCHAR* monsterToken;
			WCHAR* nextMonsterToken = nullptr;
			monsterToken = wcstok_s(stageBuffer, L"\r\n", &nextMonsterToken);
			//stageMonsterCount�� �ش� ������������ ������ ���� ������
			int stageMonsterCount = _wtoi(monsterToken);

			monsterToken = wcstok_s(NULL, L"\r\n", &nextMonsterToken);
			for (int j = 0; j < stageMonsterCount; j++)
			{
				//�̹� �о�� ���͸� �ٽ� �о�� �ʿ䰡 ����
				if (wcschr(this->_readMonsterList, monsterToken[j]) == NULL)
				{
					//������ �̸��� WCHAR 1���ڷ� ����
					WCHAR catTempArr[2] = { monsterToken[j], L'\0' };
					wcscat_s(this->_readMonsterList, 100, catTempArr);

					MonsterInfo* monsterInfo = new MonsterInfo(catTempArr);
					this->_monsterInfoList.push_back(monsterInfo);
				}
			}

			monsterToken = wcstok_s(NULL, L"\r\n", &nextMonsterToken);

			Stage* stage = new Stage(stageLv + 1);
			//������ y�� 2ĭ�� ������ �̸� �ΰ� ����
			int y = 2;
			while (monsterToken != NULL)
			{
				int len = wcslen(monsterToken);
				//������ x�� ȭ�� �߾ӿ� ������ �ϱ����� ����
				int x = (screenBuffer->getWidth() - len) / 2;
				for (int i = 0; i < len; i++)
				{
					if (monsterToken[i] != L' ')
					{
						Monster* monster = nullptr;
						if(monsterToken[i] != L'k')
							monster = new Monster(y, x + i, monsterToken[i], type);
						else
							monster = new Monster(y, x + i, monsterToken[i], BOSS_OBJECT);
						stage->addObject(monster);
					}
				}
				monsterToken = wcstok_s(NULL, L"\r\n", &nextMonsterToken);
				y++;
			}
			
			this->_stageMonsterList.push_back(stage);
			delete[] stageBuffer;
			CloseHandle(hStage);
		}

	}
	delete[] buffer;
	CloseHandle(hHandle);
	return true;
}

MonsterInfo* GameManager::getMonsterInfo(WCHAR monsterType)
{
	List<MonsterInfo*>::iterator iter;
	MonsterInfo* result = nullptr;
	for (iter = this->_monsterInfoList.begin(); iter != this->_monsterInfoList.end(); ++iter)
	{
		if ((*iter)->getMonsterType() == monsterType)
		{
			result = *iter;
			break;
		}
	}
	return result;
}

bool GameManager::stageObjectSet()
{
	bool result = false;
	List<Stage*>::iterator iter;
	for (iter = this->_stageMonsterList.begin(); iter != this->_stageMonsterList.end(); ++iter)
	{
		if ((*iter)->getStage() == this->_currentStage)
		{
			(*iter)->monsterSet(&this->_objectList, &this->_monsterCount);
			result = true;
			this->_isClear = false;
			this->_stageChange = true;
			break;
		}
	}
	return result;
}

bool GameManager::Update()
{
	if (this->_isClear)
	{
		this->_currentStage++;
		bool result = stageObjectSet();
		//fasle�� ���ϵǸ� ���̻� �о�� ���������� ���°��̹Ƿ� �������� / ���Ѹ���� ��� 1�������� ���� �ٽ� ����
		if (!result)
		{
			if (!(this->_isEndless))
			{
				this->_isEnd = true;
				return true;
			}
			else
			{
				this->_currentStage = 1;
				_endlessCount++;
				EndlessMonsterInfoSet();
				stageObjectSet();
			}
		}
		//���������� ����Ǹ� ���� ��ü�� �Ѿ˰�ü�� �÷��̾� ��ü�� �Ѿ˰�ü�� ��������
		this->BulletErase();
	}

	//���� ���� �� ���۸� Ŭ��������
	ScreenBuffer::GetInstance()->ClearBuffer();

	//�� ������Ʈ�� ������ ������
	List<BaseObject*>::iterator iter;
	for (iter = this->_objectList.begin(); iter != this->_objectList.end(); ++iter)
	{
		(*iter)->Update();
	}

	//�浹 ó��
	CollisionCheck();

	//���� ó��
	Erase();

	return true;
}

void GameManager::Render()
{
	List<BaseObject*>::iterator iter;
	for (iter = this->_objectList.begin(); iter != this->_objectList.end(); ++iter)
	{
		(*iter)->Render();
	}
}

bool isCollision(BaseObject* obj1, BaseObject* obj2)
{
	//�Ѿ��� List�� ���ʿ� �����ְ�, �÷��̾�-���Ͱ� �浹ó���� ���⶧���� �Ѿ��϶��� �浹�˻�
	if (obj1->getObjectType() == SKILL_BULLET_OBJECT || obj1->getObjectType() == BULLET_OBJECT)
	{
		return dynamic_cast<Bullet*>(obj1)->SkillCollsionCheck(obj2);
	}
	return false;
}

void GameManager::CollisionCheck()
{
	List<BaseObject*>::iterator iter1;
	List<BaseObject*>::iterator iter2;
	for (iter1 = this->_objectList.begin(); iter1 != this->_objectList.end(); ++iter1)
	{
		BaseObject* obj1 = *iter1;
		for (iter2 = this->_objectList.next(iter1); iter2 != this->_objectList.end(); ++iter2)
		{
			BaseObject* obj2 = *iter2;
			//��� �Ѿ˰�ü�� ���ʿ� �����ֱ⶧���� obj2�� ����, �÷��̾� ��ü�� Ȯ����� ��
			//�Ѿ�-�÷��̾� �Ѿ�-���� �Ѿ�-�Ѿ� ������ �浹�� üũ (����-�÷��̾��� �浹�� ����)
			if (isCollision(obj1, obj2))
			{
				bool obj1Bullet = obj1->getObjectType() == BULLET_OBJECT || obj1->getObjectType() == SKILL_BULLET_OBJECT;
				bool obj2Bullet = obj2->getObjectType() == BULLET_OBJECT || obj2->getObjectType() == SKILL_BULLET_OBJECT;
				if (obj1Bullet && obj2->getObjectType() == PLAYER_OBJECT)
				{
					Bullet* bullet = dynamic_cast<Bullet*>(obj1);
					if (bullet->getOwner() == MONSTER_OBJECT || bullet->getOwner() == BOSS_OBJECT)
					{
						dynamic_cast<Player*>(obj2)->PlayerHit(bullet->getDamage());
						bullet->BulletErase();
					}
				}
				else if (obj1Bullet && (obj2->getObjectType() == MONSTER_OBJECT || obj2->getObjectType() == BOSS_OBJECT))
				{
					Bullet* bullet = dynamic_cast<Bullet*>(obj1);
					if (bullet->getOwner() == PLAYER_OBJECT)
					{
						dynamic_cast<Monster*>(obj2)->MonsterHit(bullet->getDamage());
						bullet->BulletErase();
					}
				}
				else if (obj1Bullet && obj2Bullet)
				{
					//�÷��̾ �� Ư������� �ǰݵ� ������ �Ѿ˶��� ������
					Bullet* bullet1 = dynamic_cast<Bullet*>(obj1);
					Bullet* bullet2 = dynamic_cast<Bullet*>(obj2);
					if (bullet1->getObjectType() == SKILL_BULLET_OBJECT && bullet1->getOwner() == PLAYER_OBJECT)
					{
						if (bullet2->getOwner() != PLAYER_OBJECT)
						{
							bullet2->BulletErase();
						}
					}
					if (bullet2->getObjectType() == SKILL_BULLET_OBJECT && bullet2->getOwner() == PLAYER_OBJECT)
					{
						if (bullet1->getOwner() != PLAYER_OBJECT)
						{
							bullet1->BulletErase();
						}
					}
				}
			}
		}
	}
}

void GameManager::Erase()
{
	List<BaseObject*>::iterator iter;
	for (iter = this->_objectList.begin(); iter != this->_objectList.end();)
	{
		if ((*iter)->isDeath())
		{
			if ((*iter)->getObjectType() == PLAYER_OBJECT)
			{
				this->_isEnd = true;
				break;
			}
			else if ((*iter)->getObjectType() == MONSTER_OBJECT || (*iter)->getObjectType() == BOSS_OBJECT)
			{
				this->_monsterCount--;
				if (this->_monsterCount == 0)
					this->_isClear = true;
			}
			delete* iter;
			iter = this->_objectList.erase(iter);
		}
		else
			++iter;
	}
}

void GameManager::BulletErase()
{
	List<BaseObject*>::iterator iter;
	for (iter = this->_objectList.begin(); iter != this->_objectList.end();)
	{
			if ((*iter)->getObjectType() == SKILL_BULLET_OBJECT || (*iter)->getObjectType() == BULLET_OBJECT)
			{
				delete* iter;
				iter = this->_objectList.erase(iter);
			}
			else
				++iter;
	}
}
