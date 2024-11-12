#pragma warning(disable: 26495)

#include "MonsterInfo.h"
#include "GameManager.h"
#include "Bullet.h"
#include <iostream>

MonsterInfo::MonsterInfo(WCHAR* name) : _monsterType(name[0])
{
	WCHAR monsterFileName[10] = L"";
	wcscat_s(monsterFileName, name);
	wcscat_s(monsterFileName, L".txt");

	//파일을 읽는 과정은 같음
	HANDLE hHandle = CreateFileW(monsterFileName, GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	WCHAR temp[1024];
	DWORD bytesRead;
	bool isSuccess = ReadFile(hHandle, temp, sizeof(temp) - sizeof(WCHAR), &bytesRead, NULL);

	int size = bytesRead / sizeof(WCHAR);
	temp[size] = L'\0';
	WCHAR* buffer = new WCHAR[size];
	wcscpy_s(buffer, size, temp + 1);

	WCHAR* token;
	WCHAR* nextToken = nullptr;
	token = wcstok_s(buffer, L"\r\n", &nextToken);
	int switchCount = 1;
	while (token != NULL)
	{
		switch (switchCount)
		{
		case 1:
			//몬스터 외형
			this->_icon = token[0];
			break;
		case 2:
			//몬스터 공격모션
			this->_atkIcon = token[0];
			break;
		case 3:
			//몬스터 hp
			this->_hp = _wtoi(token);
			break;
		case 4:
			//몬스터 공격력
			this->_atk = _wtoi(token);
			break;
		case 5:
			//몬스터 공격 주기
			this->_atkCoolDown = _wtoi(token);
			break;
		case 6:
			//몬스터 공격 주기 가중치
			this->_atkWeight = _wtoi(token);
			break;
		case 7:
			//몬스터 이동 주기
			this->_moveCoolDown = _wtoi(token);
			break;
		case 8:
		{
			//몬스터 이동 패턴 삽입
			HANDLE hMove = CreateFileW(token, GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
			WCHAR movePtn[1024];
			DWORD moveBytesRead;
			bool isSuccess = ReadFile(hMove, movePtn, sizeof(movePtn) - sizeof(WCHAR), &moveBytesRead, NULL);

			int size = moveBytesRead / sizeof(WCHAR);
			movePtn[size] = L'\0';

			wcscpy_s(this->_movePattern, size, movePtn + 1);
			CloseHandle(hMove);
			break;
		}
		case 9:
			//몬스터 총알 이동 주기
			this->_bulletMoveCoolDown = _wtoi(token);
			break;
		}
		token = wcstok_s(NULL, L"\r\n", &nextToken);
		switchCount++;
	}
	delete[] buffer;
	CloseHandle(hHandle);
}

WCHAR MonsterInfo::getMonsterType() const
{
	return this->_monsterType;
}

WCHAR MonsterInfo::getIcon() const
{
	return this->_icon;
}

WCHAR MonsterInfo::getMovePattern(int idx) const
{
	return this->_movePattern[idx];
}

void MonsterInfo::resetHp(int* current)
{
	*current = this->_hp;
}

void MonsterInfo::resetAtkCoolDown(int* current)
{
	*current = (rand() % this->_atkWeight) + this->_atkCoolDown;
}

void MonsterInfo::resetMoveCoolDown(int* current)
{
	*current = this->_moveCoolDown;
}

void MonsterInfo::monsterAttack(int y, int x, int owner)
{
	GameManager* gameManager = GameManager::GetInstance();

	//총알 객체 생성
	Bullet* bullet = new Bullet(y, x, owner, this->_atkIcon, this->_atk, this->_bulletMoveCoolDown, false);
	gameManager->CreateObject(bullet);
}

void MonsterInfo::EndlessInfoSet(int endlessCount)
{
	//한번 순회할때마다 몬스터 hp 1씩 증가
	this->_hp += endlessCount;
	if (this->_atk >= 5)
	{
		//몬스터의 공격력이 5 이상일때 부턴 공격 쿨타임이 5번 순회당 1프레임씩 감소(최소 1)
		this->_atkCoolDown -= endlessCount / 5;
		if (this->_atkCoolDown < 1) this->_atkCoolDown = 1;
	}
	else {
		//두번 순회 할때마다 공격력 1씩 증가
		this->_atk += endlessCount / 2;
	}
}
