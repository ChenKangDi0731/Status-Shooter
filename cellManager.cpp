#include "gameLogic.h"
#include "gameObject.h"
#include "enemy.h"
#include "sceneObjManager.h"
#include "cellManager.h"

#pragma region cell_manager

CellMgr::CellMgr()
{
	enemy_1_shaderParam = nullptr;
	enemy_2_shaderParam = nullptr;
	enemy_3_shaderParam = nullptr;
	cellDic.clear();
}

CellMgr::~CellMgr() {
	cellDic.clear();

	if (enemy_1_shaderParam != nullptr) {
		delete enemy_1_shaderParam;
	}
	enemy_1_shaderParam = nullptr;

	if (enemy_2_shaderParam != nullptr) {
		delete enemy_2_shaderParam;
	}
	enemy_2_shaderParam = nullptr;

	if (enemy_3_shaderParam != nullptr) {
		delete enemy_3_shaderParam;
	}
	enemy_3_shaderParam = nullptr;

	if (grass_shaderParam != nullptr) {
		delete grass_shaderParam;
	}
	grass_shaderParam = nullptr;
}

void CellMgr::DoInit()
{
	ShaderParam_Normal* temp_enemy_1_shader_normal = new ShaderParam_Normal();
	temp_enemy_1_shader_normal->shaderProgram = GameMgr::GetInstance()->shaderMgr->GetShaderProgram(E_Shader::Shader_Normal);
	temp_enemy_1_shader_normal->texNo = GameMgr::GetInstance()->textureMgr->GetTextureID(E_Texture::Texture_Character_Package);
	temp_enemy_1_shader_normal->texIndex = vector2s(ENEMY_1_IDLE_INDEX_X, ENEMY_1_IDLE_INDEX_Y);
	temp_enemy_1_shader_normal->tex_w_count = ENEMY_1_TEX_W_COUNT;
	temp_enemy_1_shader_normal->tex_texel_size = character_cell_tex_size;
	enemy_1_shaderParam = temp_enemy_1_shader_normal;

	ShaderParam_Normal* temp_enemy_2_shader_normal = new ShaderParam_Normal();
	temp_enemy_2_shader_normal->shaderProgram = GameMgr::GetInstance()->shaderMgr->GetShaderProgram(E_Shader::Shader_Normal);
	temp_enemy_2_shader_normal->texNo = GameMgr::GetInstance()->textureMgr->GetTextureID(E_Texture::Texture_Character_Package);
	temp_enemy_2_shader_normal->texIndex = vector2s(ENEMY_2_IDLE_INDEX_X, ENEMY_2_IDLE_INDEX_Y);
	temp_enemy_2_shader_normal->tex_w_count = ENEMY_2_TEX_W_COUNT;
	temp_enemy_2_shader_normal->tex_texel_size = character_cell_tex_size;
	enemy_2_shaderParam = temp_enemy_2_shader_normal;

	ShaderParam_Normal* temp_enemy_3_shader_normal = new ShaderParam_Normal();
	temp_enemy_3_shader_normal->shaderProgram = GameMgr::GetInstance()->shaderMgr->GetShaderProgram(E_Shader::Shader_Normal);
	temp_enemy_3_shader_normal->texNo = GameMgr::GetInstance()->textureMgr->GetTextureID(E_Texture::Texture_Character_Package);
	temp_enemy_3_shader_normal->texIndex = vector2s(ENEMY_3_IDLE_INDEX_X, ENEMY_3_IDLE_INDEX_Y);
	temp_enemy_3_shader_normal->tex_w_count = ENEMY_3_TEX_W_COUNT;
	temp_enemy_3_shader_normal->tex_texel_size = character_cell_tex_size;
	enemy_3_shaderParam = temp_enemy_3_shader_normal;

#pragma region other

#pragma region grass

	ShaderParam_Normal* tempGrassShaderParam = new ShaderParam_Normal();
	tempGrassShaderParam->shaderProgram = GameMgr::GetInstance()->shaderMgr->GetShaderProgram(E_Shader::Shader_Normal);
	tempGrassShaderParam->texNo = GameMgr::GetInstance()->textureMgr->GetTextureID(E_Texture::Texture_Grass);
	tempGrassShaderParam->texIndex = vector2s(GRASS_TEX_START_INDEX_X, GRASS_TEX_START_INDEX_Y);
	tempGrassShaderParam->tex_w_count = GRASS_TEX_CELL_COUNT_W;
	tempGrassShaderParam->tex_texel_size = grass_tex_size;

	grass_shaderParam = tempGrassShaderParam;
#pragma endregion grass

#pragma endregion other

}

