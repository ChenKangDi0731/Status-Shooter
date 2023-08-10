#include <map>
#include <list>
#include "main.h"
#include "gameLogic.h"
#include "gameObject.h"
#include "player.h"
#include "sceneObjManager.h"
#include "effectManager.h"
#include "battleManager.h"
#include "uiManager.h"
#include "ui.h"


#pragma region SceneObjMgr_method

SceneObjMgr::SceneObjMgr() :cellIdCounter(0), initOnce(false) {

	light_1_id = 0;
	light_2_id = 0;
	light_3_id = 0;

	drawCellDic.clear();
	cellDic.clear();
	bulletPools.clear();


	m_player = nullptr;

	normalBullet_ShaderParam = nullptr;
	laserBullet_ShaderParam = nullptr;
	cannonBullet_ShaderParam = nullptr;
	enemyBullet_normal_ShaderParam = nullptr;
	enemyBullet_normal_reflected_ShaderParam = nullptr;

	tempCollider = nullptr;

}

SceneObjMgr::~SceneObjMgr()
{
	drawCellDic.clear();
	cellDic.clear();

	m_player = nullptr;


	if (normalBullet_ShaderParam != nullptr) {
		delete normalBullet_ShaderParam;
	}
	normalBullet_ShaderParam = nullptr;

	if (laserBullet_ShaderParam != nullptr) {
		delete laserBullet_ShaderParam;
	}
	laserBullet_ShaderParam = nullptr;

	if (cannonBullet_ShaderParam != nullptr) {
		delete cannonBullet_ShaderParam;
	}
	cannonBullet_ShaderParam = nullptr;

	if (enemyBullet_normal_ShaderParam != nullptr) {
		delete enemyBullet_normal_ShaderParam;
	}
	enemyBullet_normal_ShaderParam = nullptr;

	if (enemyBullet_normal_reflected_ShaderParam != nullptr) {
		delete enemyBullet_normal_reflected_ShaderParam;
	}
	enemyBullet_normal_reflected_ShaderParam = nullptr;

	if (tempCollider != nullptr) {
		delete tempCollider;
	}
}

