//===========================================================================
// [ゲームパラメータデータの定義]
// 1．オートマトン
// 2．スキルカードクラス
// 3．敵を生成するのに使うデータ
// 4．ゲーム時間を管理するクラス
//===========================================================================

#pragma once
#ifndef _GAMELOGIC_H_
#define _GAMELOGIC_H_

#include "gameObject.h"

class FSMState;

struct CardRuleRawData;
struct FSMStateAction;

enum class E_GameState {
	GameState_None,
	GameState_Game_Title,
	GameState_Game_Normal,
	GameState_Game_Lv_Up,
	GameState_Game_Menu,
	GameState_Game_Lose,
	GameState_Game_End,
};

enum class E_InputMode {
	InputMode_None,
	InputMode_Title,
	InputMode_Game,
	InputMode_Game_Lv_Up,
	InputMode_Game_Menu,
	InputMode_Game_Lose_Menu,
	InputMode_Game_Win_Menu,
};

enum class E_Shader {
	Shader_None = -1,
	Shader_Normal = 0,
	Shader_Normal_1,
	Shader_WhiteLightObj,
	Shader_Mask,
	Shader_SceneObj,
};

enum class E_HitType {
	HitType_NormalBullet,
	HitType_Laser,
};

enum class E_Effect {
	Effect_None,
	Effect_EnemyHit,
	Effect_PlayerHit,
	Effect_Explosion,

	Effect_ShootLight,
};

enum class E_CardGenreType {
	//CardGenreType_Normal = 0,
	//CardGenreType_Extra = 1,
	CardGenreType_None = -1,
	CardGenreType_Hp_Life_Time = 0,
	CardGenreType_Hp_Bullet_Reflect = 1,
	CardGenreType_Hp_Dmg = 2,
	CardGenreType_Hp_Consume = 3,

	CardGenreType_Atk_Bullet_Count = 10,
	CardGenreType_Atk_Bullet_Range = 11,
	CardGenreType_Atk_Dmg = 12,
	CardGenreType_Atk_Consume = 13,

	CardGenreType_Def_Bullet_Count = 20,
	CardGenreType_Def_Recovery_Cap = 21,
	CardGenreType_Def_Bullet_Reflect_Count = 22,
	CardGenreType_Def_Dmg = 23,
	CardGenreType_Def_Consume = 24,

	CardGenreType_Exp_Gain = 30,
	CardGenreType_Exp_Bullet_Count = 31,
	CardGenreType_Exp_Bullet_Reflect_Count = 32,
	CardGenreType_Exp_Dmg = 33,
	CardGenreType_Exp_Consume = 34,

	CardGenreType_Attack_Speed = 100,
	CardGenreType_Move_Speed = 110,
	CardGenreType_Reflect_Enemy_Bullet = 120,

};

enum class E_CardType {
	CardType_None = 0,
#pragma region composite_single

	/// <summary>
	/// 基盤としたステータス
	/// </summary>
	CardType_Max_Hp = 1,//最大値
	CardType_Max_Atk = 2,
	CardType_Max_Def = 3,

	CardType_Hp_Consume= 11 ,//消耗量
	CardType_Atk_Consume = 12,
	CardType_Def_Consume = 13,
	CardType_Exp_Consume = 14,

	CardType_Hp_Dmg = 21,//基本的なダメージ
	CardType_Atk_Dmg = 22,
	CardType_Def_Dmg = 23,
	CardType_Exp_Dmg = 24,

	/// <summary>
	/// HPのエキストラカード
	/// </summary>
	CardType_Hp_Life_Time = 51,
	CardType_Hp_Bullet_Reflect = 52,

	/// <summary>
	/// ATKのエキストラカード
	/// </summary>
	CardType_Atk_Bullet_Count = 61,
	CardType_Atk_Bullet_Range = 62,

