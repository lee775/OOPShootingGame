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
	//플레이어, 몬스터, 총알 객체가 담길 리스트
	List<BaseObject*> _objectList;
	//몬스터의 종류별 스텟 정보가 담긴 리스트
	List<MonsterInfo*> _monsterInfoList;
	//스테이지 정보가 담긴 리스트
	List<Stage*> _stageMonsterList;
	//이미 읽어온 몬스터의 이름이 담긴 리스트
	WCHAR _readMonsterList[100];
	//현재 스테이지
	int _currentStage;
	//스테이지별 남은 몬스터
	int _monsterCount;
	//스테이지를 교체할때 사용하는 함수
	bool stageObjectSet();
	//충돌체크
	void CollisionCheck();
	//수명이 끝난 객체를 삭제하는 함수
	void Erase();
	//스테이지가 교체되면 총알과 플레이어 객체만 남게 되는데 여기서 총알 객체만 지워주는 함수
	void BulletErase();

	GameManager();
	~GameManager();
	//초기 세팅여부
	bool _isInitialize;
	//무한모드는 모든 스테이지를 한번씩 순회하는 방식임, 순회 횟수
	int _endlessCount;
	//무한 모드 난이도 조절
	void EndlessMonsterInfoSet();
public:
	//초기 세팅
	void Initialize();
	//객체를 직접 리스트에 담을때 사용하는 함수
	bool CreateObject(BaseObject* newObject);
	//읽어온 파일을 토대로 객체를 만들때 사용하는 함수
	bool CreateObject(const WCHAR* fileName, int type);
	//몬스터 객체에 담길 스텟정보를 가져오는 함수
	MonsterInfo* getMonsterInfo(WCHAR monsterType);

	bool Update();
	void Render();

	//게임 종료 키
	bool _isEnd;
	//게임 종료시 게임 클리어 여부
	bool _isClear;
	//스테이지가 교체되었는지 확인하는 변수
	bool _stageChange;
	//무한모드 스위치
	bool _isEndless;
};