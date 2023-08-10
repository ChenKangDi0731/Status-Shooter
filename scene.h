#ifndef _SCENE_H_
#define _SCENE_H_

#include "gameObject.h"

class Scene {
protected:
	E_Scene sceneType;
	int sceneID;
	int nextSceneID;

	bool isSceneActive;

	std::map<int, BaseCell*> sceneObjDic;

	vector3s sceneCameraPos;

public:

	Scene();
	Scene(E_Scene sceneT);
	virtual ~Scene();

	void RegisterSceneObj(BaseCell* cell);
	void UnregisterSceneObj(BaseCell* cell);

	virtual void DoUpdate(float deltatime);
	virtual void DoLateUpdate(float deltatime);

	virtual void Load();
	virtual void Unload();
	virtual void Reset();

	virtual void OnSceneLoad();
	virtual void OnSceneUnload();

	virtual void SetSceneActive(bool state);

	void SetSceneID(int id);
	void SetNextSceneID(int nextID);
	int GetSceneID();
	int GetNextSceneID();
	E_Scene GetSceneType();

	virtual vector3s GetSceneCameraPos();
};

class Scene_1 :public Scene {
public:
	Scene_1();
	~Scene_1();

	void Load();
	void Unload();
	void Reset();

	void DoUpdate(float deltatime);
	void DoLateUpdate(float deltatime);

	void OnSceneLoad();
	void OnSceneUnload();
};

#endif