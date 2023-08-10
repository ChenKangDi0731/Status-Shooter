#include "main.h"
#include "scene.h"
#include "sceneManager.h"
#include "manager.h"
#include "sceneObjManager.h"

#pragma region scene_manager_method

SceneMgr::SceneMgr() :sceneObjMgr(nullptr), defaultSceneType(E_Scene::Scene_None), curSceneType(E_Scene::Scene_None)
{
	this->curScene = nullptr;
	sceneDic.clear();
}

SceneMgr::~SceneMgr()
{
	sceneObjMgr = nullptr;
	curScene = nullptr;
}

void SceneMgr::DoInit()
{
	//シーンをリストに追加
	RegisterScene(new Scene_1(), true);
}


void SceneMgr::DoUpdate(float deltatime)
{
	if (curScene != nullptr) {
		curScene->DoUpdate(deltatime);
	}
}

void SceneMgr::DoLateUpdate(float deltatime)
{
}

void SceneMgr::RegisterScene(Scene* s, bool setDefaultScene)
{
	if (s == nullptr)return;
	E_Scene sceneType = s->GetSceneType();
	if (sceneDic.size() == 0 || sceneDic.find(sceneType) == sceneDic.end()) {
		sceneDic[sceneType] = s;

		if (setDefaultScene == true) {
			defaultSceneType = sceneType;
		}
	}
	else {
		//error
	}
}

void SceneMgr::UnregisterScene(Scene* s)
{
	if (s == nullptr || sceneDic.size() == 0)return;

	std::map<E_Scene, Scene*>::iterator iter = sceneDic.find(s->GetSceneType());
	if (iter != sceneDic.end()) {
		sceneDic.erase(iter);
	}
}

void SceneMgr::SwitchScene(E_Scene sceneType, bool reload)
{
	if (sceneType == curSceneType && sceneType != E_Scene::Scene_None && reload == false) {
		ResetCurScene();
		return;
	}

	if (sceneDic.find(sceneType) == sceneDic.end() && sceneDic[sceneType] != nullptr) {
		return;
	}

	if (curScene != nullptr) {
		curScene->Unload();
		curScene = nullptr;
	}

	curScene = sceneDic[sceneType];
	if (curScene != nullptr) {
		curScene->Load();
		curSceneType = sceneType;
	}
}

void SceneMgr::SwitchNextScene()
{
}

void SceneMgr::ResetCurScene()
{
	if (curScene != nullptr) {
		curScene->Reset();
	}
}

void SceneMgr::ClearSceneDic()
{
	std::map<E_Scene, Scene*>::iterator iter;
	for (iter = sceneDic.begin(); iter != sceneDic.end(); iter++) {
		if (iter->second != nullptr) {
			delete iter->second;
		}
	}
	sceneDic.clear();
}

E_Scene SceneMgr::GetCurSceneType()
{
	return curSceneType;
}

vector3s SceneMgr::GetSceneCameraPos()
{
	if (curScene != nullptr) {
		return curScene->GetSceneCameraPos();
	}
	return vector3s(0, 0, 0);
}

void SceneMgr::UnInit()
{
	//release scene
	ClearSceneDic();
}

#pragma endregion scene_manager_method
