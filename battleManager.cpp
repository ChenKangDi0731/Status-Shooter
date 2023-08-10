#pragma once
#include "main.h"
#include "dataManager.h"
#include "gameLogic.h"
#include "battleManager.h"
#include "sceneObjManager.h"
#include "effectManager.h"
#include "uiManager.h"
#include "player.h"
#include "enemy.h"

#pragma region battle_manager

BattleMgr::BattleMgr() :curRuleData(nullptr), card_1_data(nullptr), card_2_data(nullptr), card_3_data(nullptr), card_4_data(nullptr), m_initCardDataOnce(false)
{

	//======================================================デフォルトパラメータを設定
	hpBaseDmg = BULLET_LASER_BASE_DMG;
	hpLifeTime = LASER_LIFE_TIME;

	atkBaseDmg = BULLET_CANNON_BASE_DMG;
	atkRange = BULLET_CANNON_DEFAULT_EXPLODE_RANGE;
	atkExplodeSize = BULLET_CANNON_DEFAULT_EXPLODE_EFFECT_SIZE;

	defBaseDmg = BULLET_SHOTGUN_BASE_DMG;
	defBulletCount = BULLET_SHOTGUN_DEFAULT_BULLET_COUNT;
	defRecoveryCap = DEF_BULLET_RECOVERY_CAP_DEFAULT;
	defBulletReflectCount = BULLET_SHOTGUN_DEFAULT_REFLECT_COUNT;

	expBaseDmg = BULLET_NORMAL_BASE_DMG;
	expBulletCount = BULLET_NORMAL_DEFAULT_COUNT;
	expGain = EXP_BULLET_GAIN_DEFAULT;
	expBulletReflectCount = BULLET_NORMAL_DEFAULT_REFLECT_COUNT;

	hpRecoveryCounter = 0.0f;
	atkRecoveryCounter = 0.0f;
	defRecoveryCounter = 0.0f;
	expRecoveryCounter = 0.0f;

	cardRuleDataDic.clear();

	bulletReflectSign = false;

	//エキストラカードの種類のリストを初期化
	m_extraCardTypeList.push_back(E_CardGenreType::CardGenreType_Hp_Life_Time);
	m_extraCardTypeList.push_back(E_CardGenreType::CardGenreType_Hp_Bullet_Reflect);
	m_extraCardTypeList.push_back(E_CardGenreType::CardGenreType_Hp_Dmg);
	m_extraCardTypeList.push_back(E_CardGenreType::CardGenreType_Hp_Consume);

	m_extraCardTypeList.push_back(E_CardGenreType::CardGenreType_Atk_Bullet_Count);
	m_extraCardTypeList.push_back(E_CardGenreType::CardGenreType_Atk_Bullet_Range);
	m_extraCardTypeList.push_back(E_CardGenreType::CardGenreType_Atk_Dmg);
	m_extraCardTypeList.push_back(E_CardGenreType::CardGenreType_Atk_Consume);

	m_extraCardTypeList.push_back(E_CardGenreType::CardGenreType_Def_Bullet_Count);
	m_extraCardTypeList.push_back(E_CardGenreType::CardGenreType_Def_Recovery_Cap);
	m_extraCardTypeList.push_back(E_CardGenreType::CardGenreType_Def_Bullet_Reflect_Count);
	m_extraCardTypeList.push_back(E_CardGenreType::CardGenreType_Def_Dmg);
	m_extraCardTypeList.push_back(E_CardGenreType::CardGenreType_Def_Consume);

	m_extraCardTypeList.push_back(E_CardGenreType::CardGenreType_Exp_Gain);
	m_extraCardTypeList.push_back(E_CardGenreType::CardGenreType_Exp_Bullet_Count);
	m_extraCardTypeList.push_back(E_CardGenreType::CardGenreType_Exp_Bullet_Reflect_Count);
	m_extraCardTypeList.push_back(E_CardGenreType::CardGenreType_Exp_Dmg);
	m_extraCardTypeList.push_back(E_CardGenreType::CardGenreType_Exp_Consume);

	m_extraCardTypeList.push_back(E_CardGenreType::CardGenreType_Attack_Speed);
	m_extraCardTypeList.push_back(E_CardGenreType::CardGenreType_Move_Speed);
	m_extraCardTypeList.push_back(E_CardGenreType::CardGenreType_Reflect_Enemy_Bullet);

}

BattleMgr::~BattleMgr()
{
	std::list<CardRuleData*>::iterator iter;
	for (iter = cardRuleDataDic.begin(); iter != cardRuleDataDic.end(); iter++) {
		if ((*iter) != nullptr) {
			delete (*iter);
		}
	}

	for (auto& temp : m_cardRuleDataDic) {
		if (temp.second != nullptr) {
			delete temp.second;
		}
	}
	m_cardRuleDataDic.clear();

	cardRuleDataDic.clear();

	if (card_1_data != nullptr) {
		delete card_1_data;
		card_1_data = nullptr;
	}

	if (card_2_data != nullptr) {
		delete card_2_data;
		card_2_data = nullptr;
	}

	if (card_3_data != nullptr) {
		delete card_3_data;
		card_3_data = nullptr;
	}

	if (card_4_data != nullptr) {
		delete card_4_data;
		card_4_data = nullptr;
	}
}

