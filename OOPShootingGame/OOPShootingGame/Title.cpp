#include "Title.h"
#include "ScreenBuffer.h"
#include "SceneManager.h"
#include "GameManager.h"
#include <iostream>

Title::Title() : _nextScene(TITLE_SCENE), _selector(new Selector(3)), _firstSelectFrame(5)
{
	//selector�� ������ �⺻���� Ÿ��Ʋ ȭ���� �̸� �׷��� �� ���� screenBuffer�� ��������
	int width = ScreenBuffer::GetInstance()->getWidth();
	int height = ScreenBuffer::GetInstance()->getHeight();

	this->_titleScreenBuffer = new WCHAR * [height];
	WCHAR* buffer = new WCHAR[height * (width)];
	for (int i = 0; i < height; i++)
	{
		this->_titleScreenBuffer[i] = buffer + i * (width);
	}
	//���۸� �����ϰ� Ŭ������
	wmemset(this->_titleScreenBuffer[0], L' ', height * width);

	for (int i = 0; i < height; i++)
	{
		//�ٴ����� ����� �Ұ��̱� ������ �� ���� �ǳ��� null�� ��������
		this->_titleScreenBuffer[i][width - 1] = L'\0';
	}

	int heightOffset = 10;
	const WCHAR* title[] = {
		L"                                        ",
		L"                                        ",
		L"                                        ",
		L" #####  #     #  #####   #####  ####### ",
		L"#     # #     # #     # #     #    #    ",
		L"#       #     # #     # #     #    #    ",
		L" #####  ####### #     # #     #    #    ",
		L"      # #     # #     # #     #    #    ",
		L"#     # #     # #     # #     #    #    ",
		L" #####  #     #  #####   #####     #    "
	};
	//�߾����� �б����� ������
	int offset = (width - 40) / 2;

	for (int i = 0; i < heightOffset; i++)
	{
		size_t titleLength = wcslen(title[i]);
		//���ڿ� ����
		wcsncpy_s(this->_titleScreenBuffer[i] + offset, width - offset, title[i], titleLength);
		// ���� �κ��� �������� ä��
		wmemset(this->_titleScreenBuffer[i] + offset + titleLength, L' ', width - offset - titleLength);
	}


	heightOffset++;
	offset = (width - 31) / 2;
	const WCHAR* selectGuide = L"Press the SPACE key to select.";

	size_t guideLength = wcslen(selectGuide);
	wcsncpy_s(this->_titleScreenBuffer[heightOffset] + offset, width - offset, selectGuide, guideLength);
	wmemset(this->_titleScreenBuffer[heightOffset] + offset + guideLength, L' ', width - offset - guideLength);

	heightOffset++;
	const WCHAR* selectMenu[] = {
		L" Start Game ",
		L" Endless Mode ",
		L" Exit Game "
	};

	offset = (width - 15) / 2;
	heightOffset += 2;
	for (int i = 0; i < 3; i++)
	{
		size_t menuLength = wcslen(selectMenu[i]);
		wcsncpy_s(this->_titleScreenBuffer[heightOffset + i] + offset, width - offset, selectMenu[i], menuLength);
		wmemset(this->_titleScreenBuffer[heightOffset + i] + offset + menuLength, L' ', width - offset - menuLength);

		if (i == 0)
		{
			this->_selector->_pos._x = offset - 2;
			this->_selector->_pos._y = heightOffset;
		}
	}
}

Title::~Title()
{
	//0��° �ּҿ� 2�����迭 ó�� ����� 1���� �迭�� �����ּҰ� �������
	delete[] this->_titleScreenBuffer[0];
	//�� ���� ��ŭ�� ���۸� ���� �Ҵ� �޾ұ� ������ ����ó��
	delete[] this->_titleScreenBuffer;

	delete this->_selector;
}

void Title::Update()
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
			if (this->_selector->_selectorY + 1 <= 3)
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

	_firstSelectFrame--;
	if (_firstSelectFrame <= 0)
	{
		if (GetAsyncKeyState(VK_SPACE) & 0x8001)
		{
			switch (this->_selector->_selectorY)
			{
				case 3:
					this->_nextScene = GAME_SCENE;
					GameManager::GetInstance()->_isEndless = false;
					break;
				case 2:
					this->_nextScene = GAME_SCENE;
					GameManager::GetInstance()->_isEndless = true;
					break;
				case 1:
					SceneManager::GetInstance()->_isExit = true;
					break;
			}
		}
	}
}

void Title::Render()
{
	ScreenBuffer* screenBuffer = ScreenBuffer::GetInstance();
	
	for (int i = 0; i < screenBuffer->getHeight(); i++) {
		memcpy(screenBuffer->_screenBuffer[i], this->_titleScreenBuffer[i], screenBuffer->getWidth() * sizeof(WCHAR));
	}
	screenBuffer->_screenBuffer[this->_selector->_pos._y][this->_selector->_pos._x] = this->_selector->_currentIcon;
}

int Title::getNextScene()
{
	return this->_nextScene;
}
