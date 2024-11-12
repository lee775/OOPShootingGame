#pragma once

#include <Windows.h>
#include "BaseObject.h"
#include "MyList.h"

class Bullet : public BaseObject
{
private:
	//총알을 발사한 객체
	int _owner;
	//총알 아이콘
	WCHAR _icon;
	//총알 데미지
	int _damage;
	//총알 이동 프레임
	int _bulletMoveCoolDown;
	//총알 이동 여부 확인
	int _currentMoveCoolDown;
	//현재 총알 좌표 - 여러칸을 차지하면 해당 모든 좌표가 기록되어있음
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