#include <sstream>

#include "main.h"
#include "gameObject.h"
#include "dataManager.h"
#include "sceneObjManager.h"
#include "cellManager.h"
#include "battleManager.h"
#include "manager.h"
#include "player.h"
#include "gameLogic.h"

#pragma region other

#pragma region fsm

FSMSystem::FSMSystem() :curState(nullptr), parent(nullptr) {

}

FSMSystem::FSMSystem(BaseCell* p) : parent(p), curState(nullptr) {

}

FSMSystem::~FSMSystem() {
	if (stateDic.size() != 0) {
		std::map<E_FSMState, FSMState*>::iterator iter;
		for (iter = stateDic.begin(); iter != stateDic.end(); iter++) {
			if (iter->second != nullptr) {
				delete iter->second;
			}
		}
	}
	stateDic.clear();

	curState = nullptr;
}


void FSMSystem::DoInit() {
	//register state
	SwitchState(defaultStateType, true);
}

void FSMSystem::RegisterState(E_FSMState stateType, FSMState* state, bool setDefaultState) {
	if (state == nullptr)return;

	if (stateDic.find(stateType) != stateDic.end()) {
		//error
		return;
	}

	stateDic[stateType] = state;

	if (setDefaultState) {
		defaultStateType = stateType;
	}
}

void FSMSystem::DoUpdate(float deltatime) {

	if (curState != nullptr) {
		curState->DoUpdate(deltatime);
	}

}

void FSMSystem::SwitchState(E_FSMState targetStateType, bool resetState) {
	if (curStateType == targetStateType && resetState == false) {
		return;
	}

	if (stateDic.find(targetStateType) == stateDic.end()) {
		return;
	}

	if (curState != nullptr) {
		curState->EndState();
		curState = nullptr;
	}

	curState = stateDic[targetStateType];

	if (curState != nullptr) {
		curState->DoInit();
		curState->StartState();
	}
}

void FSMSystem::SetParent(BaseCell* p)
{
	parent = p;
}

BaseCell* FSMSystem::GetParent() {
	return parent;
}


//state
FSMState::FSMState() :parentSystem(nullptr), action(nullptr), enable(false) {

}

void FSMState::CheckStateTrigger(float deltatime) {
	if (triggerDic.size() == 0) {
		return;
	}

	std::map<E_FSMState, FSMTriggerGroup*>::iterator iter;
	for (iter = triggerDic.begin(); iter != triggerDic.end(); iter++) {
		if (iter->second == nullptr)continue;
		if (iter->first == E_FSMState::FSMState_Attack) {
			int a = 0;
		}
		if (iter->second->CheckTrigger(deltatime) == true) {
			//switch state
			if (parentSystem != nullptr) {
				this->parentSystem->SwitchState(iter->first, true);
				return;
			}
		}
	}
}

FSMState::FSMState(FSMSystem* parent) :action(nullptr), parentSystem(parent), enable(false) {

}

FSMState::~FSMState() {
	action = nullptr;
}

void FSMState::DoInit() {
	std::map<E_FSMState, FSMTriggerGroup*>::iterator iter;
	for (iter = triggerDic.begin(); iter != triggerDic.end(); iter++) {
		if (iter->second == nullptr)continue;
		iter->second->ResetTriggerState();
	}

	if (action != nullptr) {
		(*action).Reset();
	}
}

void FSMState::DoUpdate(float deltatime) {
	if (enable == true) {
		DoAction(deltatime);

		//check state trigger
		CheckStateTrigger(deltatime);
	}
}

void FSMState::SetAction(FSMStateAction* a)
{
	if (a == nullptr)return;
	action = a;
}

void FSMState::DoAction(float deltatime) {
	if (action != nullptr) {
		(*action)(parentSystem->GetParent(), deltatime);
	}
}


void FSMState::StartState() {
	//start cb

	enable = true;
}

void FSMState::EndState() {
	//end cb

	enable = false;
}

void FSMState::PauseState(bool state)
{
	enable = state;
}

BaseCell* FSMState::GetParentCell() {
	if (parentSystem == nullptr) {
		return nullptr;
	}

	return parentSystem->GetParent();
}

