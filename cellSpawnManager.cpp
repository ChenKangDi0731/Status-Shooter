#include "cellSpawnManager.h"

#pragma region spawn_manager


CellSpawnerMgr::CellSpawnerMgr()
{
	tempSpawnCount = 0;
	spawnDataArray.clear();

	//register spawn data:TODO
	//=======================================lv 1
	SpawnData* spawnData_lv_1 = new SpawnData();
	spawnData_lv_1->SetSpawnDataParam(E_SpawnData::SpawnData_Level_0, E_SpawnType::SpawnType_Sequence, 20, true);
	//spawnData_lv_1->useLvCondition = true;
	//spawnData_lv_1->lvCondition = vector2s(0, 3);
	spawnData_lv_1->useTimeCondition = true;
	//spawnData_lv_1->timeCondition = vector2s(250, 305);
	spawnData_lv_1->timeCondition = vector2s(280, 305);


	SpawnDataCell* spawnData_1_cell_1 = new SpawnDataCell();
	spawnData_1_cell_1->SetSpawnParam(E_CellType::CellType_Enemy_Skirmisher, 0.5f, 60, false);
	spawnData_lv_1->RegisterSpawnDataCell(spawnData_1_cell_1);


	//=======================================lv 2
	SpawnData* spawnData_lv_2 = new SpawnData();
	spawnData_lv_2->SetSpawnDataParam(E_SpawnData::SpawnData_Level_1, E_SpawnType::SpawnType_Sequence, 30, true);
	//spawnData_lv_2->useLvCondition = true;
	//spawnData_lv_2->lvCondition = vector2s(4, 5);
	spawnData_lv_2->useTimeCondition = true;
	//spawnData_lv_2->timeCondition = vector2s(200, 250);
	spawnData_lv_2->timeCondition = vector2s(260, 280);


	SpawnDataCell* spawnData_2_cell_1 = new SpawnDataCell();
	spawnData_2_cell_1->SetSpawnParam(E_CellType::CellType_Enemy_Skirmisher_2, 0.5f, 4, false);
	spawnData_lv_2->RegisterSpawnDataCell(spawnData_2_cell_1);

	SpawnDataCell* spawnData_2_cell_2 = new SpawnDataCell();
	spawnData_2_cell_2->SetSpawnParam(E_CellType::CellType_Enemy_Skirmisher_2, 1.0f, 10, false);
	spawnData_lv_2->RegisterSpawnDataCell(spawnData_2_cell_2);

	//========================================lv 3
	SpawnData* spawnData_lv_3 = new SpawnData();
	spawnData_lv_3->SetSpawnDataParam(E_SpawnData::SpawnData_Level_2, E_SpawnType::SpawnType_Parallel, 25, true);
	//spawnData_lv_3->useLvCondition = true;
	//spawnData_lv_3->lvCondition = vector2s(6, 7);
	spawnData_lv_3->useTimeCondition = true;
	//spawnData_lv_3->timeCondition = vector2s(150, 200);
	spawnData_lv_3->timeCondition = vector2s(150, 260);


	SpawnDataCell* spawnData_3_cell_1 = new SpawnDataCell();
	spawnData_3_cell_1->SetSpawnParam(E_CellType::CellType_Enemy_Skirmisher, 0.5f, 10, true);
	spawnData_lv_3->RegisterSpawnDataCell(spawnData_3_cell_1);

	SpawnDataCell* spawnData_3_cell_2 = new SpawnDataCell();
	spawnData_3_cell_2->SetSpawnParam(E_CellType::CellType_Enemy_Dragonfly, 5.0f, 5, true);
	spawnData_lv_3->RegisterSpawnDataCell(spawnData_3_cell_2);


	//==========================================lv 4 
	SpawnData* spawnData_lv_4 = new SpawnData();
	spawnData_lv_4->SetSpawnDataParam(E_SpawnData::SpawnData_Level_3, E_SpawnType::SpawnType_Parallel, 25, true);
	spawnData_lv_4->useTimeCondition = true;
	spawnData_lv_4->timeCondition = vector2s(100, 150);

	SpawnDataCell* spawnData_4_cell_1 = new SpawnDataCell();
	spawnData_4_cell_1->SetSpawnParam(E_CellType::CellType_Enemy_Skirmisher_2, 0.5f, 15, true);
	spawnData_lv_4->RegisterSpawnDataCell(spawnData_4_cell_1);

	SpawnDataCell* spawnData_4_cell_2 = new SpawnDataCell();
	spawnData_4_cell_2->SetSpawnParam(E_CellType::CellType_Enemy_Dragonfly, 5.0f, 10, true);
	spawnData_lv_4->RegisterSpawnDataCell(spawnData_4_cell_2);

	//==========================================lv 5
	SpawnData* spawnData_lv_5 = new SpawnData();
	spawnData_lv_5->SetSpawnDataParam(E_SpawnData::SpawnData_Level_3, E_SpawnType::SpawnType_Parallel, 40, true);
	spawnData_lv_5->useTimeCondition = true;
	spawnData_lv_5->timeCondition = vector2s(0, 100);

	SpawnDataCell* spawnData_5_cell_1 = new SpawnDataCell();
	spawnData_5_cell_1->SetSpawnParam(E_CellType::CellType_Enemy_Skirmisher_2, 0.5f, 15, true);
	spawnData_lv_5->RegisterSpawnDataCell(spawnData_5_cell_1);

	SpawnDataCell* spawnData_5_cell_2 = new SpawnDataCell();
	spawnData_5_cell_2->SetSpawnParam(E_CellType::CellType_Enemy_Skirmisher, 0.5f, 10, true);
	spawnData_lv_5->RegisterSpawnDataCell(spawnData_5_cell_2);

	SpawnDataCell* spawnData_5_cell_3 = new SpawnDataCell();
	spawnData_5_cell_3->SetSpawnParam(E_CellType::CellType_Enemy_Dragonfly, 5.0f, 10, true);
	spawnData_lv_5->RegisterSpawnDataCell(spawnData_5_cell_3);

	RegisterSpawnData(spawnData_lv_1);
	RegisterSpawnData(spawnData_lv_2);
	RegisterSpawnData(spawnData_lv_3);
	RegisterSpawnData(spawnData_lv_4);
	RegisterSpawnData(spawnData_lv_5);

	spawnState = false;
}