void SceneObjMgr::DoInit() {

	if (initOnce == false) {

		drawCellDic.clear();

		initOnce = true;
	}

	cellIdCounter = 0;
	cellDic.clear();
	lightDic.clear();

	//create temp collider
	tempCollider = new Collider();

	//=============================弾の描画データを作る
	ShaderParam_Normal* tempNormalBullet_ShaderParam = new ShaderParam_Normal();
	//拳銃
	tempNormalBullet_ShaderParam->shaderProgram = GameMgr::GetInstance()->shaderMgr->GetShaderProgram(E_Shader::Shader_Normal);
	tempNormalBullet_ShaderParam->texNo = GameMgr::GetInstance()->textureMgr->GetTextureID(E_Texture::Texture_Bullet_Package);
	tempNormalBullet_ShaderParam->texIndex = vector2s(BULLET_NEW_TEX_BULLET_START_INDEX_X, BULLET_NEW_TEX_BULLET_START_INDEX_Y);
	tempNormalBullet_ShaderParam->tex_w_count = BULLET_NEW_TEX_WIDTH_COUNT;
	tempNormalBullet_ShaderParam->tex_texel_size = bullet_tex_size;
	normalBullet_ShaderParam = tempNormalBullet_ShaderParam;

	ShaderParam_Normal* tempLaserBullet_ShaderParam = new ShaderParam_Normal();
	//レーザー
	tempLaserBullet_ShaderParam->shaderProgram = GameMgr::GetInstance()->shaderMgr->GetShaderProgram(E_Shader::Shader_Normal);
	tempLaserBullet_ShaderParam->texNo = GameMgr::GetInstance()->textureMgr->GetTextureID(E_Texture::Texture_Bullet_Package);
	tempLaserBullet_ShaderParam->texIndex = vector2s(BULLET_NEW_TEX_LASER_START_INDEX_X, BULLET_NEW_TEX_LASER_START_INDEX_Y);
	tempLaserBullet_ShaderParam->tex_w_count = BULLET_NEW_TEX_WIDTH_COUNT;
	tempLaserBullet_ShaderParam->tex_texel_size = bullet_tex_size;
	laserBullet_ShaderParam = tempLaserBullet_ShaderParam;

	ShaderParam_Normal* tempCannonBullet_ShaderParam = new ShaderParam_Normal();
	//大砲
	tempCannonBullet_ShaderParam->shaderProgram = GameMgr::GetInstance()->shaderMgr->GetShaderProgram(E_Shader::Shader_Normal);
	tempCannonBullet_ShaderParam->texNo = GameMgr::GetInstance()->textureMgr->GetTextureID(E_Texture::Texture_Bullet_Package);
	tempCannonBullet_ShaderParam->texIndex = vector2s(BULLET_NEW_TEX_CANNON_START_INDEX_X, BULLET_NEW_TEX_CANNON_START_INDEX_Y);
	tempCannonBullet_ShaderParam->tex_w_count = BULLET_NEW_TEX_WIDTH_COUNT;
	tempCannonBullet_ShaderParam->tex_texel_size = bullet_tex_size;
	cannonBullet_ShaderParam = tempCannonBullet_ShaderParam;

	ShaderParam_Normal* tempEnemyNormalBullet_ShaderParam = new ShaderParam_Normal();
	//敵の弾
	tempEnemyNormalBullet_ShaderParam->shaderProgram = GameMgr::GetInstance()->shaderMgr->GetShaderProgram(E_Shader::Shader_Normal);
	tempEnemyNormalBullet_ShaderParam->texNo = GameMgr::GetInstance()->textureMgr->GetTextureID(E_Texture::Texture_Bullet_Package);
	tempEnemyNormalBullet_ShaderParam->texIndex = vector2s(BULLET_NEW_TEX_ENEMY_NORMAL_BULLET_START_INDEX_X, BULLET_NEW_TEX_ENEMY_NORMAL_BULLET_START_INDEX_Y);
	tempEnemyNormalBullet_ShaderParam->tex_w_count = BULLET_NEW_TEX_WIDTH_COUNT;
	tempEnemyNormalBullet_ShaderParam->tex_texel_size = bullet_tex_size;
	enemyBullet_normal_ShaderParam = tempEnemyNormalBullet_ShaderParam;

	ShaderParam_Normal* tempEnemyNormalBullet_Reflected_ShaderParam = new ShaderParam_Normal();
	//敵の弾（反射された
	tempEnemyNormalBullet_Reflected_ShaderParam->shaderProgram = GameMgr::GetInstance()->shaderMgr->GetShaderProgram(E_Shader::Shader_Normal);
	tempEnemyNormalBullet_Reflected_ShaderParam->texNo = GameMgr::GetInstance()->textureMgr->GetTextureID(E_Texture::Texture_Bullet_Package);
	tempEnemyNormalBullet_Reflected_ShaderParam->texIndex = vector2s(BULLET_NEW_TEX_ENEMY_NORMAL_BULLET_REFLECTED_START_INDEX_X, BULLET_NEW_TEX_ENEMY_NORMAL_BULLET_REFLECTED_START_INDEX_Y);
	tempEnemyNormalBullet_Reflected_ShaderParam->tex_w_count = BULLET_NEW_TEX_WIDTH_COUNT;
	tempEnemyNormalBullet_Reflected_ShaderParam->tex_texel_size = bullet_tex_size;
	enemyBullet_normal_reflected_ShaderParam = tempEnemyNormalBullet_Reflected_ShaderParam;

#pragma region Player
	//==============================================プレイヤーを作る
	Player* p = new Player();
	p->SetID(this->GetID());
	p->SetPos(vector3s(0.0f, 0.0f, -30.0f), true);
	p->SetScale(aspect_factor);

	ShaderParam_Normal* player_shader_normal = new ShaderParam_Normal();
	player_shader_normal->shaderProgram = GameMgr::GetInstance()->shaderMgr->GetShaderProgram(E_Shader::Shader_Normal);
	player_shader_normal->texNo = GameMgr::GetInstance()->textureMgr->GetTextureID(E_Texture::Texture_Character_Package);
	player_shader_normal->texIndex = vector2s(PLAYER_IDLE_INDEX_X, PLAYER_IDLE_INDEX_Y);
	player_shader_normal->tex_w_count = PLAYER_TEX_W_COUNT;
	player_shader_normal->tex_texel_size = character_cell_tex_size;

	//アニメーションコンポーネントを追加
	Animation* playerAnimComponent = new Animation();
	if (p->RegisterComponent(E_Component::Component_Animation, playerAnimComponent) == false) {//register component first
		delete playerAnimComponent;
		playerAnimComponent = nullptr;
	}
	else {
		//アニメーションを追加
		//待機アニメーション
		AnimClip* playerIdleAnim = new AnimClip();
		int startTexIndex = PLAYER_IDLE_INDEX_Y * PLAYER_TEX_W_COUNT + PLAYER_IDLE_INDEX_X;
		int endTexIndex = startTexIndex + PLAYER_IDLE_FRAME_COUNT;
		playerIdleAnim->SetAnimParam(E_Anim::Anim_Idle, vector2s(startTexIndex, endTexIndex), 0.5f, true, true);
		playerAnimComponent->RegisterAnim(E_Anim::Anim_Idle, playerIdleAnim, true);
		//移動アニメーション
		AnimClip* playerMoveAnim = new AnimClip();
		startTexIndex = PLAYER_MOVE_INDEX_Y * PLAYER_TEX_W_COUNT + PLAYER_MOVE_INDEX_X;
		endTexIndex = startTexIndex + PLAYER_MOVE_FRAME_COUNT;
		playerMoveAnim->SetAnimParam(E_Anim::Anim_Move, vector2s(startTexIndex, endTexIndex), 0.79f, true, true);
		playerAnimComponent->RegisterAnim(E_Anim::Anim_Move, playerMoveAnim, false);
		//攻撃を受けるアニメーション
		AnimClip* playerHitedAnim = new AnimClip();
		startTexIndex = PLAYER_HITED_INDEX_Y * PLAYER_TEX_W_COUNT + PLAYER_HITED_INDEX_X;
		endTexIndex = startTexIndex + PLAYER_HITED_FRAME_COUNT;
		playerHitedAnim->SetAnimParam(E_Anim::Anim_Damage, vector2s(startTexIndex, endTexIndex), 0.79f, true, true);
		playerAnimComponent->RegisterAnim(E_Anim::Anim_Damage, playerHitedAnim, false);
	}

	//描画データを作る
	DrawData* playerDrawData = RenderMgr::GetNewDrawData(nullptr, true, E_BatchingType::BatchingType_Enemy, true);
	playerDrawData->SetShaderParam(player_shader_normal);
	playerDrawData->SetSize(vector2s(PLAYER_TEX_SIZE_X, PLAYER_TEX_SIZE_Y));
	p->SetDrawData(playerDrawData);

	//コライダーコンポーネントを追加（当たり判定
	Collider* playerColliderComponent = new Collider();
	p->RegisterComponent(E_Component::Component_Collider, playerColliderComponent);

	ColliderData* playerCollider_1 = new ColliderData(1, playerColliderComponent, true,
		E_ColliderType::Collider_Circle, p->GetSize(true) * 0.7f, vector3s(0, 0, 0));

	playerColliderComponent->AddColliderData(playerCollider_1);

#pragma endregion Player

#pragma region light_obj

	Light* gunLight = new Light();
	gunLight->SetID(this->GetID());
	gunLight->SetCellType(E_CellType::CellType_Light);
	gunLight->SetLightSize(vector2s(GUN_LIGHT_TEX_SIZE_X, GUN_LIGHT_TEX_SIZE_X));
	gunLight->SetScale(vector3s(0.5f, 0.5f, 1.0f));

	ShaderParam_Normal* gunLight_shaderParam = new ShaderParam_Normal();
	gunLight_shaderParam->shaderProgram = GameMgr::GetInstance()->shaderMgr->GetShaderProgram(E_Shader::Shader_Normal);
	gunLight_shaderParam->texNo = GameMgr::GetInstance()->textureMgr->GetTextureID(E_Texture::Texture_Character_Package);
	gunLight_shaderParam->texIndex = vector2s(GUN_LIGHT_START_INDEX_X, GUN_LIGHT_START_INDEX_Y);
	gunLight_shaderParam->tex_w_count = GUN_LIGHT_TEX_W_COUNT;
	gunLight_shaderParam->tex_texel_size = character_cell_tex_size;

	DrawData* gunLightDrawData = RenderMgr::GetNewDrawData(nullptr, true, E_BatchingType::BatchingType_Enemy, true);
	gunLightDrawData->SetSize(vector2s(GUN_LIGHT_TEX_SIZE_X, GUN_LIGHT_TEX_SIZE_Y));
	gunLightDrawData->SetShaderParam(gunLight_shaderParam);

	gunLight->SetDrawData(gunLightDrawData);

	//アニメーション
	Animation* gunAnimator = new Animation();
	gunLight->RegisterComponent(E_Component::Component_Animation, gunAnimator, true);

	AnimClip* gunLightAnim = new AnimClip();
	int startTexIndex = GUN_LIGHT_START_INDEX_Y * GUN_LIGHT_TEX_W_COUNT + GUN_LIGHT_START_INDEX_X;
	int endTexIndex = startTexIndex + GUN_LIGHT_FRAME_COUNT;
	gunLightAnim->SetAnimParam(E_Anim::Anim_Idle, vector2s(startTexIndex, endTexIndex), 0.5f, true, true);
	gunAnimator->RegisterAnim(E_Anim::Anim_Idle, gunLightAnim, true);

	if (gunLight_shaderParam != nullptr) {
		delete gunLight_shaderParam;
	}

	this->light_1_id = gunLight->GetID();

#pragma endregion light_obj


	RegisterCell(p);
	RegisterCell(gunLight);

	p->SetPlayerLight(gunLight);

	if (player_shader_normal != nullptr) {
		delete player_shader_normal;
	}

	p->SetState(false);
	gunLight->SetState(false);

}

void SceneObjMgr::UnInit(bool deleteCell) {
	if (deleteCell) {
		//リスト内全部オブジェクトを削除
		std::map<int, BaseCell*>::iterator iter;
		for (iter = cellDic.begin(); iter != cellDic.end(); iter++) {
			if (iter->second == nullptr)continue;
			BaseCell* temp = iter->second;
			iter->second = nullptr;
			delete temp;
		}

		drawCellDic.clear();

		cellDic.clear();
		lightDic.clear();
		bulletPools.clear();
		cellIdCounter = 0;


		if (normalBullet_ShaderParam != nullptr) {
			delete normalBullet_ShaderParam;
		}
		normalBullet_ShaderParam = nullptr;

		if (laserBullet_ShaderParam != nullptr) {
			delete laserBullet_ShaderParam;
		}
		laserBullet_ShaderParam = nullptr;

		if (cannonBullet_ShaderParam != nullptr) {
			delete cannonBullet_ShaderParam;
		}
		cannonBullet_ShaderParam = nullptr;

		if (enemyBullet_normal_ShaderParam != nullptr) {
			delete enemyBullet_normal_ShaderParam;
		}
		enemyBullet_normal_ShaderParam = nullptr;

		if (enemyBullet_normal_reflected_ShaderParam != nullptr) {
			delete enemyBullet_normal_reflected_ShaderParam;
		}
		enemyBullet_normal_reflected_ShaderParam = nullptr;

		if (tempCollider != nullptr) {
			delete tempCollider;
			tempCollider = nullptr;
		}
	}
	else {
		//TODO
	}
}

