#ifndef _CELLSPAWNMANAGER_H_
#define _CELLSPAWNMANAGER_H_

#include "gameLogic.h"

class CellSpawnerMgr {
private:

	std::vector<SpawnData*> spawnDataArray;

	E_SpawnData defaultSpawnDataType;
	E_SpawnData curSpawnDataType;
	SpawnData* curSpawnData;

	bool spawnState;
	int tempSpawnCount;
public:
	CellSpawnerMgr();
	~CellSpawnerMgr();

	void DoInit();

	void RegisterSpawnData(SpawnData* data);

	void DoUpdate(float deltatime);

	void StartSpawn();
	void StopSpawn();
	void PauseSpawn(bool pause);


	void ChangeSpawnCounter(int changeValue);
};


#endif