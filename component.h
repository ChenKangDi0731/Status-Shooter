//===========================================================================
// [コンポーネント定義]
// 1．Transform：移動管理
// 2．Animation：アニメション管理
// 3．AIComponent：AI管理
// 4．Status：キャラクターステータス管理
// 5．Collider：当たり判定管理
//===========================================================================
#pragma once
#ifndef _COMPONENT_H_
#define _COMPONENT_H_

#include"main.h"
#pragma warning(disable:4244)
#pragma warning(disable:4065)
enum class E_Component {
	Component_None = -1,
	Component_Transform = 0,
	Component_Animation = 1,
	Component_Collider = 2,
	Component_Status = 3,
	Component_AI = 4,
};

enum class E_Anim {
	Anim_None,
	Anim_Test_1,
	Anim_Test_2,
	Anim_Test_3,


	//base
	Anim_Idle,
	Anim_Move,
	Anim_Attack,
	Anim_Damage,

};

enum class E_MovementType {
	MovementType_None,
	MovementType_Axis,
	MovementType_Appear,
	MovementType_Scale,
};

enum class E_LightEffect {
	LightEffect_None,

	LightEffect_Active,//continue
	LightEffect_Inactive,//pause
	LightEffect_RestoreAcitve,//reset & active
	LightEffect_RestoreInactive,//reset & inactive

	LightEffect_ActiveForward,//direction forward
	LightEffect_ActiveBackward,//direction backward
};

enum class E_Status {
	Status_None = -2,
	Status_All = -1,
	Status_Hp = 0,
	Status_Atk = 1,
	Status_Def = 2,
	Status_Exp = 3,

	Status_Max_Hp = 10,
	Status_Max_Atk = 11,
	Status_Max_Def = 12,
	Status_Max_Exp = 13,

	Status_Consume_Hp = 20,
	Status_Consume_Atk = 21,
	Status_Consume_Def = 22,
	Status_Consume_Exp = 23,

	//other
	Status_Level = 101,
	Status_Invincible_Time = 102,

	//out of status
	Status_Hp_Base_Dmg = 201,
	Status_Atk_Base_Dmg = 202,
	Status_Def_Base_Dmg = 203,
	Status_Exp_Base_Dmg = 204,

};

inline int I_Lerp(float, float, float);

class DrawData;
class BaseCell;
class AnimFSM;
class Animation;
class AnimClip;
class Collider;

class FSMSystem;

struct CardEffectData;

enum class E_SceneLayer;
enum class E_Timer;
enum class E_CardType;

#pragma region base_component
/// <summary>
/// コンポーネント親クラス
/// </summary>
class Component {
protected:
	E_Component componentType;//コンポーネントの種類

	BaseCell* parent;//親ゲームオブジェクト
	bool enable;//使われてるかどうか
public:
	Component();
	virtual ~Component();
	Component(BaseCell* p, E_Component cType);

	E_Component GetComponentType() const {
		return componentType;
	}

	virtual void SetParent(BaseCell* p);
	virtual BaseCell* GetParent();

	//=============================================コンポーネント状態フラグを設定
	virtual void SetComponentState(bool state) {
		enable = state;
	}
	virtual bool CheckComponentState() const {
		return enable;
	}
	//=============================================

	virtual void DoInit() {
	}

	virtual void UnInit() {

	}

	virtual void DoUpdate(float deltatime) {

	}

	virtual void DoLastUpdate(float deltatime) {

	}

	virtual void Copy(Component& src) {

	}

	virtual void RecordCurState() {

	}
	virtual void Reset() {

	}

};


/// <summary>
/// 移動を制御するコンポーネント
/// </summary>
class Transform :public Component {
public:
	//デフォルトパラメータ
	vector3s defaultPos;
	vector3s defaultRot;
	vector3s defaultScale;

	//今のパラメータ
	vector3s m_pos;
	vector3s m_rot;
	vector3s m_scale;

	bool tempPosSign = false;
	vector3s tempPos;
	vector3s prePos;

public:
	Transform();

	Transform(vector3s pos, vector3s rot, vector3s scale);

	void DoInit();

	void UnInit();

	vector3s GetPos() {
		return m_pos;
	}

	vector3s GetPrePos() {
		return prePos;
	}
	
	// 移動量を取得
	vector3s GetMovement() {
		vector3s result = m_pos - prePos;
		return result;
	}

	vector3s GetRot() {
		return m_rot;
	}

