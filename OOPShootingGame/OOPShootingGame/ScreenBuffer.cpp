#pragma warning(disable: 26495)

#include "ScreenBuffer.h"
#include "SceneManager.h"
#include <iostream>

ScreenBuffer::ScreenBuffer()
{
	this->_console = Console::GetInstance();

	//콘솔창의 크기만큼 버퍼를 만들예정
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	GetConsoleScreenBufferInfo(this->_console->GetConsole(), &csbi);

	//콘솔창의 크기를 가져옴
	int width = csbi.srWindow.Right - csbi.srWindow.Left + 1;
	int height = csbi.srWindow.Bottom - csbi.srWindow.Top;

	this->_width = width;
	this->_height = height;

	this->_screenBuffer = new WCHAR * [height];
	WCHAR* buffer = new WCHAR[height * (width)];
	for (int i = 0; i < height; i++)
	{
		//2차원 배열처럼 사용하기 위해 각 행의 시작위치를 가로길이만큼 밀어줌
		this->_screenBuffer[i] = buffer + i * (width);
	}
}

ScreenBuffer::~ScreenBuffer()
{
	//0번째 주소에 2차원배열 처럼 사용할 1차원 배열의 시작주소가 들어있음
	delete[] this->_screenBuffer[0];
	//행 길이 만큼의 버퍼를 동적 할당 받았기 때문에 삭제처리
	delete[] this->_screenBuffer;

	//콘솔은 싱글톤 객체이지만 버퍼에 종속적이므로 버퍼가 사라질때 함께 사라짐
	Console::DestroyInstance();
}

int ScreenBuffer::getHeight() const
{
	return this->_height;
}

int ScreenBuffer::getWidth() const
{
	return this->_width;
}

void ScreenBuffer::ClearBuffer()
{
	//버퍼를 깨끗하게 클리어함
	wmemset(this->_screenBuffer[0], L' ', this->_height * this->_width);

	for (int i = 0; i < this->_height; i++)
	{
		//줄단위로 출력을 할것이기 때문에 각 줄의 맨끝에 null을 삽입해줌
		this->_screenBuffer[i][this->_width - 1] = L'\0';
	}
}

void ScreenBuffer::FilpHp()
{
	this->FirstLineClear();
	this->_console->MoveCursor(0, 0);
	wprintf(L"Hp : ");
	for (int i = 0; i < *(this->_playerHp); i++)
	{
		wprintf(L"#");
	}
}

void ScreenBuffer::FirstLineClear()
{
	this->_console->MoveCursor(0, 0);
	wprintf(L"                                  ");
}

void ScreenBuffer::Flip()
{
	for (int i = 0; i < this->_height; i++)
	{
		this->_console->MoveCursor(0, 1 + i);
		wprintf(L"%s",this->_screenBuffer[i]);
	}
}