void FSMState::RegisterTrigger(E_FSMState targetState, FSMTriggerGroup* triggerGroup)
{
	if (triggerGroup == nullptr)return;

	if (triggerDic.find(targetState) == triggerDic.end()) {
		triggerDic[targetState] = triggerGroup;
	}
}


//trigger group
FSMTriggerGroup::FSMTriggerGroup() :checkTriggerType(E_CheckTriggerType::CheckTriggerType_AllTrue), parentSystem(nullptr) {

}

FSMTriggerGroup::FSMTriggerGroup(E_CheckTriggerType checkType, FSMSystem* parent) : checkTriggerType(checkType), parentSystem(parent) {

}

FSMTriggerGroup::~FSMTriggerGroup()
{
	if (triggerArray.size() != 0) {
		std::list<FSMStateTrigger*>::iterator iter;
		for (iter = triggerArray.begin(); iter != triggerArray.end(); iter++) {
			if ((*iter) != nullptr) {
				delete (*iter);
			}
		}
	}
	triggerArray.clear();
}

E_CheckTriggerType FSMTriggerGroup::GetCheckType() {
	return checkTriggerType;
}

std::list<FSMStateTrigger*>& FSMTriggerGroup::GetTriggerArray() {
	return triggerArray;
}

void FSMTriggerGroup::AddTrigger(FSMStateTrigger* trigger) {
	if (trigger == nullptr)return;
	triggerArray.push_back(trigger);
}

bool FSMTriggerGroup::CheckTrigger(float deltatime) {
	if (triggerArray.size() == 0) {
		return false;
	}
	int falseSign = 0;
	int trueSign = 0;
	int allFalseSign = 1;
	int allTrueSign = 1;
	bool tempSign = 0;
	int tempSignInt = 0;
	std::list<FSMStateTrigger*>::iterator iter;
	for (iter = triggerArray.begin(); iter != triggerArray.end(); iter++) {
		if ((*iter) == nullptr)continue;
		tempSign = (*iter)->Check(parentSystem, deltatime);
		tempSignInt = tempSign == true ? 1 : 0;

		trueSign |= tempSignInt;
		allTrueSign &= tempSignInt;

		falseSign |= (~tempSignInt) & 1;
		allFalseSign &= (~tempSignInt) & 1;
	}

	switch (checkTriggerType) {
	case E_CheckTriggerType::CheckTriggerType_OneTrue:
		return trueSign ? true : false;
	case E_CheckTriggerType::CheckTriggerType_OneFalse:
		return falseSign ? true : false;
	case E_CheckTriggerType::CheckTriggerType_AllTrue:
		return allTrueSign ? true : false;
	case E_CheckTriggerType::CheckTriggerType_AllFalse:
		return allFalseSign ? true : false;
	case E_CheckTriggerType::CheckTriggerType_AlwaysTrue:
		return true;
	case E_CheckTriggerType::CheckTriggerType_AlwaysFalse:
		return false;
	default:
		return false;
	}
	return true;
}

void FSMTriggerGroup::ResetTriggerState()
{
	std::list<FSMStateTrigger*>::iterator iter;
	for (iter = triggerArray.begin(); iter != triggerArray.end(); iter++) {
		if ((*iter) == nullptr)continue;
		(*iter)->ResetTriggerState();
	}
}


//trigger

FSMStateTrigger::FSMStateTrigger() :parentState(nullptr), triggerAction(nullptr)
{

}

FSMStateTrigger::~FSMStateTrigger()
{
	parentState = nullptr;
	if (triggerAction != nullptr) {
		delete triggerAction;
	}
	triggerAction = nullptr;
}

void FSMStateTrigger::SetTriggerParam(FSMState* parent, FSMStateTriggerAction* action) {
	if (parent == nullptr || action == nullptr)return;

	parentState = parent;
	triggerAction = action;
}

bool FSMStateTrigger::Check(FSMSystem* parentSystem, float deltatime) {
	if (parentSystem == nullptr)return false;

	//judge
	if (triggerAction == nullptr || parentState == nullptr)return false;

	return (*triggerAction)(parentState->GetParentCell(), deltatime);
}