/// <summary>
/// リスト内全部オブジェクトを初期化
/// </summary>
void SceneObjMgr::DoInitCell() {
	for (auto& cell : cellDic) {
		if (cell.second == nullptr || cell.second->CheckState() == false) {
			continue;
		}

		cell.second->DoInit();
	}
}

/// <summary>
/// オブジェクト更新処理
/// </summary>
/// <param name="deltatime"></param>
void SceneObjMgr::DoUpdate(float deltatime) {

	for (auto& cell : cellDic) {
		if (cell.second == nullptr || cell.second->CheckState() == false) {
			continue;
		}
		float dTime = deltatime;
		if (cell.second->GetCellType() == E_CellType::CellType_UI) {
			dTime = deltatime * GameMgr::GetInstance()->timer->uiTimer;
		}
		else {
			dTime = deltatime * GameMgr::GetInstance()->timer->gameTimer_1;
		}
		cell.second->DoUpdate(dTime);

	}

}

void SceneObjMgr::DoLateUpdate(float deltatime) {
}

/// <summary>
/// 描画処理
/// </summary>
void SceneObjMgr::DoDraw()
{
	if (GameMgr::GetInstance()->GetCurGameState() == E_GameState::GameState_Game_Normal) {
		if (m_player != nullptr && m_player->inputLock == false) {
			DrawLine(m_player->GetTransform()->GetPos() + m_player->GetCurShootPos(), m_player->GetCurShootPos(), 2000.0f);
		}
	}
#pragma region draw
	std::map<int, std::map<int, BaseCell*>>::iterator iter;
	for (iter = drawCellDic.begin(); iter != drawCellDic.end(); iter++) {
		std::map<int, BaseCell*>& tempDic = iter->second;
		for (auto& cur : tempDic) {
			if (cur.second == nullptr)continue;

			if (cur.second->CheckState() == false) {
				continue;
			}
			Transform::AdjustZPos(cur.second);//Z座標を調整
			
			DrawData* tempDrawData = cur.second->GetDrawData();
			if (tempDrawData != nullptr) {
				if (tempDrawData->GetDrawDataType() == E_DrawDataType::DrawData_Tile && tempDrawData->GetTileData() != nullptr) {
					DrawTile(cur.second->GetTransform(), cur.second->GetDrawData());
				}
				else {
					DrawCell(cur.second->GetTransform(), cur.second->GetDrawData());
				}
			}
			else {
				if (cur.second->GetCellType() == E_CellType::CellType_UI) {//オブジェクトはUIの場合
					if (((UI*)(cur.second))->GetUIType() == E_UIType::UI_Word) {//文字の場合
						UISprite* childArray = ((UIWord*)cur.second)->GetSpriteArray();
						if (childArray != nullptr) {
							for (int index = 0; index < UI_WORD_CHILD_COUNT; index++) {
								if (childArray[index].CheckState() == false)continue;
								DrawCell(childArray[index].GetTransform(), childArray[index].GetDrawData());
							}
						}
					}
					else if (((UI*)cur.second)->GetUIType() == E_UIType::UI_Group) {//グループの場合（複数のUIオブジェクト
						std::map<int, UI*>& childDic = ((UIGroup*)cur.second)->GetChildDic();
						if (childDic.size() != 0) {
							std::map<int, UI*>::iterator childDic_iter;
							for (childDic_iter = childDic.begin(); childDic_iter != childDic.end(); childDic_iter++) {
								if (childDic_iter->second == nullptr || childDic_iter->second->CheckState() == false)continue;

								if (childDic_iter->second->GetUIType() == E_UIType::UI_Word) {//文字の場合
									UISprite* childArray = ((UIWord*)childDic_iter->second)->GetSpriteArray();
									if (childArray != nullptr) {
										for (int index = 0; index < UI_WORD_CHILD_COUNT; index++) {
											if (childArray[index].CheckState() == false)continue;
											DrawCell(childArray[index].GetTransform(), childArray[index].GetDrawData());
										}
									}
								}
								else {
									DrawCell(childDic_iter->second->GetTransform(), childDic_iter->second->GetDrawData());
								}
							}
						}
					}
					else {
						DrawCell(cur.second->GetTransform(), cur.second->GetDrawData());
					}
				}
			}

		}
	}
#pragma endregion draw

}

/// <summary>
/// オブジェクトをリストに追加
/// </summary>
/// <param name="id"></param>
/// <param name="cell"></param>
void SceneObjMgr::RegisterCell(int id, BaseCell* cell) {
	if (cell == nullptr)return;

	if (cellDic.find(id) == cellDic.end()) {
		cellDic[id] = cell;
	}
	else {
		//error
	}

	int layer = (int)cell->GetDrawLayer();
	std::map<int, BaseCell*>& tempDic = drawCellDic[layer];
	if (tempDic.find(id) == cellDic.end()) {
		tempDic[id] = cell;
	}
}

/// <summary>
/// オブジェクトをリストに追加
/// </summary>
/// <param name="cell"></param>
void SceneObjMgr::RegisterCell(BaseCell* cell) {
	if (cell == nullptr) {
		return;
	}
	int id = cell->GetID();
	if (cellDic.find(id) == cellDic.end()) {
		cellDic[id] = cell;
	}
	else {
		//error
	}

	int layer = (int)cell->GetDrawLayer();
	std::map<int, BaseCell*>& tempDic = drawCellDic[layer];
	if (tempDic.find(id) == tempDic.end()) {
		tempDic[id] = cell;
	}

	E_CellType cellType = cell->GetCellType();

	if (cellType == E_CellType::CellType_Floor) {
	}
	else if (cellType == E_CellType::CellType_Light) {
		lightDic[id] = (Light*)cell;
	}
	else if (cellType == E_CellType::CellType_Player) {
		m_player = (Player*)cell;
	}
	else if (cellType == E_CellType::CellType_Bullet) {
		bulletPools.push_back((Bullet*)cell);
	}

	//storage in cellTypeDic
	if (cellTypeDic.find(cellType) == cellTypeDic.end()) {
		cellTypeDic[cellType].clear();//init
	}
	if (cellTypeDic[cellType].find(id) != cellTypeDic[cellType].end()) {
		return; //error
	}
	cellTypeDic[cellType][id] = cell;
}

void SceneObjMgr::UnRegisterCell(int id) {
	if (cellDic.find(id) == cellDic.end()) {
		return;
	}
	BaseCell* cell = cellDic[id];
	cellDic.erase(id);

	int layer = (int)cell->GetDrawLayer();
	std::map<int, BaseCell*>& tempDic = drawCellDic[layer];
	if (tempDic.find(id) != tempDic.end()) {
		tempDic.erase(id);
	}

	if (cell != nullptr) {
		delete cell;
	}
}

void SceneObjMgr::UnRegisterCell(BaseCell* cell) {
	if (cell == nullptr)return;
	int id = cell->GetID();
	if (cellDic.find(id) == cellDic.end()) {
		return;
	}

	cellDic.erase(id);

	int layer = (int)cell->GetDrawLayer();
	std::map<int, BaseCell*>& tempDic = drawCellDic[layer];
	std::map<int, BaseCell*>::iterator iter = tempDic.find(id);
	if (iter != tempDic.end()) {
		tempDic.erase(iter);
	}

	if (cell != nullptr) {
		delete cell;
	}

}

