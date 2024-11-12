#pragma once

#include "BaseObject.h"
#include <Windows.h>

class Player : public BaseObject
{
private:	
	//외형
	WCHAR _icon;
	//공격 모션
	WCHAR _atkIcon;
	//체력
	int _hp;
	//공격력
	int _atk;
	//공격쿨타임
	int _atkCoolDown;
	//현재 공격 쿨타임
	int _currentAtkCoolDown;
	//최소 이동주기
	int _moveCoolDown;
	//현재 이동 쿨타임
	int _currentMoveCoolDown;
	//총알 이동 주기
	int _bulletMoveCoolDown;
	//스테이지당 특수 공격 카운트
	int _skillCount;
	//현재 특수 공격 카운트
	int _currentSkillCount;
	//특수 공격 쿨타임
	int _skillCoolDown;
	//특수 공격 현재 쿨타임
	int _currentSkillCoolDown;

	////외형 크기(중간이 비어있는 형태여도 사각형 기준으로 측정 예) V자 모양을 가진 가로3 세로2의 적은 상단 양쪽과 하단 정가운데만 icon이 존재하지만 6칸으로 생각함 (피격판정과 별도))
	////가로길이
	//int _width;
	////세로길이
	//int _height;

	void Attack();
	void Skill();
public:
	Player(WCHAR* data, int objType);
	bool Update();
	void Render();
	void PlayerHit(int damage);
};