void FSMStateTrigger::ResetTriggerState()
{
	if (triggerAction != nullptr) {
		triggerAction->Reset();
	}
}

#pragma endregion fsm

#pragma region status_logic

//effect data
CardEffect_StatusData::CardEffect_StatusData() :statusType_1(E_Status::Status_None), increaseValue_1(0), statusType_2(E_Status::Status_None), increaseValue_2(0) {

}

void CardEffect_StatusData::ApplyEffect(Status_Player* playerStatus) {
	CardEffectData::ApplyEffect(playerStatus);

	if (playerStatus == nullptr)return;

	if (((int)statusType_1) > 200) {//base dmg
		GameMgr::GetInstance()->battleMgr->ChangeBaseDmg(statusType_1, increaseValue_1);
	}
	else {
		playerStatus->ChangeDefaultData(statusType_1, increaseValue_1, true, true);
	}

	if (((int)statusType_2) > 200) {//base dmg
		GameMgr::GetInstance()->battleMgr->ChangeBaseDmg(statusType_2, increaseValue_2);
	}
	else {
		playerStatus->ChangeDefaultData(statusType_2, increaseValue_2, true, true);
	}

}

void CardEffect_StatusData::RecoveryEffect(Status_Player* playerStatus) {
	CardEffectData::RecoveryEffect(playerStatus);

	if (playerStatus == nullptr)return;
	//TODO
}

void CardEffect_Movement::ApplyEffect(Status_Player* playerStatus) {
	CardEffectData::ApplyEffect(playerStatus);

	if (playerStatus == nullptr)return;
}

void CardEffect_Movement::RecoveryEffect(Status_Player* playerStatus) {
	CardEffectData::RecoveryEffect(playerStatus);

	if (playerStatus == nullptr)return;
}

void CardEffect_Extra::ApplyEffect(Status_Player* playerStatus) {
	CardEffectData::ApplyEffect(playerStatus);

	if (playerStatus == nullptr)return;

}

void CardEffect_Extra::RecoveryEffect(Status_Player* playerStatus) {
	CardEffectData::RecoveryEffect(playerStatus);
}


//rule
CardRuleData::CardRuleData() :m_levelRange(0, 0)
{
	cardTypeArray.clear();
	cardValueRangeDic.clear();

	m_cardDataVec.clear();
}

CardRuleData::~CardRuleData()
{
	cardTypeArray.clear();
	cardValueRangeDic.clear();

	m_cardDataVec.clear();
}

bool CardRuleData::CheckLv(int lv)
{
	if (lv< m_levelRange.x || lv> m_levelRange.y) {
		return false;
	}
	return true;
}

void CardRuleData::AddCardData(CardRuleRawData* _data)
{
	if (_data == nullptr)return;
	m_cardDataVec.push_back(_data);
}

void CardRuleData::SetLevelRange(vector2s lvRange)
{
	m_levelRange = lvRange;
}

void CardRuleData::SetLevelRange(int minLv, int maxLv)
{
	m_levelRange.x = minLv;
	m_levelRange.y = maxLv;
}

void CardRuleData::AddCardType(E_CardType cardType, bool useValue, vector2s valueRange)
{
	if (cardType == E_CardType::CardType_None)return;
	cardTypeArray.push_back(cardType);
	if (useValue == true) {
		cardValueRangeDic[cardType] = valueRange;
	}
}

