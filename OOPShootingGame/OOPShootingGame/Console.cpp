#include "Console.h"

Console::Console()
{
	//콘솔 핸들 가져옴
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

//50프레임 기준 한프레임(20ms)이상 밀렸을때만 스킵 그전까진 유지
bool FrameSkip(DWORD sleepTime, DWORD startTime)
{
	//-1이면 첫 프레임임
	if (sleepTime == -1)
		sleepTime = timeGetTime() - startTime;
	//50프레임 기준 한프레임에 걸리는 시간 20ms의 2배 이상이면 렌더를 한번 건너뛸것임
	if (sleepTime >= 40)
	{
		return false;
	}
	else {
		return true;
	}
}