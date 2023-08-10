#include "main.h"
#include "inputManager.h"
#include "player.h"
#include "gameLogic.h"
#include "sceneObjManager.h"
#include "effectManager.h"
#include "uiManager.h"

#pragma region player_method

Player::Player() :curStatusBulletType(E_Status::Status_Exp)
{
	cellType = E_CellType::CellType_Player;

	//register status:TODO
	status = new Status_Player();
	status->needRecordChangingData = true;
	RegisterComponent(E_Component::Component_Status, status);

	playerLight = nullptr;
	lightFieldRadius = LIGHT_RANGE_FIELD;
	curLightPos = vector3s(lightFieldRadius, 0.0f, 0.0f);
	targetLightPos = curLightPos * 0.3f;

	preMouseMovement = vector3s(0.0f, 0.0f, 0.0f);
}

Player::~Player()
{
	playerLight = nullptr;
	status = nullptr;
}

void Player::DoUpdate(float deltatime)
{
	BaseCell::DoUpdate(deltatime);
	if (enable == false)return;

	if (GameMgr::GetInstance()->GetCurGameState() == E_GameState::GameState_Game_Normal) {
		//DrawLine(this->GetTransform()->GetPos() + curLightPos, curLightPos, 2000.0f);
	}

	if (status != nullptr && drawData != nullptr) {
		if (status->CheckCanBeAttack() == false) {//ƒ_ƒ[ƒW‚ğó‚¯‚½Œã‚µ‚Î‚ç‚­–³“G‚É‚È‚é
			if (m_beHitedTimePass < BE_HITED_COLOR_CHANGE_TIME) {
				m_beHitedTimePass += deltatime;
			}
			else {
				m_beHitedTimePass = 0.0f;
				m_beHitedColorSign = !m_beHitedColorSign;
				if (m_beHitedColorSign == true) {
					drawData->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
				}
				else {
					drawData->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.1f));
				}
			}
		}
		else {
			drawData->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
		}
	}

	if (GetKeyboardTrigger(DIK_K)) {
		if (status != nullptr) {
			//status->ResetData(curStatusBulletType);
		}
	}
}

void Player::DoLateUpdate(float deltatime)
{

}

void Player::Input(float deltatime)
{
	if (GameMgr::GetInstance()->inputMgr->GetCurInputMode() != E_InputMode::InputMode_Game)return;
	//input
	bool playerMovement = false;
	bool xMovement = false;
	bool lightMovement = false;
	if (lockMove == false) {

		vector3s curPos = transform->GetPos();
		if (transform != nullptr) {
			vector2s moveV = vector2s(0, 0);
			if (GetKeyboardPress(DIK_W)) {
				moveV.y += 5.0f;
				playerMovement = true;
			}
			else if (GetKeyboardPress(DIK_S)) {
				moveV.y -= 5.0f;
				playerMovement = true;
			}
			if (GetKeyboardPress(DIK_D)) {
				moveV.x += 5.0f;
				playerMovement = true;
			}
			else if (GetKeyboardPress(DIK_A)) {
				moveV.x -= 5.0f;
				playerMovement = true;
			}

			if (moveV.x > 0) {
				xMovement = true;
				this->transform->SetRot(vector3s(0.0f, 0.0f, 0.0f));
			}
			else if (moveV.x < 0) {
				xMovement = true;
				this->transform->SetRot(vector3s(-180.0f, 0.0f, 0.0f));
			}

			curPos.x += moveV.x;
			curPos.y += moveV.y;

			AdjustScreenPoint(curPos, this->GetSize(true));

			this->SetPos(curPos, true);


			//anim
			Animation* anim = (Animation*)GetComponent(E_Component::Component_Animation);
			if (anim != nullptr) {
				if (playerMovement) {
					anim->SwitchAnim(E_Anim::Anim_Move, false, true);
				}
				else {
					anim->SwitchAnim(E_Anim::Anim_Idle, false, true);
				}
			}
		}
	}
	//collision
	GameMgr::GetInstance()->sceneObjMgr->CheckPlayerCollision(this);

	//‘_‚¢‚ğ’è‚ß
	if (playerLight != nullptr) {

		vector3s mouseMovement = vector3s();
		POINT mouseM = POINT();
		mouseM.x = 0;
		mouseM.y = 0;
		if (inputLock == false) {
			GetCursorPos(&mouseM);
			ScreenToClient(GetHWnd(), &mouseM);
			mouseMovement.x = (float)mouseM.x;
			mouseMovement.y = (float)mouseM.y;

			mouseMovement.x -= SCREEN_WIDTH * 0.5f;

			mouseMovement.y = -(mouseMovement.y - SCREEN_HEIGHT * 0.5f);

		}
		else {
			mouseMovement = preMouseMovement;
		}
		preMouseMovement = mouseMovement;

		vector3s curPos = this->GetTransform()->GetPos();
		curPos.z = 0.0f;

		vector3s shootRayPosTargetPos = mouseMovement - curPos;

		shootRayPosTargetPos.z = 0.0f;
		shootRayPosTargetPos = VecNormalize(shootRayPosTargetPos);

		if (playerLight != nullptr) {
			vector3s tempV = VecNormalize(shootRayPosTargetPos);
			curLightPos = tempV * SHOOT_POINT_OFFSET;
			playerLight->SetPos(this->transform->GetPos() + curLightPos,true);
		}
	}

	//¶‰E‰ñ“]ˆ—
	if (xMovement == false) {
		float dirDotValue = VecDot(vector2s(curLightPos.x, curLightPos.y), rightVector);
		if (dirDotValue > 0) {
			xMovement = true;
			this->transform->SetRot(vector3s(0.0f, 0.0f, 0.0f));
		}
		else {
			xMovement = true;
			this->transform->SetRot(vector3s(-180.0f, 0.0f, 0.0f));
		}
	}
}