	/// <summary>
	/// DEFのエキストラカード
	/// </summary>
	CardType_Def_Bullet_Count = 71,
	CardType_Def_Recovery_Cap = 72,
	CardType_Def_Bullet_Reflect_Count = 73,

	/// <summary>
	/// EXPのエキストラカード
	/// </summary>
	CardType_Exp_Gain = 81,
	CardType_Exp_Bullet_Count = 82,
	CardType_Exp_Bullet_Reflect_Count = 83,

	/// <summary>
	/// スペシャルエキストラカード
	/// </summary>
	CardType_Attack_Speed_Up = 101,//movement
	CardType_Move_Speed_Up = 102,
	CardType_Reflect_Enemy_Bullet = 103,//extra reflect
	//... TODO


#pragma endregion composite_single
};

enum class E_AttackType {
	AttackType_Hit,
	AttackType_Defeat,
	AttackType_EnemyBulletHit,
};

enum class E_Timer {
	Timer_Game_1,
	Timer_Game_2,
	Timer_UI,
};

#pragma region scene



enum class E_SpawnType {
	SpawnType_Sequence,
	SpawnType_Parallel,
};

enum class E_SpawnData {
	SpawnData_Level_0 = 0,
	SpawnData_Level_1 = 1,
	SpawnData_Level_2 = 2,
	SpawnData_Level_3 = 3,
	SpawnData_Level_4 = 4,
};

#pragma endregion scene

#endif

#pragma region fsm（オートマトン
enum class E_FSMState {
	FSMState_Idle,
	FSMState_MoveToPlayer,
	FSMState_Attack,
	FSMState_Dmg,
};

enum class E_CheckTriggerType {
	CheckTriggerType_None,
	CheckTriggerType_AllTrue,
	CheckTriggerType_AllFalse,
	CheckTriggerType_OneTrue,
	CheckTriggerType_OneFalse,

	CheckTriggerType_AlwaysTrue,
	CheckTriggerType_AlwaysFalse,
};

class FSMSystem {
private:
	BaseCell* parent;//親オブジェクト
private:

	std::map<E_FSMState, FSMState*> stateDic;//ステートリスト

	//今のステート
	E_FSMState curStateType;
	FSMState* curState;

	E_FSMState defaultStateType;//デフォルトステート

public:
	FSMSystem();
	FSMSystem(BaseCell* p);

	~FSMSystem();

	void DoInit();

	/// <summary>
	/// 新しいステートをリストに追加
	/// </summary>
	/// <param name="stateType"></param>
	/// <param name="state"></param>
	/// <param name="setDefaultState"></param>
	void RegisterState(E_FSMState stateType, FSMState* state, bool setDefaultState);

	void DoUpdate(float deltatime);

	/// <summary>
	/// ステートを切り替え
	/// </summary>
	/// <param name="targetStateType"></param>
	/// <param name="resetState"></param>
	void SwitchState(E_FSMState targetStateType, bool resetState);

	/// <summary>
	/// 親オブジェクトを設定
	/// </summary>
	/// <param name="p"></param>
	void SetParent(BaseCell* p);
	BaseCell* GetParent();
};

/// <summary>
/// ステートクラス
/// </summary>
class FSMState {
private:

	bool enable;

	FSMSystem* parentSystem;//親FSM

	E_FSMState stateType;//ステートの種類
	FSMStateAction* action;//ステートの行動（デリゲート）

private:
	std::map<E_FSMState, FSMTriggerGroup*> triggerDic;//次のステートに移行する条件のリスト

private:
	FSMState();

	/// <summary>
	/// 条件をチェックする
	/// </summary>
	/// <param name="deltatime"></param>
	void CheckStateTrigger(float deltatime);


public:
	FSMState(FSMSystem* parent);


	~FSMState();

	void DoInit();

	void DoUpdate(float deltatime);

	void SetAction(FSMStateAction* a);