CellSpawnerMgr::~CellSpawnerMgr()
{
	if (spawnDataArray.size() != 0) {
		std::vector<SpawnData*>::iterator iter;
		for (iter = spawnDataArray.begin(); iter != spawnDataArray.end(); iter++) {
			if ((*iter) == nullptr)continue;
			delete (*iter);
		}
	}

	spawnDataArray.clear();

}

void CellSpawnerMgr::DoInit()
{
	std::vector<SpawnData*>::iterator iter;
	for (iter = spawnDataArray.begin(); iter != spawnDataArray.end(); iter++) {
		if ((*iter) != nullptr) {
			(*iter)->DoInit();
		}
	}

	StopSpawn();
	tempSpawnCount = 0;
}

void CellSpawnerMgr::RegisterSpawnData(SpawnData* data)
{
	if (data == nullptr)return;
	spawnDataArray.push_back(data);
}

void CellSpawnerMgr::DoUpdate(float deltatime)
{
	if (curSpawnData == nullptr || curSpawnData->spawnDone == true) {
		if (curSpawnData != nullptr) {
			tempSpawnCount = curSpawnData->GetCurSpawnCount();
		}
		curSpawnData = nullptr;
		std::vector<SpawnData*>::iterator iter;
		for (iter = spawnDataArray.begin(); iter != spawnDataArray.end(); iter++) {
			if ((*iter)->CheckConditionState() == true && (*iter)->spawnOnceSign == false) {
				curSpawnData = (*iter);
				curSpawnData->SetCurSpawnCount(tempSpawnCount);
				curSpawnData->StartSpawn();
				return;
			}
		}
		//all spawn done
		StopSpawn();
	}
	else {
		curSpawnData->DoUpdate(deltatime);
	}
}


void CellSpawnerMgr::StartSpawn()
{
	//init
	//state true
	DoInit();
	spawnState = true;

}

void CellSpawnerMgr::StopSpawn() {
	//state false
	curSpawnData = nullptr;
	spawnState = false;
}

void CellSpawnerMgr::PauseSpawn(bool pause) {
	if (curSpawnData != nullptr) {
		curSpawnData->SetSpawnState(pause);
	}
}

void CellSpawnerMgr::ChangeSpawnCounter(int changeValue)
{
	if (curSpawnData != nullptr) {
		curSpawnData->ChangeSpawnCount(changeValue);
	}
}

#pragma endregion spawn_manager
