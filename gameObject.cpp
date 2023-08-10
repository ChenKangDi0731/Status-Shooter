#include "main.h"
#include "gameLogic.h"
#include "gameObject.h"
#include "sceneObjManager.h"
#include "battleManager.h"

#pragma region baseCell_method
BaseCell::BaseCell() :cellId(-1), enable(true)
{
	componentDic.clear();

	deadlyCell = false;
	checkWhiteLightTimes = CHECK_LIGHT_ENTER_TIMES;
	curCheckWhiteLightCounter = 0;
	checkBlackLightTimes = CHECK_LIGHT_ENTER_TIMES;
	curCheckWhiteLightCounter = 0;

	drawLayer = E_DrawLayer::DrawLayer_Top;//default top
	cellType = E_CellType::CellType_None;
	//create drawData

	//add transform component
	Transform* transformComponent = new Transform();
	//transformComponent->SetParent(this);

	transform = transformComponent;

	movementDic.clear();
	lightEffectLinkCells.clear();
	if (RegisterComponent(E_Component::Component_Transform, transformComponent) == false && transformComponent != nullptr) {
		delete transformComponent;
	}

	canChangeColorLerpSign = true;
	colorLerpOffset = 1.0f;

	lockMove = false;
	forceMoveDir = vector3s(0, 0, 0);
	forceMoveSpeed = 0.0f;
	forceReduceFactor = 0.0f;
	forceMovementTime = 0.0f;
	forceMovementTimePass = 0.0f;

	followUI = nullptr;

	drawData = nullptr;

	spawnCounterSign = false;
}

BaseCell::~BaseCell()
{
	//delete all componenet
	std::map<E_Component, Component*>::iterator iter;
	for (iter = componentDic.begin(); iter != componentDic.end(); iter++) {
		if (iter->second != nullptr) {
			Component* temp = iter->second;
			iter->second = nullptr;
			delete temp;
		}
	}

	//delete all movement
	std::map<E_MovementType, Movement*>::iterator iter_movement;
	for (iter_movement = movementDic.begin(); iter_movement != movementDic.end(); iter_movement++) {
		if (iter_movement->second != nullptr) {
			delete iter_movement->second;
		}
	}

	componentDic.clear();
	movementDic.clear();
	lightEffectLinkCells.clear();

	if (drawData != nullptr) {
		delete drawData;
		drawData = nullptr;
	}

	followUI = nullptr;
	releaseUIWhenInactive = true;
}

void BaseCell::DoInit() {
	//init compoennt
	std::map<E_Component, Component*>::iterator iter;
	for (iter = componentDic.begin(); iter != componentDic.end(); iter++) {
		if (iter->second != nullptr && iter->second->CheckComponentState()) {
			iter->second->DoInit();
		}
	}

	//init movement
	std::map<E_MovementType, Movement*>::iterator iter_movement;
	for (iter_movement = movementDic.begin(); iter_movement != movementDic.end(); iter_movement++) {
		if (iter_movement->second != nullptr) {
			iter_movement->second->DoInit();
		}
	}

}

void BaseCell::UnInit() {

}


bool BaseCell::RegisterComponent(E_Component componentType, Component* componentEntity, bool replace)
{

	if (componentEntity == nullptr) {
		//cout<<"error\n";
		return false;
	}

	std::map<E_Component, Component*>::iterator iter = componentDic.find(componentType);
	if (iter != componentDic.end()) {
		//already had same component
		if (replace) {
			componentDic[componentType] = componentEntity;
		}
		else {
			return false;
		}
	}
	else {
		componentDic[componentType] = componentEntity;
	}

	componentEntity->SetParent(this);

	return true;

}

/// <summary>
/// Transform interface
/// </summary>
/// <param name="pos"></param>
void BaseCell::SetPos(vector3s pos, bool updatePrePos = true)
{
	//Transform* transform = (Transform*) GetComponent(E_Component::Component_Transform);
	if (transform == nullptr || lockMove == true) {
		//cout<<"error\n";
		return;
	}

	transform->SetPos(pos, updatePrePos);
}

void BaseCell::SetRot(vector3s rot)
{
	//Transform* transform = (Transform*)GetComponent(E_Component::Component_Transform);
	if (transform == nullptr) {
		//cout<<"error\n";
		return;
	}

	transform->SetRot(rot);
}

