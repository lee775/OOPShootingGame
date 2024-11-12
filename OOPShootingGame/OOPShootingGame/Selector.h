#pragma once

#include <Windows.h>
#include "Position.h"

class Selector
{
private:
public:
	//�޴� ���� ȭ��ǥ ��ġ
	Position _pos;
	//ȭ��ǥ �����Ÿ� ǥ���� ���� ������
	const WCHAR _icon1;
	const WCHAR _icon2;
	//���� ������
	WCHAR _currentIcon;
	//�����Ÿ��� �ֱ�
	const int _iconChangeFrame;
	int _currentFrame;
	//ȭ��ǥ �̵� �ֱ�
	const int _seletorMoveFrame;
	int _currentMoveFrame;
	//���õ� �޴��� ���� �˱� ���ؼ��� ���� ������ ��ġ�� ��Ȯ�ϰ� �˾ƾ���
	int _selectorY;

	Selector(int defaultY);

};