	/// <summary>
	/// ステートの行動を取る
	/// </summary>
	/// <param name="deltatime"></param>
	void DoAction(float deltatime);

	void StartState();

	void EndState();

	void PauseState(bool state);

	BaseCell* GetParentCell();

	/// <summary>
	/// 条件をリストに追加する
	/// </summary>
	/// <param name="targetState"> 次のステート </param>
	/// <param name="triggerGroup"> 条件オブジェクト </param>
	void RegisterTrigger(E_FSMState targetState, FSMTriggerGroup* triggerGroup);
};

/// <summary>
/// 次のステートに移行する条件のグループ
/// </summary>
class FSMTriggerGroup {
private:
	FSMSystem* parentSystem;//親FSM
	E_CheckTriggerType checkTriggerType;//チェック種類（全部が真の場合や全部が偽の場合など
	std::list<FSMStateTrigger*> triggerArray;//条件リスト
private:

	FSMTriggerGroup();
public:

	FSMTriggerGroup(E_CheckTriggerType checkType, FSMSystem* parent);

	~FSMTriggerGroup();

	/// <summary>
	/// チェック種類を取得
	/// </summary>
	/// <returns></returns>
	E_CheckTriggerType GetCheckType();

	/// <summary>
	/// 条件リストを取得
	/// </summary>
	/// <returns></returns>
	std::list<FSMStateTrigger*>& GetTriggerArray();

	/// <summary>
	/// 新しい条件をリストに追加
	/// </summary>
	/// <param name="trigger"></param>
	void AddTrigger(FSMStateTrigger* trigger);

	/// <summary>
	/// 条件をチェック
	/// </summary>
	/// <param name="deltatime"></param>
	/// <returns></returns>
	bool CheckTrigger(float deltatime);

	void ResetTriggerState();
};

/// <summary>
/// 条件クラス
/// </summary>
class FSMStateTrigger {
private:
	FSMState* parentState;//親ステート
	FSMStateTriggerAction* triggerAction;//条件函数（デリゲート）
public:
	FSMStateTrigger();
	~FSMStateTrigger();

	/// <summary>
	/// パラメータを設定（親ステートと条件関数）
	/// </summary>
	/// <param name="parent"></param>
	/// <param name="action"></param>
	void SetTriggerParam(FSMState* parent, FSMStateTriggerAction* action);

	/// <summary>
	/// 条件をチェック（条件関数を呼び出す）
	/// </summary>
	/// <param name="parentSystem"></param>
	/// <param name="deltatime"></param>
	/// <returns></returns>
	bool Check(FSMSystem* parentSystem, float deltatime);
	void ResetTriggerState();
};


#pragma region action_method

#pragma region enemy_base_action

struct FSMStateAction {
	bool init = false;
	E_Anim setAnim;
	FSMStateAction();
	~FSMStateAction() {

	}
	virtual void operator()(BaseCell* parentCell, float deltatime);
	virtual void Reset();
};

struct FSMState_Base_Action_Move2Player :public FSMStateAction {

	float moveSpeed;
	FSMState_Base_Action_Move2Player() :moveSpeed(1.0f) {

	}
	void operator()(BaseCell* parentCell, float deltatime);
};

struct FSMState_Base_Action_Idle :public FSMStateAction {
	void operator()(BaseCell* parentCell, float deltatime);
};

struct FSMState_Base_Action_Attack :public FSMStateAction {
	bool invokeOnce;
	bool invokeSign;
	float attackDelayTime;
	float attackTimePass;
	E_ActionType attackType;
	FSMState_Base_Action_Attack();
	void operator()(BaseCell* _parentCell, float _deltatime);
	void Reset();
};

struct FSMState_Base_Action_Dmg :public FSMStateAction {
	void operator()(BaseCell* parentCell, float delattime);
};

#pragma endregion enemy_base_action

#pragma endregion action_method

#pragma region trigger_method