void Player::SetPlayerLight(Light* light)
{
	if (light == nullptr)return;
	if (playerLight != nullptr) {
		playerLight->SetState(false);
	}
	playerLight = light;

	playerLight->SetPos(this->transform->GetPos() + curLightPos,true);

	lightPosOffset = vector3s(0, 0, 0);

	light->SetLightState(true);
	light->isPlayerLight = true;
}

void Player::SetCurBulletType(E_Status bulletType)
{
	switch (bulletType) {
	case E_Status::Status_Hp:
	case E_Status::Status_Atk:
	case E_Status::Status_Def:
	case E_Status::Status_Exp:
		curStatusBulletType = bulletType;
	default:
		return;
	}
}

E_Status Player::GetCurBulletType()
{
	return curStatusBulletType;
}

void Player::SwitchBulletType(bool forward)
{
	int curBulletIndex = (int)curStatusBulletType;

	curBulletIndex = (forward ? curBulletIndex + 1 : curBulletIndex + (STATUS_COUNT - 1)) % STATUS_COUNT;

	curStatusBulletType = (E_Status)curBulletIndex;

	GameMgr::GetInstance()->uiMgr->UpdateStatusSelector(curStatusBulletType);
}

void Player::ShootBullet()
{
	if (status == nullptr || GameMgr::GetInstance()->GetCurGameState() != E_GameState::GameState_Game_Normal) {
		return;
	}
	else {

		E_BulletType bulletType = E_BulletType::BulletType_Normal;
		switch (curStatusBulletType) {
		case E_Status::Status_Hp:
			bulletType = E_BulletType::BulletType_Laser;
			break;
		case E_Status::Status_Atk:
			bulletType = E_BulletType::BulletType_Cannon;
			break;
		case E_Status::Status_Def:
			bulletType = E_BulletType::BulletType_Shotgun;
			break;
		case E_Status::Status_Exp:
		default:
			bulletType = E_BulletType::BulletType_Normal;
			break;
		}
		int atk = status->GetValue(E_Status::Status_Atk);
		//check status
		if (status->ShootStatus(curStatusBulletType)) {
			vector3s shootPos = this->GetTransform()->GetPos() + curLightPos;
			vector3s shootDir = VecNormalize(curLightPos);
			vector3s effectDir = shootDir;
			effectDir.y = -effectDir.y;
			GameMgr::GetInstance()->sceneObjMgr->ShootBullet(this, atk, bulletType, shootPos, shootDir, 1500.0f);
			GameMgr::GetInstance()->effectMgr->CreateEffect(E_Effect::Effect_ShootLight, nullptr, shootPos + (curLightPos * 2.0f), effectDir);
			GameMgr::GetInstance()->effectMgr->SetCameraShake(CAMERA_SHAKE_PLAYER_SHOOT_TIME, CAMERA_SHAKE_PLAYER_SHOOT_FORCE);
		}
	}
}

int Player::GetStatusValue(E_Status statusType)
{
	if (status != nullptr) {
		return status->GetValue(statusType);
	}

	return 0;
}

vector3s Player::GetCurShootPos()
{
	return curLightPos;
}



#pragma endregion player_method