void BaseCell::SetScale(vector3s scale)
{
	//Transform* transform = (Transform*)GetComponent(E_Component::Component_Transform);
	if (transform == nullptr) {
		//cout<<"error\n";
		return;
	}

	transform->SetScale(scale);
}

void BaseCell::SetTransform(const Transform& t)
{
	if (transform != nullptr) {
		transform->SetPos(t.m_pos, true);
		transform->SetRot(t.m_rot);
		transform->SetScale(t.m_scale);
	}
}

void BaseCell::ResetPos(bool x, bool y)
{
	if (transform == nullptr)return;

}

vector2s BaseCell::GetSize(bool withScale = true)
{
	if (drawData == nullptr) {
		if (withScale) {
			return transform == nullptr ? vector2s(0, 0) : vector2s(transform->GetScale().x, transform->GetScale().y);
		}
		else {
			return vector2s(0, 0);
		}
	}
	else {
		if (withScale) {
			return transform == nullptr ? drawData->GetSize() : drawData->GetSize() * vector2s(transform->GetScale().x, transform->GetScale().y);
		}
		else {
			return drawData->GetSize();
		}
	}
}

void BaseCell::SetForceMovement(vector3s dir, float force, float movementTime, bool canMove)
{
	forceMoveDir = VecNormalize(dir);
	forceMoveSpeed = force;
	forceMovementTime = movementTime;
	forceMovementTimePass = 0.0f;

	lockMove = !canMove;
}

void BaseCell::DoUpdate(float deltatime)
{
	if (enable == false)return;

	//compoennt update
	std::map<E_Component, Component*>::iterator iter;
	for (iter = componentDic.begin(); iter != componentDic.end(); iter++) {
		if (iter->second == nullptr || iter->second->CheckComponentState() == false) {
			continue;
		}

		iter->second->DoUpdate(deltatime);
	}

	//movement update
	std::map<E_MovementType, Movement*>::iterator iter_movement;
	for (iter_movement = movementDic.begin(); iter_movement != movementDic.end(); iter_movement++) {
		if (iter_movement->second == nullptr)continue;
		iter_movement->second->DoUpdate(deltatime);
	}

	//force move
	if (lockMove) {
		if (forceMovementTimePass > forceMovementTime) {
			lockMove = false;
		}
		else {
			forceMovementTimePass += deltatime;

			if (transform != nullptr) {
				vector3s curPos = transform->GetPos();
				vector3s movement = forceMoveDir * (forceMoveSpeed * deltatime);
				curPos += movement;
				if (cellType != E_CellType::CellType_Enemy) {
					AdjustScreenPoint(curPos, this->GetSize(true));
				}

				this->transform->SetPos(curPos, true);

				forceMoveSpeed *= REPULSE_REDUCE_FACTOR;
			}
		}
	}
}

void BaseCell::DoLateUpdate(float deltatime)
{
	std::map<E_Component, Component*>::iterator iter;
	for (iter = componentDic.begin(); iter != componentDic.end(); iter++) {
		if (iter->second == nullptr || iter->second->CheckComponentState() == false) {
			continue;
		}

		iter->second->DoLastUpdate(deltatime);
	}

	//movement update
	std::map<E_MovementType, Movement*>::iterator iter_movement;
	for (iter_movement = movementDic.begin(); iter_movement != movementDic.end(); iter_movement++) {
		if (iter_movement->second == nullptr)continue;
		iter_movement->second->DoLateUpdate(deltatime);
	}
}

void BaseCell::AddMovement(Movement* movement)
{
	if (movement == nullptr)return;

	std::map<E_MovementType, Movement*>::iterator iter = movementDic.find(movement->GetMovementType());
	if (iter == movementDic.end()) {
		movementDic[movement->GetMovementType()] = movement;
	}
	else {
		//error
		return;
	}
}

void BaseCell::RemoveMovement(E_MovementType movementType) {
	if (movementDic.size() == 0)return;

	std::map<E_MovementType, Movement*>::iterator iter = movementDic.find(movementType);
	if (iter != movementDic.end()) {
		Movement* tempMovement = iter->second;
		movementDic.erase(iter);

		if (tempMovement != nullptr) {
			delete tempMovement;
		}
	}
}