struct FSMStateTriggerAction {
	bool init = false;
	bool inverseResult = false;
	virtual bool operator()(BaseCell* parentCell, float deltatime);
	virtual void Reset();
};

struct FSMStateTrigger_Base_Trigger_IdleTime :public FSMStateTriggerAction {
	float idleTime;
	float timePass = 0;
	FSMStateTrigger_Base_Trigger_IdleTime() :idleTime(0.0f), timePass(0.0f) {

	}
	FSMStateTrigger_Base_Trigger_IdleTime(const FSMStateTrigger_Base_Trigger_IdleTime& src) {
		this->idleTime = src.idleTime;
		this->timePass = src.timePass;
	}

	bool operator()(BaseCell* parentCell, float deltatime);
};

struct FSMStateTrigger_Base_Trigger_Dist2Player :public FSMStateTriggerAction {
	float dist;
	FSMStateTrigger_Base_Trigger_Dist2Player() :dist(0.0f) {

	}
	FSMStateTrigger_Base_Trigger_Dist2Player(const FSMStateTrigger_Base_Trigger_Dist2Player& src) {
		this->dist = src.dist;
	}
	bool operator()(BaseCell* parentCell, float deltatime);
};

struct FSMStateTrigger_Base_Trigger_TakeDmg :public FSMStateTriggerAction {

	FSMStateTrigger_Base_Trigger_TakeDmg() {

	}
	FSMStateTrigger_Base_Trigger_TakeDmg(const FSMStateTrigger_Base_Trigger_TakeDmg& src) {

	}

	bool operator()(BaseCell* parentCell, float deltatime);
};

#pragma endregion trigger_method


#pragma endregion fsm（オートマトン

#pragma region status_logic

class CardRuleData {
private:
	std::vector<E_CardType> cardTypeArray;
	std::map<E_CardType, vector2s> cardValueRangeDic;

	vector2s m_levelRange;
	std::vector<CardRuleRawData*> m_cardDataVec;
	std::vector<CardRuleRawData*> m_usedCardVec;
public:
	CardRuleData();
	~CardRuleData();

	bool CheckLv(int lv);

	void SetLevelRange(vector2s lvRange);
	void AddCardData(CardRuleRawData* _data);

	void SetLevelRange(int minLv, int maxLv);
	void AddCardType(E_CardType cardType, bool useValue, vector2s valueRange);

	CardEffectData* GetCard(E_CardType typeMask);
	CardEffectData* GetCard(std::list<int>& _maskVec);

	static CardEffectData* GetExtraCard(CardRuleRawData* _data);
};

struct CardEffectData {
	E_CardType cardType;
	int id;
	float param;
	CardEffectData* conditionData;
	CardEffectData* effectData;

	CardEffectData() :cardType(E_CardType::CardType_None), id(-1),param(0.0f),conditionData(nullptr), effectData(nullptr) {

	}

	virtual ~CardEffectData() {
		if (conditionData != nullptr) {
			delete conditionData;
			conditionData = nullptr;
		}

		if (effectData != nullptr) {
			delete effectData;
			effectData = nullptr;
		}
	}

	virtual void ApplyEffect(Status_Player* playerStatus) {
		if (conditionData != nullptr) {
			conditionData->ApplyEffect(playerStatus);
		}
		if (effectData != nullptr) {
			effectData->ApplyEffect(playerStatus);
		}
	}
	virtual void RecoveryEffect(Status_Player* playerStatus) {
		if (conditionData != nullptr) {
			conditionData->RecoveryEffect(playerStatus);
		}
		if (effectData != nullptr) {
			effectData->RecoveryEffect(playerStatus);
		}
	}
};

struct CardEffect_StatusData : public CardEffectData {
	E_Status statusType_1;//condition
	int increaseValue_1;
	E_Status statusType_2;//effect
	int increaseValue_2;

	CardEffect_StatusData();

	~CardEffect_StatusData() {

	}