CardEffectData* CardRuleData::GetCard(std::list<int>& _maskVec)
{
	if (m_cardDataVec.size() == 0)return nullptr;
	int checkMaxTimes = (int)m_cardDataVec.size() * 3;
	int checkCount = 0;

	int dataIndex = rand() % m_cardDataVec.size();
	if (_maskVec.size() != 0) {
		do {
			
			if (m_cardDataVec[dataIndex] == nullptr) {
				checkCount++;
				continue;
			}
			bool checkFaild = false;
			for (int temp : _maskVec) {
				if (temp == m_cardDataVec[dataIndex]->card_id) {
					checkFaild = true;
				}
			}
			if (checkFaild == true) {
				checkCount++;
				dataIndex = rand() % m_cardDataVec.size();
			}
			else {
				break;
			}

		} while (checkCount < checkMaxTimes);
	}

	CardRuleRawData* curRawData = m_cardDataVec[dataIndex];

	if (checkCount < checkMaxTimes) {
		_maskVec.push_back(curRawData->card_id);
	}

	CardEffectData* resultData = new CardEffectData();
	E_CardType conditionCardType = (E_CardType)curRawData->condition_type;
	if (conditionCardType != E_CardType::CardType_None) {
		CardEffectData* conditionData = new CardEffectData();
		conditionData->cardType = conditionCardType;
		conditionData->param = curRawData->condition_param_range.x;
		resultData->conditionData = conditionData;
	}
	E_CardType effectCardType = (E_CardType)curRawData->effect_type;
	if (effectCardType != E_CardType::CardType_None) {
		CardEffectData* effectData = new CardEffectData();
		effectData->cardType = effectCardType;
		effectData->param = curRawData->effect_param_range.x;
		resultData->effectData = effectData;
	}

	return resultData;
}

CardEffectData* CardRuleData::GetExtraCard(CardRuleRawData* _data)
{
	if (_data == nullptr)return new CardEffectData();

	CardEffectData* resultData = new CardEffectData();
	E_CardType conditionCardType = (E_CardType)_data->condition_type;
	if (conditionCardType != E_CardType::CardType_None) {
		CardEffectData* conditionData = new CardEffectData();
		conditionData->cardType = conditionCardType;
		conditionData->param = _data->condition_param_range.x;
		resultData->conditionData = conditionData;
	}
	E_CardType effectCardType = (E_CardType)_data->effect_type;
	if (effectCardType != E_CardType::CardType_None) {
		CardEffectData* effectData = new CardEffectData();
		effectData->cardType = effectCardType;
		effectData->param = _data->effect_param_range.x;
		resultData->effectData = effectData;
	}

	return resultData;
}


#pragma endregion status_logic

#pragma region spawn_data
#pragma region spawn_data_cell

SpawnDataCell::SpawnDataCell() :spawnCellType(E_CellType::CellType_None), dataCellID(-1),
spawnInterval(0.0f), spawnTimePass(0.0f), maxSpawnCount(0), curSpawnCount(0),
isInfinite(false), cellState(false), spawnDone(false), canSpawnSign(true)
{

}

SpawnDataCell::~SpawnDataCell()
{

}

void SpawnDataCell::SetSpawnCellID(int id)
{
	dataCellID = id;
}

int SpawnDataCell::GetSpawnCellID()
{
	return dataCellID;
}

void SpawnDataCell::SetSpawnParam(E_CellType cellType, float spawnTime, int maxCount, bool infinite)
{
	spawnCellType = cellType;
	spawnInterval = spawnTime;
	maxSpawnCount = maxCount;
	isInfinite = infinite;
}

void SpawnDataCell::StartSpawn()
{
	spawnTimePass = 0.0f;
	curSpawnCount = 0;
	cellState = true;
	spawnDone = false;
	canSpawnSign = false;
}

void SpawnDataCell::StopSpawn()
{
	cellState = false;
	spawnDone = true;
	canSpawnSign = false;
}

void SpawnDataCell::DoUpdate(float deltatime)
{
	if (cellState == true && spawnDone == false) {

		if (curSpawnCount >= maxSpawnCount) {
			if (isInfinite == true) {
				curSpawnCount = 0;
			}
			else {
				StopSpawn();
				return;
			}
		}

		if (canSpawnSign == false) {
			spawnTimePass += deltatime;
			if (spawnTimePass >= spawnInterval) {
				//spawn
				canSpawnSign = true;
			}
		}
	}
}

E_CellType SpawnDataCell::GetSpawnType()
{
	return spawnCellType;
}

void SpawnDataCell::SpawnActionInvoke()
{
	curSpawnCount++;
	canSpawnSign = false;
	spawnTimePass = 0.0f;
}

void SpawnDataCell::Reset()
{
	spawnTimePass = 0.0f;
	curSpawnCount = 0;
	spawnDone = false;
}