void BattleMgr::DoInit()
{
	if (m_initCardDataOnce == false) {
		//init card rule data
		m_cardLvDic = GameMgr::GetInstance()->dataMgr->GetLvDataDic();

		std::map<int, std::list<CardRuleRawData*>>& tempDic = GameMgr::GetInstance()->dataMgr->GetNormalCardDataDic();
		std::map<int, std::list<CardRuleRawData*>>::iterator temp_iter;
		for (temp_iter = tempDic.begin(); temp_iter != tempDic.end(); temp_iter++) {
			if (m_cardRuleDataDic.find(temp_iter->first) == m_cardRuleDataDic.end()) {
				//新しいCardRuleDataを作る
				CardRuleData* newRuleData = new CardRuleData();

				//レベルの範囲を獲得し、Dataの中に保存する
				if (m_cardLvDic.find(temp_iter->first) != m_cardLvDic.end() && m_cardLvDic[temp_iter->first] != nullptr) {
					newRuleData->SetLevelRange(m_cardLvDic[temp_iter->first]->lv_range);
				}

				m_cardRuleDataDic[temp_iter->first] = newRuleData;
			}
			for (auto& temp_2 : temp_iter->second) {
				m_cardRuleDataDic[temp_iter->first]->AddCardData(temp_2);
			}
		}

		m_extraCardRuleDataDic = GameMgr::GetInstance()->dataMgr->GetExtraCardDic();

		m_playerLvUpParamDataDic = GameMgr::GetInstance()->dataMgr->GetPlayerLvUpDataDic();

		m_initCardDataOnce = true;

	}


	hpBaseDmg = BULLET_LASER_BASE_DMG;
	hpLifeTime = LASER_LIFE_TIME;

	atkBaseDmg = BULLET_CANNON_BASE_DMG;
	atkRange = BULLET_CANNON_DEFAULT_EXPLODE_RANGE;
	atkExplodeSize = BULLET_CANNON_DEFAULT_EXPLODE_EFFECT_SIZE;

	defBaseDmg = BULLET_SHOTGUN_BASE_DMG;
	defBulletCount = BULLET_SHOTGUN_DEFAULT_BULLET_COUNT;
	defRecoveryCap = DEF_BULLET_RECOVERY_CAP_DEFAULT;
	defBulletReflectCount = BULLET_SHOTGUN_DEFAULT_REFLECT_COUNT;

	expBaseDmg = BULLET_NORMAL_BASE_DMG;
	expBulletCount = BULLET_NORMAL_DEFAULT_COUNT;
	expGain = EXP_BULLET_GAIN_DEFAULT;
	expBulletReflectCount = BULLET_NORMAL_DEFAULT_REFLECT_COUNT;

	hpRecoveryCounter = 0.0f;
	atkRecoveryCounter = 0.0f;
	defRecoveryCounter = 0.0f;
	expRecoveryCounter = 0.0f;

	bulletReflectSign = false;

	SwitchCardRuleData(1);
	ResetCardData();
}

void BattleMgr::DoUpdate(float deltatime)
{

}

void BattleMgr::SwitchCardRuleData(int lv)
{
	std::list<CardRuleData*>::iterator iter;
	for (iter = cardRuleDataDic.begin(); iter != cardRuleDataDic.end(); iter++) {
		if ((*iter) == nullptr)continue;
		if ((*iter)->CheckLv(lv) == true) {
			curRuleData = (*iter);
			return;
		}
	}
}

CardEffectData* BattleMgr::GetCardData(int cardIndex, int playerCurLv, bool& isExtraCard)
{

	E_CardType mask = E_CardType::CardType_None;
	CardEffectData* curCardData = nullptr;

	int judgeValue = playerCurLv % EXTRA_CARD_MULTIPLE_FACTOR;
	if (judgeValue == 0 && playerCurLv != 0) {//extra card
		if (m_extraCardRuleDataDic.size() != 0) {

			E_CardGenreType mostType = E_CardGenreType::CardGenreType_None;
			if (m_gotExtraCardDic.size() != 0) {//数が最も多いエキストラカードのタイプはどれかを見つけ出す
				std::map<E_CardGenreType, std::list<CardRuleRawData*>>::iterator iter = m_gotExtraCardDic.begin();
				mostType = iter->first;
				int mostCount = (int)iter->second.size();
				iter++;//次のデータから比較する
				for (; iter != m_gotExtraCardDic.end(); iter++) {
					if (mostCount <= (int)iter->second.size()) {
						mostCount = (int)iter->second.size();
						mostType = iter->first;
					}
				}
			}

			std::map<E_CardGenreType, std::vector<CardRuleRawData*>>::iterator iter = m_extraCardRuleDataDic.begin();
			int checkMaxTime = EXTRA_CARD_GENRE_COUNT * 3;
			int checkCount = 0;
			E_CardGenreType cardGenreType = E_CardGenreType::CardGenreType_None;
			int genreTypeIndex = 0;
			do {
				genreTypeIndex = rand() % EXTRA_CARD_GENRE_COUNT;
				if (m_extraCardTypeList.size() <= genreTypeIndex) {
					checkCount++;
					continue;
				}
				cardGenreType = m_extraCardTypeList[genreTypeIndex];
				if (m_extraCardRuleDataDic.find(cardGenreType) != m_extraCardRuleDataDic.end()) {
					bool checkSameSign = false;
					if (m_tempCardDataDic.size() != 0) {
						for (auto cur : m_tempCardDataDic) {
							if (cur.second == nullptr)continue;
							if (cur.second->card_genre_type == (int)cardGenreType) {
								checkSameSign = true;
								break;
							}
						}
						if (checkSameSign == true) {
							checkCount++;
							continue;
						}
					}

					//このタイプのエキストラカードはもう全部手に入ったかどうかをチェックする
					if (m_gotExtraCardDic.find(cardGenreType) == m_gotExtraCardDic.end() || m_extraCardRuleDataDic[cardGenreType].size() > m_gotExtraCardDic[cardGenreType].size()) {
						CardRuleRawData* tempRawData = m_extraCardRuleDataDic[cardGenreType][m_gotExtraCardDic[cardGenreType].size()];
						m_tempCardDataDic[cardIndex] = tempRawData;
						curCardData = CardRuleData::GetExtraCard(tempRawData);
						isExtraCard = true;
						break;
					}
				}

				checkCount++;
			} while (checkCount <= checkMaxTime);

			if (curCardData == nullptr) {
				if (m_cardRuleDataDic.size() == 0)return nullptr;
				for (auto& temp : m_cardRuleDataDic) {
					if (temp.second == nullptr)continue;
					if (temp.second->CheckLv(playerCurLv) == true) {
						curCardData = temp.second->GetCard(m_tempNormalCardVec);
						break;
					}
				}
			}

		}
		else {
			if (m_cardRuleDataDic.size() == 0)return nullptr;
			for (auto& temp : m_cardRuleDataDic) {
				if (temp.second == nullptr)continue;
				if (temp.second->CheckLv(playerCurLv) == true) {
					curCardData = temp.second->GetCard(m_tempNormalCardVec);
					break;
				}
			}
		}
	}
	else {//normal card
		if (m_cardRuleDataDic.size() == 0)return nullptr;
		for (auto& temp : m_cardRuleDataDic) {
			if (temp.second == nullptr)continue;
			if (temp.second->CheckLv(playerCurLv) == true) {
				curCardData = temp.second->GetCard(m_tempNormalCardVec);
				break;
			}
		}
	}

	if (cardIndex == 1) {

		if (card_1_data != nullptr) {
			delete card_1_data;
			card_1_data = nullptr;
		}
		card_1_data = curCardData;
	}
	else if (cardIndex == 2) {
		if (card_1_data != nullptr) {
			mask = card_1_data->cardType;
		}

		if (card_2_data != nullptr) {
			delete card_2_data;
			card_2_data = nullptr;
		}
		card_2_data = curCardData;
	}
	else if (cardIndex == 3) {
		if (card_2_data != nullptr) {
			mask = card_2_data->cardType;
		}

		if (card_3_data != nullptr) {
			delete card_3_data;
			card_3_data = nullptr;
		}
		card_3_data = curCardData;
	}
	else if (cardIndex == 4) {
		if (card_3_data != nullptr) {
			mask = card_3_data->cardType;
		}

		if (card_4_data != nullptr) {
			delete card_4_data;
			card_4_data = nullptr;
		}
		card_4_data = curCardData;
	}

	return curCardData;
}