void BaseCell::StartMovement(bool reset) {
	if (movementDic.size() == 0)return;

	std::map<E_MovementType, Movement*>::iterator iter;
	for (iter = movementDic.begin(); iter != movementDic.end(); iter++) {
		if (iter->second == nullptr)continue;

		iter->second->StartMovement(reset);
	}
}

void BaseCell::StopMovement(bool stopForward) {
	if (movementDic.size() == 0)return;

	std::map<E_MovementType, Movement*>::iterator iter;
	for (iter = movementDic.begin(); iter != movementDic.end(); iter++) {
		if (iter->second == nullptr)continue;

		iter->second->StopMovement(stopForward);
	}
}

void BaseCell::SetMovementState(bool state) {
	if (movementDic.size() == 0)return;

	std::map<E_MovementType, Movement*>::iterator iter;
	for (iter = movementDic.begin(); iter != movementDic.end(); iter++) {
		if (iter->second == nullptr)continue;
		iter->second->SetMovementState(state);
	}

}

std::map<E_MovementType, Movement*>& BaseCell::GetMovementDic()
{
	// TODO: insert return statement here
	return movementDic;
}

void BaseCell::SetFollowUI(UI* ui, bool releaseWhenInactive)
{
	followUI = ui;
	releaseUIWhenInactive = releaseWhenInactive;
}

UI* BaseCell::GetFollowUI()
{
	return followUI;
}

void BaseCell::SetLightEnterSign(bool isWhiteLight)
{
	if (isWhiteLight) {
		curCheckWhiteLightCounter = 1;
	}
	else {
		curCheckBlackLightCounter = 1;
	}
}

void BaseCell::ResetLightSign()
{
	curCheckWhiteLightCounter = 0;
	curCheckBlackLightCounter = 0;
}

bool BaseCell::CheckInScreenRect(BaseCell* cell, bool checkWholeCell)
{
	if (cell == nullptr)return false;

	if (checkWholeCell == false) {
		return Collider::IsRectContainPoint(vector3s(0, 0, 0), vector2s(SCREEN_WIDTH, SCREEN_HEIGHT), vector3s(0, 0, 0), cell->GetTransform()->GetPos());
	}
	else {
		//TODO
		return false;
	}

}

void BaseCell::RecordCurState()
{
	//transform
	if (transform != nullptr) {
		transform->RecordCurState();
	}
	//movement

}

void BaseCell::Reset()
{
	//transform
	if (transform != nullptr) {
		transform->Reset();
	}
	//movement
	if (movementDic.size() != 0) {
		std::map<E_MovementType, Movement*>::iterator iter;
		for (iter = movementDic.begin(); iter != movementDic.end(); iter++) {
			if (iter->second == nullptr)continue;
			iter->second->ResetDefault();
		}
	}
}

bool BaseCell::CheckCanMove()
{
	return !lockMove;
}

void BaseCell::SetSpawnCounterSign(bool sign)
{
	spawnCounterSign = sign;
}

#pragma endregion baseCell_method


#pragma region light_method

Light::Light() :lightState(false), lightType(E_LightType::LightType_WhiteCicle), lightSize(vector2s(0, 0))
{
	followObj = nullptr;
}

Light::~Light()
{
	followObj = nullptr;
}

void Light::SetLightSize(float size, bool xAxis = true)
{
	if (size < 0) {
		if (xAxis == true) {
			lightSize.x = 0;
		}
		else {
			lightSize.y = 0;
		}
		return;
	}

	if (xAxis == true) {
		lightSize.x = size;
	}
	else {
		lightSize.y = size;
	}
	//set collider size
	Collider* collider = (Collider*)GetComponent(E_Component::Component_Collider);
	if (collider == nullptr) {
		//error
		return;
	}
	if (collider->GetColliderDataArraySize() == 0)return;
	std::vector<ColliderData*>& colliderDataArray = collider->GetColliderDataArray();
	if (colliderDataArray[0] != nullptr) {
		if (xAxis == true) {
			colliderDataArray[0]->size.x = size;
		}
		else {
			colliderDataArray[0]->size.y = size;
		}
	}
}