#pragma endregion spawn_data_cell

#pragma region spawn_data

SpawnData::SpawnData() :spawnType(E_SpawnType::SpawnType_Sequence), dataID(-1), maxSpawnCount(0), curSpawnCount(0), useLvCondition(false), useTimeCondition(false),
lvCondition(vector2s()), timeCondition(vector2s()), spawnOnceSign(false)
{
	spawnDataCellArray.clear();
}

SpawnData::~SpawnData()
{
	if (spawnDataCellArray.size() != 0) {
		std::vector<SpawnDataCell*>::iterator iter;
		for (iter = spawnDataCellArray.begin(); iter != spawnDataCellArray.end(); iter++) {
			if ((*iter) == nullptr)continue;
			delete (*iter);
		}
	}
	spawnDataCellArray.clear();
}

void SpawnData::DoInit()
{
	spawnOnceSign = false;
	curSpawnCount = 0;
	curSequencySpawnIndex = 0;

	spawnDone = false;
	//cellInit
	std::vector<SpawnDataCell*>::iterator iter;
	for (iter = spawnDataCellArray.begin(); iter != spawnDataCellArray.end(); iter++) {
		if ((*iter) == nullptr)continue;
		(*iter)->Reset();
		(*iter)->StopSpawn();
	}
}

int SpawnData::GetID()
{
	return dataID;
}

E_SpawnType SpawnData::GetSpawnType()
{
	return spawnType;
}

E_SpawnData SpawnData::GetSpawnDataType()
{
	return spawnDataType;
}

void SpawnData::SetSpawnDataParam(E_SpawnData spawnDataT, E_SpawnType spawnT, int maxCount, bool infinite)
{
	dataID = (int)spawnDataT;
	spawnDataType = spawnDataT;
	spawnType = spawnT;
	maxSpawnCount = maxCount;
	isInfinite = infinite;
}

void SpawnData::RegisterSpawnDataCell(SpawnDataCell* cell)
{
	if (cell == nullptr)return;
	spawnDataCellArray.push_back(cell);
}

void SpawnData::SetSpawnCountIncrease(int increaseValue)
{
	curSpawnCount += increaseValue;
	if (curSpawnCount < 0) {
		//error
	}
}

void SpawnData::DoUpdate(float deltatime)
{
	if (spawnDone == true) return;

	if (CheckConditionState() == false) {
		spawnDone = true;
		return;
	}

	if (spawnType == E_SpawnType::SpawnType_Parallel) {
		bool allSpawnDone = true;
		std::vector<SpawnDataCell*>::iterator iter;
		for (iter = spawnDataCellArray.begin(); iter != spawnDataCellArray.end(); iter++) {
			if ((*iter) == nullptr)continue;
			if ((*iter)->spawnDone == true)continue;

			(*iter)->DoUpdate(deltatime);
			if ((*iter)->canSpawnSign == true) {
				if (curSpawnCount < maxSpawnCount) {
					//spawn
					this->Spawn((*iter)->GetSpawnType());
					(*iter)->SpawnActionInvoke();
				}
			}

			allSpawnDone = false;
		}

		spawnDone = allSpawnDone;
	}
	else if (spawnType == E_SpawnType::SpawnType_Sequence) {
		int arraySize = (int)spawnDataCellArray.size();
		if (arraySize != 0) {
			if (curSequencySpawnIndex >= arraySize) {
				if (isInfinite == true) {
					curSequencySpawnIndex %= arraySize;
				}
				else {
					// spawn done
					spawnDone = true;
					return;
				}
			}

			SpawnDataCell* curCell = spawnDataCellArray[curSequencySpawnIndex];
			if (curCell == nullptr || curCell->spawnDone == true) {
				curSequencySpawnIndex++;
				if (isInfinite == true) {
					curSequencySpawnIndex %= arraySize;
				}
				else {
					if (curSequencySpawnIndex >= arraySize) {
						spawnDone = true;
						return;
					}
				}
				SpawnDataCell* curCell = spawnDataCellArray[curSequencySpawnIndex];
				if (curCell != nullptr) {
					curCell->Reset();
					curCell->StartSpawn();
				}
			}
			else {
				curCell->DoUpdate(deltatime);

				if (curCell->canSpawnSign == true) {
					if (curSpawnCount < maxSpawnCount) {
						//spawn
						this->Spawn(curCell->GetSpawnType());
						curCell->SpawnActionInvoke();
					}
				}
			}
		}
		else {
			spawnDone = true;
		}
	}

}

