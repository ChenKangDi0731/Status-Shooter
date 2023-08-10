//===========================================================================
// [�Q�[���p�����[�^�f�[�^�̒�`]
// 1�D�I�[�g�}�g��
// 2�D�X�L���J�[�h�N���X
// 3�D�G�𐶐�����̂Ɏg���f�[�^
// 4�D�Q�[�����Ԃ��Ǘ�����N���X
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
	/// ��ՂƂ����X�e�[�^�X
	/// </summary>
	CardType_Max_Hp = 1,//�ő�l
	CardType_Max_Atk = 2,
	CardType_Max_Def = 3,

	CardType_Hp_Consume= 11 ,//���՗�
	CardType_Atk_Consume = 12,
	CardType_Def_Consume = 13,
	CardType_Exp_Consume = 14,

	CardType_Hp_Dmg = 21,//��{�I�ȃ_���[�W
	CardType_Atk_Dmg = 22,
	CardType_Def_Dmg = 23,
	CardType_Exp_Dmg = 24,

	/// <summary>
	/// HP�̃G�L�X�g���J�[�h
	/// </summary>
	CardType_Hp_Life_Time = 51,
	CardType_Hp_Bullet_Reflect = 52,

	/// <summary>
	/// ATK�̃G�L�X�g���J�[�h
	/// </summary>
	CardType_Atk_Bullet_Count = 61,
	CardType_Atk_Bullet_Range = 62,

	/// <summary>
	/// DEF�̃G�L�X�g���J�[�h
	/// </summary>
	CardType_Def_Bullet_Count = 71,
	CardType_Def_Recovery_Cap = 72,
	CardType_Def_Bullet_Reflect_Count = 73,

	/// <summary>
	/// EXP�̃G�L�X�g���J�[�h
	/// </summary>
	CardType_Exp_Gain = 81,
	CardType_Exp_Bullet_Count = 82,
	CardType_Exp_Bullet_Reflect_Count = 83,

	/// <summary>
	/// �X�y�V�����G�L�X�g���J�[�h
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

#pragma region fsm�i�I�[�g�}�g��
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
	BaseCell* parent;//�e�I�u�W�F�N�g
private:

	std::map<E_FSMState, FSMState*> stateDic;//�X�e�[�g���X�g

	//���̃X�e�[�g
	E_FSMState curStateType;
	FSMState* curState;

	E_FSMState defaultStateType;//�f�t�H���g�X�e�[�g

public:
	FSMSystem();
	FSMSystem(BaseCell* p);

	~FSMSystem();

	void DoInit();

	/// <summary>
	/// �V�����X�e�[�g�����X�g�ɒǉ�
	/// </summary>
	/// <param name="stateType"></param>
	/// <param name="state"></param>
	/// <param name="setDefaultState"></param>
	void RegisterState(E_FSMState stateType, FSMState* state, bool setDefaultState);

	void DoUpdate(float deltatime);

	/// <summary>
	/// �X�e�[�g��؂�ւ�
	/// </summary>
	/// <param name="targetStateType"></param>
	/// <param name="resetState"></param>
	void SwitchState(E_FSMState targetStateType, bool resetState);

	/// <summary>
	/// �e�I�u�W�F�N�g��ݒ�
	/// </summary>
	/// <param name="p"></param>
	void SetParent(BaseCell* p);
	BaseCell* GetParent();
};

/// <summary>
/// �X�e�[�g�N���X
/// </summary>
class FSMState {
private:

	bool enable;

	FSMSystem* parentSystem;//�eFSM

	E_FSMState stateType;//�X�e�[�g�̎��
	FSMStateAction* action;//�X�e�[�g�̍s���i�f���Q�[�g�j

private:
	std::map<E_FSMState, FSMTriggerGroup*> triggerDic;//���̃X�e�[�g�Ɉڍs��������̃��X�g

private:
	FSMState();

	/// <summary>
	/// �������`�F�b�N����
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
	/// �X�e�[�g�̍s�������
	/// </summary>
	/// <param name="deltatime"></param>
	void DoAction(float deltatime);

	void StartState();

	void EndState();

	void PauseState(bool state);

	BaseCell* GetParentCell();

	/// <summary>
	/// ���������X�g�ɒǉ�����
	/// </summary>
	/// <param name="targetState"> ���̃X�e�[�g </param>
	/// <param name="triggerGroup"> �����I�u�W�F�N�g </param>
	void RegisterTrigger(E_FSMState targetState, FSMTriggerGroup* triggerGroup);
};

/// <summary>
/// ���̃X�e�[�g�Ɉڍs��������̃O���[�v
/// </summary>
class FSMTriggerGroup {
private:
	FSMSystem* parentSystem;//�eFSM
	E_CheckTriggerType checkTriggerType;//�`�F�b�N��ށi�S�����^�̏ꍇ��S�����U�̏ꍇ�Ȃ�
	std::list<FSMStateTrigger*> triggerArray;//�������X�g
private:

	FSMTriggerGroup();
public:

	FSMTriggerGroup(E_CheckTriggerType checkType, FSMSystem* parent);

	~FSMTriggerGroup();

	/// <summary>
	/// �`�F�b�N��ނ��擾
	/// </summary>
	/// <returns></returns>
	E_CheckTriggerType GetCheckType();

	/// <summary>
	/// �������X�g���擾
	/// </summary>
	/// <returns></returns>
	std::list<FSMStateTrigger*>& GetTriggerArray();

	/// <summary>
	/// �V�������������X�g�ɒǉ�
	/// </summary>
	/// <param name="trigger"></param>
	void AddTrigger(FSMStateTrigger* trigger);

	/// <summary>
	/// �������`�F�b�N
	/// </summary>
	/// <param name="deltatime"></param>
	/// <returns></returns>
	bool CheckTrigger(float deltatime);

	void ResetTriggerState();
};

/// <summary>
/// �����N���X
/// </summary>
class FSMStateTrigger {
private:
	FSMState* parentState;//�e�X�e�[�g
	FSMStateTriggerAction* triggerAction;//���������i�f���Q�[�g�j
public:
	FSMStateTrigger();
	~FSMStateTrigger();

	/// <summary>
	/// �p�����[�^��ݒ�i�e�X�e�[�g�Ə����֐��j
	/// </summary>
	/// <param name="parent"></param>
	/// <param name="action"></param>
	void SetTriggerParam(FSMState* parent, FSMStateTriggerAction* action);

	/// <summary>
	/// �������`�F�b�N�i�����֐����Ăяo���j
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


#pragma endregion fsm�i�I�[�g�}�g��

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

#pragma region �G�����f�[�^

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

#pragma endregion �G�����f�[�^

#pragma region �Q�[������

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

#pragma endregion �Q�[������