/// <summary>
/// IDでオブジェクトを取得
/// </summary>
/// <param name="id"></param>
/// <returns></returns>
BaseCell* SceneObjMgr::GetCell(int id) {
	std::map<int, BaseCell*>::iterator iter = cellDic.find(id);
	if (iter != cellDic.end()) {
		return iter->second;
	}

	return nullptr;
}

/// <summary>
/// 半径以内のオブジェクトを取得
/// </summary>
/// <param name="cellType">　オブジェクトの種類　</param>
/// <param name="pos">　円の中心　</param>
/// <param name="range">　半径　</param>
/// <param name="includeInactive">　表示してないオブジェクトも含めるかどうか　</param>
/// <returns></returns>
std::list<BaseCell*> SceneObjMgr::GetCellsByRange(E_CellType cellType, vector3s pos, float range, bool includeInactive)
{
	std::list<BaseCell*> resultArray = std::list<BaseCell*>();
	resultArray.clear();

	if (range > 0) {
		if (cellTypeDic.find(cellType) == cellTypeDic.end()) {
			std::map<int, BaseCell*>::iterator iter;
			for (iter = cellDic.begin(); iter != cellDic.end(); iter++) {
				if (iter->second == nullptr)continue;
				if (iter->second->GetCellType() != cellType || (iter->second->CheckState() == false && includeInactive == false))continue;

				float dist = VecDistS(pos, iter->second->GetTransform()->GetPos());
				if (dist < range * range) {
					resultArray.push_back(iter->second);
				}
			}
		}
		else {
			std::map<int, BaseCell*>::iterator iter;
			for (iter = cellTypeDic[cellType].begin(); iter != cellTypeDic[cellType].end(); iter++) {
				if (iter->second == nullptr)continue;
				if (iter->second->GetCellType() != cellType || (iter->second->CheckState() == false && includeInactive == false))continue;

				float dist = VecDistS(pos, iter->second->GetTransform()->GetPos());
				if (dist < range * range) {
					resultArray.push_back(iter->second);
				}
			}
		}
	}
	return resultArray;
}

/// <summary>
/// 特定な種類のオブジェクトを全部表示・非表示する
/// </summary>
/// <param name="cellType"></param>
/// <param name="show"></param>
void SceneObjMgr::ShowByCellType(E_CellType cellType, bool show)
{
	if (cellTypeDic.find(cellType) == cellTypeDic.end()) {
		return;
	}

	std::map<int, BaseCell*>& tempDic = cellTypeDic[cellType];
	std::map<int, BaseCell*>::iterator iter;
	for (iter = tempDic.begin(); iter != tempDic.end(); iter++) {
		if (iter->second == nullptr)continue;
		iter->second->SetState(show);
	}
}

void SceneObjMgr::ChangeCellLayer(BaseCell* cell, E_DrawLayer layer)
{
	if (cell == nullptr)return;
	int curLayer = (int)cell->GetDrawLayer();
	int targetLayer = (int)layer;

	if (curLayer == targetLayer)return;

	int id = cell->GetID();
	std::map<int, BaseCell*>& tempDic = drawCellDic[curLayer];
	if (tempDic.find(id) != tempDic.end()) {
		tempDic.erase(id);
	}

	cell->SetDrawLayer(layer);

	tempDic = drawCellDic[targetLayer];
	if (tempDic.find(id) == tempDic.end()) {
		tempDic[id] = cell;
	}
	else {
		//error
	}
}

/// <summary>
/// プレイヤーポインタを取得
/// </summary>
/// <returns></returns>
Player* SceneObjMgr::GetPlayer()
{
	return m_player;
}

Light* SceneObjMgr::GetLightWithIndex(int index)
{
	if (index == 1) {
		return GetLight(light_1_id);
	}
	else if (index == 2) {
		return GetLight(light_2_id);
	}
	else if (index == 3) {
		return GetLight(light_3_id);
	}
	return nullptr;
}
Light* SceneObjMgr::GetLight(int lightID)
{
	if (lightID == 0 || lightDic.size() == 0) {
		return nullptr;
	}

	return lightDic[lightID];
}

/// <summary>
/// プレイヤー当たり判定
/// </summary>
/// <param name="player"></param>
void SceneObjMgr::CheckPlayerCollision(Player* player) {
	if (player == nullptr)return;

	if (cellTypeDic.find(E_CellType::CellType_Enemy) != cellTypeDic.end()) {

		Status_Player* playerStatus = (Status_Player*)player->GetComponent(E_Component::Component_Status);
		if (playerStatus != nullptr && playerStatus->CheckCanBeAttack() == true) {
			std::map<int, BaseCell*>& tempDic = cellTypeDic[E_CellType::CellType_Enemy];

			std::map<int, BaseCell*>::iterator iter;
			for (iter = tempDic.begin(); iter != tempDic.end(); iter++) {
				if (iter->second == nullptr || iter->second->CheckState() == false) {
					continue;
				}

				if (Collider::CollisionDetect(player, iter->second)) {

					//dmg
					int dmg = GameMgr::GetInstance()->battleMgr->GetHurtDmg(player, ((Enemy*)iter->second));
					playerStatus->ChangeData(E_Status::Status_Hp, -dmg, false);

					vector3s effectPosOffset = vector3s(0.0f, 20.0f, -10.0f);
					effectPosOffset.x += rand() % 20 - 10;
					effectPosOffset.y += rand() % 15 - 8;
					GameMgr::GetInstance()->uiMgr->ShowDmgUI(dmg, 0.8f, player->GetTransform()->GetPos() + effectPosOffset);
					GameMgr::GetInstance()->effectMgr->CreateEffect(E_Effect::Effect_PlayerHit, nullptr, player->GetTransform()->GetPos(), vector3s(0, 0, 0));
					if (playerStatus->IsDead()) {
						//game over
						GameMgr::GetInstance()->SwitchSubGameState(E_GameState::GameState_Game_Lose, true);
					}
					else {

						//hit player 
						vector3s repulseDir = player->GetTransform()->GetPos() - iter->second->GetTransform()->GetPos();
						repulseDir.z = 0.0f;
						player->SetForceMovement(repulseDir, PLAYER_REPULSE_FORCE, PLAYER_REPULSE_TIME, false);

						//hurt anim
						Animation* playerAnim = (Animation*)player->GetComponent(E_Component::Component_Animation);
						if (playerAnim != nullptr) {
							playerAnim->SwitchAnim(E_Anim::Anim_Damage, true, false);
						}
						playerStatus->SetInvincible(PLAYER_BE_HIT_INVINCIBLE_TIME);
					}
					GameMgr::GetInstance()->effectMgr->SetCameraShake(CAMERA_SHAKE_PLAYER_HURT_TIME, CAMERA_SHAKE_PLAYER_HURT_FORCE);

					return;

				}
			}
		}
	}

}

Collider* SceneObjMgr::GetTempCollider()
{
	return tempCollider;
}