void SpawnData::Spawn(E_CellType cellType)
{
	if (curSpawnCount >= maxSpawnCount)return;
	//transform data,dataId
	vector3s dir = vector3s();
	dir.x = (float)((rand() / double(RAND_MAX)) * 2.0f - 1.0f);
	dir.y = (float)((rand() / double(RAND_MAX)) * 2.0f - 1.0f);
	dir.z = 0.0f;
	dir = VecNormalize(dir);
	float dist = rand() % 300 + SCREEN_WIDTH * 0.5f + 1;

	vector3s spawnPos = dir * dist;
	spawnPos.z = -2.0f;

	//CellMgr->Spawn()
	BaseCell* cell = GameMgr::GetInstance()->cellMgr->GetCell(cellType);
	if (cell != nullptr) {

		Animation* anim = (Animation*)cell->GetComponent(E_Component::Component_Animation);
		if (anim != nullptr) {
			anim->SwitchAnim(E_Anim::Anim_Idle, true, false);
		}
		curSpawnCount++;
		cell->SetSpawnCounterSign(true);

		cell->SetPos(spawnPos, true);
		cell->SetState(true);
	}
}

void SpawnData::SetSpawnState(bool state)
{
	spawnState = state;
}

void SpawnData::StartSpawn()
{
	spawnState = true;
	DoInit();
	spawnOnceSign = true;
	if (spawnType == E_SpawnType::SpawnType_Parallel) {
		std::vector<SpawnDataCell*>::iterator iter;
		for (iter = spawnDataCellArray.begin(); iter != spawnDataCellArray.end(); iter++) {
			if ((*iter) == nullptr)continue;
			(*iter)->StartSpawn();
		}
	}
	else if (spawnType == E_SpawnType::SpawnType_Sequence) {
		if (spawnDataCellArray.size() != 0) {
			int indexCount = 0;
			bool initIndex = false;
			std::vector<SpawnDataCell*>::iterator iter;
			for (iter = spawnDataCellArray.begin(); iter != spawnDataCellArray.end(); iter++, indexCount++) {
				if ((*iter) == nullptr)continue;
				(*iter)->Reset();
				(*iter)->StopSpawn();
				if (initIndex == false) {
					curSequencySpawnIndex = indexCount;
					initIndex = true;
					(*iter)->StartSpawn();
				}
			}
		}
	}
}

void SpawnData::StopSpawn()
{
	//DoInit();
	spawnDone = true;
}

void SpawnData::ChangeSpawnCount(int increasement)
{
	curSpawnCount += increasement;
	if (curSpawnCount < 0) {
		curSpawnCount = 0;//error
	}
}

void SpawnData::SetCurSpawnCount(int c)
{
	curSpawnCount = c;
	if (curSpawnCount < 0) {
		curSpawnCount = 0;//error
	}
	else if (curSpawnCount > maxSpawnCount) {
		curSpawnCount = maxSpawnCount;
	}
}

int SpawnData::GetCurSpawnCount()
{
	return curSpawnCount;
}

bool SpawnData::CheckConditionState()
{
	bool result = true;
	//codition judge
	if (useLvCondition == true) {
		Player* player = GameMgr::GetInstance()->sceneObjMgr->GetPlayer();
		if (player != nullptr) {
			int lv = player->GetStatusValue(E_Status::Status_Level);
			if (lv >= (int)(lvCondition.x + 0.5f) && lv <= (int)(lvCondition.y + 0.5f)) {
				result = true;
			}
			else {
				return false;
			}
		}
	}
	if (useTimeCondition == true) {
		float remainTime = GameMgr::GetInstance()->timer->GetRemainTime();
		if (remainTime > timeCondition.x && remainTime < timeCondition.y) {
			result = true;
		}
		else {
			return false;
		}
	}
	return result;
}

