#pragma warning(disable: 26495)

#include "Player.h"
#include <iostream>
#include "GameManager.h"
#include "ScreenBuffer.h"
#include "Bullet.h"

Player::Player(WCHAR* data, int objType) : _currentSkillCoolDown(0), _currentAtkCoolDown(0), _currentMoveCoolDown(0), BaseObject(objType)
{
	ScreenBuffer* screenBuffer = ScreenBuffer::GetInstance();

	//\n������ ���ڿ��� �߶� ������ ������ ��������
	WCHAR* token;
	WCHAR* nextToken = nullptr;
	token = wcstok_s(data, L"\r\n", &nextToken);

	int statCount = 1;
	while (token != NULL)
	{
		switch (statCount)
		{
		case 1:
			//�÷��̾� ������
			this->_icon = token[0];
			break;
		case 2:
			//�÷��̾� ���� ������
			this->_atkIcon = token[0];
			break;
		case 3:
			//�÷��̾� hp
			this->_hp = _wtoi(token);
			break;
		case 4:
			//�÷��̾� ���ݷ�
			this->_atk = _wtoi(token);
			break;
		case 5:
			//�÷��̾� ���� �ֱ�
			this->_atkCoolDown = _wtoi(token);
			break;
		case 6:
			//�÷��̾� �̵� �ֱ�
			this->_moveCoolDown = _wtoi(token);
			break;
		case 7:
			//�÷��̾� �Ѿ� �̵� �ֱ�
			this->_bulletMoveCoolDown = _wtoi(token);
			break;
		case 8:
			//�÷��̾� Ư�� ���� Ƚ��
			this->_skillCount = _wtoi(token);
			this->_currentSkillCount = _wtoi(token);
			break;
		case 9:
			//�÷��̾� Ư�� ���� ��Ÿ��
			this->_skillCoolDown = _wtoi(token);
			break;
		}
		token = wcstok_s(NULL, L"\r\n", &nextToken);
		statCount++;
	}
	this->_x = screenBuffer->getWidth() / 2 - 1;
	this->_y = screenBuffer->getHeight() - 4;
	screenBuffer->_playerHp = &this->_hp;
	screenBuffer->_playerMaxHp = this->_hp;
}

bool Player::Update()
{
	GameManager* gameManager = GameManager::GetInstance();
	ScreenBuffer* screenBuffer = ScreenBuffer::GetInstance();

	//ESC�������� ��� ��Ȳ�� �͵� ��� �����ϵ��� ����
	if (GetAsyncKeyState(VK_ESCAPE) & 0x8001)
	{
		gameManager->_isEnd = true;
		return true;
	}
	if (gameManager->_stageChange)
	{
		this->_currentSkillCount = this->_skillCount;
		gameManager->_stageChange = false;
	}

	this->_currentAtkCoolDown--;
	this->_currentMoveCoolDown--;
	this->_currentSkillCoolDown--;

	//�÷��̾� �̵�
	if (this->_currentMoveCoolDown <= 0)
	{
		if (GetAsyncKeyState(VK_LEFT) & 0x8001)
		{
			if (this->_x - 1 >= 0)
				this->_x -= 1;
			this->_currentMoveCoolDown = this->_moveCoolDown;
		}
		if (GetAsyncKeyState(VK_UP) & 0x8001)
		{
			if (this->_y - 1 >= 0)
				this->_y -= 1;
			this->_currentMoveCoolDown = this->_moveCoolDown;
		}
		if (GetAsyncKeyState(VK_RIGHT) & 0x8001)
		{
			if (this->_x + 1 <= screenBuffer->getWidth() - 2)
				this->_x += 1;
			this->_currentMoveCoolDown = this->_moveCoolDown;
		}
		if (GetAsyncKeyState(VK_DOWN) & 0x8001)
		{
			if (this->_y + 1 <= screenBuffer->getHeight() - 1)
				this->_y += 1;
			this->_currentMoveCoolDown = this->_moveCoolDown;
		}
	}

	//�÷��̾� ����
	if (this->_currentAtkCoolDown <= 0)
	{
		if (GetAsyncKeyState(L'A') & 0x8001)
		{
			this->Attack();
			this->_currentAtkCoolDown = this->_atkCoolDown;
		}
	}

	//�÷��̾� Ư�� ����
	if (this->_currentSkillCoolDown <= 0)
	{
		if (this->_skillCount > 0)
		{
			if (GetAsyncKeyState(L'S') & 0x8001)
			{
				this->Skill();
				this->_currentSkillCoolDown = this->_skillCoolDown;
				this->_currentSkillCount--;
			}
		}
	}


    return true;
}

void Player::Render()
{
	ScreenBuffer* screenBuffer = ScreenBuffer::GetInstance();
	screenBuffer->_screenBuffer[this->_y][this->_x] = this->_icon;
}

void Player::PlayerHit(int damage)
{
	this->_hp -= damage;
	if (this->_hp <= 0)
		this->_isDeath = true;
}

void Player::Attack()
{
	GameManager* gameManager = GameManager::GetInstance();

	//�Ѿ� ��ü ����
	Bullet* bullet = new Bullet(this->_y - 1, this->_x, this->_objectType, this->_atkIcon, this->_atk, this->_bulletMoveCoolDown, false);
	gameManager->CreateObject(bullet);
}

void Player::Skill()
{
	GameManager* gameManager = GameManager::GetInstance();

	//�Ѿ� ��ü ����
	Bullet* bullet = new Bullet(this->_y - 1, this->_x, this->_objectType, this->_atkIcon, (this->_atk * 3), this->_bulletMoveCoolDown, true);
	gameManager->CreateObject(bullet);
}
