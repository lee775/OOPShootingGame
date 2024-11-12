#pragma once

#include <Windows.h>

class MonsterInfo
{
private:
	WCHAR _monsterType;
	//외형
	WCHAR _icon;
	//공격 모션
	WCHAR _atkIcon;
	//체력
	int _hp;
	//공격력
	int _atk;
	//최소 공격 주기
	int _atkCoolDown;
	//공격 주기 가중치
	int _atkWeight;
	//최소 이동주기
	int _moveCoolDown;
	//이동패턴
	WCHAR _movePattern[100];
	//총알 이동 주기
	int _bulletMoveCoolDown;

	////외형 크기(중간이 비어있는 형태여도 사각형 기준으로 측정 예) V자 모양을 가진 가로3 세로2의 적은 상단 양쪽과 하단 정가운데만 icon이 존재하지만 6칸으로 생각함 (피격판정과 별도))
	////가로길이
	//int _width;
	////세로길이
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