/// <summary>
/// カードを選ぶ
/// </summary>
/// <param name="cardIndex"></param>
void BattleMgr::SelectCard(int cardIndex)
{
	Player* player = GameMgr::GetInstance()->sceneObjMgr->GetPlayer();
	if (player == nullptr)return;
	Status_Player* playerStatus = (Status_Player*)player->GetComponent(E_Component::Component_Status);
	if (playerStatus == nullptr)return;

	if (cardIndex == 1) {
		if (card_1_data != nullptr) {
			BattleMgr::ApplyCard(playerStatus, card_1_data->conditionData);
			BattleMgr::ApplyCard(playerStatus, card_1_data->effectData);
		}
	}
	else if (cardIndex == 2) {
		if (card_2_data != nullptr) {
			BattleMgr::ApplyCard(playerStatus, card_2_data->conditionData);
			BattleMgr::ApplyCard(playerStatus, card_2_data->effectData);
		}
	}
	else if (cardIndex == 3) {
		if (card_3_data != nullptr) {
			BattleMgr::ApplyCard(playerStatus, card_3_data->conditionData);
			BattleMgr::ApplyCard(playerStatus, card_3_data->effectData);
		}
	}
	else if (cardIndex == 4) {
		if (card_4_data != nullptr) {
			BattleMgr::ApplyCard(playerStatus, card_4_data->conditionData);
			BattleMgr::ApplyCard(playerStatus, card_4_data->effectData);
		}
	}

	if (m_tempCardDataDic.find(cardIndex) != m_tempCardDataDic.end() && m_tempCardDataDic[cardIndex] != nullptr) {
		CardRuleRawData* tempData = m_tempCardDataDic[cardIndex];
		if (tempData->card_type == 1) {
			if (m_gotExtraCardDic.find((E_CardGenreType)tempData->card_genre_type) == m_gotExtraCardDic.end()) {
				m_gotExtraCardDic[(E_CardGenreType)tempData->card_genre_type].clear();
			}

			m_gotExtraCardDic[(E_CardGenreType)tempData->card_genre_type].push_back(tempData);
		}
	}

	m_tempCardDataDic.clear();
}

void BattleMgr::ResetCardData()
{
	if (card_1_data != nullptr) {
		delete card_1_data;
		card_1_data = nullptr;
	}
	if (card_2_data != nullptr) {
		delete card_2_data;
		card_2_data = nullptr;
	}
	if (card_3_data != nullptr) {
		delete card_3_data;
		card_3_data = nullptr;
	}
	if (card_4_data != nullptr) {
		delete card_4_data;
		card_4_data = nullptr;
	}

	m_tempCardDataDic.clear();
	m_tempNormalCardVec.clear();
}

void BattleMgr::ResetGotData()
{
	m_gotExtraCardDic.clear();
}

