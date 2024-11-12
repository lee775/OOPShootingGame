#pragma once

#include <Windows.h>

class MonsterInfo
{
private:
	WCHAR _monsterType;
	//����
	WCHAR _icon;
	//���� ���
	WCHAR _atkIcon;
	//ü��
	int _hp;
	//���ݷ�
	int _atk;
	//�ּ� ���� �ֱ�
	int _atkCoolDown;
	//���� �ֱ� ����ġ
	int _atkWeight;
	//�ּ� �̵��ֱ�
	int _moveCoolDown;
	//�̵�����
	WCHAR _movePattern[100];
	//�Ѿ� �̵� �ֱ�
	int _bulletMoveCoolDown;

	////���� ũ��(�߰��� ����ִ� ���¿��� �簢�� �������� ���� ��) V�� ����� ���� ����3 ����2�� ���� ��� ���ʰ� �ϴ� ������� icon�� ���������� 6ĭ���� ������ (�ǰ������� ����))
	////���α���
	//int _width;
	////���α���
	//int _height;
public:
	MonsterInfo(WCHAR* name);
	WCHAR getMonsterType() const;
	WCHAR getIcon() const;
	WCHAR getMovePattern(int idx) const;
	void resetHp(int* current);
	void resetAtkCoolDown(int* current);
	void resetMoveCoolDown(int* current);
	void monsterAttack(int y, int x, int owner);
	void EndlessInfoSet(int endlessCount);
};