#pragma endregion spawn_data

#pragma endregion spawn_data

#pragma endregion other

#pragma region ai

#pragma region state_action

FSMStateAction::FSMStateAction() :init(false), setAnim(E_Anim::Anim_None)
{

}

void FSMStateAction::operator()(BaseCell* parentCell, float deltatime) {
	if (parentCell == nullptr) {
		return;
	}

	//do action
}

void FSMStateAction::Reset()
{
	init = false;
}
#pragma region enemy_base_action

void FSMState_Base_Action_Idle::operator()(BaseCell* parentCell, float deltatime)
{
	if (parentCell == nullptr)return;
	if (init == false) {
		if (setAnim != E_Anim::Anim_None) {
			Animation* cellAnimator = (Animation*)parentCell->GetComponent(E_Component::Component_Animation);
			if (cellAnimator != nullptr) {
				cellAnimator->SwitchAnim(setAnim, true, true);
			}
		}
		init = true;
	}
}

FSMState_Base_Action_Attack::FSMState_Base_Action_Attack() : invokeOnce(true), invokeSign(false),
attackDelayTime(0.0f), attackTimePass(0.0f),
attackType(E_ActionType::ActionType_Enemy_Attack) {

}

void FSMState_Base_Action_Attack::operator()(BaseCell* _parentCell, float _deltatime)
{
	if (_parentCell == nullptr)return;
	if (init == false) {
		if (setAnim != E_Anim::Anim_None) {
			Animation* cellAnimator = (Animation*)_parentCell->GetComponent(E_Component::Component_Animation);
			if (cellAnimator != nullptr) {
				cellAnimator->SwitchAnim(setAnim, true, true);
			}
		}
		invokeSign = false;
		attackTimePass = 0.0f;
		init = true;
	}

	if (attackTimePass <= attackDelayTime) {
		attackTimePass += _deltatime;
	}
	else {
		if (invokeOnce == true && invokeSign == false) {
			_parentCell->TakeAction(attackType,_deltatime);
			invokeSign = true;
		}
		else if(invokeOnce== false) {
			_parentCell->TakeAction(attackType,_deltatime);
		}
	}

}

void FSMState_Base_Action_Attack::Reset()
{
	init = false;
	invokeSign = false;
	attackTimePass = 0.0f;
}

void FSMState_Base_Action_Move2Player::operator()(BaseCell* parentCell, float deltatime) {
	if (parentCell == nullptr)return;
	if (init == false) {
		if (setAnim != E_Anim::Anim_None) {
			Animation* cellAnimator = (Animation*)parentCell->GetComponent(E_Component::Component_Animation);
			if (cellAnimator != nullptr) {
				cellAnimator->SwitchAnim(setAnim, true, true);
			}
		}
		init = true;
	}

	Player* player = GameMgr::GetInstance()->sceneObjMgr->GetPlayer();
	if (player == nullptr)return;

	vector3s moveDir = player->GetTransform()->GetPos() - parentCell->GetTransform()->GetPos();
	moveDir.z = 0.0f;

	moveDir = VecNormalize(moveDir);
	moveDir = moveDir * (moveSpeed * deltatime);
	vector3s newPos = parentCell->GetTransform()->GetPos() + moveDir;
	parentCell->SetPos(newPos, true);

}

void FSMState_Base_Action_Dmg::operator()(BaseCell* parentCell, float delattime) {
	if (parentCell == nullptr)return;
	if (init == false) {
		if (setAnim != E_Anim::Anim_None) {
			Animation* cellAnimator = (Animation*)parentCell->GetComponent(E_Component::Component_Animation);
			if (cellAnimator != nullptr) {
				cellAnimator->SwitchAnim(setAnim, true, true);
			}
		}
		init = true;
	}
}

#pragma endregion enemy_base_action

#pragma endregion state_action

#pragma region trigger_action

bool FSMStateTriggerAction::operator()(BaseCell* parentCell, float deltatime) {
	if (parentCell == nullptr)return false;

	return false;
}

