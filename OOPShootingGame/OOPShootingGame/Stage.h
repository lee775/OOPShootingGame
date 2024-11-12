#pragma once

#include "Monster.h"
#include "MyList.h"

class Stage
{
private:
	int _stage;
	List<Monster*> _stageMonsterList;
public:
	Stage(int stage);
	bool addObject(Monster* obj);
	int getStage() const;
	void monsterSet(List<BaseObject*>* list, int* monsterCount);
	
};