/// <summary>
/// 弾ダメージ処理
/// </summary>
/// <param name="bullet"></param>
/// <param name="target"></param>
void BattleMgr::BulletHit(Bullet* bullet, BaseCell* target)
{
	if (bullet == nullptr || target == nullptr)return;

	//check target state
	Status* targetStatus = (Status*)target->GetComponent(E_Component::Component_Status);
	if (targetStatus == nullptr || targetStatus->CheckCanBeAttack() == false)return;

	int dmg = 0;

	bool isPlayer = false;
	BaseCell* attacker = bullet->GetAttacker();
	Status_Player* playerStatus = nullptr;
	if (attacker != nullptr && attacker->GetCellType() == E_CellType::CellType_Player) {
		isPlayer = true;
		playerStatus = (Status_Player*)attacker->GetComponent(E_Component::Component_Status);

	}

	E_BulletType bulletType = bullet->GetBulletType();
	vector3s repulseDir = vector3s();

	bool recoverySign = false;

	if (bulletType == E_BulletType::BulletType_Normal) {
		repulseDir = target->GetTransform()->GetPos() - bullet->GetTransform()->GetPos();

		dmg = isPlayer ? GameMgr::GetInstance()->battleMgr->GetDmg((Player*)attacker, playerStatus, bullet)
			: GameMgr::GetInstance()->battleMgr->GetBaseDmg(E_Status::Status_Hp);

		targetStatus->ChangeData(E_Status::Status_Hp, -dmg, false);
	}
	else if (bulletType == E_BulletType::BulletType_Shotgun) {
		repulseDir = target->GetTransform()->GetPos() - bullet->GetTransform()->GetPos();
		dmg = isPlayer ? GameMgr::GetInstance()->battleMgr->GetDmg((Player*)attacker, playerStatus, bullet)
			: GameMgr::GetInstance()->battleMgr->GetBaseDmg(E_Status::Status_Hp);

		targetStatus->ChangeData(E_Status::Status_Hp, -dmg, false);
		recoverySign = true;
	}
	else if (bulletType == E_BulletType::BulletType_Laser) {
		repulseDir = target->GetTransform()->GetPos() - bullet->shootPos;
		targetStatus->SetInvincible(0.1f);

		dmg = isPlayer ? GameMgr::GetInstance()->battleMgr->GetDmg((Player*)attacker, playerStatus, bullet)
			: GameMgr::GetInstance()->battleMgr->GetBaseDmg(E_Status::Status_Hp);
		targetStatus->ChangeData(E_Status::Status_Hp, -dmg, false);

		GameMgr::GetInstance()->battleMgr->ShootRecovery(playerStatus, E_Status::Status_Hp, E_Status::Status_Exp);
	}
	else if (bulletType == E_BulletType::BulletType_Cannon) {
		BattleMgr::Explosion(bullet);
		return;
	}

	if (targetStatus->IsDead() == true) {
		if (isPlayer == true) {
			if (playerStatus != nullptr) {
				int baseEXP = ((Enemy*)target)->GetEnemyType() == E_CellType::CellType_Enemy_Skirmisher ? DEFAULT_EXP_VALUE : DEFAULT_EXP_2_VALUE;
				int exp = baseEXP * GameMgr::GetInstance()->battleMgr->GetExpGain();
				playerStatus->ChangeData(E_Status::Status_Exp, exp, false);
			}
		}
		if (recoverySign == true) {
			GameMgr::GetInstance()->battleMgr->Recovery(playerStatus, E_AttackType::AttackType_Defeat);
		}

	}
	else {

		repulseDir.z = 0.0f;
		target->SetForceMovement(repulseDir, REPULSE_FORCE, REPULSE_TIME, false);//repulse

		vector3s effectPosOffset = vector3s(0.0f, 20.0f, -10.0f);
		effectPosOffset.x += rand() % 20 - 10;
		effectPosOffset.y += rand() % 15 - 8;
		if (target->GetFollowUI() == nullptr) {
			//ダメージUIを表示
			UI* tempUI = GameMgr::GetInstance()->uiMgr->ShowDmgUI(dmg, 0.8f, target->GetTransform()->GetPos() + effectPosOffset);//dmg ui
			target->SetFollowUI(tempUI, true);
		}
		else {
			GameMgr::GetInstance()->uiMgr->ShowDmgUI(target->GetFollowUI(), dmg, 0.8f, target->GetTransform()->GetPos() + effectPosOffset);
		}

		if (recoverySign == true) {
			GameMgr::GetInstance()->battleMgr->Recovery(playerStatus, E_AttackType::AttackType_Hit);
		}
	}
	GameMgr::GetInstance()->effectMgr->CreateEffect(E_Effect::Effect_EnemyHit, nullptr, target->GetTransform()->GetPos(), vector3s(0, 0, 0));//effect
}

