#pragma once

#include "BaseObject.h"
#include <Windows.h>

class Player : public BaseObject
{
private:	
	//����
	WCHAR _icon;
	//���� ���
	WCHAR _atkIcon;
	//ü��
	int _hp;
	//���ݷ�
	int _atk;
	//������Ÿ��
	int _atkCoolDown;
	//���� ���� ��Ÿ��
	int _currentAtkCoolDown;
	//�ּ� �̵��ֱ�
	int _moveCoolDown;
	//���� �̵� ��Ÿ��
	int _currentMoveCoolDown;
	//�Ѿ� �̵� �ֱ�
	int _bulletMoveCoolDown;
	//���������� Ư�� ���� ī��Ʈ
	int _skillCount;
	//���� Ư�� ���� ī��Ʈ
	int _currentSkillCount;
	//Ư�� ���� ��Ÿ��
	int _skillCoolDown;
	//Ư�� ���� ���� ��Ÿ��
	int _currentSkillCoolDown;

	////���� ũ��(�߰��� ����ִ� ���¿��� �簢�� �������� ���� ��) V�� ����� ���� ����3 ����2�� ���� ��� ���ʰ� �ϴ� ������� icon�� ���������� 6ĭ���� ������ (�ǰ������� ����))
	////���α���
	//int _width;
	////���α���
	//int _height;

	void Attack();
	void Skill();
public:
	Player(WCHAR* data, int objType);
	bool Update();
	void Render();
	void PlayerHit(int damage);
};