void SceneObjMgr::InitPlayerTitleState()
{
	if (m_player != nullptr) {
		m_player->SetPos(vector3s(0, 0, 0), true);
		m_player->SetState(true);

		Animation* anim = (Animation*)m_player->GetComponent(E_Component::Component_Animation);
		if (anim != nullptr) {
			anim->SwitchAnim(E_Anim::Anim_Idle, true, false);
		}

		Status_Player* playerStatus = (Status_Player*)m_player->GetComponent(E_Component::Component_Status);

		if (playerStatus != nullptr) {
			playerStatus->SetDefaultData(E_Status::Status_Hp, PLAYER_DEFAULT_STATUS_MAX_HP, true);
			playerStatus->SetDefaultData(E_Status::Status_Atk, PLAYER_DEFAULT_STATUS_MAX_ATK, true);
			playerStatus->SetDefaultData(E_Status::Status_Def, PLAYER_DEFAULT_STATUS_MAX_DEF, true);
			playerStatus->SetDefaultData(E_Status::Status_Exp, PLAYER_DEFAULT_STATUS_MAX_EXP, true);

			playerStatus->SetDefaultData(E_Status::Status_Consume_Hp, PLAYER_DEFAULT_STATUS_CONSUME_HP, true);
			playerStatus->SetDefaultData(E_Status::Status_Consume_Atk, PLAYER_DEFAULT_STATUS_CONSUME_ATK, true);
			playerStatus->SetDefaultData(E_Status::Status_Consume_Def, PLAYER_DEFAULT_STATUS_CONSUME_DEF, true);
			playerStatus->SetDefaultData(E_Status::Status_Consume_Exp, PLAYER_DEFAULT_STATUS_CONSUME_EXP, true);

			playerStatus->ResetLevel();
			playerStatus->ResetData(E_Status::Status_All);
		}
	}
}

/// <summary>
/// プレイヤー初期化
/// </summary>
void SceneObjMgr::InitPlayerObjState()
{
	//位置初期化
	if (m_player != nullptr) {
		m_player->SetPos(vector3s(0, 0, 0), true);
		m_player->SetState(true);
	}
	for (int index = 0; index < 3; index++) {
		Light* light = nullptr;
		light = GetLightWithIndex(index + 1);
		if (light != nullptr) {
			light->SetState(true);
		}
	}

	//ステータスを初期化
	Status_Player* playerStatus = (Status_Player*)m_player->GetComponent(E_Component::Component_Status);

	if (playerStatus != nullptr) {
		playerStatus->SetDefaultData(E_Status::Status_Hp, PLAYER_DEFAULT_STATUS_MAX_HP, true);
		playerStatus->SetDefaultData(E_Status::Status_Atk, PLAYER_DEFAULT_STATUS_MAX_ATK, true);
		playerStatus->SetDefaultData(E_Status::Status_Def, PLAYER_DEFAULT_STATUS_MAX_DEF, true);
		playerStatus->SetDefaultData(E_Status::Status_Exp, PLAYER_DEFAULT_STATUS_MAX_EXP, true);

		playerStatus->SetDefaultData(E_Status::Status_Consume_Hp, PLAYER_DEFAULT_STATUS_CONSUME_HP, true);
		playerStatus->SetDefaultData(E_Status::Status_Consume_Atk, PLAYER_DEFAULT_STATUS_CONSUME_ATK, true);
		playerStatus->SetDefaultData(E_Status::Status_Consume_Def, PLAYER_DEFAULT_STATUS_CONSUME_DEF, true);
		playerStatus->SetDefaultData(E_Status::Status_Consume_Exp, PLAYER_DEFAULT_STATUS_CONSUME_EXP, true);

		playerStatus->ResetLevel();
		playerStatus->ResetData(E_Status::Status_All);
	}

}


void SceneObjMgr::UninitPlayerObjState()
{
	if (m_player != nullptr) {
		//m_player->SetState(false);
	}
	for (int index = 0; index < 3; index++) {
		Light* light = nullptr;
		light = GetLightWithIndex(index + 1);
		if (light != nullptr) {
			light->SetState(false);
		}
	}

}

