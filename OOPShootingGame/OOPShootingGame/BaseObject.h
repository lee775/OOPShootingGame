#pragma once

#include "Position.h"
#include "MyList.h"

class BaseObject
{
public:
	virtual bool Update() = 0;
	virtual void Render() = 0;

	int getObjectType() const;
	//중심부 위치만 포지션으로 반환
	Position getPosition() const;
	bool isDeath() const;
	//크기가 1x1이 아닌 객체의 모든 위치를 가진 리스트
	List<Position*> _pos;
	virtual ~BaseObject();
protected:
	BaseObject(int objType, int y, int x);
	BaseObject(int y, int x);
	BaseObject(int objType);
	//타입(몬스터, 플레이어, 총알 모두 define된 값으로 판단함)
	int _objectType;
	//중심부 위치 크기가 1x1이 아닌경우 중심부를 기준으로 그려질것임
	int _x;
	int _y;
	//삭제 여부
	bool _isDeath;
};
