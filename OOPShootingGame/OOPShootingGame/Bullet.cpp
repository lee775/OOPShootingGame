#include "HeaderSet.h"
#include "ScreenBuffer.h"
#include "Bullet.h"
#include "Monster.h"

Bullet::Bullet(int y, int x, int owner, WCHAR icon, int damage, int coolDown, bool isSkill) : _owner(owner), _icon(icon), _damage(damage), _bulletMoveCoolDown(coolDown), _currentMoveCoolDown(coolDown), BaseObject(y, x)
{
    if (isSkill)
        this->_objectType = SKILL_BULLET_OBJECT;
    else if(owner == BOSS_OBJECT)
        this->_objectType = SKILL_BULLET_OBJECT;
    else
        this->_objectType = BULLET_OBJECT;
}

bool Bullet::Update()
{
    this->_currentMoveCoolDown--;
    if (this->_currentMoveCoolDown == 0)
    {
        ScreenBuffer* screenBuffer = ScreenBuffer::GetInstance();

        if (!(this->_pos.empty()))
        {
            //기존에 저장된 포지션 전부 삭제
            List<Position*>::iterator iter;
            for (iter = this->_pos.begin(); iter != this->_pos.end();)
            {
                delete* iter;
                iter = this->_pos.erase(iter);
            }
        }

        //몬스터는 항상 밑으로 플레이어는 위로만 발사 될것임
        switch (this->_owner)
        {
            case PLAYER_OBJECT:
            {
                if (this->_objectType != SKILL_BULLET_OBJECT)
                {
                    //일반공격
                    this->_y--;
                    if (this->_y < 0)
                        this->_isDeath = true;
                    Position* pos = new Position{ this->_x, this->_y };
                    this->_pos.push_back(pos);
                }
                else
                {
                    //특수공격
                    this->_y--;
                    if (this->_y < 0)
                        this->_isDeath = true;

                    int offsets[5][2] = { {0, 0}, {0, 1}, {0, -1}, {1, -2}, {1, 2} };
                    for (const auto& offset : offsets) {
                        Position* pos = new Position{ this->_x + offset[1], this->_y + offset[0] };
                        this->_pos.push_back(pos);
                    }
                }
                break;
            }
            case MONSTER_OBJECT:
            {
                this->_y++;
                if (this->_y > screenBuffer->getHeight() - 1)
                    this->_isDeath = true;
                Position* pos = new Position{ this->_x, this->_y };
                this->_pos.push_back(pos);
                break;
            }
            case BOSS_OBJECT:
            {
                this->_y++;
                if (this->_y > screenBuffer->getHeight() - 1)
                    this->_isDeath = true;

                int offsets[4][2] = {{0, 0}, {0, 1}, {-1, 2}, {-1, -1}};
                for (const auto& offset : offsets) {
                    Position* pos = new Position{ this->_x + offset[1], this->_y + offset[0] };
                    this->_pos.push_back(pos);
                }
                break;
            }
        }

        this->_currentMoveCoolDown = this->_bulletMoveCoolDown;
    }
    return true;
}

void Bullet::Render()
{
    if (!(this->_isDeath))
    {
        ScreenBuffer* screenBuffer = ScreenBuffer::GetInstance();
        
        List<Position*>::iterator iter;
        for (iter = this->_pos.begin(); iter != this->_pos.end(); ++iter)
        {
            screenBuffer->_screenBuffer[(*iter)->_y][(*iter)->_x] = this->_icon;
        }
    }
}

bool Bullet::SkillCollsionCheck(BaseObject* obj)
{
    bool result = false;
    if (obj->getObjectType() != MONSTER_OBJECT && obj->getObjectType() != BOSS_OBJECT)
    {
        List<Position*>::iterator iter;
        for (iter = this->_pos.begin(); iter != this->_pos.end(); ++iter)
        {
            if (**iter == obj->getPosition())
            {
                result = true;
                break;
            }
        }
    }
    else if(this->_owner == PLAYER_OBJECT)
    {
        List<Position*>::iterator iter1;
        List<Position*>::iterator iter2;
        Monster* monster = dynamic_cast<Monster*>(obj);
        bool isBreak = false;
        for (iter1 = this->_pos.begin(); iter1 != this->_pos.end(); ++iter1)
        {
            for (iter2 = monster->_pos.begin(); iter2 != monster->_pos.end(); ++iter2)
            {
                if ((*iter1)->_x == (*iter2)->_x && (*iter1)->_y == (*iter2)->_y)
                {
                    result = true;
                    isBreak = true;
                    break;
                }
            }
            if (isBreak)
                break;
        }
    }
    return result;
}

int Bullet::getOwner()
{
    return this->_owner;
}

void Bullet::BulletErase()
{
    this->_isDeath = true;
}

int Bullet::getDamage()
{
    return this->_damage;
}