void FSMStateTriggerAction::Reset()
{
	init = false;
}

//idle time
bool FSMStateTrigger_Base_Trigger_IdleTime::operator()(BaseCell* parentCell, float deltatime)
{
	if (init == false) {
		timePass = 0.0f;
		init = true;
	}

	timePass += deltatime;
	if (timePass > idleTime) {
		return  !inverseResult ? true : false;
	}

	return !inverseResult ? false : true;
}

//dist 2 player
bool FSMStateTrigger_Base_Trigger_Dist2Player::operator()(BaseCell* parentCell, float deltatime)
{
	if (parentCell == nullptr)return false;
	if (init == false) {
		init = true;
	}

	Player* player = GameMgr::GetInstance()->sceneObjMgr->GetPlayer();
	if (player == nullptr)return false;

	vector3s cell2Player = player->GetTransform()->GetPos() - parentCell->GetTransform()->GetPos();
	cell2Player.z = 0.0f;

	float curDist = VecLengthS(cell2Player);
	if (curDist < dist * dist) {
		return !inverseResult ? true : false;
	}

	return !inverseResult ? false : true;
}

//dmg time
bool FSMStateTrigger_Base_Trigger_TakeDmg::operator()(BaseCell* parentCell, float deltatime)
{
	if (init == false) {
		init = true;
	}

	if (parentCell == nullptr) {
		return true;
	}

	if (parentCell->CheckCanMove()) {
		return !inverseResult ? false : true;
	}
	else {
		return !inverseResult ? true : false;
	}

}


#pragma endregion trigger_action


#pragma endregion ai

#pragma region tool_method

#pragma region timer_method

Timer::Timer() :gameTimer_1(1.0f), gameTimer_2(1.0f), uiTimer(1.0f) {
	defaultGameCountDownTime = DEFAULT_GAME_TIME;

	gameCountDownState = false;
	gameCountDownTime = defaultGameCountDownTime;
	gameCountDownTimePass = gameCountDownTime;
}

Timer::~Timer() {

}

void Timer::SetTimer(E_Timer timerType, bool limitForward) {
	switch (timerType) {
	case E_Timer::Timer_Game_1:
		gameTimer_1 = limitForward ? 1.0f : 0.0f;
		break;
	case E_Timer::Timer_Game_2:
		gameTimer_2 = limitForward ? 1.0f : 0.0f;
		break;
	case E_Timer::Timer_UI:
		uiTimer = limitForward ? 1.0f : 0.0f;
		break;
	default:
		break;
	}
}

void Timer::SetTimer(E_Timer timerType, float value)
{
	switch (timerType) {
	case E_Timer::Timer_Game_1:
		gameTimer_1 = value;
		break;
	case E_Timer::Timer_Game_2:
		gameTimer_2 = value;
		break;
	case E_Timer::Timer_UI:
		uiTimer = value;
		break;
	default:
		break;
	}
}

void Timer::Reset()
{
	gameTimer_1 = 1.0f;
	gameTimer_2 = 1.0f;
	uiTimer = 1.0f;
}

void Timer::DoUpdate(float deltatime)
{
	if (gameCountDownState == true) {
		gameCountDownTimePass = F_Clamp(gameCountDownTimePass - deltatime * gameTimer_1, 0.0f, gameCountDownTime);
	}
}

void Timer::SetGameCountDownState(bool state)
{
	gameCountDownState = state;
}

bool Timer::CheckCountDownDone()
{
	return ((int)gameCountDownTimePass) <= 0;
}

int Timer::GetCountDownMinute()
{
	return (int)((gameCountDownTimePass + 0.5f) / 60.0f);
}

int Timer::GetCountDownSecond()
{
	return (int)(gameCountDownTimePass + 0.5f) % 60;
}

void Timer::ResetCountDownParam()
{
	gameCountDownState = false;
	gameCountDownTime = defaultGameCountDownTime;
	gameCountDownTimePass = gameCountDownTime;
}

float Timer::GetRemainTime()
{
	return gameCountDownTimePass;
}

#pragma endregion timer_method

#pragma endregion tool_method