/// <summary>
/// 弾を生成
/// </summary>
/// <param name="bulletType">　種類　</param>
/// <returns></returns>
Bullet* SceneObjMgr::CreateBulletEntity(E_BulletType bulletType)
{
	if (bulletType == E_BulletType::BulletType_Normal) {//拳銃弾
		Bullet* bullet = new Bullet_Normal();
		if (bullet != nullptr) {
			bullet->SetCellType(E_CellType::CellType_Bullet);
			bullet->SetID(this->GetID());

			DrawData* bulletDrawData = RenderMgr::GetNewDrawData(nullptr, true, E_BatchingType::BatchingType_Bullet, true);
			if (bulletDrawData == nullptr) {
				delete bullet;
				return nullptr;
			}
			bulletDrawData->SetSize(vector2s(BULLET_CELL_TEX_SIZE_X, BULLET_CELL_TEX_SIZE_Y));
			bulletDrawData->SetShaderParam(normalBullet_ShaderParam);
			bullet->SetDrawData(bulletDrawData);

			Collider* bulletCollider = new Collider();
			bullet->RegisterComponent(E_Component::Component_Collider, bulletCollider);
			ColliderData* bulletColliderData = new ColliderData(0, bulletCollider,
				true, E_ColliderType::Collider_Rect, bullet->GetSize(false), vector3s(0, 0, 0));
			bulletCollider->AddColliderData(bulletColliderData);
			bullet->SetState(false);

			RegisterCell(bullet);
		}
		return bullet;
	}
	else if (bulletType == E_BulletType::BulletType_Shotgun) {//ショットガン
		Bullet* bullet = new Bullet_Shotgun();
		if (bullet != nullptr) {
			bullet->SetCellType(E_CellType::CellType_Bullet);
			bullet->SetID(this->GetID());

			DrawData* bulletDrawData = RenderMgr::GetNewDrawData(nullptr, true, E_BatchingType::BatchingType_Bullet, true);
			if (bulletDrawData == nullptr) {
				delete bullet;
				return nullptr;
			}
			bulletDrawData->SetSize(vector2s(BULLET_CELL_TEX_SIZE_X, BULLET_CELL_TEX_SIZE_Y));
			bulletDrawData->SetShaderParam(normalBullet_ShaderParam);
			bullet->SetDrawData(bulletDrawData);

			Collider* bulletCollider = new Collider();
			bullet->RegisterComponent(E_Component::Component_Collider, bulletCollider);
			ColliderData* bulletColliderData = new ColliderData(0, bulletCollider,
				true, E_ColliderType::Collider_Rect, bullet->GetSize(false), vector3s(0, 0, 0));
			bulletCollider->AddColliderData(bulletColliderData);
			bullet->SetState(false);

			RegisterCell(bullet);
		}
		return bullet;
	}
	else if (bulletType == E_BulletType::BulletType_Laser) {//レーザー
		Bullet* bullet = new Bullet_Laser();
		if (bullet != nullptr) {
			bullet->SetCellType(E_CellType::CellType_Bullet);
			bullet->SetID(this->GetID());
			vector3s defaultScale = vector3s(BULLET_LASER_DEFAULT_SCALE_X, BULLET_LASER_DEFAULT_SCALE_Y, 1.0f);
			bullet->SetScale(defaultScale);

			DrawData* bulletDrawData = RenderMgr::GetNewDrawData(nullptr, true, E_BatchingType::BatchingType_Bullet, true);
			if (bulletDrawData == nullptr) {
				delete bullet;
				return nullptr;
			}
			bulletDrawData->SetSize(vector2s(BULLET_LASER_TEX_SIZE_X, BULLET_LASER_TEX_SIZE_Y));
			bulletDrawData->SetShaderParam(laserBullet_ShaderParam);
			bullet->SetDrawData(bulletDrawData);

			Collider* bulletCollider = new Collider();
			bullet->RegisterComponent(E_Component::Component_Collider, bulletCollider);
			ColliderData* bulletColliderData = new ColliderData(0, bulletCollider,
				true, E_ColliderType::Collider_Rect, bullet->GetSize(false), vector3s(0, 0, 0));
			bulletCollider->AddColliderData(bulletColliderData);
			bullet->SetState(false);

			//movement(scale anim
			Movement_Scale* scaleMovement = new Movement_Scale(bullet, true, defaultScale, vector3s(defaultScale.x, 0.0f, 1.0f), LASER_LIFE_TIME, 0.0f);
			bullet->AddMovement(scaleMovement);

			RegisterCell(bullet);
		}
		return bullet;
	}
	else if (bulletType == E_BulletType::BulletType_Cannon) {//大砲
		Bullet* bullet = new Bullet_Cannon();
		if (bullet != nullptr) {
			bullet->SetCellType(E_CellType::CellType_Bullet);
			bullet->SetID(this->GetID());
			bullet->SetScale(vector3s(3.0f, 3.0f, 1.0f));

			DrawData* bulletDrawData = RenderMgr::GetNewDrawData(nullptr, true, E_BatchingType::BatchingType_Bullet, true);
			if (bulletDrawData == nullptr) {
				delete bullet;
				return nullptr;
			}
			bulletDrawData->SetSize(vector2s(BULLET_CANNON_CELL_SIZE_X, BULLET_CANNON_CELL_SIZE_Y));
			bulletDrawData->SetShaderParam(cannonBullet_ShaderParam);
			bullet->SetDrawData(bulletDrawData);

			Collider* bulletCollider = new Collider();
			bullet->RegisterComponent(E_Component::Component_Collider, bulletCollider);
			ColliderData* bulletColliderData = new ColliderData(0, bulletCollider,
				true, E_ColliderType::Collider_Circle, bullet->GetSize(false), vector3s(0, 0, 0));
			bulletCollider->AddColliderData(bulletColliderData);
			bullet->SetState(false);

			RegisterCell(bullet);
		}
		return bullet;
	}
	//敵の弾
	else if (bulletType == E_BulletType::BulletType_Enemy_Normal) {
		Bullet* bullet = new Bullet_Normal();
		if (bullet != nullptr) {
			bullet->SetCellType(E_CellType::CellType_Bullet);
			bullet->SetBulletType(E_BulletType::BulletType_Enemy_Normal);
			bullet->SetID(this->GetID());

			DrawData* bulletDrawData = RenderMgr::GetNewDrawData(nullptr, true, E_BatchingType::BatchingType_Bullet, true);
			if (bulletDrawData == nullptr) {
				delete bullet;
				return nullptr;
			}
			bulletDrawData->SetSize(vector2s(BULLET_ENEMY_CELL_TEX_SIZE_X, BULLET_ENEMY_CELL_TEX_SIZE_Y));
			bulletDrawData->SetShaderParam(enemyBullet_normal_ShaderParam);
			bullet->SetDrawData(bulletDrawData);

			Collider* bulletCollider = new Collider();
			bullet->RegisterComponent(E_Component::Component_Collider, bulletCollider);
			ColliderData* bulletColliderData = new ColliderData(0, bulletCollider,
				true, E_ColliderType::Collider_Rect, bullet->GetSize(false), vector3s(0, 0, 0));
			bulletCollider->AddColliderData(bulletColliderData);
			bullet->SetState(false);

			RegisterCell(bullet);
		}
		return bullet;
	}
	else if (bulletType == E_BulletType::BulletType_Enemy_Normal_Reflected) {//反射された敵の弾
		Bullet* bullet = new Bullet_Normal();
		if (bullet != nullptr) {
			bullet->SetCellType(E_CellType::CellType_Bullet);
			bullet->SetBulletType(E_BulletType::BulletType_Enemy_Normal_Reflected);
			bullet->SetID(this->GetID());

			DrawData* bulletDrawData = RenderMgr::GetNewDrawData(nullptr, true, E_BatchingType::BatchingType_Bullet, true);
			if (bulletDrawData == nullptr) {
				delete bullet;
				return nullptr;
			}
			bulletDrawData->SetSize(vector2s(BULLET_ENEMY_CELL_TEX_SIZE_X, BULLET_ENEMY_CELL_TEX_SIZE_Y));
			bulletDrawData->SetShaderParam(enemyBullet_normal_reflected_ShaderParam);
			bullet->SetDrawData(bulletDrawData);

			Collider* bulletCollider = new Collider();
			bullet->RegisterComponent(E_Component::Component_Collider, bulletCollider);
			ColliderData* bulletColliderData = new ColliderData(0, bulletCollider,
				true, E_ColliderType::Collider_Rect, bullet->GetSize(false), vector3s(0, 0, 0));
			bulletCollider->AddColliderData(bulletColliderData);
			bullet->SetState(false);

			RegisterCell(bullet);
		}
		return bullet;
	}

	return nullptr;
}

/// <summary>
/// 弾を獲得
/// </summary>
/// <param name="bulletType"></param>
/// <returns></returns>
Bullet* SceneObjMgr::GetBullet(E_BulletType bulletType)
{
	//リスト内に何もない場合は弾を生成する
	if (bulletPools.size() == 0) {
		Bullet* bullet = CreateBulletEntity(bulletType);

		return bullet;
	}

	std::list<Bullet*>::iterator iter;
	for (iter = bulletPools.begin(); iter != bulletPools.end(); iter++) {
		if (*iter == nullptr || (*iter)->bulletState == true || (*iter)->GetBulletType() != bulletType) {
			continue;
		}

		return (*iter);
	}

	//弾を生成する
	Bullet* bullet = CreateBulletEntity(bulletType);

	return bullet;
}

