//===========================================================================
// [ゲームオブジェクトヘッダーファイル]
// ‐ゲームオブジェクトのクラスの定義
//===========================================================================

#pragma once
#ifndef _GAMEOBJECT_H_
#define _GAMEOBJECT_H_

#include "Component.h"

#define LIGHT_RANGE_FIELD 200

#define SHOOT_POINT_OFFSET 20


//オブジェクトの種類
enum class E_CellType :int {
	CellType_None = -1,

	//unit type
	CellType_Player = 0,
	//obstacle
	CellType_Obstacle = 1,
	CellType_Floor = 2,
	CellType_Light = 3,
	CellType_Bullet = 11,
	CellType_Effect = 101,

	//enemy type
	CellType_Enemy = 4,
	CellType_Enemy_None = 5,
	CellType_Enemy_Skirmisher = 6,
	CellType_Enemy_Skirmisher_2 = 7,
	CellType_Enemy_Dragonfly = 8,
	CellType_Enemy_Firefly = 9,


	//other
	CellType_UI = 800,
	CellType_Grass = 901,

	CellType_SceneConstObj = 999,
	CellType_GuideUI = 1000,
};

enum class E_LightType {
	LightType_None = 0,
	LightType_WhiteCicle = 1,
	LightType_BlackCircle = 2,
};

enum class E_ActionType {
	ActionType_Enemy_Attack,
};

//弾の種類
enum class E_BulletType {
	BulletType_Normal,//exp
	BulletType_Laser,//hp
	BulletType_Cannon,//atk
	BulletType_Shotgun,//def

	//敵の弾
	BulletType_Enemy_Normal,
	BulletType_Enemy_Normal_Reflected,
};

enum class E_Component;
enum class E_MovementType;
enum class E_DrawLayer;
enum class E_Dir;
enum class E_Effect;
enum class E_Status;

class Transform;
class DrawData;
class Component;
class Movement;
class Status;
class Status_Player;
class Status_Enemy;

class Light;
class Enemy;

class Bullet;

class UI;

#pragma region base_obj

class BaseCell {
protected:

	std::map<E_Component, Component*> componentDic;

	std::map<E_MovementType, Movement*> movementDic;

	std::map<int, BaseCell*> lightEffectLinkCells;
protected:

	Transform* transform;

	E_DrawLayer drawLayer;
	E_CellType cellType;
	int cellId;

	bool enable;
	DrawData* drawData;

protected:
	//ui
	UI* followUI;
	bool releaseUIWhenInactive;

protected:
	int checkWhiteLightTimes;
	int curCheckWhiteLightCounter;
	int checkBlackLightTimes;
	int curCheckBlackLightCounter;

	float colorLerpOffset;

	bool lockMove;
	vector3s forceMoveDir;
	float forceMoveSpeed;
	float forceReduceFactor;
	float forceMovementTime;
	float forceMovementTimePass;

public:
	bool canChangeColorLerpSign;
	bool deadlyCell;
	bool spawnCounterSign;
public:

	BaseCell();
	virtual ~BaseCell();

	virtual void DoInit();
	virtual void UnInit();

	/// <summary>
	/// Property
	/// </summary>
	/// <returns></returns>
	int GetID() {
		return cellId;
	}

	void SetID(int id) {
		cellId = id;
	}

	void SetState(bool state) {
		enable = state;

		if (state == false) {
			if (releaseUIWhenInactive) {
				followUI = nullptr;

			}

			lockMove = false;
		}

	}

	bool CheckState() {
		return enable;
	}

	void SetCellType(E_CellType cType) {
		cellType = cType;
	}

	E_CellType GetCellType() {
		return cellType;
	}

	void SetDrawLayer(E_DrawLayer layer) {
		drawLayer = layer;
	}

	E_DrawLayer GetDrawLayer() {
		return drawLayer;
	}

	/// <summary>
	/// コンポーネント函数
	/// </summary>
	/// <param name="componentType"></param>
	/// <returns></returns>
	Component* GetComponent(E_Component componentType) {
		std::map<E_Component, Component*>::iterator iter = componentDic.find(componentType);
		if (iter == componentDic.end()) {
			return nullptr;
		}
		else {
			return iter->second;
		}
	}
	Transform* GetTransform() {
		return transform;
	}
	bool RegisterComponent(E_Component componentType, Component* componentEntity, bool replace = false);

	void RemoveComponent(E_Component componentType) {
		std::map<E_Component, Component*>::iterator iter = componentDic.find(componentType);
		if (iter == componentDic.end()) {
			return;
		}

		componentDic.erase(componentType);
	}

	void SetDrawData(DrawData* data) {
		//save pointer
		drawData = data;
	}

	DrawData* GetDrawData() {
		return drawData;
	}
	virtual void DoUpdate(float deltatime);
	virtual void DoLateUpdate(float deltatime);

	/// <summary>
	/// トランスフォーマーコンポーネント 函数（
	/// </summary>
	/// <param name="pos"></param>
	void SetPos(vector3s pos, bool updatePrePos);
	void SetRot(vector3s rot);
	void SetScale(vector3s scale);
	void SetTransform(const Transform& t);
	void ResetPos(bool x, bool y);
	vector2s GetSize(bool withScale);
	virtual void SetForceMovement(vector3s dir, float force, float movementTime, bool canMove);