BaseCell* CellMgr::CreateCellEntity(E_CellType cellType)
{
	if (cellType == E_CellType::CellType_Enemy_Skirmisher) {
#pragma region enemy_1
		Enemy* enemy = new Enemy();
		enemy->SetID(GameMgr::GetInstance()->sceneObjMgr->GetID());
		vector3s bodyInitPos = vector3s(0, 0, -0.5f);
		enemy->SetPos(bodyInitPos, true);
		enemy->SetCellType(E_CellType::CellType_Enemy);
		enemy->SetEnemyType(E_CellType::CellType_Enemy_Skirmisher);
		enemy->SetScale(aspect_factor);

		//描画データ
		DrawData* enemyDrawData = RenderMgr::GetNewDrawData(nullptr, true, E_BatchingType::BatchingType_Enemy, true);
		if (enemyDrawData == nullptr) {
			delete enemyDrawData;
			return nullptr;
		}
		enemyDrawData->SetShaderParam(enemy_1_shaderParam);
		enemyDrawData->SetSize(vector2s(ENEMY_TEX_SIZE_X, ENEMY_TEX_SIZE_Y));
		enemy->SetDrawData(enemyDrawData);


		//コライダーコンポーネント（当たり判定
		Collider* enemyCollider = new Collider();
		enemy->RegisterComponent(E_Component::Component_Collider, enemyCollider);
		ColliderData* enemyColliderData = new ColliderData(0, enemyCollider,
			true, E_ColliderType::Collider_Circle, enemy->GetSize(true), vector3s(0, 0, 0));

		enemyCollider->AddColliderData(enemyColliderData);

		//ステータスコンポーネント
		Status* tempStatus = (Status*)enemy->GetComponent(E_Component::Component_Status);
		if (tempStatus != nullptr) {
			tempStatus->SetDefaultData(E_Status::Status_Hp, ENEMY_1_STATUS_HP, true);
			tempStatus->SetDefaultData(E_Status::Status_Atk, ENEMY_1_STATUS_ATK, true);
		}

		//アニメーションコンポーネント
		Animation* enemyAnimator = new Animation();
		enemy->RegisterComponent(E_Component::Component_Animation, enemyAnimator);

		AnimClip* enemyIdleAnim = new AnimClip();
		int startTexIndex = ENEMY_1_IDLE_INDEX_Y * ENEMY_1_TEX_W_COUNT + ENEMY_1_IDLE_INDEX_X;
		int endTexIndex = startTexIndex + ENEMY_1_IDLE_FRAME_COUNT;
		enemyIdleAnim->SetAnimParam(E_Anim::Anim_Idle, vector2s(startTexIndex, endTexIndex), 2.0f, true, true);
		enemyAnimator->RegisterAnim(E_Anim::Anim_Idle, enemyIdleAnim, true);

		AnimClip* enemyDmgAnim = new AnimClip();
		startTexIndex = ENEMY_1_DMG_INDEX_Y * ENEMY_1_TEX_W_COUNT + ENEMY_1_DMG_INDEX_X;
		endTexIndex = startTexIndex + ENEMY_1_DMG_FRAME_COUNT;
		enemyDmgAnim->SetAnimParam(E_Anim::Anim_Damage, vector2s(startTexIndex, endTexIndex), 0.5f, true, false);
		enemyAnimator->RegisterAnim(E_Anim::Anim_Damage, enemyDmgAnim, false);

		//==================================================================AIを設定(オートマトン
		AIComponent* enemyAI = new AIComponent();
		enemy->RegisterComponent(E_Component::Component_AI, enemyAI);

		FSMSystem* enemyFSM = enemyAI->GetFSMSystem();

		//state
		FSMState* idleState = new FSMState(enemyFSM);
		FSMState* move2PlayerState = new FSMState(enemyFSM);
		FSMState* dmgState = new FSMState(enemyFSM);

		FSMState_Base_Action_Idle* idleAction = new FSMState_Base_Action_Idle();
		idleAction->setAnim = E_Anim::Anim_Idle;
		idleState->SetAction(idleAction);
		FSMState_Base_Action_Move2Player* move2PlayerAction = new FSMState_Base_Action_Move2Player();
		move2PlayerAction->setAnim = E_Anim::Anim_Idle;
		move2PlayerAction->moveSpeed = ENEMY_SKIRMISHER_MOVE_SPEED;
		move2PlayerState->SetAction(move2PlayerAction);
		FSMState_Base_Action_Dmg* dmgAction = new FSMState_Base_Action_Dmg();
		dmgAction->setAnim = E_Anim::Anim_Damage;
		dmgState->SetAction(dmgAction);

		//trigger delegate
			//idle
		FSMStateTrigger_Base_Trigger_IdleTime* idleTimeTriggerAction = new FSMStateTrigger_Base_Trigger_IdleTime();
		idleTimeTriggerAction->idleTime = ENEMY_SKIRMISHER_IDLE_TIME;
		FSMStateTrigger_Base_Trigger_TakeDmg* idleTakeDmgTriggerAction = new FSMStateTrigger_Base_Trigger_TakeDmg();
		//move2player
		FSMStateTrigger_Base_Trigger_Dist2Player* dist2PlayerTriggerAction = new FSMStateTrigger_Base_Trigger_Dist2Player();
		dist2PlayerTriggerAction->dist = ENEMY_SKIRMISHER_DIST_TO_PLAYER;
		FSMStateTrigger_Base_Trigger_IdleTime* moveTimeTrrigerAction = new  FSMStateTrigger_Base_Trigger_IdleTime();
		moveTimeTrrigerAction->idleTime = 6.0f;
		FSMStateTrigger_Base_Trigger_TakeDmg* moveTakeDmgTriggerAction = new FSMStateTrigger_Base_Trigger_TakeDmg();
		//dmg
		FSMStateTrigger_Base_Trigger_TakeDmg* dmgTriggerAction = new FSMStateTrigger_Base_Trigger_TakeDmg();

		//trigger
			//idle
		FSMStateTrigger* idleTimeEndTrigger = new FSMStateTrigger();//idle end
		idleTimeEndTrigger->SetTriggerParam(idleState, idleTimeTriggerAction);
		FSMTriggerGroup* triggerGroup_idleTimeEnd = new FSMTriggerGroup(E_CheckTriggerType::CheckTriggerType_AllTrue, enemyFSM);
		triggerGroup_idleTimeEnd->AddTrigger(idleTimeEndTrigger);

		FSMStateTrigger* idleTakeDmgTrigger = new FSMStateTrigger();//take dmg
		idleTakeDmgTrigger->SetTriggerParam(idleState, idleTakeDmgTriggerAction);
		FSMTriggerGroup* triggerGroup_idleTakeDmg = new FSMTriggerGroup(E_CheckTriggerType::CheckTriggerType_OneTrue, enemyFSM);
		triggerGroup_idleTakeDmg->AddTrigger(idleTakeDmgTrigger);

		//register state trigger
		idleState->RegisterTrigger(E_FSMState::FSMState_MoveToPlayer, triggerGroup_idleTimeEnd);
		idleState->RegisterTrigger(E_FSMState::FSMState_Dmg, triggerGroup_idleTakeDmg);

		//move2Player
		FSMStateTrigger* dist2PlayerTrigger = new FSMStateTrigger();
		dist2PlayerTrigger->SetTriggerParam(move2PlayerState, dist2PlayerTriggerAction);//dist
		FSMStateTrigger* move2PlayerContinueTimeTrigger = new FSMStateTrigger();
		move2PlayerContinueTimeTrigger->SetTriggerParam(move2PlayerState, moveTimeTrrigerAction);//time
		FSMTriggerGroup* triggerGroup_continueTime = new FSMTriggerGroup(E_CheckTriggerType::CheckTriggerType_OneTrue, enemyFSM);
		triggerGroup_continueTime->AddTrigger(dist2PlayerTrigger);
		triggerGroup_continueTime->AddTrigger(move2PlayerContinueTimeTrigger);

		FSMStateTrigger* moveTakeDmgTrigger = new FSMStateTrigger();
		moveTakeDmgTrigger->SetTriggerParam(move2PlayerState, moveTakeDmgTriggerAction);
		FSMTriggerGroup* triggerGroup_moveTakeDmg = new FSMTriggerGroup(E_CheckTriggerType::CheckTriggerType_OneTrue, enemyFSM);
		triggerGroup_moveTakeDmg->AddTrigger(moveTakeDmgTrigger);

		//register state trigger
		move2PlayerState->RegisterTrigger(E_FSMState::FSMState_Idle, triggerGroup_continueTime);
		move2PlayerState->RegisterTrigger(E_FSMState::FSMState_Dmg, triggerGroup_moveTakeDmg);

		//dmg
		FSMStateTrigger* dmgTimeTrigger = new FSMStateTrigger();
		dmgTimeTrigger->SetTriggerParam(dmgState, dmgTriggerAction);
		FSMTriggerGroup* dmgTriggerGroup = new FSMTriggerGroup(E_CheckTriggerType::CheckTriggerType_AllFalse, enemyFSM);
		dmgTriggerGroup->AddTrigger(dmgTimeTrigger);
		//register state trigger
		dmgState->RegisterTrigger(E_FSMState::FSMState_MoveToPlayer, dmgTriggerGroup);

		//register fsm state
		enemyFSM->RegisterState(E_FSMState::FSMState_Idle, idleState, true);
		enemyFSM->RegisterState(E_FSMState::FSMState_MoveToPlayer, move2PlayerState, false);
		enemyFSM->RegisterState(E_FSMState::FSMState_Dmg, dmgState, false);
		//==================================================================AIを設定(オートマトン

		enemy->SetState(false);//default false state

		enemy->RecordCurState();
		enemy->DoInit();

		//register
		GameMgr::GetInstance()->sceneObjMgr->RegisterCell(enemy);
		this->RegisterCell(enemy);

		return enemy;
#pragma endregion enemy_1
	}
	else if (cellType == E_CellType::CellType_Enemy_Skirmisher_2) {
#pragma region enemy_2
		Enemy* enemy = new Enemy();
		enemy->SetID(GameMgr::GetInstance()->sceneObjMgr->GetID());
		vector3s bodyInitPos = vector3s(0, 0, -0.5f);
		enemy->SetPos(bodyInitPos, true);
		enemy->SetCellType(E_CellType::CellType_Enemy);
		enemy->SetEnemyType(E_CellType::CellType_Enemy_Skirmisher_2);
		enemy->SetScale(aspect_factor);


		//描画データ
		DrawData* enemyDrawData = RenderMgr::GetNewDrawData(nullptr, true, E_BatchingType::BatchingType_Enemy, true);
		if (enemyDrawData == nullptr) {
			delete enemyDrawData;
			return nullptr;
		}
		enemyDrawData->SetShaderParam(enemy_2_shaderParam);
		enemyDrawData->SetSize(vector2s(ENEMY_TEX_SIZE_X, ENEMY_TEX_SIZE_Y));
		enemy->SetDrawData(enemyDrawData);


		//コライダーコンポーネント（当たり判定
		Collider* enemyCollider = new Collider();
		enemy->RegisterComponent(E_Component::Component_Collider, enemyCollider);
		ColliderData* enemyColliderData = new ColliderData(0, enemyCollider,
			true, E_ColliderType::Collider_Circle, enemy->GetSize(true), vector3s(0, 0, 0));

		enemyCollider->AddColliderData(enemyColliderData);

		//ステータス
		Status* tempStatus = (Status*)enemy->GetComponent(E_Component::Component_Status);
		if (tempStatus != nullptr) {
			tempStatus->SetDefaultData(E_Status::Status_Hp, ENEMY_2_STATUS_HP, true);
			tempStatus->SetDefaultData(E_Status::Status_Atk, ENEMY_2_STATUS_ATK, true);
		}

		//アニメーションコンポーネント
		Animation* enemyAnimator = new Animation();
		enemy->RegisterComponent(E_Component::Component_Animation, enemyAnimator);

		AnimClip* enemyIdleAnim = new AnimClip();
		int startTexIndex = ENEMY_2_IDLE_INDEX_Y * ENEMY_2_TEX_W_COUNT + ENEMY_2_IDLE_INDEX_X;
		int endTexIndex = startTexIndex + ENEMY_2_IDLE_FRAME_COUNT;
		enemyIdleAnim->SetAnimParam(E_Anim::Anim_Idle, vector2s(startTexIndex, endTexIndex), 2.0f, true, true);
		enemyAnimator->RegisterAnim(E_Anim::Anim_Idle, enemyIdleAnim, true);

		AnimClip* enemyDmgAnim = new AnimClip();
		startTexIndex = ENEMY_2_DMG_INDEX_Y * ENEMY_2_TEX_W_COUNT + ENEMY_2_DMG_INDEX_X;
		endTexIndex = startTexIndex + ENEMY_2_DMG_FRAME_COUNT;
		enemyDmgAnim->SetAnimParam(E_Anim::Anim_Damage, vector2s(startTexIndex, endTexIndex), 0.5f, true, false);
		enemyAnimator->RegisterAnim(E_Anim::Anim_Damage, enemyDmgAnim, false);

		//==================================================================AIを設定(オートマトン
		AIComponent* enemyAI = new AIComponent();
		enemy->RegisterComponent(E_Component::Component_AI, enemyAI);

		FSMSystem* enemyFSM = enemyAI->GetFSMSystem();

		//state
		FSMState* idleState = new FSMState(enemyFSM);
		FSMState* move2PlayerState = new FSMState(enemyFSM);
		FSMState* dmgState = new FSMState(enemyFSM);

		FSMState_Base_Action_Idle* idleAction = new FSMState_Base_Action_Idle();
		idleAction->setAnim = E_Anim::Anim_Idle;
		idleState->SetAction(idleAction);
		FSMState_Base_Action_Move2Player* move2PlayerAction = new FSMState_Base_Action_Move2Player();
		move2PlayerAction->setAnim = E_Anim::Anim_Idle;
		move2PlayerAction->moveSpeed = ENEMY_SKIRMISHER_MOVE_SPEED;
		move2PlayerState->SetAction(move2PlayerAction);
		FSMState_Base_Action_Dmg* dmgAction = new FSMState_Base_Action_Dmg();
		dmgAction->setAnim = E_Anim::Anim_Damage;
		dmgState->SetAction(dmgAction);

		//trigger delegate
			//idle
		FSMStateTrigger_Base_Trigger_IdleTime* idleTimeTriggerAction = new FSMStateTrigger_Base_Trigger_IdleTime();
		idleTimeTriggerAction->idleTime = ENEMY_SKIRMISHER_IDLE_TIME;
		FSMStateTrigger_Base_Trigger_TakeDmg* idleTakeDmgTriggerAction = new FSMStateTrigger_Base_Trigger_TakeDmg();
		//move2player
		FSMStateTrigger_Base_Trigger_Dist2Player* dist2PlayerTriggerAction = new FSMStateTrigger_Base_Trigger_Dist2Player();
		dist2PlayerTriggerAction->dist = ENEMY_SKIRMISHER_DIST_TO_PLAYER;
		FSMStateTrigger_Base_Trigger_IdleTime* moveTimeTrrigerAction = new  FSMStateTrigger_Base_Trigger_IdleTime();
		moveTimeTrrigerAction->idleTime = 6.0f;
		FSMStateTrigger_Base_Trigger_TakeDmg* moveTakeDmgTriggerAction = new FSMStateTrigger_Base_Trigger_TakeDmg();
		//dmg
		FSMStateTrigger_Base_Trigger_TakeDmg* dmgTriggerAction = new FSMStateTrigger_Base_Trigger_TakeDmg();

		//trigger
			//idle
		FSMStateTrigger* idleTimeEndTrigger = new FSMStateTrigger();//idle end
		idleTimeEndTrigger->SetTriggerParam(idleState, idleTimeTriggerAction);
		FSMTriggerGroup* triggerGroup_idleTimeEnd = new FSMTriggerGroup(E_CheckTriggerType::CheckTriggerType_AllTrue, enemyFSM);
		triggerGroup_idleTimeEnd->AddTrigger(idleTimeEndTrigger);

		FSMStateTrigger* idleTakeDmgTrigger = new FSMStateTrigger();//take dmg
		idleTakeDmgTrigger->SetTriggerParam(idleState, idleTakeDmgTriggerAction);
		FSMTriggerGroup* triggerGroup_idleTakeDmg = new FSMTriggerGroup(E_CheckTriggerType::CheckTriggerType_OneTrue, enemyFSM);
		triggerGroup_idleTakeDmg->AddTrigger(idleTakeDmgTrigger);

		//register state trigger
		idleState->RegisterTrigger(E_FSMState::FSMState_MoveToPlayer, triggerGroup_idleTimeEnd);
		idleState->RegisterTrigger(E_FSMState::FSMState_Dmg, triggerGroup_idleTakeDmg);

		//move2Player
		FSMStateTrigger* dist2PlayerTrigger = new FSMStateTrigger();
		dist2PlayerTrigger->SetTriggerParam(move2PlayerState, dist2PlayerTriggerAction);//dist
		FSMStateTrigger* move2PlayerContinueTimeTrigger = new FSMStateTrigger();
		move2PlayerContinueTimeTrigger->SetTriggerParam(move2PlayerState, moveTimeTrrigerAction);//time
		FSMTriggerGroup* triggerGroup_continueTime = new FSMTriggerGroup(E_CheckTriggerType::CheckTriggerType_OneTrue, enemyFSM);
		triggerGroup_continueTime->AddTrigger(dist2PlayerTrigger);
		triggerGroup_continueTime->AddTrigger(move2PlayerContinueTimeTrigger);

		FSMStateTrigger* moveTakeDmgTrigger = new FSMStateTrigger();
		moveTakeDmgTrigger->SetTriggerParam(move2PlayerState, moveTakeDmgTriggerAction);
		FSMTriggerGroup* triggerGroup_moveTakeDmg = new FSMTriggerGroup(E_CheckTriggerType::CheckTriggerType_OneTrue, enemyFSM);
		triggerGroup_moveTakeDmg->AddTrigger(moveTakeDmgTrigger);

		//register state trigger
		move2PlayerState->RegisterTrigger(E_FSMState::FSMState_Idle, triggerGroup_continueTime);
		move2PlayerState->RegisterTrigger(E_FSMState::FSMState_Dmg, triggerGroup_moveTakeDmg);

		//dmg
		FSMStateTrigger* dmgTimeTrigger = new FSMStateTrigger();
		dmgTimeTrigger->SetTriggerParam(dmgState, dmgTriggerAction);
		FSMTriggerGroup* dmgTriggerGroup = new FSMTriggerGroup(E_CheckTriggerType::CheckTriggerType_AllFalse, enemyFSM);
		dmgTriggerGroup->AddTrigger(dmgTimeTrigger);
		//register state trigger
		dmgState->RegisterTrigger(E_FSMState::FSMState_MoveToPlayer, dmgTriggerGroup);

		//register fsm state
		enemyFSM->RegisterState(E_FSMState::FSMState_Idle, idleState, true);
		enemyFSM->RegisterState(E_FSMState::FSMState_MoveToPlayer, move2PlayerState, false);
		enemyFSM->RegisterState(E_FSMState::FSMState_Dmg, dmgState, false);
		//==================================================================AIを設定(オートマトン

		enemy->SetState(false);//default false state

		enemy->RecordCurState();
		enemy->DoInit();

		//register
		GameMgr::GetInstance()->sceneObjMgr->RegisterCell(enemy);
		this->RegisterCell(enemy);

		return enemy;
#pragma endregion enemy_2
	}
	else if (cellType == E_CellType::CellType_Enemy_Dragonfly) {
#pragma region enemy_3
		Enemy* enemy = new Enemy_Dragonfly();
		enemy->SetID(GameMgr::GetInstance()->sceneObjMgr->GetID());
		vector3s bodyInitPos = vector3s(0, 0, -0.5f);
		enemy->SetPos(bodyInitPos, true);
		enemy->SetCellType(E_CellType::CellType_Enemy);
		enemy->SetEnemyType(E_CellType::CellType_Enemy_Dragonfly);
		enemy->SetScale(aspect_factor);


		//描画データ
		DrawData* enemyDrawData = RenderMgr::GetNewDrawData(nullptr, true, E_BatchingType::BatchingType_Enemy, true);
		if (enemyDrawData == nullptr) {
			delete enemyDrawData;
			return nullptr;
		}
		enemyDrawData->SetShaderParam(enemy_3_shaderParam);
		enemyDrawData->SetSize(vector2s(ENEMY_TEX_SIZE_X, ENEMY_TEX_SIZE_Y));
		enemy->SetDrawData(enemyDrawData);


		//コライダーコンポーネント（当たり判定
		Collider* enemyCollider = new Collider();
		enemy->RegisterComponent(E_Component::Component_Collider, enemyCollider);
		ColliderData* enemyColliderData = new ColliderData(0, enemyCollider,
			true, E_ColliderType::Collider_Circle, enemy->GetSize(true), vector3s(0, 0, 0));

		enemyCollider->AddColliderData(enemyColliderData);

		//ステータス
		Status* tempStatus = (Status*)enemy->GetComponent(E_Component::Component_Status);
		if (tempStatus != nullptr) {
			tempStatus->SetDefaultData(E_Status::Status_Hp, ENEMY_3_STATUS_HP, true);
			tempStatus->SetDefaultData(E_Status::Status_Atk, ENEMY_3_STATUS_ATK, true);
		}

		//アニメーションコンポーネント
		Animation* enemyAnimator = new Animation();
		enemy->RegisterComponent(E_Component::Component_Animation, enemyAnimator);

		AnimClip* enemyIdleAnim = new AnimClip();
		int startTexIndex = ENEMY_3_IDLE_INDEX_Y * ENEMY_3_TEX_W_COUNT + ENEMY_3_IDLE_INDEX_X;
		int endTexIndex = startTexIndex + ENEMY_3_IDLE_FRAME_COUNT;
		enemyIdleAnim->SetAnimParam(E_Anim::Anim_Idle, vector2s(startTexIndex, endTexIndex), 2.0f, true, true);
		enemyAnimator->RegisterAnim(E_Anim::Anim_Idle, enemyIdleAnim, true);

		AnimClip* enemyAttackAnim = new AnimClip();
		startTexIndex = ENEMY_3_ATTACK_INDEX_Y * ENEMY_3_TEX_W_COUNT + ENEMY_3_ATTACK_INDEX_X;
		endTexIndex = startTexIndex + ENEMY_3_ATTACK_FRAME_COUNT;
		enemyAttackAnim->SetAnimParam(E_Anim::Anim_Attack, vector2s(startTexIndex, endTexIndex), 0.9f, true, true);
		enemyAnimator->RegisterAnim(E_Anim::Anim_Attack, enemyAttackAnim, true);

		AnimClip* enemyDmgAnim = new AnimClip();
		startTexIndex = ENEMY_3_DMG_INDEX_Y * ENEMY_3_TEX_W_COUNT + ENEMY_3_DMG_INDEX_X;
		endTexIndex = startTexIndex + ENEMY_3_DMG_FRAME_COUNT;
		enemyDmgAnim->SetAnimParam(E_Anim::Anim_Damage, vector2s(startTexIndex, endTexIndex), 0.5f, true, false);
		enemyAnimator->RegisterAnim(E_Anim::Anim_Damage, enemyDmgAnim, false);

		//==================================================================AIを設定(オートマトン

		AIComponent* enemyAI = new AIComponent();
		enemy->RegisterComponent(E_Component::Component_AI, enemyAI);

		FSMSystem* enemyFSM = enemyAI->GetFSMSystem();

		//===========================================================================state
		FSMState* idleState = new FSMState(enemyFSM);
		FSMState* move2PlayerState = new FSMState(enemyFSM);
		FSMState* attackState = new FSMState(enemyFSM);
		FSMState* dmgState = new FSMState(enemyFSM);

		FSMState_Base_Action_Idle* idleAction = new FSMState_Base_Action_Idle();
		idleAction->setAnim = E_Anim::Anim_Idle;
		idleState->SetAction(idleAction);
		FSMState_Base_Action_Move2Player* move2PlayerAction = new FSMState_Base_Action_Move2Player();
		move2PlayerAction->setAnim = E_Anim::Anim_Idle;
		move2PlayerAction->moveSpeed = ENEMY_DRAGONFLY_MOVE_SPEED;
		move2PlayerState->SetAction(move2PlayerAction);
		FSMState_Base_Action_Attack* attackAction = new FSMState_Base_Action_Attack();
		attackAction->setAnim = E_Anim::Anim_Attack;
		attackAction->invokeOnce = true;
		attackAction->attackDelayTime = ENEMY_DRAGONFLY_ATTACK_DELAY_TIME;
		attackAction->attackType = E_ActionType::ActionType_Enemy_Attack;
		attackState->SetAction(attackAction);
		FSMState_Base_Action_Dmg* dmgAction = new FSMState_Base_Action_Dmg();
		dmgAction->setAnim = E_Anim::Anim_Damage;
		dmgState->SetAction(dmgAction);

		//==================================================================trigger delegate
			//idle
							//idle - move
		FSMStateTrigger_Base_Trigger_IdleTime* idleTimeTriggerAction = new FSMStateTrigger_Base_Trigger_IdleTime();
		idleTimeTriggerAction->idleTime = ENEMY_DRAGONFLY_IDLE_TIME;
		FSMStateTrigger_Base_Trigger_Dist2Player* idle2Move_AttackOutOfRange2PlayerAction = new FSMStateTrigger_Base_Trigger_Dist2Player();
		idle2Move_AttackOutOfRange2PlayerAction->inverseResult = true;
		idle2Move_AttackOutOfRange2PlayerAction->dist = ENEMY_DRAGONFLY_ATTACK_RANGE;
		//idle - attack
		FSMStateTrigger_Base_Trigger_IdleTime* idle2Attack_IdleTimeTriggerAction = new FSMStateTrigger_Base_Trigger_IdleTime();
		idle2Attack_IdleTimeTriggerAction->idleTime = ENEMY_DRAGONFLY_IDLE_TIME;
		FSMStateTrigger_Base_Trigger_Dist2Player* idle2Attack_AttackInRange2PlayerAction = new FSMStateTrigger_Base_Trigger_Dist2Player();
		idle2Attack_AttackInRange2PlayerAction->dist = ENEMY_DRAGONFLY_ATTACK_RANGE;
		//idle - dmg
		FSMStateTrigger_Base_Trigger_TakeDmg* idleTakeDmgTriggerAction = new FSMStateTrigger_Base_Trigger_TakeDmg();

		//move2player
						//move - idle
		FSMStateTrigger_Base_Trigger_IdleTime* moveTimeTrrigerAction = new  FSMStateTrigger_Base_Trigger_IdleTime();
		moveTimeTrrigerAction->idleTime = 6.0f;
		FSMStateTrigger_Base_Trigger_Dist2Player* move2Idle_AttackInRangeTriggerAction = new FSMStateTrigger_Base_Trigger_Dist2Player();
		//outOfAttackRangeTriggerAction->inverseResult = true;//ブル型の戻り値を逆転させる
		move2Idle_AttackInRangeTriggerAction->dist = ENEMY_DRAGONFLY_DIST_TO_PLAYER;
		//move - attack
					//move - dmg
		FSMStateTrigger_Base_Trigger_TakeDmg* moveTakeDmgTriggerAction = new FSMStateTrigger_Base_Trigger_TakeDmg();
		//AttackTime
						//attack - move
		FSMStateTrigger_Base_Trigger_IdleTime* attackTimeTriggerAction = new FSMStateTrigger_Base_Trigger_IdleTime();
		attackTimeTriggerAction->idleTime = ENEMY_DRAGONFLY_ATTACK_TIME;
		FSMStateTrigger_Base_Trigger_TakeDmg* attackTakeDmgTriggerAction = new FSMStateTrigger_Base_Trigger_TakeDmg();
		//dmg
							// - dmg
		FSMStateTrigger_Base_Trigger_TakeDmg* dmgTriggerAction = new FSMStateTrigger_Base_Trigger_TakeDmg();

		//==================================================trigger
			//idle
							//idle - move
		FSMStateTrigger* idleTimeEndTrigger = new FSMStateTrigger();//idle end
		idleTimeEndTrigger->SetTriggerParam(idleState, idleTimeTriggerAction);
		FSMStateTrigger* idle2Move_AttackRangeTrigger = new FSMStateTrigger();
		idle2Move_AttackRangeTrigger->SetTriggerParam(idleState, idle2Move_AttackOutOfRange2PlayerAction);
		FSMTriggerGroup* triggerGroup_idleTimeEnd = new FSMTriggerGroup(E_CheckTriggerType::CheckTriggerType_AllTrue, enemyFSM);
		triggerGroup_idleTimeEnd->AddTrigger(idleTimeEndTrigger);
		triggerGroup_idleTimeEnd->AddTrigger(idle2Move_AttackRangeTrigger);
		//idle - attack
		FSMStateTrigger* idle2Attack_AttackInRangeTrigger = new FSMStateTrigger();
		idle2Attack_AttackInRangeTrigger->SetTriggerParam(idleState, idle2Attack_AttackInRange2PlayerAction);
		FSMStateTrigger* idle2Attack_IdleTimeTrigger = new FSMStateTrigger();
		idle2Attack_IdleTimeTrigger->SetTriggerParam(idleState, idle2Attack_IdleTimeTriggerAction);
		FSMTriggerGroup* triggerGroup_idle2Attack = new FSMTriggerGroup(E_CheckTriggerType::CheckTriggerType_AllTrue, enemyFSM);
		triggerGroup_idle2Attack->AddTrigger(idle2Attack_AttackInRangeTrigger);
		triggerGroup_idle2Attack->AddTrigger(idle2Attack_IdleTimeTrigger);
		//idle 2 dmg
		FSMStateTrigger* idleTakeDmgTrigger = new FSMStateTrigger();//take dmg
		idleTakeDmgTrigger->SetTriggerParam(idleState, idleTakeDmgTriggerAction);
		FSMTriggerGroup* triggerGroup_idleTakeDmg = new FSMTriggerGroup(E_CheckTriggerType::CheckTriggerType_OneTrue, enemyFSM);
		triggerGroup_idleTakeDmg->AddTrigger(idleTakeDmgTrigger);

		//register state trigger
		idleState->RegisterTrigger(E_FSMState::FSMState_MoveToPlayer, triggerGroup_idleTimeEnd);
		idleState->RegisterTrigger(E_FSMState::FSMState_Attack, triggerGroup_idle2Attack);
		idleState->RegisterTrigger(E_FSMState::FSMState_Dmg, triggerGroup_idleTakeDmg);

		//move2Player
		FSMStateTrigger* move2PlayerContinueTimeTrigger = new FSMStateTrigger();
		move2PlayerContinueTimeTrigger->SetTriggerParam(move2PlayerState, moveTimeTrrigerAction);//time
		FSMStateTrigger* outOfAttackRangeTrigger = new FSMStateTrigger();
		outOfAttackRangeTrigger->SetTriggerParam(move2PlayerState, move2Idle_AttackInRangeTriggerAction);//out of range
		FSMTriggerGroup* triggerGroup_continueTime = new FSMTriggerGroup(E_CheckTriggerType::CheckTriggerType_OneTrue, enemyFSM);
		triggerGroup_continueTime->AddTrigger(move2PlayerContinueTimeTrigger);
		triggerGroup_continueTime->AddTrigger(outOfAttackRangeTrigger);

				//move - dmg
		FSMStateTrigger* moveTakeDmgTrigger = new FSMStateTrigger();
		moveTakeDmgTrigger->SetTriggerParam(move2PlayerState, moveTakeDmgTriggerAction);
		FSMTriggerGroup* triggerGroup_moveTakeDmg = new FSMTriggerGroup(E_CheckTriggerType::CheckTriggerType_OneTrue, enemyFSM);
		triggerGroup_moveTakeDmg->AddTrigger(moveTakeDmgTrigger);

		//register state trigger
		move2PlayerState->RegisterTrigger(E_FSMState::FSMState_Idle, triggerGroup_continueTime);
		move2PlayerState->RegisterTrigger(E_FSMState::FSMState_Dmg, triggerGroup_moveTakeDmg);

		//attack
		FSMStateTrigger* attackTimeTrigger = new FSMStateTrigger();
		attackTimeTrigger->SetTriggerParam(attackState, attackTimeTriggerAction);
		FSMTriggerGroup* triggerGroup_attackEnd = new FSMTriggerGroup(E_CheckTriggerType::CheckTriggerType_AllTrue, enemyFSM);
		triggerGroup_attackEnd->AddTrigger(attackTimeTrigger);
		FSMTriggerGroup* triggerGroup_attackTakeDmg = new FSMTriggerGroup(E_CheckTriggerType::CheckTriggerType_AllTrue, enemyFSM);
		triggerGroup_attackTakeDmg->AddTrigger(attackTimeTrigger);

		//register state trigger
		attackState->RegisterTrigger(E_FSMState::FSMState_Idle, triggerGroup_attackEnd);
		attackState->RegisterTrigger(E_FSMState::FSMState_Dmg, triggerGroup_attackTakeDmg);

		//dmg
		FSMStateTrigger* dmgTimeTrigger = new FSMStateTrigger();
		dmgTimeTrigger->SetTriggerParam(dmgState, dmgTriggerAction);
		FSMTriggerGroup* dmgTriggerGroup = new FSMTriggerGroup(E_CheckTriggerType::CheckTriggerType_AllFalse, enemyFSM);
		dmgTriggerGroup->AddTrigger(dmgTimeTrigger);
		//register state trigger
		dmgState->RegisterTrigger(E_FSMState::FSMState_MoveToPlayer, dmgTriggerGroup);

		//register fsm state
		enemyFSM->RegisterState(E_FSMState::FSMState_Idle, idleState, true);
		enemyFSM->RegisterState(E_FSMState::FSMState_MoveToPlayer, move2PlayerState, false);
		enemyFSM->RegisterState(E_FSMState::FSMState_Attack, attackState, false);
		enemyFSM->RegisterState(E_FSMState::FSMState_Dmg, dmgState, false);

		//==================================================================AIを設定(オートマトン


		enemy->SetState(false);//default false state

		enemy->RecordCurState();
		enemy->DoInit();

		//register
		GameMgr::GetInstance()->sceneObjMgr->RegisterCell(enemy);
		this->RegisterCell(enemy);

		return enemy;
#pragma endregion enemy_3
	}
	else if (cellType == E_CellType::CellType_Enemy_Firefly) {
#pragma region enemy_firefly

#pragma endregion enemy_firefly
	}
	else if (cellType == E_CellType::CellType_Grass) {
		BaseCell* grass = new BaseCell();
		grass->SetID(GameMgr::GetInstance()->sceneObjMgr->GetID());
		grass->SetCellType(E_CellType::CellType_Grass);
		grass->SetScale(aspect_factor);
		grass->SetDrawLayer(E_DrawLayer::DrawLayer_Bottom);
		//draw data
		DrawData* grassDrawData = RenderMgr::GetNewDrawData(nullptr, true, E_BatchingType::BatchingType_Enemy, true);
		if (grassDrawData == nullptr) {
			delete grassDrawData;
			return nullptr;
		}
		grassDrawData->SetShaderParam(grass_shaderParam);
		grassDrawData->SetSize(vector2s(GRASS_TEX_CELL_SIZE_X * 0.6f, GRASS_TEX_CELL_SIZE_Y * 0.6f));
		grass->SetDrawData(grassDrawData);

		GameMgr::GetInstance()->sceneObjMgr->RegisterCell(grass);

		return grass;
	}
	return nullptr;
}

