//===========================================================================
// [Cゲームシーンマネージャー]
// ‐シーンの切り替えを管理する
//===========================================================================
#ifndef _SCENEMANAGER_H_
#define _SCENEMANAGER_H_

class Scene;
enum class E_Scene;

class SceneMgr {
private:
	SceneObjMgr* sceneObjMgr;
	Scene* curScene;//今のシーン
	std::map<E_Scene, Scene*> sceneDic;//シーンリスト

	E_Scene defaultSceneType;//デフォルトシーン
	E_Scene curSceneType;//今のシーン

public:
	int bgm_title;
	int bgm_game;
	int bgm_game_over;
	int bgm_game_clear;

	int se_ui_item_select;
	int se_cannon_shoot;
	int se_cannon_explosion;
	int se_enemy_hit;
	int se_laser_shoot;
	int se_normal_shoot;

public:
	SceneMgr();
	~SceneMgr();

	void DoInit();


	void DoUpdate(float deltatime);
	void DoLateUpdate(float deltatime);

	void RegisterScene(Scene* s, bool setDefaultScene);
	void UnregisterScene(Scene* s);
	//シーンを切り替え
	void SwitchScene(E_Scene sceneType, bool reload);
	void SwitchNextScene();

	void ResetCurScene();

	void ClearSceneDic();

	E_Scene GetCurSceneType();

	vector3s GetSceneCameraPos();

	void UnInit();
};



#endif