void Light::SetLightSize(vector2s size)
{
	if (size.x < 0 || size.y < 0) {
		return;
	}

	lightSize = size;
	//set collider size
	Collider* collider = (Collider*)GetComponent(E_Component::Component_Collider);
	if (collider == nullptr) {
		//error
		return;
	}
	if (collider->GetColliderDataArraySize() == 0)return;
	std::vector<ColliderData*>& colliderDataArray = collider->GetColliderDataArray();
	if (colliderDataArray[0] != nullptr) {
		colliderDataArray[0]->size = size;
	}
}

bool Light::CheckLightState() {
	return lightState;
}

void Light::SetLightType(E_LightType lType)
{
	//TODO
	if (lType == lightType)return;

	lightType = lType;
	if (lightType == E_LightType::LightType_WhiteCicle) {

	}
}

void Light::SetFollowObj(BaseCell* obj, bool alignPos)
{
	if (followObj != nullptr && obj != nullptr && followObj->GetID() == obj->GetID())return;

	followObj = obj;

	if (obj != nullptr && alignPos == true) {
		this->SetPos(obj->GetTransform()->GetPos());
	}
}

void Light::DoUpdate(float deltatime)
{
	BaseCell::DoUpdate(deltatime);
	if (enable == false)return;
	if (followObj != nullptr) {
		vector3s pos = this->GetTransform()->GetPos();
		vector3s movement = followObj->GetTransform()->GetMovement();
		if (VecLengthS(movement) != 0) {
			pos = pos + movement;
			this->SetPos(pos);
		}
		else {

			//this->SetPos(followObj->GetTransform()->GetPos());
		}
	}

	//GameMgr::GetInstance()->sceneObjMgr->CheckLightCollision(this);

}

bool Light::CheckHasFollowObj() {
	return followObj != nullptr;
}

bool Light::CheckIsSameFollowObj(BaseCell* cell)
{
	if (cell == nullptr || followObj == nullptr)return false;
	return cell->GetID() == this->cellId;
}


#pragma endregion light_method

#pragma region bullet_method

Bullet::Bullet() :bulletType(E_BulletType::BulletType_Normal), shootPos(vector3s(0, 0, 0)), dir(vector3s(0, 0, 0)), bulletState(false), bulletSpeed(0.0f), sizeScale(vector3s(1, 1, 1)),
bulletLifeTime(0.0f), bulletLifeTimePass(0.0f), bulletCurAtk(0)
{
	curMaxReflectCount = 0;
	reflectCount = 0;
	reflectTempIdArray.clear();
}

Bullet::Bullet(E_BulletType bulletT) : bulletType(bulletT), shootPos(vector3s(0, 0, 0)), dir(vector3s(0, 0, 0)), bulletState(false), bulletSpeed(0.0f), sizeScale(vector3s(1, 1, 1)),
bulletLifeTime(0.0f), bulletLifeTimePass(0.0f)
{
	curMaxReflectCount = 0;
	reflectCount = 0;
	reflectTempIdArray.clear();
}

Bullet::~Bullet() {

}

void Bullet::BindAttacker(BaseCell* cell)
{
	attacker = cell;
}

BaseCell* Bullet::GetAttacker()
{
	return attacker;
}

void Bullet::DoInit(bool active)
{
	Reset();
	StartMovement(true);
	bulletState = active;
}

void Bullet::DoUpdate(float deltatime) {
	if (bulletState) {

	}
	BaseCell::DoUpdate(deltatime);
}

void Bullet::DoLateUpdate(float deltatime) {

}

void Bullet::SetBulletLifeTime(float lifeTime)
{
	bulletLifeTime = lifeTime;
}

void Bullet::SetStartPos(vector3s startPos)
{
	shootPos = startPos;
	this->SetPos(startPos, true);
}

void Bullet::SetDir(vector3s direction)
{
	//calc euler,set transform.rot

	dir = VecNormalize(direction);
	vector3s tempDir = dir;
	tempDir.y = -dir.y;
	float angle = (atan2(rightVector.x, rightVector.y) - atan2(tempDir.x, tempDir.y)) * RAD2DEG;
	this->GetTransform()->SetRot(vector3s(0, 0, angle));
}

void Bullet::SetSizeScale(vector3s scale)
{
	sizeScale = scale;
}

void Bullet::SetSpeed(float speed)
{
	bulletSpeed = speed <= 0.0f ? 0.0f : speed;
}

void Bullet::SetBulletType(E_BulletType bulletT)
{
	bulletType = bulletT;
}

E_BulletType Bullet::GetBulletType() {
	return bulletType;
}