BaseCell* CellMgr::GetCell(E_CellType cellType)
{
	int cellTypeIndex = (int)cellType;
	if (cellTypeIndex >= 4 && cellTypeIndex < 10) {//enemy

		if (cellDic.find(E_CellType::CellType_Enemy) == cellDic.end()) {
			BaseCell* targetCell = nullptr;
			targetCell = CreateCellEntity(cellType);
			Status* cellStatus = (Status*)targetCell->GetComponent(E_Component::Component_Status);
			if (cellStatus != nullptr) {
				cellStatus->ResetData(E_Status::Status_All);
			}

			return targetCell;
		}

		BaseCell* targetCell = nullptr;
		std::list<BaseCell*>::iterator iter;
		for (iter = cellDic[E_CellType::CellType_Enemy].begin(); iter != cellDic[E_CellType::CellType_Enemy].end(); iter++) {
			if ((*iter) == nullptr || (*iter)->CheckState() == true)continue;
			if (((Enemy*)(*iter))->GetEnemyType() != cellType)continue;
			targetCell = (*iter);
			break;
		}

		if (targetCell == nullptr) {
			targetCell = CreateCellEntity(cellType);
		}

		Status* cellStatus = (Status*)targetCell->GetComponent(E_Component::Component_Status);
		if (cellStatus != nullptr) {
			cellStatus->ResetData(E_Status::Status_All);
		}

		targetCell->DoInit();

		return targetCell;
	}
	else {

		BaseCell* targetCell = nullptr;
		std::list<BaseCell*>::iterator iter;
		for (iter = cellDic[cellType].begin(); iter != cellDic[cellType].end(); iter++) {
			if ((*iter) == nullptr || (*iter)->CheckState() == true)continue;

			targetCell = (*iter);
			break;
		}

		if (targetCell == nullptr) {
			targetCell = CreateCellEntity(cellType);
		}

		Status* cellStatus = (Status*)targetCell->GetComponent(E_Component::Component_Status);
		if (cellStatus != nullptr) {
			cellStatus->ResetData(E_Status::Status_All);
		}

		targetCell->DoInit();

		return targetCell;
	}
	return nullptr;
}


void CellMgr::RegisterCell(BaseCell* cell)
{
	if (cell == nullptr)return;

	E_CellType cellType = cell->GetCellType();
	if (cellDic.find(cellType) == cellDic.end()) {
		cellDic[cellType].clear();
	}

	cellDic[cellType].push_back(cell);
}

#pragma endregion cell_manager
