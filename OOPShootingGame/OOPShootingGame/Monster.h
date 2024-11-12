#pragma once

#include "BaseObject.h"
#include "MonsterInfo.h"
#include "MyList.h"
#include "Position.h"

class Monster : public BaseObject
{
private:
	int _currentHp;
	int _currentAtkCoolDown;
	int _currentMoveCoolDown;
	int _currentMovePattern;
	MonsterInfo* _monsterInfo;
public:
	Monster(int y, int x, WCHAR monsterName, int objType);
	bool Update();
	void Render();
	void MonsterHit(int damage);
	//List<Position*>::iterator getBegin();
	//List<Position*>::iterator getEnd();
};
