#include "Over.h"
#include "SceneManager.h"
#include "ScreenBuffer.h"
#include "GameManager.h"
#include <iostream>

Over::Over() : _nextScene(OVER_SCENE), _selector(new Selector(2))
{
	//selector를 제외한 기본적인 타이틀 화면을 미리 그려둔 후 실제 screenBuffer에 덮어씌울것임
	int width = ScreenBuffer::GetInstance()->getWidth();
	int height = ScreenBuffer::GetInstance()->getHeight();

	this->_overScreenBuffer = new WCHAR * [height];
	WCHAR* buffer = new WCHAR[height * (width)];
	for (int i = 0; i < height; i++)
	{
		this->_overScreenBuffer[i] = buffer + i * (width);
	}
	//버퍼를 깨끗하게 클리어함
	wmemset(this->_overScreenBuffer[0], L' ', height * width);

	for (int i = 0; i < height; i++)
	{
		//줄단위로 출력을 할것이기 때문에 각 줄의 맨끝에 null을 삽입해줌
		this->_overScreenBuffer[i][width - 1] = L'\0';
	}

	int heightOffset = 18;
	int addOffset;
	const WCHAR* clearText[] = {
			L"                                        ",
			L"                                        ",
			L"                                        ",
			L"     #####     #    #     # #######     ",
			L"    #     #   # #   ##    # #           ",
			L"    #       #     # # # # # #           ",
			L"    #  #### #     # #  #  # #######     ",
			L"    #     # ####### #     # #           ",
			L"    #     # #     # #     # #           ",
			L"     #####  #     # #     # #######     ",
			L"                                        ",
			L" #####  #       #######    #    ######  ",
			L"#     # #       #         # #   #     # ",
			L"#       #       #       #     # #     # ",
			L"#       #       ####### #     # ######  ",
			L"#       #       #       ####### #   #   ",
			L"#     # #       #       #     # #    #  ",
			L" #####  ####### ####### #     # #     # "
	};

	const WCHAR* overText[] = {
		L"                                ",
		L"                                ",
		L"                                ",
		L" #####     #    #     # ####### ",
		L"#     #   # #   ##    # #       ",
		L"#       #     # # # # # #       ",
		L"#  #### #     # #  #  # ####### ",
		L"#     # ####### #     # #       ",
		L"#     # #     # #     # #       ",
		L" #####  #     # #     # ####### ",
		L"                                ",
		L" #####  #     # ####### ######  ",
		L"#     # #     # #       #     # ",
		L"#     # #     # #       #     # ",
		L"#     # #     # ####### ######  ",
		L"#     #  #   #  #       #   #   ",
		L"#     #   # #   #       #    #  ",
		L" #####     #    ####### #     # "
	};

	const WCHAR** text;
	if (GameManager::GetInstance()->_isClear)
	{
		text = clearText;
		addOffset = 40;
	}
	else
	{
		text = overText;
		addOffset = 32;
	}

	//중앙으로 밀기위한 오프셋
	int offset = (width - addOffset) / 2;

	for (int i = 0; i < heightOffset; i++)
	{
		size_t length = wcslen(text[i]);
		wcsncpy_s(this->_overScreenBuffer[i] + offset, width - offset, text[i], length);
		wmemset(this->_overScreenBuffer[i] + offset + length, L' ', width - offset - length);
	}


	heightOffset++;
	offset = (width - 31) / 2;
	const WCHAR* selectGuide = L"Press the SPACE key to select.";
	size_t guideLength = wcslen(selectGuide);
	wcsncpy_s(this->_overScreenBuffer[heightOffset] + offset, width - offset, selectGuide, guideLength);
	wmemset(this->_overScreenBuffer[heightOffset] + offset + guideLength, L' ', width - offset - guideLength);

	heightOffset++;
	const WCHAR* selectMenu[] = {
		L" Title Menu ",
		L" Exit Game "
	};

	offset = (width - 15) / 2;
	heightOffset += 2;
	for (int i = 0; i < 2; i++)
	{
		size_t menuLength = wcslen(selectMenu[i]);
		wcsncpy_s(this->_overScreenBuffer[heightOffset + i] + offset, width - offset, selectMenu[i], menuLength);
		wmemset(this->_overScreenBuffer[heightOffset + i] + offset + menuLength, L' ', width - offset - menuLength);
		if (i == 0)
		{
			this->_selector->_pos._x = offset - 2;
			this->_selector->_pos._y = heightOffset;
		}
	}
}

Over::~Over()
{
	//0번째 주소에 2차원배열 처럼 사용할 1차원 배열의 시작주소가 들어있음
	delete[] this->_overScreenBuffer[0];
	//행 길이 만큼의 버퍼를 동적 할당 받았기 때문에 삭제처리
	delete[] this->_overScreenBuffer;

	delete this->_selector;
}

void Over::Update()
{
	//로직 시작 전 버퍼를 클리어해줌
	ScreenBuffer::GetInstance()->ClearBuffer();

	this->_selector->_currentFrame--;
	//아이콘 변경
	if (this->_selector->_currentFrame <= 0)
	{
		this->_selector->_currentFrame = this->_selector->_iconChangeFrame;
		this->_selector->_currentIcon = this->_selector->_currentIcon == this->_selector->_icon1 ? this->_selector->_icon2 : this->_selector->_icon1;
	}

	this->_selector->_currentMoveFrame--;
	//메뉴 선택 이동로직
	if (this->_selector->_currentMoveFrame <= 0)
	{
		if (GetAsyncKeyState(VK_UP) & 0x8001)
		{
			if (this->_selector->_selectorY + 1 <= 2)
			{
				this->_selector->_pos._y -= 1;
				this->_selector->_selectorY++;
				this->_selector->_currentMoveFrame = this->_selector->_seletorMoveFrame;
			}
		}
		if (GetAsyncKeyState(VK_DOWN) & 0x8001)
		{
			if (this->_selector->_selectorY - 1 >= 1)
			{
				this->_selector->_pos._y += 1;
				this->_selector->_selectorY--;
				this->_selector->_currentMoveFrame = this->_selector->_seletorMoveFrame;
			}
		}
	}

	if (GetAsyncKeyState(VK_SPACE) & 0x8001)
	{
		switch (this->_selector->_selectorY)
		{
		case 2:
			this->_nextScene = TITLE_SCENE;
			break;
		case 1:
			SceneManager::GetInstance()->_isExit = true;
			break;
		}
	}
}

void Over::Render()
{
	ScreenBuffer* screenBuffer = ScreenBuffer::GetInstance();
	for (int i = 0; i < screenBuffer->getHeight(); i++) {
		memcpy(screenBuffer->_screenBuffer[i], this->_overScreenBuffer[i], screenBuffer->getWidth() * sizeof(WCHAR));
	}
	screenBuffer->_screenBuffer[this->_selector->_pos._y][this->_selector->_pos._x] = this->_selector->_currentIcon;
}

int Over::getNextScene()
{
	return this->_nextScene;
}