	vector3s GetScale() {
		return m_scale;
	}

	void SetPos(vector3s p, bool updatePrePos = true);

	void SetRot(vector3s r) {
		m_rot = r;
	}

	void SetScale(vector3s s) {
		m_scale = s;
	}

	void ResetPos(bool x, bool y) {
		if (x) {
			m_pos.x = prePos.x;
		}
		if (y) {
			m_pos.y = prePos.y;
		}
	}

	void Copy(Component& src);

	void RecordCurState();
	void Reset();

	static bool AdjustPosition(BaseCell* cell, vector3s center, vector2s rect, bool inner);
	static void AdjustPositionDirection(vector3s& pos, vector2s size, vector3s rectPos, vector2s rectSize, bool isHorizontal);

	static void AdjustZPos(BaseCell* cell);

};

union ColliderSize {
	vector2s size;
	float radius;
};

enum class E_ColliderType {
	Collider_None = -1,
	Collider_Rect = 0,
	Collider_Circle = 1,
};

struct ColliderData {
	int id;
	Collider* parent;//所有者ポインター
	bool isTrigger;

	E_ColliderType colliderType;//コライダーの種類　：円　OR　四角形
	vector2s size;//コライダーボックスのサイズ
	vector3s offset;//コライダーボックスの位置はオブジェクトの原点よりどれくらいずれてるか

	ColliderData();
	ColliderData(const ColliderData& src);
	/// <summary>
	/// 
	/// </summary>
	/// <param name="i">　ID</param>
	/// <param name="p">　所有者ポインター（コライダーコンポーネント）</param>
	/// <param name="trigger"></param>
	/// <param name="cType">　コライダーの種類（円OR四角形）</param>
	/// <param name="s">　コライダーボックスのサイズ</param>
	/// <param name="o">	コライダーボックスの位置はオブジェクトの原点よりどれくらいずれてるか</param>
	ColliderData(int i, Collider* p, bool trigger, E_ColliderType cType, vector2s s, vector3s o);
	~ColliderData();

	void Copy(const ColliderData& src);
};

/// <summary>
/// コライダーコンポーネント（当たり判定に使う
/// </summary>
class Collider :public Component {
private:
	std::vector<ColliderData*> colliderDataArray;//コライダーボックスのデータのリスト（一つのゲームオブジェクトに複数のコライダーを設定することは可能
public:
	D3DXCOLOR colliderColor = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);//コライダー枠の色（デバッグ用

public:
	Collider();
	Collider(const Collider& collider);

	~Collider();

	void DoInit();
	void UnInit();

	ColliderData* GetColliderData(int id);
	void AddColliderData(ColliderData* data);

	std::vector<ColliderData*>& GetColliderDataArray()
	{
		return colliderDataArray;
	}

	int GetColliderDataArraySize()
	{
		return (int)colliderDataArray.size();
	}

	void ClearColliderDataArray();
	void SetColliderSize(vector2s size, int colliderDataIndex);

	void Copy(const Component& dest);
	void RecordCurState() {
	}
	void Reset() {
	}

	//=====================================当たり判定函数
	static bool CollisionDetect(BaseCell* cell_1, BaseCell* cell_2);//当たり判定：円と円、円と四角形、四角形と四角形
	static bool CollisionDetect(BaseCell* cell_1, Transform* transform_2, Collider* c_2);
	static bool RectCircleCollisionDetect(Transform& transform_1, Transform& transform_2, ColliderData& data_1, ColliderData& data_2);
	static bool RectCollisionDetect(Transform& transform_1, Transform& transform_2, ColliderData& data_1, ColliderData& data_2);
	static bool CircleCollisionDetect(Transform& transform_1, Transform& transform_2, ColliderData& data_1, ColliderData& data_2);

	static bool IsContainPoint(BaseCell* cell, vector3s point);
	static bool IsRectContainPoint(Transform* transform, ColliderData& data, vector3s point);
	static bool IsRectContainPoint(vector3s pos, vector2s size, vector3s rot, vector3s point);
	static bool IsCircleContainPoint(Transform* transform, ColliderData& data, vector3s point);
	static bool IsCircleContainPoint(vector3s pos, float radius, vector3s point);

	static bool SceneObjCollisionDetect(BaseCell* cell_1, Transform* transform_2, Collider* c_2);

	static bool LineCollisionDetect(BaseCell* cell_1, BaseCell* cell_2, vector3s& collisionPoint, vector3s& collisionNormal);
	static bool LineCollisionDetect(vector3s startPos, vector3s endPos, BaseCell* cell, vector3s& collisionPoint, vector3s& collisionNormal);
	static bool LineCollisionDetect(vector3s startPos_1, vector3s endPos_1, vector3s startPos_2, vector3s endPos_2, vector3s& collisionPos);
};
#pragma endregion base_component

