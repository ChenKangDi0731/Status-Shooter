#include "enemy.h"
#include "manager.h"
#include "player.h"
#include "sceneObjManager.h"
#include "cellSpawnManager.h"

#pragma region body_method
#pragma region enemy_base
Enemy::Enemy() :enemyType(E_CellType::CellType_Enemy) {
	followObj = nullptr;

	cellType = E_CellType::CellType_Enemy;
	//register status : TODO
	status = new Status_Enemy();
	status->needRecordChangingData = false;
	RegisterComponent(E_Component::Component_Status, status);
}

Enemy::~Enemy() {
	followObj = nullptr;

	status = nullptr;
}

void Enemy::SetFollowObj(BaseCell* obj, bool alignPos) {
	if (followObj != nullptr && obj != nullptr && followObj->GetID() == obj->GetID())return;

	followObj = obj;

	if (obj != nullptr && alignPos == true) {
		this->SetPos(obj->GetTransform()->GetPos(),true);
	}
}

void Enemy::DoUpdate(float deltatime) {
	BaseCell::DoUpdate(deltatime);
	if (enable == false)return;
	vector3s pos = this->GetTransform()->GetPos();
	if (followObj != nullptr) {
		vector3s movement = followObj->GetTransform()->GetMovement();
		if (VecLengthS(movement) != 0) {
			pos = pos + movement;
			//this->SetPos(pos);
		}
		else {

			//this->SetPos(followObj->GetTransform()->GetPos());
		}
	}

	//TODO
	if (enable != false) {
		if (status != nullptr && status->IsDead()) {
			if (spawnCounterSign == true) {
				spawnCounterSign = false;
				GameMgr::GetInstance()->cellSpawnerMgr->ChangeSpawnCounter(-1);
			}
			this->SetState(false);
		}
	}
}

void Enemy::DoLateUpdate(float deltatime) {

}

bool Enemy::CheckHasFollowObj() {
	return followObj != nullptr;
}

bool Enemy::CheckIsSameFollowObj(BaseCell* cell) {
	if (cell == nullptr || followObj == nullptr)return false;
	return cell->GetID() == this->cellId;
}


void Enemy::SetEnemyType(E_CellType enemyT)
{
	enemyType = enemyT;
}

E_CellType Enemy::GetEnemyType() {
	return enemyType;
}

bool Enemy::CheckCanMove()
{
	return !lockMove || (forceMovementTimePass >= forceMovementTime);
}
#pragma endregion enemy_base

#pragma region enemy_dragonfly


void Enemy_Dragonfly::ShootBullet()
{
	Player* player = GameMgr::GetInstance()->sceneObjMgr->GetPlayer();
	vector3s shootPos = this->transform->GetPos();
	vector3s shootDir = vector3s(downVector.x, downVector.y, 0.0f);
	if (player != nullptr) {
		vector3s dir2Player = player->GetTransform()->GetPos() - shootPos;
		dir2Player.z = 0.0f;
		dir2Player = VecNormalize(dir2Player);
		shootDir = dir2Player;
	}

	if (status != nullptr) {
		GameMgr::GetInstance()->sceneObjMgr->ShootBullet(this, status->GetValue(E_Status::Status_Atk), E_BulletType::BulletType_Enemy_Normal, shootPos, shootDir, BULLET_ENEMY_NORMAL_MOVE_SPEED);
	}
	else {
		//default param
		GameMgr::GetInstance()->sceneObjMgr->ShootBullet(this, 100.0f, E_BulletType::BulletType_Enemy_Normal, shootPos, shootDir, BULLET_ENEMY_NORMAL_MOVE_SPEED);
	}
}

Enemy_Dragonfly::Enemy_Dragonfly()
{

}

Enemy_Dragonfly::~Enemy_Dragonfly() {

}

void Enemy_Dragonfly::TakeAction(E_ActionType _actionType, float _deltatime)
{
	if (_actionType == E_ActionType::ActionType_Enemy_Attack) {
		ShootBullet();
	}
}


#pragma endregion enemy_dragonfly

#pragma region enemy_firefly

Enemy_Firefly::Enemy_Firefly()
{
}

Enemy_Firefly::~Enemy_Firefly()
{

}

void Enemy_Firefly::SetForceMovement(vector3s dir, float force, float movementTime, bool canMove)
{
	//empty
}

void Enemy_Firefly::TakeAction(E_ActionType _actionType, float _deltatime)
{
	//move2player
}


#pragma endregion enemy_firefly

#pragma endregion body_method
