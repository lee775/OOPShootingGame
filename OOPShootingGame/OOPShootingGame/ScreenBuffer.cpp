#pragma warning(disable: 26495)

#include "ScreenBuffer.h"
#include "SceneManager.h"
#include <iostream>

ScreenBuffer::ScreenBuffer()
{
	this->_console = Console::GetInstance();

	//�ܼ�â�� ũ�⸸ŭ ���۸� ���鿹��
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	GetConsoleScreenBufferInfo(this->_console->GetConsole(), &csbi);

	//�ܼ�â�� ũ�⸦ ������
	int width = csbi.srWindow.Right - csbi.srWindow.Left + 1;
	int height = csbi.srWindow.Bottom - csbi.srWindow.Top;

	this->_width = width;
	this->_height = height;

	this->_screenBuffer = new WCHAR * [height];
	WCHAR* buffer = new WCHAR[height * (width)];
	for (int i = 0; i < height; i++)
	{
		//2���� �迭ó�� ����ϱ� ���� �� ���� ������ġ�� ���α��̸�ŭ �о���
		this->_screenBuffer[i] = buffer + i * (width);
	}
}

ScreenBuffer::~ScreenBuffer()
{
	//0��° �ּҿ� 2�����迭 ó�� ����� 1���� �迭�� �����ּҰ� �������
	delete[] this->_screenBuffer[0];
	//�� ���� ��ŭ�� ���۸� ���� �Ҵ� �޾ұ� ������ ����ó��
	delete[] this->_screenBuffer;

	//�ܼ��� �̱��� ��ü������ ���ۿ� �������̹Ƿ� ���۰� ������� �Բ� �����
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
	//���۸� �����ϰ� Ŭ������
	wmemset(this->_screenBuffer[0], L' ', this->_height * this->_width);

	for (int i = 0; i < this->_height; i++)
	{
		//�ٴ����� ����� �Ұ��̱� ������ �� ���� �ǳ��� null�� ��������
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