/// <summary>
/// 大砲ダメージ処理
/// </summary>
/// <param name="bullet"></param>
void BattleMgr::Explosion(Bullet* bullet)
{
	if (bullet == nullptr || bullet->GetBulletType() != E_BulletType::BulletType_Cannon)return;

	GameMgr::GetInstance()->effectMgr->SetCameraShake(CAMERA_SHAKE_EXPLOSION_TIME, CAMERA_SHAKE_EXPLOSION_FORCE);

	Bullet_Cannon* cannon = ((Bullet_Cannon*)bullet);
	if (cannon == nullptr)return;

	//effect
	Effect* explodeEffect = GameMgr::GetInstance()->effectMgr->CreateEffect(E_Effect::Effect_Explosion, nullptr, bullet->GetTransform()->GetPos(), vector3s(0, 0, 0));
	if (explodeEffect != nullptr) {
		float effectSize = GameMgr::GetInstance()->battleMgr->GetAtkBulletExplodeEffectSize();
		explodeEffect->SetScale(vector3s(effectSize, effectSize, 1.0f));
	}
	float range = cannon->GetExplodeRange();

	//get target by range;
	//========================敵との当たり判定
	std::list<BaseCell*> targets = GameMgr::GetInstance()->sceneObjMgr->GetCellsByRange(E_CellType::CellType_Enemy, cannon->GetTransform()->GetPos(),
		range, false);
	if (targets.size() != 0) {
		bool isPlayer = false;
		BaseCell* attacker = bullet->GetAttacker();
		Status_Player* playerStatus = nullptr;
		if (attacker != nullptr && attacker->GetCellType() == E_CellType::CellType_Player) {
			isPlayer = true;
			playerStatus = (Status_Player*)attacker->GetComponent(E_Component::Component_Status);

		}

		int dmg = isPlayer ? GameMgr::GetInstance()->battleMgr->GetDmg((Player*)attacker, playerStatus, bullet)
			: GameMgr::GetInstance()->battleMgr->GetBaseDmg(E_Status::Status_Hp);
		std::list<BaseCell*>::iterator iter;
		for (iter = targets.begin(); iter != targets.end(); iter++) {
			if ((*iter) == nullptr)continue;

			Status* targetStatus = (Status*)(*iter)->GetComponent(E_Component::Component_Status);
			if (targetStatus == nullptr || targetStatus->CheckCanBeAttack() == false)continue;

			targetStatus->ChangeData(E_Status::Status_Hp, -dmg, false);

			GameMgr::GetInstance()->battleMgr->ShootRecovery(playerStatus, E_Status::Status_Atk, E_Status::Status_Exp);

			if (targetStatus->IsDead() == true) {
				//dead effect
				//calc exp
				if (playerStatus != nullptr) {
					int exp = DEFAULT_EXP_VALUE * GameMgr::GetInstance()->battleMgr->GetExpGain();
					playerStatus->ChangeData(E_Status::Status_Exp, exp, true);
				}
			}
			else {
				vector3s repulseDir = (*iter)->GetTransform()->GetPos() - cannon->GetTransform()->GetPos();
				repulseDir.z = 0.0f;
				(*iter)->SetForceMovement(repulseDir, REPULSE_FORCE, REPULSE_TIME, false);
				//effect

				vector3s effectPosOffset = vector3s(0.0f, 20.0f, -10.0f);
				effectPosOffset.x += rand() % 20 - 10;
				effectPosOffset.y += rand() % 15 - 8;
				if ((*iter)->GetFollowUI() == nullptr) {
					//ダメージUIを表示
					UI* tempUI = GameMgr::GetInstance()->uiMgr->ShowDmgUI(dmg, 0.8f, (*iter)->GetTransform()->GetPos() + effectPosOffset);//dmg ui
					(*iter)->SetFollowUI(tempUI, true);
				}
				else {
					GameMgr::GetInstance()->uiMgr->ShowDmgUI((*iter)->GetFollowUI(), dmg, 0.8f, (*iter)->GetTransform()->GetPos() + effectPosOffset);
				}
			}
			GameMgr::GetInstance()->effectMgr->CreateEffect(E_Effect::Effect_EnemyHit, nullptr, (*iter)->GetTransform()->GetPos(), vector3s(0, 0, 0));
		}
	}
	targets.clear();
	//敵の弾との当たり判定
	targets = GameMgr::GetInstance()->sceneObjMgr->GetCellsByRange(E_CellType::CellType_Bullet, cannon->GetTransform()->GetPos(),
		range, false);
	if (targets.size() != 0) {
		std::list<BaseCell*>::iterator iter;
		for (iter = targets.begin(); iter != targets.end(); iter++) {
			if ((*iter) == nullptr || (*iter)->CheckState() == false)continue;
			Bullet* tempBullet = ((Bullet*)*iter);
			if (tempBullet->GetAttacker() == nullptr || tempBullet->GetAttacker()->GetCellType() == E_CellType::CellType_Player)continue;
			//弾の反射　テスト
			if (GameMgr::GetInstance()->battleMgr->bulletReflectSign == true) {
				//new dir
				vector3s curDir = bullet->GetCurDir();
				vector3s tempDir = bullet->GetTransform()->GetPos() - tempBullet->GetTransform()->GetPos();

				curDir.z = 0.0f;
				tempDir.z = 0.0f;
				tempDir = VecNormalize(tempDir);
				tempDir = tempDir + curDir;
				vector3s newBulletPos = tempBullet->GetTransform()->GetPos() + tempDir * 2.0f;
				GameMgr::GetInstance()->sceneObjMgr->ShootBullet(bullet->GetAttacker(), tempBullet->bulletCurAtk, E_BulletType::BulletType_Enemy_Normal_Reflected, newBulletPos, tempDir, tempBullet->bulletSpeed);

			}
			tempBullet->CollisionTrigger();
		}
	}
}

/// <summary>
/// 弾のパラメータをリセット
/// </summary>
void BattleMgr::ResetBulletParam()
{
	hpBaseDmg = BULLET_LASER_BASE_DMG;
	hpLifeTime = LASER_LIFE_TIME;

	atkBaseDmg = BULLET_CANNON_BASE_DMG;
	atkRange = BULLET_CANNON_DEFAULT_EXPLODE_RANGE;
	atkExplodeSize = BULLET_CANNON_DEFAULT_EXPLODE_EFFECT_SIZE;

	defBaseDmg = BULLET_SHOTGUN_BASE_DMG;
	defBulletCount = BULLET_SHOTGUN_DEFAULT_BULLET_COUNT;
	defRecoveryCap = DEF_BULLET_RECOVERY_CAP_DEFAULT;
	defBulletReflectCount = BULLET_SHOTGUN_DEFAULT_REFLECT_COUNT;

	expBaseDmg = BULLET_NORMAL_BASE_DMG;
	expBulletCount = BULLET_NORMAL_DEFAULT_COUNT;
	expGain = EXP_BULLET_GAIN_DEFAULT;
	expBulletReflectCount = BULLET_NORMAL_DEFAULT_REFLECT_COUNT;

	hpRecoveryCounter = 0.0f;
	atkRecoveryCounter = 0.0f;
	defRecoveryCap = 0.0f;
	expRecoveryCounter = 0.0f;
}

void BattleMgr::SetBaseDmg(E_Status status, int value)
{

	switch (status) {
	case E_Status::Status_Hp:
	case E_Status::Status_Hp_Base_Dmg:
		hpBaseDmg = F_Clamp(value + 0.5f, BULLET_LASER_BASE_DMG, BULLET_LASER_BASE_DMG_MAX);
		break;
	case E_Status::Status_Atk:
	case E_Status::Status_Atk_Base_Dmg:
		atkBaseDmg = F_Clamp(value + 0.5f, BULLET_CANNON_BASE_DMG, BULLET_CANNON_BASE_DMG_MAX);
		break;
	case E_Status::Status_Def:
	case E_Status::Status_Def_Base_Dmg:
		defBaseDmg = F_Clamp(value + 0.5f, BULLET_SHOTGUN_BASE_DMG, BULLET_SHOTGUN_BASE_DMG_MAX);
		break;
	case E_Status::Status_Exp:
	case E_Status::Status_Exp_Base_Dmg:
		expBaseDmg = F_Clamp(value + 0.5f, BULLET_NORMAL_BASE_DMG, BULLET_NORMAL_BASE_DMG_MAX);
		break;
	default:
		break;
	}
}