	void ApplyEffect(Status_Player* playerStatus);

	void RecoveryEffect(Status_Player* playerStatus);
};

struct CardEffect_Movement :public CardEffectData {
	int movementIncreaseValue;

	CardEffect_Movement() :movementIncreaseValue(0) {

	}

	~CardEffect_Movement() {

	}

	void ApplyEffect(Status_Player* playerStatus);

	void RecoveryEffect(Status_Player* playerStatus);
};

struct CardEffect_Extra :public CardEffectData {

	float increasement;

	CardEffect_Extra() :increasement(0.0f) {

	}

	~CardEffect_Extra() {

	}

	void ApplyEffect(Status_Player* playerStatus);
	void RecoveryEffect(Status_Player* playerStatus);
};

#pragma endregion status_logic

#pragma region 敵生成データ

class SpawnDataCell {
private:

	E_CellType spawnCellType;

	int dataCellID;
	float spawnInterval;
	float spawnTimePass;

	int maxSpawnCount;
	int curSpawnCount;

	bool isInfinite;

public:
	bool spawnDone;
	bool cellState;

	bool canSpawnSign;
public:

	SpawnDataCell();
	~SpawnDataCell();

	void SetSpawnCellID(int id);
	int GetSpawnCellID();

	void SetSpawnParam(E_CellType cellType, float spawnTime, int maxCount, bool infinite);

	void StartSpawn();
	void StopSpawn();

	void DoUpdate(float deltatime);

	E_CellType GetSpawnType();
	void SpawnActionInvoke();//spawn cell,continu timer
	void Reset();
};

class SpawnData {
private:
	E_SpawnData spawnDataType;
	E_SpawnType spawnType;
	int dataID;

	std::vector<SpawnDataCell*> spawnDataCellArray;

	bool isInfinite;
	int maxSpawnCount;
	int curSpawnCount;

	bool spawnState;

private:
	//sequency spawn param
	int curSequencySpawnIndex;

private:
	//other spawn param(pos

public:
	bool spawnOnceSign;
	bool useLvCondition;
	bool useTimeCondition;
	vector2s lvCondition;
	vector2s timeCondition;

	bool spawnDone;
public:
	SpawnData();
	~SpawnData();

	void DoInit();

	int GetID();
	E_SpawnType GetSpawnType();
	E_SpawnData GetSpawnDataType();

	void SetSpawnDataParam(E_SpawnData spawnDataT, E_SpawnType spawnT, int maxCount, bool infinite);

	void RegisterSpawnDataCell(SpawnDataCell* cell);

	void SetSpawnCountIncrease(int increaseValue);

	void DoUpdate(float deltatime);

	void Spawn(E_CellType cellType);

	void SetSpawnState(bool state);
	void StartSpawn();
	void StopSpawn();

	void ChangeSpawnCount(int increasement);
	void SetCurSpawnCount(int c);
	int GetCurSpawnCount();
	bool CheckConditionState();
};

#pragma endregion 敵生成データ

#pragma region ゲーム時間

class Timer {
private:
	//game timer
	bool gameCountDownState;
	float defaultGameCountDownTime;
	float gameCountDownTime;
	float gameCountDownTimePass;
public:
	float gameTimer_1;//game player
	float gameTimer_2;
	float uiTimer;//ui timer
public:
	Timer();
	~Timer();

	void SetTimer(E_Timer timerType, bool limitForwar);
	void SetTimer(E_Timer timerType, float value);

	void DoUpdate(float deltatime);

	//game count down
	void SetGameCountDownState(bool state);
	bool CheckCountDownDone();
	int GetCountDownMinute();
	int GetCountDownSecond();
	float GetRemainTime();

	void Reset();
	void ResetCountDownParam();
};

class ComponentHelper {
private:


public:
	ComponentHelper() {

	}

	~ComponentHelper() {

	}

};

#pragma endregion ゲーム時間