void Bullet::SetMaxReflectCount(int _count)
{
	curMaxReflectCount = _count;
}

void Bullet::Reset() {
	bulletLifeTimePass = 0.0f;
	reflectTempIdArray.clear();
	curMaxReflectCount = 0;
	reflectCount = 0;
	StopMovement(false);
}

bool Bullet::ReflectCountIncrease(int id)
{
	reflectTempIdArray.push_back(id);
	if (reflectCount >= curMaxReflectCount) {
		return false;
	}
	else {
		reflectCount++;
		return true;
	}
}

void Bullet::CollisionTrigger()
{
	GameMgr::GetInstance()->sceneObjMgr->RecycleBullet(this);
}

bool Bullet::CheckReflectID(int id)
{
	for (auto& temp : reflectTempIdArray) {
		if (temp == id) {
			return false;
		}
	}

	return true;
}

vector3s Bullet::GetCurDir()
{
	return dir;
}

#pragma region bullet_normal
Bullet_Normal::Bullet_Normal() : Bullet(E_BulletType::BulletType_Normal) {

}

Bullet_Normal::~Bullet_Normal() {

}

void Bullet_Normal::DoUpdate(float deltatime) {
	if (bulletState == true) {
		vector3s newMovement = dir * (bulletSpeed * deltatime);
		this->SetPos(this->GetTransform()->GetPos() + newMovement, true);

		if (CheckInScreenRect(this, false) == false) {
			//recycle this bullet
			GameMgr::GetInstance()->sceneObjMgr->RecycleBullet(this);
			return;
		}

		if (GameMgr::GetInstance()->sceneObjMgr->CheckBulletCollision(this) == true) {
			//recycle this bullet
			GameMgr::GetInstance()->sceneObjMgr->RecycleBullet(this);
			return;
		}

		if (bulletLifeTime > 0) {
			bulletLifeTimePass += deltatime;
			if (bulletLifeTimePass > bulletLifeTime) {
				//recycle this bullet
				GameMgr::GetInstance()->sceneObjMgr->RecycleBullet(this);
				return;
			}
		}
	}
	Bullet::DoUpdate(deltatime);
}

void Bullet_Normal::DoLateUpdate(float deltatime) {

}
#pragma endregion bullet_normal



#pragma region bullet_shotgun

Bullet_Shotgun::Bullet_Shotgun() : Bullet(E_BulletType::BulletType_Shotgun), curSpeed(0.0f) {

}

Bullet_Shotgun::~Bullet_Shotgun() {

}

void Bullet_Shotgun::DoUpdate(float deltatime) {
	if (bulletState == true) {

		curSpeed -= deltatime * BULLET_SHOTGUN_SPEED_FACTOR;
		curSpeed = F_Clamp(curSpeed, 0, bulletSpeed);

		vector3s newMovement = dir * (curSpeed * deltatime);
		this->SetPos(this->GetTransform()->GetPos() + newMovement, true);

		if (CheckInScreenRect(this, false) == false) {
			//recycle this bullet
			GameMgr::GetInstance()->sceneObjMgr->RecycleBullet(this);
			return;
		}

		if (GameMgr::GetInstance()->sceneObjMgr->CheckBulletCollision(this) == true) {
			//recycle this bullet
			GameMgr::GetInstance()->sceneObjMgr->RecycleBullet(this);
			return;
		}

		if (bulletLifeTime > 0) {
			bulletLifeTimePass += deltatime;
			if (bulletLifeTimePass > bulletLifeTime) {
				//recycle this bullet
				GameMgr::GetInstance()->sceneObjMgr->RecycleBullet(this);
				return;
			}
		}
	}
	Bullet::DoUpdate(deltatime);
}

void Bullet_Shotgun::DoLateUpdate(float deltatime) {

}

void Bullet_Shotgun::Reset()
{
	curSpeed = bulletSpeed;
	Bullet::Reset();
}

#pragma endregion bullet_shotgun

#pragma region bullet_laser

Bullet_Laser::Bullet_Laser() :Bullet(E_BulletType::BulletType_Laser) {

}

Bullet_Laser::~Bullet_Laser() {

}