void BattleMgr::ChangeBaseDmg(E_Status status, int increasement)
{
	switch (status) {
	case E_Status::Status_Hp:
	case E_Status::Status_Hp_Base_Dmg:
		hpBaseDmg = F_Clamp(hpBaseDmg + increasement + 0.5f, BULLET_LASER_BASE_DMG, BULLET_LASER_BASE_DMG_MAX);
		break;
	case E_Status::Status_Atk:
	case E_Status::Status_Atk_Base_Dmg:
		atkBaseDmg = F_Clamp(atkBaseDmg + increasement + 0.5f, BULLET_CANNON_BASE_DMG, BULLET_CANNON_BASE_DMG_MAX);
		break;
	case E_Status::Status_Def:
	case E_Status::Status_Def_Base_Dmg:
		defBaseDmg = F_Clamp(defBaseDmg + increasement + 0.5f, BULLET_SHOTGUN_BASE_DMG, BULLET_SHOTGUN_BASE_DMG_MAX);
		break;
	case E_Status::Status_Exp:
	case E_Status::Status_Exp_Base_Dmg:
		expBaseDmg = F_Clamp(expBaseDmg + increasement + 0.5f, BULLET_NORMAL_BASE_DMG, BULLET_NORMAL_BASE_DMG_MAX);
		break;
	default:
		break;
	}
}

int BattleMgr::GetBaseDmg(E_Status status)
{
	switch (status) {
	case E_Status::Status_Hp:
	case E_Status::Status_Hp_Base_Dmg:
		return hpBaseDmg;
	case E_Status::Status_Atk:
	case E_Status::Status_Atk_Base_Dmg:
		return atkBaseDmg;
	case E_Status::Status_Def:
	case E_Status::Status_Def_Base_Dmg:
		return defBaseDmg;
	case E_Status::Status_Exp:
	case E_Status::Status_Exp_Base_Dmg:
		if (GameMgr::GetInstance()->sceneObjMgr->GetPlayer() != nullptr && GameMgr::GetInstance()->sceneObjMgr->GetPlayer()->GetStatusValue(E_Status::Status_Exp) == 0) {
			return BULLET_NORMAL_MIN_DMG;
		}
		return expBaseDmg;
	default:
		break;
	}

	return 1.0f;
}

void BattleMgr::SetBulletCount(E_Status status, int _value)
{
	if (status == E_Status::Status_Def) {
		defBulletCount = F_Clamp(_value, BULLET_SHOTGUN_DEFAULT_BULLET_COUNT, BULLET_SHOTGUN_MAX_BULLET_COUNT);
	}
	else if (status == E_Status::Status_Exp) {
		expBulletCount = F_Clamp(_value, BULLET_NORMAL_DEFAULT_COUNT, BULLET_NORMAL_MAX_COUNT);
	}
}

int BattleMgr::GetBulletCount(E_Status status)
{
	if (status == E_Status::Status_Def) {
		return defBulletCount;
	}
	else if (status == E_Status::Status_Exp) {
		return expBulletCount;
	}

	return 1;
}

void BattleMgr::ChangeBulletCount(E_Status status, int increasement)
{
	if (status == E_Status::Status_Def) {
		defBulletCount = F_Clamp(defBulletCount + increasement, BULLET_SHOTGUN_DEFAULT_BULLET_COUNT, BULLET_SHOTGUN_MAX_BULLET_COUNT);
	}
	else if (status == E_Status::Status_Exp) {
		expBulletCount = F_Clamp(expBulletCount + increasement, BULLET_NORMAL_DEFAULT_COUNT, BULLET_NORMAL_MAX_COUNT);
	}
}

void BattleMgr::SetDefRecoveryCap(float capValue)
{
}

void BattleMgr::ChangeDefRecoveryCap(float increasement)
{
}

float BattleMgr::GetDefRecoveryCap()
{
	return 0.0f;
}

void BattleMgr::ShootRecovery(Status_Player* playerStatus, E_Status bulletTypeStatus, E_Status recoveryStatus)
{
	if (playerStatus == nullptr)return;

	if (bulletTypeStatus == E_Status::Status_Hp) {
		playerStatus->ChangeData(recoveryStatus, 1.0f, true);
	}
	else if (bulletTypeStatus == E_Status::Status_Atk) {
		playerStatus->ChangeData(recoveryStatus, 2.0f, true);
	}
}

void BattleMgr::SetExpGain(float gain)
{
	expGain = F_Clamp(gain, EXP_GAIN_MIN, EXP_GAIN_MAX);
}

void BattleMgr::ChangeExpGain(float increasement)
{
	expGain = F_Clamp(expGain + increasement, EXP_GAIN_MIN, EXP_GAIN_MAX);
}

float BattleMgr::GetExpGain()
{
	return expGain;
}

void BattleMgr::SetHpBulletTime(float _value)
{
	hpLifeTime = F_Clamp(_value + 0.5f, LASER_LIFE_TIME, LASER_MAX_LIFE_TIME);
}

void BattleMgr::ChangeHpBulletTime(float _increasementFactor)
{
	hpLifeTime = F_Clamp(hpLifeTime + (_increasementFactor * LASER_LIFE_TIME), LASER_LIFE_TIME, LASER_MAX_LIFE_TIME);
}

float BattleMgr::GetHpBulletTime()
{
	return hpLifeTime;
}

void BattleMgr::SetAtkBulletRange(float _value)
{
	atkRange = F_Clamp(_value, BULLET_CANNON_DEFAULT_EXPLODE_RANGE, BULLET_CANNON_MAX_EXPLODE_RANGE);
	if (fabs(BULLET_CANNON_DEFAULT_EXPLODE_RANGE) > 0.00001f) {
		float factor = _value / BULLET_CANNON_DEFAULT_EXPLODE_RANGE;
		atkExplodeSize = F_Clamp(BULLET_CANNON_DEFAULT_EXPLODE_EFFECT_SIZE * factor, BULLET_CANNON_DEFAULT_EXPLODE_EFFECT_SIZE, BULLET_CANNON_MAX_EXPLODE_EFFECT_SIZE);
	}
}

