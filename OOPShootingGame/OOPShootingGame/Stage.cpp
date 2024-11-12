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
	//�������� ��ü�� �����ؼ� ��Ȱ�� �Ұ��̱� ������ ���� ��ü�� �����ؼ� ����Ʈ�� �����
	for (iter = this->_stageMonsterList.begin(); iter != this->_stageMonsterList.end(); ++iter)
	{
		Monster* monster = new Monster(*(*iter));
		list->push_back(monster);
		(*monsterCount)++;
	}
}
