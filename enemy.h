#pragma once
#include "gameObject.h"

enum class E_CellType;

#pragma region enemy
class Enemy :public BaseCell {
protected:
	E_CellType enemyType;

	Status* status;

	BaseCell* followObj;
public:
	bool adjustPosSign = false;
public:
	Enemy();
	virtual ~Enemy();

	virtual void SetForceMovement(vector3s dir, float force, float movementTime, bool canMove) {
		BaseCell::SetForceMovement(dir, force, movementTime, canMove);
	}

	void SetFollowObj(BaseCell* obj, bool alignPos);
	virtual void DoUpdate(float deltatime);
	virtual void DoLateUpdate(float deltatime);
	bool CheckHasFollowObj();
	bool CheckIsSameFollowObj(BaseCell* cell);

	void SetEnemyType(E_CellType enemyT);
	E_CellType GetEnemyType();
	//other
	bool CheckCanMove();

	virtual void TakeAction(E_ActionType _actionType, float _deltatime) {

	}
};

class Enemy_Dragonfly :public Enemy {
private:

private:
	//action
	void ShootBullet();
public:
	Enemy_Dragonfly();
	~Enemy_Dragonfly();

	void TakeAction(E_ActionType _actionType, float _deltatime);

};

class Enemy_Firefly : public Enemy {
private:

public:
	Enemy_Firefly();
	~Enemy_Firefly();

	void SetForceMovement(vector3s dir, float force, float movementTime, bool canMove);

	void TakeAction(E_ActionType _actionType, float _deltatime);
};

#pragma endregion enemy