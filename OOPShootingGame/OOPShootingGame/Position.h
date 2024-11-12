#pragma once

class Position
{
private:
public:
	int _x;
	int _y;
	Position();
	Position(int x, int y);
	bool operator==(const Position& other) const;
};