#pragma region other_component

class Status : public Component {
protected:

	//base
	bool isDead;

	float maxHp;
	float curHp;
	float maxAtk;
	float curAtk;
	float maxDef;
	float curDef;
protected:
	std::list<E_Status> changingStatusQueue;

	bool dataChangeSign;

	bool isInvincible;
	float invincibleTime;
	float invincibleTimePass;

protected:
	void EnqueueStatus(E_Status status);

public:
	bool needRecordChangingData;
public:
	Status();
	virtual ~Status();

	virtual void SetDefaultData(E_Status statusType, float value, bool changeCur);
	virtual void ChangeDefaultData(E_Status statusType, float increaseValue, bool changeCur, bool lockMin);
	virtual void ChangeData(E_Status statusType, float increaseValue, bool lockMin);

	virtual void ResetData(E_Status statusType);

	void SetInvincible(float invincibleT);
	bool CheckCanBeAttack();

	virtual void DoUpdate(float deltatime);
	virtual void DoLateUpdate(float deltatime);

	virtual int GetValue(E_Status statusType);

	bool IsDead();

	std::list<E_Status>& GetChangingStatusQueue();
};

class Status_Player : public Status {
private:
	int curLv;

	int defaultExp;
	int maxExp;
	int curExp;

private:
	//弾の消耗量
	int hpBulletConsume;
	int atkBulletConsume;
	int defBulletConsume;
	int expBulletConsume;

private:
	std::map<int, CardEffectData*> statusCardDic;
	std::map<E_CardType, CardEffectData*> extraCardDic;

public:
	Status_Player();
	~Status_Player();

	void SetDefaultData(E_Status statusType, float value, bool changeCur);
	void ChangeDefaultData(E_Status statusType, float increaseValue, bool changeCur, bool lockMin);
	void ChangeData(E_Status statusType, float increaseValue, bool lockMin);

	void DoUpdate(float deltatime);

	void ResetData(E_Status statusType);

	bool ShootStatus(E_Status statusType);

	int GetValue(E_Status statusType);

	void ResetLevel();

	//card
	void AddCard(bool isStatusCard, int lv, CardEffectData* card);
};

class Status_Enemy : public Status {
private:
	int containExp;

public:
	Status_Enemy();
	~Status_Enemy();

	void SetDefaultData(E_Status statusType, float value, bool changeCur);
	void ChangeDefaultData(E_Status statusType, float increaseValue, bool changeCur, bool lockMin);
	void ChangeData(E_Status statusType, float increaseValue, bool lockMin);

	void ResetData(E_Status statusType);
};

class AIComponent :public Component {
private:

	FSMSystem* fsmSystem;

public:
	AIComponent();
	~AIComponent();

	void DoInit();
	void UnInit();

	void DoUpdate(float deltatime);

	void Reset();

	FSMSystem* GetFSMSystem();
};

#pragma endregion other_component

#pragma region anim

#pragma region anim_callback（コールバック関数

struct Anim_CallBack {

	virtual void Func(AnimClip* baseClip) {

	}
};

struct Anim_Effect_Normal_CB : public Anim_CallBack {
	void Func(AnimClip* baseClip);
};

#pragma endregion anim_callback（コールバック関数


class AnimClip {
private:
	BaseCell* parent;

	E_Anim animType;

	vector2s frameIndexRange;
	int curIndex;

	float animTime;
	float animTime_I;
	float animTimePass;
	bool animState;//play state:is End
	bool dir;

	bool loop;

public:
	//callback
	Anim_CallBack* animStart_Callback;
	Anim_CallBack* animEnd_Callback;

public:
	AnimClip() :animType(E_Anim::Anim_None), frameIndexRange(vector2s(0, 0)), curIndex(0), animTime(0.0f), animTimePass(0.0f), loop(true), dir(true) {
		animStart_Callback = nullptr;
		animEnd_Callback = nullptr;
	}