void BattleMgr::ChangeAtkBulletRange(float _increasementFactor)
{
	atkRange = F_Clamp(atkRange + _increasementFactor * BULLET_CANNON_DEFAULT_EXPLODE_RANGE, BULLET_CANNON_DEFAULT_EXPLODE_RANGE, BULLET_CANNON_MAX_EXPLODE_RANGE);
	atkExplodeSize = F_Clamp(atkExplodeSize + _increasementFactor * BULLET_CANNON_DEFAULT_EXPLODE_EFFECT_SIZE, BULLET_CANNON_DEFAULT_EXPLODE_EFFECT_SIZE, BULLET_CANNON_MAX_EXPLODE_EFFECT_SIZE);
}

float BattleMgr::GetAtkBulletRange()
{
	return atkRange;
}

float BattleMgr::GetAtkBulletExplodeEffectSize()
{
	return atkExplodeSize;
}

void BattleMgr::SetReflectCount(E_Status _status, int _value)
{
	if (_status == E_Status::Status_Def) {
		defBulletReflectCount = F_Clamp(_value, BULLET_SHOTGUN_DEFAULT_REFLECT_COUNT, BULLET_SHOTGUN_MAX_REFLECT_COUNT);
	}
	else if (_status == E_Status::Status_Exp) {
		expBulletReflectCount = F_Clamp(_value, BULLET_NORMAL_DEFAULT_REFLECT_COUNT, BULLET_NORMAL_MAX_REFLECT_COUNT);
	}
}

void BattleMgr::ChangeReflectCount(E_Status _status, int _increasement)
{
	if (_status == E_Status::Status_Def) {
		defBulletReflectCount = F_Clamp(defBulletReflectCount + _increasement, BULLET_SHOTGUN_DEFAULT_REFLECT_COUNT, BULLET_SHOTGUN_MAX_REFLECT_COUNT);
	}
	else if (_status == E_Status::Status_Exp) {
		expBulletReflectCount = F_Clamp(expBulletReflectCount + _increasement, BULLET_NORMAL_DEFAULT_REFLECT_COUNT, BULLET_NORMAL_MAX_REFLECT_COUNT);
	}
}

int BattleMgr::GetReflectCount(E_Status _status)
{
	if (_status == E_Status::Status_Def) {
		return defBulletReflectCount;
	}
	else if (_status == E_Status::Status_Exp) {
		return expBulletReflectCount;
	}

	return 0;
}

int BattleMgr::GetLvUpStatusParam(E_Status _status, int _curPlayerLv)
{
	if (m_playerLvUpParamDataDic.size() == 0)return 0;

	for (auto& temp : m_playerLvUpParamDataDic) {
		if (temp.second == nullptr)continue;
		if (temp.second->lv_range.x <= _curPlayerLv && temp.second->lv_range.y > _curPlayerLv) {
			switch (_status) {
			case E_Status::Status_Hp:
			case E_Status::Status_Max_Hp:
				return temp.second->hp_increasement;
			case E_Status::Status_Atk:
			case E_Status::Status_Max_Atk:
				return temp.second->atk_increasement;
			case E_Status::Status_Def:
			case E_Status::Status_Max_Def:
				return temp.second->def_increasement;
			case E_Status::Status_Exp:
			case E_Status::Status_Max_Exp:
				return temp.second->exp_increasement;
			default:
				break;
			}
		}
	}
	return 0;
}

void BattleMgr::SetReflectEnemyBullet(bool _state)
{
	bulletReflectSign = _state;
}

int BattleMgr::GetDmg(Player* player, Status_Player* playerStatus, Bullet* bullet)
{
	if (player == nullptr || playerStatus == nullptr) {
		return BULLET_NORMAL_MIN_DMG;
	}
	if (bullet == nullptr) {
		return 0.0f;
	}

	E_BulletType bulletType = bullet->GetBulletType();

	//dmg = baseDmg *( 1 + (atk - 10)*0.1f)
	E_Status bulletStatusType = E_Status::Status_Exp;
	if (bulletType == E_BulletType::BulletType_Cannon) {
		bulletStatusType = E_Status::Status_Atk;
	}
	else if (bulletType == E_BulletType::BulletType_Shotgun) {
		bulletStatusType = E_Status::Status_Def;
	}
	else if (bulletType == E_BulletType::BulletType_Laser) {
		bulletStatusType = E_Status::Status_Hp;
	}
	int bulletBaseDmg = GetBaseDmg(bulletStatusType);
	float atk = bullet->bulletCurAtk;
	atk = F_Clamp((float)(atk - 10), 0.0f, STATUS_LIMIT_ATK);
	float atk_factor = atk / 30.0f;
	int dmg = (int)(bulletBaseDmg * (1 + atk_factor) + 0.5f);

	return dmg;
}

int BattleMgr::GetHurtDmg(Player* player, Enemy* hurter)
{
	//dmg = max(minDmg,   BaseDmg*(1+((atk - playerDef)/atk)))

	if (player == nullptr ||
		hurter == nullptr) {
		return 0;
	}
	Status_Player* playerStatus = (Status_Player*)player->GetComponent(E_Component::Component_Status);
	if (playerStatus == nullptr)return 0;
	Status_Enemy* enemyStatus = (Status_Enemy*)hurter->GetComponent(E_Component::Component_Status);
	if (enemyStatus == nullptr)return 0;

	int playerDef = playerStatus->GetValue(E_Status::Status_Def);
	int enemyAtk = enemyStatus->GetValue(E_Status::Status_Atk);
	if (enemyAtk <= 0) {
		enemyAtk = 1;
	}

	int dmg = (int)(fmax(ENEMY_STATUS_MIN_DMG, ENEMY_STATUS_BASE_DMG * (1 + (float)(enemyAtk - playerDef) / enemyAtk)) + 0.5f);
	return dmg;
}

int BattleMgr::GetExp(Status_Player* playerStatus, Enemy* target)
{
	return 0;
}

