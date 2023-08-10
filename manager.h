#pragma once
#ifndef _MANAGER_H_
#define _MANAGER_H_

#include "main.h"

class Shader;

class Collider;
class Scene;
class Scene_1;
class Bullet;
struct BaseShaderParam;

class BaseCell;
class Light;
class Player;
class Enemy;
class Status_Player;

class UISprite;
class UIGroup;
class UI;

class GameMgr;
class DataManager;
class InputMgr;
class SceneObjMgr;
class SceneMgr;
class EffectMgr;
class ShaderMgr;
class TextureMgr;
class UIMgr;
class BattleMgr;
class CellMgr;
class CellSpawnerMgr;
class RenderMgr;
class Timer;

class GameState;
class GameState_Game;
class GameState_Title;
class GameState_Lv_Up;
class GameState_Menu;
class GameState_Lose;
class GameState_End;

class CardRuleData;
struct CardEffectData;
struct CardEffect_StatusData;
struct CardEffect_Movement;
struct CardEffect_Extra;

class Effect;

class FSMSystem;
class FSMState;
class FSMTriggerGroup;
class FSMStateTrigger;

struct CardLvRawData;
struct CardRuleRawData;
struct PlayerLvUpRawData;

struct FSMStateAction;
struct FSMStateTriggerAction;

struct Anim_Callback;
struct Anim_Effect_Normal_CB;

enum class E_BatchingType;
enum class E_Status;
enum class E_Anim;
enum class E_CellType;
enum class E_UIContent;
enum class E_DrawLayer;
enum class E_BulletType;
enum class E_ActionType;
enum class E_GameState;
enum class E_InputMode;
enum class E_SpawnType;
enum class E_SpawnData;
enum class E_Shader;
enum class E_Effect;
enum class E_AttackType;
enum class E_CardGenreType;
enum class E_CardGenreType;

enum class E_Scene {
	Scene_None = -1,

	Scene_Stage1 = 11,
	Scene_Stage2 = 12,
	Scene_Stage3 = 13,
};

#pragma region manager
/// <summary>
/// �Q�[���}�l�[�W��
/// </summary>
class GameMgr {

private:
	static GameMgr* instance;

private:
	std::map<E_GameState, GameState*> gameStateDic;//���C���Q�[���X�e�[�g���X�g
	std::map<E_GameState, GameState*> subGameStateDic;//�T�u�Q�[���X�e�[�g���X�g

	E_GameState defaultGameStateType;
	E_GameState defaultSubGameStateType;

	E_GameState curGameStateType;
	E_GameState curSubGameStateType;
	GameState* curGameState;
	GameState* curSubGameState;
public:
	DataManager* dataMgr;//�f�[�^�}�l�[�W���[�iCSV�f�[�^�ǂݍ���
	SceneMgr* sceneMgr;//�V�[���}�l�[�W���[�i�V�[���؂�ւ�
	TextureMgr* textureMgr;
	ShaderMgr* shaderMgr;
	InputMgr* inputMgr;//���̓}�l�[�W���[
	SceneObjMgr* sceneObjMgr;//�Q�[���I�u�W�F�N�g�}�l�[�W���[�i���ׂẴI�u�W�F�N�g���Ǘ�
	EffectMgr* effectMgr;//�G�t�F�N�g�}�l�[�W���[
	CellMgr* cellMgr;//�G�L�����N�^�[���Ǘ�
	CellSpawnerMgr* cellSpawnerMgr;//�G�𐶐�����
	UIMgr* uiMgr;//UI���Ǘ�����
	BattleMgr* battleMgr;//�X�L���J�[�h�Ǘ���_���[�W�v�Z�Ȃ�
	Timer* timer;//�Q�[�����Ԃ��Ǘ�

private:
	GameMgr();
	GameMgr(const GameMgr&) = delete;
	GameMgr& operator=(const GameMgr&) = delete;
public:
	~GameMgr();

	void DoInit();
	void DoUninit();

	void DoUpdate(float deltatime);
	void DoLateUpdate(float deltatine);

	void SwitchGameState(E_GameState gameState, bool reset);
	void SwitchSubGameState(E_GameState subGameState, bool reset);

	void RegisterGameState(E_GameState gameStateType, GameState* gameState, bool setDefault);
	void RegisterSubGameState(E_GameState gameStateType, GameState* gameState, bool setDefault);

	E_GameState GetCurGameState();
	E_GameState GetCurSubGameState();

public:
	class Deletor {
	public:
		~Deletor() {
			if (GameMgr::instance != nullptr) {
				delete GameMgr::instance;
			}
		}
	};
	static Deletor deletor;
public:
	static GameMgr* GetInstance() {
		if (instance == nullptr) {
			instance = new GameMgr();
		}

		return instance;
	}
};

class RenderMgr {
private:

public:
	RenderMgr();
	~RenderMgr();

	static DrawData* GetNewDrawData(DrawData* copySrc, bool useBatching, E_BatchingType batchingType, bool shakeWithCamera);
	static void ReleaseDrawDataVertex(int vertexDataIndex, E_BatchingType batchingType);
};

class Shader {
public:
	unsigned int Program;

	Shader(const char* _vertexPath, const char* _fragmentPath) {

	}

	void Use() {

	}
};

class ShaderMgr {
private:
	std::map<E_Shader, Shader*> shaderDic;

public:

	ShaderMgr();
	~ShaderMgr();

	void DoInit();

	void RegisterShader(E_Shader shaderType, Shader* shader);
	int GetShaderProgram(E_Shader shaderType);
};

/// <summary>
/// �e�N�X�`���[�}�l�[�W���[
/// </summary>
class TextureMgr {
private:
	Texture textureController;

	std::map<E_Texture, int> textureDic;

public:
	TextureMgr();
	~TextureMgr();

	void DoInit();

	void RegisterTexture(E_Texture textureType, int textureID);
	int GetTextureID(E_Texture textureType);
};

#pragma endregion manager

#endif 