	AnimClip(const AnimClip& src) {
		this->animType = src.animType;
		this->frameIndexRange = src.frameIndexRange;
		this->curIndex = src.curIndex;
		this->animTime = src.animTime;
		this->animTime_I = src.animTime_I;
		this->animTimePass = src.animTimePass;
		this->animState = src.animState;
		this->dir = src.dir;
		this->loop = src.loop;

		//this->parent = src.parent;
		this->parent = nullptr;
		this->animStart_Callback = src.animStart_Callback;
		this->animEnd_Callback = src.animEnd_Callback;
	}

	~AnimClip();

	void SetParent(BaseCell* p) {
		parent = p;
	}
	BaseCell* GetParent() {
		return parent;
	}
	void SetAnimParam(E_Anim animT, vector2s indexRange, float time, bool animDir, bool isLoop) {
		animType = animT;
		frameIndexRange = indexRange;
		animTime = time;
		dir = animDir;
		loop = isLoop;

		animTime_I = animTime <= 0 ? 1 : 1.0f / animTime;

		animTimePass = 0.0f;
		animState = false;
	}

	E_Anim GetAnimType() {
		return animType;
	}

	void StartAnim() {
		if (dir) {
			curIndex = frameIndexRange.x + 0.5f;
		}
		else {
			curIndex = frameIndexRange.y + 0.5f;
		}
		animState = true;
		OnAnimStart();
	}
	void SetAnimState(bool state) {
		animState = state;
	}
	void StopAnim() {
		animState = false;
		if (dir) {
			curIndex = frameIndexRange.x + 0.5f;
		}
		else {
			curIndex = frameIndexRange.y + 0.5f;
		}
		OnAnimEnd();
	}

	void DoUpdate(float deltatime);

	void DoLateUpdate(float deltatime) {
		if (animState) {


		}
	}


	//callback
	void OnAnimStart() {
		animTimePass = 0.0f;
		if (dir) {
			curIndex = frameIndexRange.x + 0.5f;
		}
		else {
			curIndex = frameIndexRange.y + 0.5f;
		}
		animState = true;

		if (animStart_Callback != nullptr) {
			animStart_Callback->Func(this);
		}
	}

	void OnAnimEnd() {
		if (animEnd_Callback != nullptr) {
			animEnd_Callback->Func(this);
		}
	}
};

class AnimFSM {
private:

	Animation* parent;

	std::map<E_Anim, AnimClip*> animDic;
	E_Anim defaultAnim;
	E_Anim curAnim;
	AnimClip* curAnimClip;

	bool animState;
public:
	AnimFSM() :parent(nullptr), curAnim(E_Anim::Anim_None), defaultAnim(E_Anim::Anim_None), animState(true) {
		animDic.clear();
	}

	~AnimFSM() {
		//delete Anim
		for (auto& cur : animDic) {
			if (cur.second != nullptr) {
				delete cur.second;
			}
		}

		animDic.clear();

		parent = nullptr;
		curAnimClip = nullptr;
	}

	void DoInit() {
		SwitchAnim(defaultAnim, true, false);
		SetAnimState(true);
	}

	void SetParent(Animation* p) {
		parent = p;
	}

	void SetAnimState(bool state) {
		animState = state;
	}

	void SwitchAnim(E_Anim animType, bool resetAnim, bool lastClipEndCB) {
		//bind baseCell first

		if (animType == curAnim && resetAnim != true) {
			return;
		}

		std::map<E_Anim, AnimClip*>::iterator iter = animDic.find(animType);
		AnimClip* targetAnimClip = nullptr;
		if (iter != animDic.end()) {
			//switch
			targetAnimClip = animDic[animType];
		}

		if (curAnimClip != nullptr && lastClipEndCB == true) {
			curAnimClip->StopAnim();
		}
		curAnimClip = targetAnimClip;
		if (curAnimClip != nullptr) {
			curAnimClip->StartAnim();

			curAnim = animType;
		}
	}

	void RegisterAnim(E_Anim animType, AnimClip* anim, bool setDefaultAnim = false);

	void DoUpdate(float deltatime) {
		if (animState == false)return;
		if (curAnimClip != nullptr) {
			curAnimClip->DoUpdate(deltatime);
		}
	}

	void DoLateUpdate(float deltatime) {
		if (animState == false)return;
		if (curAnimClip != nullptr) {
			curAnimClip->DoLateUpdate(deltatime);
		}
	}

};