void BattleMgr::Recovery(Status_Player* playerStatus, E_AttackType attackType)
{
	if (playerStatus == nullptr)return;

	if (attackType == E_AttackType::AttackType_Hit) {
		//return;
		int recoveryHp = defRecoveryCap * BULLET_LASER_BASE_RECOVERY_CAP;
		int recoveryAtk = defRecoveryCap * BULLET_CANNON_BASE_RECOVERY_CAP;
		int recoveryExp = defRecoveryCap * BULLET_NORMAL_BASE_RECOVERY_CAP;

		playerStatus->ChangeData(E_Status::Status_Hp, recoveryHp, true);
		playerStatus->ChangeData(E_Status::Status_Atk, recoveryAtk, true);
		playerStatus->ChangeData(E_Status::Status_Exp, recoveryExp, true);
	}
	else if (attackType == E_AttackType::AttackType_Defeat) {
		int recoveryHp = defRecoveryCap * BULLET_LASER_DEFEAT_RECOVERY_CAP;
		int recoveryAtk = defRecoveryCap * BULLET_CANNON_DEFEAT_RECOVERY_CAP;
		int recoveryExp = defRecoveryCap * BULLET_NORMAL_DEFEAT_RECOVERY_CAP;
		int recoveryDef = defRecoveryCap * BULLET_SHOTGUN_BASE_RECOVERY_CAP;

		playerStatus->ChangeData(E_Status::Status_Hp, recoveryHp, true);
		playerStatus->ChangeData(E_Status::Status_Atk, recoveryAtk, true);
		playerStatus->ChangeData(E_Status::Status_Exp, recoveryExp, true);
		playerStatus->ChangeData(E_Status::Status_Def, recoveryDef, true);

	}
}

void BattleMgr::ApplyCard(Status_Player* _playerStatus, CardEffectData* _cardData)
{
	if (_playerStatus == nullptr || _cardData == nullptr)return;
	switch (_cardData->cardType) {
		//====================================ステータスの最大値
	case E_CardType::CardType_Max_Hp:
		_playerStatus->ChangeDefaultData(E_Status::Status_Hp, _cardData->param, true, true);
		break;
	case E_CardType::CardType_Max_Atk:
		_playerStatus->ChangeDefaultData(E_Status::Status_Atk, _cardData->param, true, true);
		break;
	case E_CardType::CardType_Max_Def:
		_playerStatus->ChangeDefaultData(E_Status::Status_Def, _cardData->param, true, true);
		break;
		//====================================ステータスの消耗量
	case E_CardType::CardType_Hp_Consume:
		_playerStatus->ChangeDefaultData(E_Status::Status_Consume_Hp, _cardData->param, true, true);
		break;
	case E_CardType::CardType_Atk_Consume:
		_playerStatus->ChangeDefaultData(E_Status::Status_Consume_Atk, _cardData->param, true, true);
		break;
	case E_CardType::CardType_Def_Consume:
		_playerStatus->ChangeDefaultData(E_Status::Status_Consume_Def, _cardData->param, true, true);
		break;
	case E_CardType::CardType_Exp_Consume:
		_playerStatus->ChangeDefaultData(E_Status::Status_Consume_Exp, _cardData->param, true, true);
		break;
		//=====================================ベースダメージ
	case E_CardType::CardType_Hp_Dmg:
		GameMgr::GetInstance()->battleMgr->ChangeBaseDmg(E_Status::Status_Hp_Base_Dmg, _cardData->param);
		break;
	case E_CardType::CardType_Atk_Dmg:
		GameMgr::GetInstance()->battleMgr->ChangeBaseDmg(E_Status::Status_Atk_Base_Dmg, _cardData->param);
		break;
	case E_CardType::CardType_Def_Dmg:
		GameMgr::GetInstance()->battleMgr->ChangeBaseDmg(E_Status::Status_Def_Base_Dmg, _cardData->param);
		break;
	case E_CardType::CardType_Exp_Dmg:
		GameMgr::GetInstance()->battleMgr->ChangeBaseDmg(E_Status::Status_Exp_Base_Dmg, _cardData->param);
		break;
		//=================================================================エキストラカードスキル
		//=====================================HP
	case E_CardType::CardType_Hp_Life_Time:
		GameMgr::GetInstance()->battleMgr->ChangeHpBulletTime(_cardData->param);
		break;
	case E_CardType::CardType_Hp_Bullet_Reflect:
		//TODO
		break;
		//=====================================Atk
	case E_CardType::CardType_Atk_Bullet_Count:
		//TODO
		break;
	case E_CardType::CardType_Atk_Bullet_Range:
		GameMgr::GetInstance()->battleMgr->ChangeAtkBulletRange(_cardData->param);
		break;
		//=====================================Def
	case E_CardType::CardType_Def_Bullet_Count:
		GameMgr::GetInstance()->battleMgr->ChangeBulletCount(E_Status::Status_Def, _cardData->param + 0.5f);
		break;
	case E_CardType::CardType_Def_Recovery_Cap:
		break;
	case E_CardType::CardType_Def_Bullet_Reflect_Count:
		GameMgr::GetInstance()->battleMgr->ChangeReflectCount(E_Status::Status_Def, _cardData->param + 0.5f);
		break;
		//=====================================Exp
	case E_CardType::CardType_Exp_Gain:
		break;
	case E_CardType::CardType_Exp_Bullet_Count:
		GameMgr::GetInstance()->battleMgr->ChangeBulletCount(E_Status::Status_Exp, _cardData->param + 0.5f);
		break;
	case E_CardType::CardType_Exp_Bullet_Reflect_Count:
		GameMgr::GetInstance()->battleMgr->ChangeReflectCount(E_Status::Status_Exp, _cardData->param + 0.5f);
		break;
		//=====================================Special
	case E_CardType::CardType_Attack_Speed_Up:
		break;
	case E_CardType::CardType_Move_Speed_Up:
		break;
	case E_CardType::CardType_Reflect_Enemy_Bullet:
		GameMgr::GetInstance()->battleMgr->SetReflectEnemyBullet(true);
		break;
	default:
		break;
	}
}

void BattleMgr::RecoveryCard(Status_Player* _playerStatus, CardEffectData* _cardData)
{
	if (_playerStatus == nullptr || _cardData == nullptr)return;

}

#pragma endregion battle_manager