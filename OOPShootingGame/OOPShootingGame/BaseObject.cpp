#pragma warning(disable: 26495)

#include "BaseObject.h"

int BaseObject::getObjectType() const
{
    return this->_objectType;
}

Position BaseObject::getPosition() const
{
    Position pos = { this->_x, this->_y };
    return pos;
}

bool BaseObject::isDeath() const
{
    return this->_isDeath;
}

BaseObject::~BaseObject()
{
    List<Position*>::iterator iter;
    for (iter = this->_pos.begin(); iter != this->_pos.end(); ++iter)
    {
        delete* iter;
    }
    this->_pos.clear();
}

BaseObject::BaseObject(int objType, int y, int x) : _objectType(objType), _y(y), _x(x), _isDeath(false)
{
}

BaseObject::BaseObject(int y, int x) : _y(y), _x(x), _isDeath(false)
{
}

BaseObject::BaseObject(int objType) : _objectType(objType), _isDeath(false)
{
}
