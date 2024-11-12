#pragma once

#include <Windows.h>
#include "Singleton.h"

class Console : public Singleton<Console>
{
private:
	friend Singleton<Console>;
	Console();
	~Console();

	HANDLE _hConsole;
public:
	HANDLE GetConsole();
	void MoveCursor(int x, int y);
};

bool FrameSkip(DWORD sleepTime, DWORD startTime);