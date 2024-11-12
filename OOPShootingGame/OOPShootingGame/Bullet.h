#pragma once

#include <Windows.h>
#include "BaseObject.h"
#include "MyList.h"

class Bullet : public BaseObject
{
private:
	//�Ѿ��� �߻��� ��ü
	int _owner;
	//�Ѿ� ������
	WCHAR _icon;
	//�Ѿ� ������
	int _damage;
	//�Ѿ� �̵� ������
	int _bulletMoveCoolDown;
	//�Ѿ� �̵� ���� Ȯ��
	int _currentMoveCoolDown;
	//���� �Ѿ� ��ǥ - ����ĭ�� �����ϸ� �ش� ��� ��ǥ�� ��ϵǾ�����
	//List<Position*> _pos;
public:
	Bullet(int y, int x, int owner, WCHAR icon, int damage, int coolDown, bool isSkill);
	bool Update();
	void Render();
	bool SkillCollsionCheck(BaseObject* obj);
	int getOwner();
	void BulletErase();
	int getDamage();
};