/// <summary>
/// 弾を発射する
/// </summary>
/// <param name="attacker">　攻撃者</param>
/// <param name="atk">　攻撃力</param>
/// <param name="bulletType">　種類</param>
/// <param name="startPoint">　発射位置</param>
/// <param name="dir">　弾の方向</param>
/// <param name="speed">　弾のスピード</param>
void SceneObjMgr::ShootBullet(BaseCell* attacker, int atk, E_BulletType bulletType, vector3s startPoint, vector3s dir, float speed)
{
	if (attacker == nullptr)return;

	if (attacker->GetCellType() == E_CellType::CellType_Player) {

		if (bulletType == E_BulletType::BulletType_Normal) {//拳銃弾（Exp弾）
			int normalBulletCount = GameMgr::GetInstance()->battleMgr->GetBulletCount(E_Status::Status_Exp);
			if (normalBulletCount == 2) {//複数の弾を発射する場合

				float halfAngle = BULLET_NORMAL_OFFSET_ANGLE_HALF;
				vector3s tempDir = startPoint - attacker->GetTransform()->GetPos();
				vector3s tempV_2 = tempDir;

				float tempX = tempDir.x;
				float tempY = tempDir.y;
				tempDir.x = tempX * cosf(halfAngle * DEG2RAD) - tempY * sinf(halfAngle * DEG2RAD);
				tempDir.y = tempX * sinf(halfAngle * DEG2RAD) + tempY * cosf(halfAngle * DEG2RAD);

				tempX = tempV_2.x;
				tempY = tempV_2.y;

				tempV_2.x = tempX * cosf(-halfAngle * DEG2RAD) - tempY * sinf(-halfAngle * DEG2RAD);
				tempV_2.y = tempX * sinf(-halfAngle * DEG2RAD) + tempY * cosf(-halfAngle * DEG2RAD);

				vector3s shootPoint_1 = attacker->GetTransform()->GetPos() + tempDir;
				vector3s shootPoint_2 = attacker->GetTransform()->GetPos() + tempV_2;

				Bullet* bullet_1 = GetBullet(bulletType);
				if (bullet_1 != nullptr) {
					bullet_1->bulletCurAtk = atk;
					bullet_1->BindAttacker(attacker);//攻撃者を設定
					bullet_1->SetDir(dir);//方向を設定
					bullet_1->SetSpeed(speed);//スピードを設定
					bullet_1->SetStartPos(shootPoint_1);//生成位置を設定
					bullet_1->SetBulletLifeTime(-1);//弾の持続時間（レーザー以外は‐1

					bullet_1->DoInit(true);
					//反射回数を設定
					bullet_1->SetMaxReflectCount(GameMgr::GetInstance()->battleMgr->GetReflectCount(E_Status::Status_Exp));
					bullet_1->SetState(true);
				}

				Bullet* bullet_2 = GetBullet(bulletType);
				if (bullet_2 != nullptr) {
					bullet_2->bulletCurAtk = atk;
					bullet_2->BindAttacker(attacker);//攻撃者を設定
					bullet_2->SetDir(dir);//方向を設定
					bullet_2->SetSpeed(speed);//スピードを設定
					bullet_2->SetStartPos(shootPoint_2);//生成位置を設定
					bullet_2->SetBulletLifeTime(-1);//弾の持続時間（レーザー以外は‐1


					bullet_2->DoInit(true);
					//反射回数を設定
					bullet_2->SetMaxReflectCount(GameMgr::GetInstance()->battleMgr->GetReflectCount(E_Status::Status_Exp));
					bullet_2->SetState(true);
				}

			}
			else {
				Bullet* bullet = GetBullet(bulletType);
				if (bullet == nullptr) {
					return;//error
				}

				bullet->BindAttacker(attacker);//攻撃者を設定
				bullet->bulletCurAtk = atk;
				bullet->SetSpeed(speed);//スピードを設定
				bullet->SetDir(dir);//方向を設定
				bullet->SetStartPos(startPoint);//生成位置を設定
				bullet->SetBulletLifeTime(-1);//弾の持続時間（レーザー以外は‐1


				bullet->DoInit(true);
				//反射回数を設定
				bullet->SetMaxReflectCount(GameMgr::GetInstance()->battleMgr->GetReflectCount(E_Status::Status_Exp));
				bullet->SetState(true);
			}
		}
		else if (bulletType == E_BulletType::BulletType_Shotgun) {//ショットガン（Def弾）

			int bulletCount = GameMgr::GetInstance()->battleMgr->GetBulletCount(E_Status::Status_Def);
			if (bulletCount <= 1)return;//error

			float totalAngle = (bulletCount - 1) * BULLET_SHOOTPOS_OFFSET_ANGLE;
			vector3s shootDir = dir;

			float tempX = dir.x;
			float tempY = dir.y;
			shootDir.x = tempX * cosf(totalAngle * 0.5f * DEG2RAD) - tempY * sinf(totalAngle * 0.5f * DEG2RAD);
			shootDir.y = tempX * sinf(totalAngle * 0.5f * DEG2RAD) + tempY * cosf(totalAngle * 0.5f * DEG2RAD);

			Bullet* bullet = GetBullet(bulletType);
			if (bullet != nullptr) {
				bullet->bulletCurAtk = atk;
				bullet->BindAttacker(attacker);
				bullet->SetSpeed(speed);
				bullet->SetStartPos(startPoint);
				bullet->SetDir(shootDir);

				bullet->DoInit(true);
				//反射回数を設定
				bullet->SetMaxReflectCount(GameMgr::GetInstance()->battleMgr->GetReflectCount(E_Status::Status_Def));
				bullet->SetState(true);
			}

			vector3s tempDir = shootDir;

			for (int index = 1; index < bulletCount; index++) {

				tempX = shootDir.x;
				tempY = shootDir.y;

				tempDir.x = tempX * cosf(-BULLET_SHOOTPOS_OFFSET_ANGLE * index * DEG2RAD) - tempY * sinf(-BULLET_SHOOTPOS_OFFSET_ANGLE * index * DEG2RAD);
				tempDir.y = tempX * sinf(-BULLET_SHOOTPOS_OFFSET_ANGLE * index * DEG2RAD) + tempY * cosf(-BULLET_SHOOTPOS_OFFSET_ANGLE * index * DEG2RAD);

				bullet = GetBullet(bulletType);
				if (bullet != nullptr) {
					bullet->bulletCurAtk = atk;
					bullet->BindAttacker(attacker);
					bullet->SetSpeed(speed);
					bullet->SetStartPos(startPoint);
					bullet->SetDir(tempDir);

					bullet->DoInit(true);
					bullet->SetMaxReflectCount(GameMgr::GetInstance()->battleMgr->GetReflectCount(E_Status::Status_Def));
					bullet->SetState(true);
				}
			}

		}
		else if (bulletType == E_BulletType::BulletType_Laser) {//レーザー（HP弾）
			Bullet* bullet = GetBullet(bulletType);
			if (bullet == nullptr)return;
			bullet->bulletCurAtk = atk;
			bullet->BindAttacker(attacker);
			bullet->SetDir(dir);
			bullet->SetStartPos(startPoint);
			bullet->SetBulletLifeTime(GameMgr::GetInstance()->battleMgr->GetHpBulletTime());//弾の持続時間

			bullet->DoInit(true);
			bullet->SetState(true);
		}
		else if (bulletType == E_BulletType::BulletType_Cannon) {//大砲（ATK弾）
			Bullet* bullet = GetBullet(bulletType);
			if (bullet == nullptr)return;
			bullet->bulletCurAtk = atk;
			bullet->BindAttacker(attacker);
			bullet->SetDir(dir);
			bullet->SetStartPos(startPoint);
			bullet->SetBulletLifeTime(CANNON_LIFE_TIME);
			bullet->SetSpeed(speed);
			((Bullet_Cannon*)bullet)->SetExplodeRange(GameMgr::GetInstance()->battleMgr->GetAtkBulletRange());//爆発範囲

			bullet->DoInit(true);
			bullet->SetState(true);
		}
		else if (bulletType == E_BulletType::BulletType_Enemy_Normal_Reflected) {//反射された敵の弾
			Bullet* bullet = GetBullet(bulletType);
			if (bullet == nullptr) {
				return;//error
			}
			bullet->bulletCurAtk = atk;
			bullet->BindAttacker(attacker);
			bullet->SetSpeed(speed);
			bullet->SetDir(dir);
			bullet->SetStartPos(startPoint);
			bullet->SetBulletLifeTime(-1);

			bullet->DoInit(true);
			bullet->SetMaxReflectCount(0);//二度と反射できないため、反射回数を０に設定する
			bullet->SetState(true);
		}
	}
	else {//敵の弾

		Bullet* bullet = GetBullet(bulletType);
		if (bullet == nullptr) {
			return;//error
		}
		bullet->bulletCurAtk = atk;
		bullet->BindAttacker(attacker);

		if (bulletType == E_BulletType::BulletType_Normal) {
			bullet->SetSpeed(speed);
			bullet->SetDir(dir);
			bullet->SetStartPos(startPoint);
			bullet->SetBulletLifeTime(-1);
		}
		else if (bulletType == E_BulletType::BulletType_Laser) {
			bullet->SetDir(dir);
			bullet->SetStartPos(startPoint);
			bullet->SetBulletLifeTime(LASER_LIFE_TIME);
		}
		else if (bulletType == E_BulletType::BulletType_Cannon) {
			bullet->SetDir(dir);
			bullet->SetStartPos(startPoint);
			bullet->SetBulletLifeTime(CANNON_LIFE_TIME);
			bullet->SetSpeed(speed);
			((Bullet_Cannon*)bullet)->SetExplodeRange(150.0f);
		}
		else if (bulletType == E_BulletType::BulletType_Enemy_Normal) {
			if (bullet == nullptr) {
				return;//error
			}
			bullet->SetSpeed(speed);
			bullet->SetDir(dir);
			bullet->SetStartPos(startPoint);
			bullet->SetBulletLifeTime(-1);

		}
		bullet->DoInit(true);
		bullet->SetMaxReflectCount(0);
		bullet->SetState(true);
	}

}