	//スペシャルアニメション函数（平行移動など
	void AddMovement(Movement* movement);
	void RemoveMovement(E_MovementType movementType);
	void StartMovement(bool reset);
	void StopMovement(bool stopForward);
	void SetMovementState(bool state);
	std::map<E_MovementType, Movement*>& GetMovementDic();

	//結び付けられたUIに関する関数（ダメージUIなど
	void SetFollowUI(UI* ui, bool releaseWhenInactive);
	UI* GetFollowUI();

	//ガンライト
	void SetLightEnterSign(bool isWhiteLight);
	void ResetLightSign();

	virtual void RecordCurState();//オブジェクトのパラメータを保存し、デフォルトパラメータとして扱う
	virtual void Reset();//リセット（デフォルトパラメータを使用
	virtual bool CheckCanMove();//移動できるかどうかをチェック

	virtual void TakeAction(E_ActionType _actionType,float _deltatime) {}

	void SetSpawnCounterSign(bool sign);

	static bool CheckInScreenRect(BaseCell* cell, bool checkWholeCell);

};

class Light : public BaseCell {

private:
	BaseCell* followObj;
	bool lightState;

	E_LightType lightType;
	vector2s lightSize;

public:
	bool isPlayerLight = false;
public:

	Light();
	~Light();

	void SetLightSize(float size, bool xAxis);
	void SetLightSize(vector2s size);

	void SetLightState(bool state) {
		lightState = state;
	}

	bool CheckLightState();

	void SetLightType(E_LightType lType);

	void SetFollowObj(BaseCell* obj, bool alignPos);

	void DoUpdate(float deltatime);

	void DoLateUpdate(float deltatime) {

	}

	vector2s GetLightSize() {
		if (enable) {
			return lightSize;
		}
		else {
			return vector2s(0, 0);
		}
	}

	bool CheckHasFollowObj();
	bool CheckIsSameFollowObj(BaseCell* cell);
};

#pragma region 弾
class Bullet :public BaseCell {
protected:

	BaseCell* attacker;

	E_BulletType bulletType;
	vector3s dir;

	vector3s sizeScale;

	float bulletLifeTime;
	float bulletLifeTimePass;

	int curMaxReflectCount;
	int reflectCount;
	std::list<int> reflectTempIdArray;
public:

	vector3s shootPos;
	bool bulletState;
public:
	float bulletSpeed;
	int bulletCurAtk;

public:
	Bullet();
	Bullet(E_BulletType bulletT);
	virtual ~Bullet();

	void BindAttacker(BaseCell* cell);
	BaseCell* GetAttacker();

	virtual void DoInit(bool active);
	virtual void DoUpdate(float deltatime);
	virtual void DoLateUpdate(float deltatime);

	virtual void SetBulletLifeTime(float lifeTime);
	virtual void SetStartPos(vector3s startPos);
	virtual void SetDir(vector3s direction);
	virtual void SetSizeScale(vector3s scale);
	void SetSpeed(float speed);
	void SetBulletType(E_BulletType bulletT);
	E_BulletType GetBulletType();

	virtual void SetMaxReflectCount(int _count);

	virtual void Reset();

	//extra
	virtual void CollisionTrigger();
	virtual bool CheckReflectID(int id);
	virtual bool ReflectCountIncrease(int id);
	vector3s GetCurDir();

};

class Bullet_Normal :public Bullet {
public:
	Bullet_Normal();
	~Bullet_Normal();

	void DoUpdate(float deltatime);
	void DoLateUpdate(float deltatime);
};

class Bullet_Shotgun :public Bullet {
private:
	float curSpeed;
public:
	Bullet_Shotgun();
	~Bullet_Shotgun();

	void DoUpdate(float deltatime);
	void DoLateUpdate(float deltatime);

	void Reset();
};

class Bullet_Laser :public Bullet {
private:

public:
	Bullet_Laser();
	~Bullet_Laser();

	void DoUpdate(float deltatime);
	void DoLateUpdate(float deltatime);

	void SetBulletLifeTime(float lifeTime);

	void SetStartPos(vector3s startPos);

	vector3s GetCorrectRot();

	void Reset();
};

class Bullet_Cannon :public Bullet {
private:
	float explodeRange;
	float curSpeed;
public:
	Bullet_Cannon();
	~Bullet_Cannon();

	void DoUpdate(float deltatime);
	void DoLateUpdate(float deltatime);

	void Reset();

	void SetExplodeRange(float range);
	float GetExplodeRange();
};

#pragma endregion 弾

#pragma region エフェクト

class Effect : public BaseCell {
protected:
	E_Effect effectType;

public:
	Effect();
	Effect(E_Effect effectT);
	virtual ~Effect();

	void SetEffectType(E_Effect effectT);
	E_Effect GetEffectType();

	virtual void StartEffect();
	virtual void StopEffect();
	virtual void PauseEffect(bool pause);

};

#pragma endregion エフェクト





#endif