class Animation : public Component {
protected:
	AnimFSM animFSM;

public:
	Animation() {
		animFSM.SetParent(this);
		componentType = E_Component::Component_Animation;
	}
	~Animation() {

	}

	void RegisterAnim(E_Anim anim, AnimClip* animClip, bool setDefault) {
		animFSM.RegisterAnim(anim, animClip, setDefault);
	}

	void DoInit();
	void UnInit();

	void SetParent(BaseCell* p);

	void DoUpdate(float deltatime);
	void DoLateUpdate(float deltatime);

	void Copy(Component& src);

	void SwitchAnim(E_Anim targetAnim, bool resetAnim, bool lastClipEndCB);
	void SwitchToDefaultAnim();
	void StopAnim();
	void PauseAnim(bool pause);
};


#pragma endregion anim

#pragma region movement（スペシャルアニメション

class Movement {
protected:

	BaseCell* parent;

	E_MovementType movementType;

	bool isLoop;
	bool defaultForward;
	bool forward;

	float movementTime;
	float movementTime_I;
	float moveTimePass;

	bool movementState;
	bool movementDone;


public:
	Movement();
	Movement(BaseCell* p, E_MovementType mT, bool movementDefaultDir, bool loop, bool defaultActive, float movementT);
	virtual ~Movement();

	virtual void DoInit();

	virtual void StartMovement(bool reset);
	virtual void SetMovementState(bool state);
	virtual void StopMovement(bool stopForward);
	virtual void Reset();
	virtual void ResetDefault();

	virtual void SetMovementTime(float time);

	virtual void DoUpdate(float deltatime) {

	}

	virtual void DoLateUpdate(float deltatime) {

	}

	virtual void ApplyMovement() {

	}

	void SetParent(BaseCell* p);

	bool CheckMovementState() {
		return movementState;
	}
	bool CheckDefaultForward();
	E_MovementType GetMovementType() {
		return movementType;
	}

	//other method
	virtual void SetVecTParam(int index, vector3s param) {

	}

};

/// <summary>
/// 平行移動アニメション
/// </summary>
class Movement_Axis :public Movement {
private:
	bool isRelatedPos;
	vector3s defaultPos;

	vector3s startPos;
	vector3s endPos;

	vector3s curPos;
public:
	Movement_Axis();
	Movement_Axis(BaseCell* p, bool isRelated, bool movementDefaultDir, bool isLoop, bool defaultActive, float movementT, vector3s defaultP, vector3s startP, vector3s endP);
	~Movement_Axis();

	void StartMovement(bool reset);
	void SetMovementState(bool state);
	void StopMovement(bool stopForward);
	void Reset();
	void ResetDefult();

	void DoUpdate(float deltatime);
	void DoLateUpdate(float deltatime);

	void ApplyMovement();

	void SetVecTParam(int index, vector3s param);
};

/// <summary>
/// 透明度アニメション
/// </summary>
class Movement_Appear :public Movement {
private:
	bool stateChange;
	bool curCellState;
	float waitTimePass;

public:
	vector4s forwardColor;
	vector4s backwardColor;
	vector4s curColor;
	float waitTime;
public:

	Movement_Appear();
	Movement_Appear(BaseCell* p, bool defaultState, bool isLoop, bool defaultActive, float movementT, float waitT);
	~Movement_Appear();

	void StartMovement(bool reset);
	void SetMovementState(bool state);
	void StopMovement(bool stopForward);
	void Reset();
	void ResetDefault();

	void DoUpdate(float deltatime);
	void DoLateUpdate(float deltatime);

	void ApplyMovement();

	void SetMovementColor(vector4s fColor, vector4s bColor);//temp
	void SetVecTParam(int index, vector3s param);
};

/// <summary>
/// スケールアニメション
/// </summary>
class Movement_Scale :public Movement {
private:
	vector3s curScale;
	vector3s defaultScale;
	vector3s targetScale;
	float delayTime;
	float delayTimePass;
public:
	Movement_Scale();
	Movement_Scale(BaseCell* p, bool defaultState, vector3s defaultS, vector3s targetS, float movementT, float delayT);
	~Movement_Scale();

	void StartMovement(bool reset);
	void SetMovementState(bool state);
	void StopMovement(bool stopForward);
	void Reset();
	void ResetDefault();

	void SetMovementTime(float time);

	void DoUpdate(float deltatime);
	void DoLateUpdate(float deltatime);

	void ApplyMovement();

};

#pragma endregion movement（スペシャルアニメション

#endif
