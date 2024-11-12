#include "Monster.h"
#include "HeaderSet.h"
#include "ScreenBuffer.h"
#include "GameManager.h"

Monster::Monster(int y, int x, WCHAR monsterName, int objType) : _currentMovePattern(0), _currentAtkCoolDown(0), _currentMoveCoolDown(0), BaseObject(objType, y, x)
{
    MonsterInfo* monsterInfo = GameManager::GetInstance()->getMonsterInfo(monsterName);
    this->_monsterInfo = monsterInfo;

    this->_monsterInfo->resetHp(&this->_currentHp);
    //this->_monsterInfo->resetAtkCoolDown(&this->_currentAtkCoolDown);
    //this->_monsterInfo->resetMoveCoolDown(&this->_currentMoveCoolDown);
}

bool Monster::Update()
{
    ScreenBuffer* screenBuffer = ScreenBuffer::GetInstance();

    this->_currentMoveCoolDown--;
    this->_currentAtkCoolDown--;

    //몬스터 이동 로직
    if (this->_currentMoveCoolDown <= 0)
    {
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

        this->_monsterInfo->resetMoveCoolDown(&this->_currentMoveCoolDown);

        WCHAR move = this->_monsterInfo->getMovePattern(this->_currentMovePattern++);
        if (move == L'\0')
        {
            this->_currentMovePattern = 0;
            move = this->_monsterInfo->getMovePattern(this->_currentMovePattern++);
        }

        switch (move)
        {
        case L'L':
            if(this->_x - 1 > 0)
                this->_x--;
            break;
        case L'R':
            if (!(this->_x + 1 > screenBuffer->getWidth() - 2))
                this->_x++;
            break;
        case L'U':
            if (this->_y - 1 > 0)
                this->_y--;
            break;
        case L'D':
            if (!(this->_y + 1 > screenBuffer->getHeight() - 1))
                this->_y++;
            break;
        }

        //크기에 맞게 충돌처리를 모두 해야하기때문에 크기값 전부 저장
        if (this->_monsterInfo->getMonsterType() == L'a')
        {
            Position* pos = new Position(this->_x, this->_y);
            this->_pos.push_back(pos);
        }
        else if (this->_monsterInfo->getMonsterType() == L'b')
        {
            for (int i = this->_x - 1; i <= this->_x + 1; i++)
            {
                Position* pos = new Position(i, this->_y);
                this->_pos.push_back(pos);
            }
        }
        else if (this->_monsterInfo->getMonsterType() == L'c')
        {
            for (int i = this->_x - 1; i <= this->_x + 1; i += 2)
            {
                Position* pos1 = new Position(i, this->_y - 1);
                this->_pos.push_back(pos1);
            }
            Position* pos = new Position(this->_x, this->_y);
            this->_pos.push_back(pos);
        }
        else if (this->_monsterInfo->getMonsterType() == L'k')
        {
            Position* pos = new Position(this->_x, this->_y);
            this->_pos.push_back(pos);
            for (int i = 0; i < 7; i++)
            {
                Position* pos1 = new Position((this->_x - (7 / 2)) + i, this->_y - 1);
                Position* pos2 = new Position((this->_x - (7 / 2)) + i, this->_y + 1);
                this->_pos.push_back(pos1);
                this->_pos.push_back(pos2);
            }

            for (int i = 0; i < 9; i++)
            {
                Position* pos3 = new Position((this->_x - (9 / 2)) + i, this->_y);
                this->_pos.push_back(pos3);
            }
        }
    }
    //몬스터 공격 로직
    if (this->_currentAtkCoolDown <= 0)
    {
        this->_monsterInfo->resetAtkCoolDown(&this->_currentAtkCoolDown);

        if (this->_monsterInfo->getMonsterType() == L'k')
            this->_monsterInfo->monsterAttack(this->_y + 1, this->_x, BOSS_OBJECT);
        else
            this->_monsterInfo->monsterAttack(this->_y + 1, this->_x, MONSTER_OBJECT);
    }
    return true;
}

void Monster::Render()
{
    if (!(this->_isDeath))
    {
        ScreenBuffer* screenBuffer = ScreenBuffer::GetInstance();
        List<Position*>::iterator iter;
        for (iter = this->_pos.begin(); iter != this->_pos.end(); ++iter)
        {
            int a = (*iter)->_y;
            int b = (*iter)->_x;
            screenBuffer->_screenBuffer[(*iter)->_y][(*iter)->_x] = this->_monsterInfo->getIcon();
        }
    }
}

void Monster::MonsterHit(int damage)
{
    this->_currentHp -= damage;
    if (this->_currentHp <= 0)
        this->_isDeath = true;
}

//List<Position*>::iterator Monster::getBegin()
//{
//    return this->_pos.begin();
//}
//
//List<Position*>::iterator Monster::getEnd()
//{
//    return this->_pos.end();
//}