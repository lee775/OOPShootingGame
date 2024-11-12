#include "Over.h"
#include "SceneManager.h"
#include "ScreenBuffer.h"
#include "GameManager.h"
#include <iostream>

Over::Over() : _nextScene(OVER_SCENE), _selector(new Selector(2))
{
	//selector�� ������ �⺻���� Ÿ��Ʋ ȭ���� �̸� �׷��� �� ���� screenBuffer�� ��������
	int width = ScreenBuffer::GetInstance()->getWidth();
	int height = ScreenBuffer::GetInstance()->getHeight();

	this->_overScreenBuffer = new WCHAR * [height];
	WCHAR* buffer = new WCHAR[height * (width)];
	for (int i = 0; i < height; i++)
	{
		this->_overScreenBuffer[i] = buffer + i * (width);
	}
	//���۸� �����ϰ� Ŭ������
	wmemset(this->_overScreenBuffer[0], L' ', height * width);

	for (int i = 0; i < height; i++)
	{
		//�ٴ����� ����� �Ұ��̱� ������ �� ���� �ǳ��� null�� ��������
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

	//�߾����� �б����� ������
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
	//0��° �ּҿ� 2�����迭 ó�� ����� 1���� �迭�� �����ּҰ� �������
	delete[] this->_overScreenBuffer[0];
	//�� ���� ��ŭ�� ���۸� ���� �Ҵ� �޾ұ� ������ ����ó��
	delete[] this->_overScreenBuffer;

	delete this->_selector;
}

void Over::Update()
{
	//���� ���� �� ���۸� Ŭ��������
	ScreenBuffer::GetInstance()->ClearBuffer();

	this->_selector->_currentFrame--;
	//������ ����
	if (this->_selector->_currentFrame <= 0)
	{
		this->_selector->_currentFrame = this->_selector->_iconChangeFrame;
		this->_selector->_currentIcon = this->_selector->_currentIcon == this->_selector->_icon1 ? this->_selector->_icon2 : this->_selector->_icon1;
	}

	this->_selector->_currentMoveFrame--;
	//�޴� ���� �̵�����
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