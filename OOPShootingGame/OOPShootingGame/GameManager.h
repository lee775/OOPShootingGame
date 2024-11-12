#pragma once

#include "BaseObject.h"
#include "Singleton.h"
#include "MonsterInfo.h"
#include "Stage.h"
#include "MyList.h"

class GameManager : public Singleton<GameManager>
{
private:
	friend Singleton<GameManager>;
	//�÷��̾�, ����, �Ѿ� ��ü�� ��� ����Ʈ
	List<BaseObject*> _objectList;
	//������ ������ ���� ������ ��� ����Ʈ
	List<MonsterInfo*> _monsterInfoList;
	//�������� ������ ��� ����Ʈ
	List<Stage*> _stageMonsterList;
	//�̹� �о�� ������ �̸��� ��� ����Ʈ
	WCHAR _readMonsterList[100];
	//���� ��������
	int _currentStage;
	//���������� ���� ����
	int _monsterCount;
	//���������� ��ü�Ҷ� ����ϴ� �Լ�
	bool stageObjectSet();
	//�浹üũ
	void CollisionCheck();
	//������ ���� ��ü�� �����ϴ� �Լ�
	void Erase();
	//���������� ��ü�Ǹ� �Ѿ˰� �÷��̾� ��ü�� ���� �Ǵµ� ���⼭ �Ѿ� ��ü�� �����ִ� �Լ�
	void BulletErase();

	GameManager();
	~GameManager();
	//�ʱ� ���ÿ���
	bool _isInitialize;
	//���Ѹ��� ��� ���������� �ѹ��� ��ȸ�ϴ� �����, ��ȸ Ƚ��
	int _endlessCount;
	//���� ��� ���̵� ����
	void EndlessMonsterInfoSet();
public:
	//�ʱ� ����
	void Initialize();
	//��ü�� ���� ����Ʈ�� ������ ����ϴ� �Լ�
	bool CreateObject(BaseObject* newObject);
	//�о�� ������ ���� ��ü�� ���鶧 ����ϴ� �Լ�
	bool CreateObject(const WCHAR* fileName, int type);
	//���� ��ü�� ��� ���������� �������� �Լ�
	MonsterInfo* getMonsterInfo(WCHAR monsterType);

	bool Update();
	void Render();

	//���� ���� Ű
	bool _isEnd;
	//���� ����� ���� Ŭ���� ����
	bool _isClear;
	//���������� ��ü�Ǿ����� Ȯ���ϴ� ����
	bool _stageChange;
	//���Ѹ�� ����ġ
	bool _isEndless;
};