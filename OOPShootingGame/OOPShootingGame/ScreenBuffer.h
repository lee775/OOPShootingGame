#pragma once

#include "Console.h"
#include "Singleton.h"

class ScreenBuffer : public Singleton<ScreenBuffer>
{
private:
	friend Singleton<ScreenBuffer>;
	ScreenBuffer();
	~ScreenBuffer();

	int _height;
	int _width;
	Console* _console;
public:
	int* _playerHp;
	int _playerMaxHp;
	WCHAR** _screenBuffer;
	int getHeight() const;
	int getWidth() const;
	void ClearBuffer();
	void FilpHp();
	void FirstLineClear();
	void Flip();
};