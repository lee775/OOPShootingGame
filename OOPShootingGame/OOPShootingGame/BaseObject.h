#pragma once

#include "Position.h"
#include "MyList.h"

class BaseObject
{
public:
	virtual bool Update() = 0;
	virtual void Render() = 0;

	int getObjectType() const;
	//�߽ɺ� ��ġ�� ���������� ��ȯ
	Position getPosition() const;
	bool isDeath() const;
	//ũ�Ⱑ 1x1�� �ƴ� ��ü�� ��� ��ġ�� ���� ����Ʈ
	List<Position*> _pos;
	virtual ~BaseObject();
protected:
	BaseObject(int objType, int y, int x);
	BaseObject(int y, int x);
	BaseObject(int objType);
	//Ÿ��(����, �÷��̾�, �Ѿ� ��� define�� ������ �Ǵ���)
	int _objectType;
	//�߽ɺ� ��ġ ũ�Ⱑ 1x1�� �ƴѰ�� �߽ɺθ� �������� �׷�������
	int _x;
	int _y;
	//���� ����
	bool _isDeath;
};