/// <summary>
/// 弾を回収する
/// </summary>
/// <param name="bullet"></param>
void SceneObjMgr::RecycleBullet(Bullet* bullet)
{
	if (bullet == nullptr)return;

	bullet->SetState(false);
	bullet->bulletState = false;
	bullet->Reset();
}

/// <summary>
/// 弾の当たり判定
/// </summary>
/// <param name="bullet"></param>
/// <returns></returns>
bool SceneObjMgr::CheckBulletCollision(Bullet* bullet)
{
	if (bullet == nullptr || bullet->GetAttacker() == nullptr)return true;

	if (bullet->GetAttacker()->GetCellType() == E_CellType::CellType_Player) {//プレイヤーの弾

		if (cellTypeDic.find(E_CellType::CellType_Enemy) != cellTypeDic.end()) {

			std::map<int, BaseCell*>& tempDic = cellTypeDic[E_CellType::CellType_Enemy];

			std::map<int, BaseCell*>::iterator iter;
			for (iter = tempDic.begin(); iter != tempDic.end(); iter++) {
				if (iter->second == nullptr || iter->second->CheckState() == false)continue;

				if (Collider::CollisionDetect(bullet, iter->second)) {

					if (bullet->GetBulletType() == E_BulletType::BulletType_Normal ||
						bullet->GetBulletType() == E_BulletType::BulletType_Shotgun) {

						if (bullet->CheckReflectID(iter->second->GetID()) == false) {
							continue;
						}

						//反射処理
						if (bullet->ReflectCountIncrease(iter->second->GetID()) == true) {
							vector3s curDir = bullet->GetCurDir();
							vector3s tempDir = bullet->GetTransform()->GetPos() - iter->second->GetTransform()->GetPos();

							curDir.z = 0.0f;
							tempDir.z = 0.0f;
							tempDir = VecNormalize(tempDir);
							tempDir = tempDir + curDir;
							bullet->SetPos(bullet->GetTransform()->GetPos() + tempDir * 2.0f, true);
							bullet->SetDir(tempDir);
							bullet->SetSpeed(1500.0f);

							BattleMgr::BulletHit(bullet, iter->second);
							return false;
						}
						else {
							BattleMgr::BulletHit(bullet, iter->second);
							return true;
						}
					}
					else {
						BattleMgr::BulletHit(bullet, iter->second);
					}

					if (bullet->GetBulletType() != E_BulletType::BulletType_Laser) {
						return true;
					}
				}
			}
		}
		//敵の弾との当たり判定
		if (cellTypeDic.find(E_CellType::CellType_Bullet) != cellTypeDic.end() && bullet->GetBulletType() != E_BulletType::BulletType_Enemy_Normal_Reflected) {

			std::map<int, BaseCell*>& tempDic = cellTypeDic[E_CellType::CellType_Bullet];

			std::map<int, BaseCell*>::iterator iter;
			for (iter = tempDic.begin(); iter != tempDic.end(); iter++) {
				if (iter->second == nullptr || iter->second->CheckState() == false)continue;
				if (iter->second->GetID() == bullet->GetID() || ((Bullet*)iter->second)->GetAttacker()->GetCellType() == bullet->GetAttacker()->GetCellType())continue;
				if (Collider::CollisionDetect(bullet, iter->second)) {

					GameMgr::GetInstance()->effectMgr->CreateEffect(E_Effect::Effect_EnemyHit, nullptr, iter->second->GetTransform()->GetPos()/* bullet->GetTransform()->GetPos()*/, vector3s(0, 0, 0));
					Bullet* curBullet = ((Bullet*)iter->second);
					E_BulletType bulletType = curBullet->GetBulletType();

					//弾の反射
					if (GameMgr::GetInstance()->battleMgr->bulletReflectSign == true) {
						vector3s curDir = bullet->GetCurDir();
						vector3s tempDir = vector3s(0, 0, 0);
						if (bullet->GetBulletType() == E_BulletType::BulletType_Laser) {
							tempDir = iter->second->GetTransform()->GetPos() - bullet->GetTransform()->GetPos();
						}
						else {
							tempDir = bullet->GetTransform()->GetPos() - iter->second->GetTransform()->GetPos();
						}

						curDir.z = 0.0f;
						tempDir.z = 0.0f;
						tempDir = VecNormalize(tempDir);
						tempDir = tempDir + curDir;
						vector3s newBulletPos = curBullet->GetTransform()->GetPos() + tempDir * 2.0f;
						this->ShootBullet(m_player, curBullet->bulletCurAtk, E_BulletType::BulletType_Enemy_Normal_Reflected, newBulletPos, tempDir, curBullet->bulletSpeed);
					}
					curBullet->CollisionTrigger();
					if (bullet->GetBulletType() != E_BulletType::BulletType_Cannon) {
						return true;
					}
				}
			}
		}
	}
	else if (bullet->GetAttacker()->GetCellType() == E_CellType::CellType_Enemy) {
		if (m_player != nullptr) {//プレイヤーとの当たり判定
			if (Collider::CollisionDetect(bullet, m_player)) {

				Status_Player* playerStatus = (Status_Player*)m_player->GetComponent(E_Component::Component_Status);
				if (playerStatus != nullptr) {
					if (playerStatus->CheckCanBeAttack() == false)return false;
					//dmg
					int dmg = GameMgr::GetInstance()->battleMgr->GetHurtDmg(m_player, ((Enemy*)bullet->GetAttacker()));
					playerStatus->ChangeData(E_Status::Status_Hp, -dmg, false);

					vector3s effectPosOffset = vector3s(0.0f, 20.0f, -10.0f);
					effectPosOffset.x += rand() % 20 - 10;
					effectPosOffset.y += rand() % 15 - 8;
					GameMgr::GetInstance()->uiMgr->ShowDmgUI(dmg, 0.8f, m_player->GetTransform()->GetPos() + effectPosOffset);
					GameMgr::GetInstance()->effectMgr->CreateEffect(E_Effect::Effect_PlayerHit, nullptr, m_player->GetTransform()->GetPos(), vector3s(0, 0, 0));
					if (playerStatus->IsDead()) {
						//game over
						GameMgr::GetInstance()->SwitchSubGameState(E_GameState::GameState_Game_Lose, true);
					}
					else {

						//hit player 
						vector3s repulseDir = m_player->GetTransform()->GetPos() - bullet->GetTransform()->GetPos();
						repulseDir.z = 0.0f;
						m_player->SetForceMovement(repulseDir, PLAYER_REPULSE_FORCE, PLAYER_REPULSE_TIME, false);

						//hurt anim
						Animation* playerAnim = (Animation*)m_player->GetComponent(E_Component::Component_Animation);
						if (playerAnim != nullptr) {
							playerAnim->SwitchAnim(E_Anim::Anim_Damage, true, false);
						}
						playerStatus->SetInvincible(PLAYER_BE_HIT_INVINCIBLE_TIME);
					}
					GameMgr::GetInstance()->effectMgr->SetCameraShake(CAMERA_SHAKE_PLAYER_HURT_TIME, CAMERA_SHAKE_PLAYER_HURT_FORCE);

				}
				return true;

			}
		}
	}
	return false;
}
#pragma endregion SceneObjMgr_method
