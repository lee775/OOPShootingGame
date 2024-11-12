#include "GameManager.h"
#include "HeaderSet.h"
#include "ScreenBuffer.h"
#include "Bullet.h"
#include "Player.h"
#include <iostream>

GameManager::GameManager() : _isEnd(false), _currentStage(1), _isClear(false), _monsterCount(0), _stageChange(false), _isInitialize(false), _endlessCount(0)
{
	//읽어온 몬스터들의 이름이 담길배열 초기화
	wmemset(this->_readMonsterList, L' ', 100);
	this->_readMonsterList[0] = L'\0';
}

GameManager::~GameManager()
{
	//동적할당 된 모든 파일 정리
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
		//스테이지 정보 및 몬스터 정보를 읽어 옴
		CreateObject(L"stageInfo.txt", MONSTER_OBJECT);

		//플레이어 정보를 읽어옴
		CreateObject(L"player.txt", PLAYER_OBJECT);

		//초반 스테이지 세팅
		stageObjectSet();
		_isInitialize = true;
	}
}

bool GameManager::CreateObject(BaseObject* newObject)
{
	if (!newObject)
		return false;
	//BaseObject자체가 들어오는 경우는 총알 밖에 없음
	//삭제도 가장 먼저 일어나야 하기때문에 앞쪽에 삽입
	this->_objectList.push_front(newObject);
	return true;
}

bool GameManager::CreateObject(const WCHAR* fileName, int type)
{
	//콘솔 크기정보를 가져오기 위해 ScreenBuffer를 불러옴
	ScreenBuffer* screenBuffer = ScreenBuffer::GetInstance();

	//파일을 읽는 과정은 같음
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

	//BOM문자가 읽히기 때문에 지워주는 작업이 필요함
	WCHAR* buffer = new WCHAR[size];
	wcscpy_s(buffer, size, temp + 1);

	if (type == PLAYER_OBJECT)
	{
		Player* player = new Player(buffer, type);
		//게임 종료시까지 유지될 객체이기 때문에 뒤쪽에 삽입
		this->_objectList.push_back(player);
	}
	else if (type == MONSTER_OBJECT)
	{
		WCHAR* stageToken;
		WCHAR* nextStageToken = nullptr;
		stageToken = wcstok_s(buffer, L"\r\n", &nextStageToken);
		int stageCount = _wtoi(stageToken);

		//몬스터info 객체를 먼저 만들어야함
		for (int stageLv = 0; stageLv < stageCount; stageLv++)
		{
			//스테이지 파일 가져옴
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

			//읽어온 스테이지 파일의 내용으로 몬스터 정보를 읽어올 것임
			WCHAR* monsterToken;
			WCHAR* nextMonsterToken = nullptr;
			monsterToken = wcstok_s(stageBuffer, L"\r\n", &nextMonsterToken);
			//stageMonsterCount가 해당 스테이지에서 나오는 몬스터 종류임
			int stageMonsterCount = _wtoi(monsterToken);

			monsterToken = wcstok_s(NULL, L"\r\n", &nextMonsterToken);
			for (int j = 0; j < stageMonsterCount; j++)
			{
				//이미 읽어온 몬스터면 다시 읽어올 필요가 없음
				if (wcschr(this->_readMonsterList, monsterToken[j]) == NULL)
				{
					//몬스터의 이름은 WCHAR 1글자로 가정
					WCHAR catTempArr[2] = { monsterToken[j], L'\0' };
					wcscat_s(this->_readMonsterList, 100, catTempArr);

					MonsterInfo* monsterInfo = new MonsterInfo(catTempArr);
					this->_monsterInfoList.push_back(monsterInfo);
				}
			}

			monsterToken = wcstok_s(NULL, L"\r\n", &nextMonsterToken);

			Stage* stage = new Stage(stageLv + 1);
			//몬스터의 y축 2칸의 여백을 미리 두고 시작
			int y = 2;
			while (monsterToken != NULL)
			{
				int len = wcslen(monsterToken);
				//몬스터의 x축 화면 중앙에 오도록 하기위해 설정
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
		//fasle가 리턴되면 더이상 읽어올 스테이지가 없는것이므로 게임종료 / 무한모드의 경우 1스테이지 부터 다시 시작
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
		//스테이지가 종료되면 남은 객체는 총알객체와 플레이어 객체임 총알객체를 지워야함
		this->BulletErase();
	}

	//로직 시작 전 버퍼를 클리어해줌
	ScreenBuffer::GetInstance()->ClearBuffer();

	//각 오브젝트의 로직을 돌려줌
	List<BaseObject*>::iterator iter;
	for (iter = this->_objectList.begin(); iter != this->_objectList.end(); ++iter)
	{
		(*iter)->Update();
	}

	//충돌 처리
	CollisionCheck();

	//삭제 처리
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
	//총알은 List의 앞쪽에 몰려있고, 플레이어-몬스터간 충돌처리는 없기때문에 총알일때만 충돌검사
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
			//모든 총알객체는 앞쪽에 몰려있기때문에 obj2는 몬스터, 플레이어 객체로 확정지어도 됨
			//총알-플레이어 총알-몬스터 총알-총알 끼리의 충돌만 체크 (몬스터-플레이어의 충돌은 없음)
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
					//플레이어가 쓴 특수기술은 피격된 몬스터의 총알또한 삭제함
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
