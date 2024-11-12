#pragma once

#include <Windows.h>
#include "Position.h"

class Selector
{
private:
public:
	//메뉴 선택 화살표 위치
	Position _pos;
	//화살표 깜빡거림 표현을 위한 아이콘
	const WCHAR _icon1;
	const WCHAR _icon2;
	//현재 아이콘
	WCHAR _currentIcon;
	//깜빡거리는 주기
	const int _iconChangeFrame;
	int _currentFrame;
	//화살표 이동 주기
	const int _seletorMoveFrame;
	int _currentMoveFrame;
	//선택된 메뉴가 뭔지 알기 위해서는 현재 셀렉터 위치를 명확하게 알아야함
	int _selectorY;

	Selector(int defaultY);

};