void Bullet_Laser::DoUpdate(float deltatime) {

	if (bulletState == true) {

		if (bulletLifeTime > 0) {
			bulletLifeTimePass += deltatime;
			if (bulletLifeTimePass > bulletLifeTime) {
				GameMgr::GetInstance()->sceneObjMgr->RecycleBullet(this);
				return;
			}
		}
		GameMgr::GetInstance()->sceneObjMgr->CheckBulletCollision(this);
	}


	Bullet::DoUpdate(deltatime);
}

void Bullet_Laser::DoLateUpdate(float deltatime) {

}

void Bullet_Laser::SetBulletLifeTime(float lifeTime)
{
	bulletLifeTime = lifeTime;
	for (auto& temp : movementDic) {
		if (temp.second == nullptr)continue;
		if (temp.second->GetMovementType() == E_MovementType::MovementType_Scale) {
			temp.second->SetMovementTime(lifeTime);
		}
	}
}

void Bullet_Laser::SetStartPos(vector3s startPos)
{
	shootPos = startPos;
	//calc center pos
	vector2s size = GetSize(true);
	vector3s posOffset = VecNormalize(dir) * (size.x * 0.5f);
	vector3s newPos = startPos + posOffset;
	this->SetPos(newPos, true);
}

vector3s Bullet_Laser::GetCorrectRot()
{
	float angle = (atan2(rightVector.x, rightVector.y) - atan2(dir.x, dir.y)) * RAD2DEG;
	return vector3s(0, 0, angle);
}

void Bullet_Laser::Reset() {
	Bullet::Reset();
}

#pragma endregion bullet_laser

#pragma region bullet_cannon

Bullet_Cannon::Bullet_Cannon() :explodeRange(0.0f), curSpeed(0.0f), Bullet(E_BulletType::BulletType_Cannon)
{

}

Bullet_Cannon::~Bullet_Cannon()
{

}

void Bullet_Cannon::DoUpdate(float deltatime)
{
	if (bulletState == true) {

		curSpeed += deltatime * bulletSpeed * BULLET_CANNON_SPEED_FACTOR;
		//curSpeed = bulletSpeed;
		curSpeed = F_Clamp(curSpeed, 0, bulletSpeed);
		vector3s newMovement = dir * (curSpeed * deltatime);
		this->SetPos(this->transform->GetPos() + newMovement, true);

		if (bulletLifeTime > 0) {
			bulletLifeTimePass += deltatime;
			if (bulletLifeTimePass > bulletLifeTime) {
				//explosion
				BattleMgr::Explosion(this);
				//recycle
				GameMgr::GetInstance()->sceneObjMgr->RecycleBullet(this);
				return;
			}
		}
		if (GameMgr::GetInstance()->sceneObjMgr->CheckBulletCollision(this)) {
			GameMgr::GetInstance()->sceneObjMgr->RecycleBullet(this);
			return;
		}
	}
	Bullet::DoUpdate(deltatime);
}

void Bullet_Cannon::DoLateUpdate(float deltatime)
{
	Bullet::DoLateUpdate(deltatime);
}

void Bullet_Cannon::Reset()
{
	curSpeed = 0.0f;
	Bullet::Reset();
}

void Bullet_Cannon::SetExplodeRange(float range)
{
	explodeRange = range;

}

float Bullet_Cannon::GetExplodeRange()
{
	return explodeRange;
}



#pragma endregion bullet_cannon


#pragma endregion bullet_method

#pragma region effect_method
Effect::Effect() :effectType(E_Effect::Effect_None)
{

}

Effect::Effect(E_Effect effectT) : effectType(effectT) {

}

Effect::~Effect()
{
}

void Effect::SetEffectType(E_Effect effectT)
{
	effectType = effectT;
}

E_Effect Effect::GetEffectType()
{
	return effectType;
}

void Effect::StartEffect()
{
	Animation* animation = (Animation*)GetComponent(E_Component::Component_Animation);
	if (animation != nullptr) {
		animation->SwitchToDefaultAnim();
	}
}

void Effect::StopEffect()
{
	Animation* animation = (Animation*)GetComponent(E_Component::Component_Animation);
	if (animation != nullptr) {
		animation->StopAnim();
	}
}

void Effect::PauseEffect(bool pause)
{
	Animation* animation = (Animation*)GetComponent(E_Component::Component_Animation);
	if (animation != nullptr) {
		animation->PauseAnim(pause);
	}
}

#pragma endregion effect_method


