#pragma warning(disable: 26495)
#include "Position.h"

Position::Position()
{
}

Position::Position(int x, int y) : _x(x), _y(y)
{
}

bool Position::operator==(const Position& other) const
{
    return this->_x == other._x && this->_y == other._y;
}
