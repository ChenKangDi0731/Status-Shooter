#include "main.h"
#include "manager.h"
#include "scene.h"
#include "player.h"
#include "sceneObjManager.h"
#include "cellManager.h"

#pragma region scene

Scene::Scene() :sceneType(E_Scene::Scene_None), sceneID(-1), nextSceneID(-1),  sceneCameraPos(vector3s(0, 0, 0)) {
	sceneObjDic.clear();
}

Scene::Scene(E_Scene sceneT) : sceneType(sceneT), sceneID((int)sceneT), nextSceneID(-1)
{
	sceneObjDic.clear();
}

Scene::~Scene() {
	//delete all obj

	if (sceneObjDic.size() != 0) {
		std::map<int, BaseCell*>::iterator iter;
		for (iter = sceneObjDic.begin(); iter != sceneObjDic.end(); iter++) {
			GameMgr::GetInstance()->sceneObjMgr->UnRegisterCell(iter->second);//unregister from SceneObjMgr
		}
		sceneObjDic.clear();
	}
}

void Scene::RegisterSceneObj(BaseCell* cell)
{
	if (cell == nullptr)return;

	if (sceneObjDic.find(cell->GetID()) == sceneObjDic.end()) {
		sceneObjDic[cell->GetID()] = cell;
	}

	GameMgr::GetInstance()->sceneObjMgr->RegisterCell(cell);
}

void Scene::UnregisterSceneObj(BaseCell* cell)
{
	if (cell == nullptr || sceneObjDic.size() == 0) return;

	std::map<int, BaseCell*>::iterator iter = sceneObjDic.find(cell->GetID());
	if (iter == sceneObjDic.end()) {
		return;
	}

	sceneObjDic.erase(iter);

	GameMgr::GetInstance()->sceneObjMgr->UnRegisterCell(cell);//unregister from SceneObjMgr
}

void Scene::DoUpdate(float deltatime) {

}

void Scene::DoLateUpdate(float deltatime) {

}

void Scene::Load() {
	OnSceneLoad();
	isSceneActive = true;
}

void Scene::Unload() {
	if (sceneObjDic.size() != 0) {
		std::map<int, BaseCell*>::iterator iter;
		for (iter = sceneObjDic.begin(); iter != sceneObjDic.end(); iter++) {
			GameMgr::GetInstance()->sceneObjMgr->UnRegisterCell(iter->second);//unregister from SceneObjMgr
		}
		sceneObjDic.clear();
	}

	OnSceneUnload();
	isSceneActive = false;
}

void Scene::Reset()
{
	OnSceneUnload();
	if (sceneObjDic.size() != 0) {
		std::map<int, BaseCell*>::iterator iter;
		for (iter = sceneObjDic.begin(); iter != sceneObjDic.end(); iter++) {
			if (iter->second != nullptr) {
				iter->second->Reset();
			}
		}
	}
	OnSceneLoad();
}

void Scene::OnSceneLoad() {

}

void Scene::OnSceneUnload() {

}

void Scene::SetSceneActive(bool state)
{
	if (state == isSceneActive)return;
	isSceneActive = state;
	//TODO
}

void Scene::SetSceneID(int id)
{
	sceneID = id;
}

void Scene::SetNextSceneID(int nextID) {
	nextSceneID = nextID;
}

int Scene::GetSceneID() {
	return sceneID;
}

int Scene::GetNextSceneID() {
	return nextSceneID;
}

E_Scene Scene::GetSceneType()
{
	return sceneType;
}

vector3s Scene::GetSceneCameraPos()
{
	return sceneCameraPos;
}

#pragma region scene_3

Scene_1::Scene_1() :Scene(E_Scene::Scene_Stage3) {

}

Scene_1::~Scene_1() {

}

void Scene_1::Load() {
	Scene::Load();

	//load scene bg
	int randomSize = rand() % 50 + 10;
	for (int index = 0; index < randomSize; index++) {
		BaseCell* curBgItem = GameMgr::GetInstance()->cellMgr->GetCell(E_CellType::CellType_Grass);
		if (curBgItem != nullptr) {
			vector3s targetPos = vector3s(0.0f, 0.0f, 0.0f);
			targetPos.x = rand() % (int)(SCREEN_WIDTH)-SCREEN_WIDTH * 0.5f;
			targetPos.y = rand() % (int)(SCREEN_HEIGHT)-SCREEN_HEIGHT * 0.5f;
			targetPos.z = 10.0f;
			curBgItem->SetPos(targetPos, true);

			int grassType = rand() % 3;
			DrawData* data = curBgItem->GetDrawData();
			if (data != nullptr) {
				data->SetShaderIntParam(3, grassType);
			}
		}
		this->RegisterSceneObj(curBgItem);
	}
}

void Scene_1::Unload() {
	Scene::Unload();

	//hide game obj
	GameMgr::GetInstance()->sceneObjMgr->ShowByCellType(E_CellType::CellType_Enemy, false);
	GameMgr::GetInstance()->sceneObjMgr->ShowByCellType(E_CellType::CellType_Bullet, false);
	GameMgr::GetInstance()->sceneObjMgr->ShowByCellType(E_CellType::CellType_UI, false);
	GameMgr::GetInstance()->sceneObjMgr->ShowByCellType(E_CellType::CellType_Effect, false);
}

void Scene_1::Reset() {
	Scene::Reset();
}

void Scene_1::DoUpdate(float deltatime) {
	Scene::DoUpdate(deltatime);

	if (isSceneActive == true) {
		Player* player = GameMgr::GetInstance()->sceneObjMgr->GetPlayer();
		if (player != nullptr) {
			vector3s playerPos = player->GetTransform()->GetPos();

			sceneCameraPos = VecLerp(sceneCameraPos, playerPos, CAMERA_LERP_FACTOR * deltatime);
			sceneCameraPos.z = 0.0f;
		}
	}
}

void Scene_1::DoLateUpdate(float deltatime) {
	Scene::DoLateUpdate(deltatime);
}

void Scene_1::OnSceneLoad() {
	Scene::OnSceneLoad();

	GameMgr::GetInstance()->sceneObjMgr->InitPlayerObjState( );
	GameMgr::GetInstance()->sceneObjMgr->DoInitCell();

}

void Scene_1::OnSceneUnload() {
	Scene::OnSceneUnload();

	GameMgr::GetInstance()->sceneObjMgr->UninitPlayerObjState();
}


#pragma endregion scene_3


#pragma endregion scene
