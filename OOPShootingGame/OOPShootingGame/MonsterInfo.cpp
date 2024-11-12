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

	//������ �д� ������ ����
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
			//���� ����
			this->_icon = token[0];
			break;
		case 2:
			//���� ���ݸ��
			this->_atkIcon = token[0];
			break;
		case 3:
			//���� hp
			this->_hp = _wtoi(token);
			break;
		case 4:
			//���� ���ݷ�
			this->_atk = _wtoi(token);
			break;
		case 5:
			//���� ���� �ֱ�
			this->_atkCoolDown = _wtoi(token);
			break;
		case 6:
			//���� ���� �ֱ� ����ġ
			this->_atkWeight = _wtoi(token);
			break;
		case 7:
			//���� �̵� �ֱ�
			this->_moveCoolDown = _wtoi(token);
			break;
		case 8:
		{
			//���� �̵� ���� ����
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
			//���� �Ѿ� �̵� �ֱ�
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

	//�Ѿ� ��ü ����
	Bullet* bullet = new Bullet(y, x, owner, this->_atkIcon, this->_atk, this->_bulletMoveCoolDown, false);
	gameManager->CreateObject(bullet);
}

void MonsterInfo::EndlessInfoSet(int endlessCount)
{
	//�ѹ� ��ȸ�Ҷ����� ���� hp 1�� ����
	this->_hp += endlessCount;
	if (this->_atk >= 5)
	{
		//������ ���ݷ��� 5 �̻��϶� ���� ���� ��Ÿ���� 5�� ��ȸ�� 1�����Ӿ� ����(�ּ� 1)
		this->_atkCoolDown -= endlessCount / 5;
		if (this->_atkCoolDown < 1) this->_atkCoolDown = 1;
	}
	else {
		//�ι� ��ȸ �Ҷ����� ���ݷ� 1�� ����
		this->_atk += endlessCount / 2;
	}
}
