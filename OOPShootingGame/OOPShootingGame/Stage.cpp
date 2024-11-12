#include "Stage.h"

Stage::Stage(int stage) : _stage(stage)
{}

bool Stage::addObject(Monster* obj)
{
	this->_stageMonsterList.push_back(obj);
	return true;
}

int Stage::getStage() const
{
	return this->_stage;
}

void Stage::monsterSet(List<BaseObject*>* list, int* monsterCount)
{
	List<Monster*>::iterator iter;
	//스테이지 객체는 지속해서 재활용 할것이기 때문에 몬스터 객체를 복사해서 리스트에 담아줌
	for (iter = this->_stageMonsterList.begin(); iter != this->_stageMonsterList.end(); ++iter)
	{
		Monster* monster = new Monster(*(*iter));
		list->push_back(monster);
		(*monsterCount)++;
	}
}
