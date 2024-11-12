#include "Console.h"

Console::Console()
{
	//�ܼ� �ڵ� ������
	this->_hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
}

Console::~Console()
{
	CloseHandle(this->_hConsole);
}

HANDLE Console::GetConsole()
{
	return this->_hConsole;
}

void Console::MoveCursor(int x, int y)
{
	COORD stCoord;
	stCoord.X = x;
	stCoord.Y = y;

	SetConsoleCursorPosition(_hConsole, stCoord);
}

//50������ ���� ��������(20ms)�̻� �з������� ��ŵ �������� ����
bool FrameSkip(DWORD sleepTime, DWORD startTime)
{
	//-1�̸� ù ��������
	if (sleepTime == -1)
		sleepTime = timeGetTime() - startTime;
	//50������ ���� �������ӿ� �ɸ��� �ð� 20ms�� 2�� �̻��̸� ������ �ѹ� �ǳʶ۰���
	if (sleepTime >= 40)
	{
		return false;
	}
	else {
		return true;
	}
}