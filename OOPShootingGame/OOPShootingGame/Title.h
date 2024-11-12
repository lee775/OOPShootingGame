#pragma once
#include "Selector.h"

class Title
{
private:
	//메뉴 선택 객체
	Selector* _selector;
	//출력버퍼와 동일한 크기를 가진 타이틀 버퍼
	WCHAR** _titleScreenBuffer;
	int _nextScene;
	//오버화면 -> 타이틀화면으로 넘어갈경우 스페이스바 인식이 여러번 될 수 있어서 처음에 한해 딜레이 추가
	int _firstSelectFrame;
public:
	Title();
	~Title();
	void Update();
	void Render();
	int getNextScene();
};