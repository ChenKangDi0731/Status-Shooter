#include <sstream>
#include "gameLogic.h"
#include "sceneObjManager.h"
#include "battleManager.h"
#include "player.h"
#include "ui.h"
#include "renderer.h"
#include "uiManager.h"

#pragma region ui_manager

void UIMgr::ComposeCard(UIGroup* card, CardEffectData* data, bool isExtraCard)
{
	if (card == nullptr || data == nullptr)return;
	card->ClearChild();
	E_CardType cardType = data->cardType;

	//bg
	UISprite* bg = nullptr;
	if (isExtraCard == true) {
		bg = (UISprite*)GetUI(E_UIContent::UIContent_Card_Extra_Bg);
	}
	else {
		bg = (UISprite*)GetUI(E_UIContent::UIContent_Card_Bg);
	}
	card->RegisterChild(UI_BG_ID, bg, vector3s(0, 0, 0));

	int cardTypeIndex = (int)cardType;

	ComposeCardPart(card, data->conditionData, true);
	ComposeCardPart(card, data->effectData, false);
}

void UIMgr::ComposeCardPart(UIGroup* _card, CardEffectData* _data, bool _isCondition)
{
	if (_card == nullptr || _data == nullptr)return;

	int word_1_id = _isCondition ? UI_CARD_CONDITION_WORD_1_ID : UI_CARD_EFFECT_WORD_1_ID;
	int word_2_id = _isCondition ? UI_CARD_CONDITION_WORD_2_ID : UI_CARD_EFFECT_WORD_2_ID;
	int word_3_id = _isCondition ? UI_CARD_CONDITION_WORD_3_ID : UI_CARD_EFFECT_WORD_3_ID;
	vector3s word_1_offset = _isCondition ? word_condition_1_offset : word_effect_1_offset;
	vector3s word_2_offset = _isCondition ? word_condition_2_offset : word_effect_2_offset;
	vector3s word_3_offset = _isCondition ? word_condition_3_offset : word_effect_3_offset;

	int cardTypeIndex = (int)_data->cardType;
	if (cardTypeIndex > 0 && cardTypeIndex <= 10) {//ステータスの最大値
		//condition

		UISprite* word_max = (UISprite*)GetUI(E_UIContent::UIContent_Word_Max);
		word_max->GetTransform()->SetScale(word_scale);
		word_max->ShowUI(true, -1, true);
		_card->RegisterChild(word_1_id, word_max, word_1_offset);

		if (_data->cardType == E_CardType::CardType_Max_Hp) {
			UISprite* word_hp = (UISprite*)GetUI(E_UIContent::UIContent_HP);
			word_hp->GetTransform()->SetScale(status_img_word_scale);
			word_hp->ShowUI(true, -1, true);
			_card->RegisterChild(word_2_id, word_hp, word_2_offset);
		}
		else if (_data->cardType == E_CardType::CardType_Max_Atk) {
			UISprite* word_atk = (UISprite*)GetUI(E_UIContent::UIContent_Atk);
			word_atk->GetTransform()->SetScale(status_img_word_scale);
			word_atk->ShowUI(true, -1, true);
			_card->RegisterChild(word_2_id, word_atk, word_2_offset);
		}
		else if (_data->cardType == E_CardType::CardType_Max_Def) {
			UISprite* word_def = (UISprite*)GetUI(E_UIContent::UIContent_Def);
			word_def->GetTransform()->SetScale(status_img_word_scale);
			word_def->ShowUI(true, -1, true);
			_card->RegisterChild(word_2_id, word_def, word_2_offset);
		}

		std::stringstream ss;
		int value = _data->param;
		if (value >= 0) {
			ss << "+";
		}
		else {
			//ss << "-";
		}
		ss << value;
		std::string valueStr;
		ss >> valueStr;

		//num
		UIWord* word_num = (UIWord*)GetUI(E_UIContent::UIContent_Num);
		word_num->SetAlignment(E_UIWordAlignment::UIWordAlignment_Left);
		word_num->SetWord(valueStr);
		word_num->ShowUI(true, -1, true);
		_card->RegisterChild(word_3_id, word_num, word_3_offset);
	}
	else if (cardTypeIndex > 10 && cardTypeIndex <= 20) {//消耗量

		if (_data->cardType == E_CardType::CardType_Hp_Consume) {
			UISprite* word_hp = (UISprite*)GetUI(E_UIContent::UIContent_HP);
			word_hp->GetTransform()->SetScale(status_img_word_scale);
			word_hp->ShowUI(true, -1, true);
			_card->RegisterChild(word_1_id, word_hp, word_1_offset);
		}
		else if (_data->cardType == E_CardType::CardType_Atk_Consume) {
			UISprite* word_atk = (UISprite*)GetUI(E_UIContent::UIContent_Atk);
			word_atk->GetTransform()->SetScale(status_img_word_scale);
			word_atk->ShowUI(true, -1, true);
			_card->RegisterChild(word_1_id, word_atk, word_1_offset);
		}
		else if (_data->cardType == E_CardType::CardType_Def_Consume) {
			UISprite* word_def = (UISprite*)GetUI(E_UIContent::UIContent_Def);
			word_def->GetTransform()->SetScale(status_img_word_scale);
			word_def->ShowUI(true, -1, true);
			_card->RegisterChild(word_1_id, word_def, word_1_offset);
		}
		else if (_data->cardType == E_CardType::CardType_Exp_Consume) {
			UISprite* word_exp = (UISprite*)GetUI(E_UIContent::UIContent_Exp);
			word_exp->GetTransform()->SetScale(status_img_word_scale);
			word_exp->ShowUI(true, -1, true);
			_card->RegisterChild(word_1_id, word_exp, word_1_offset);
		}

		UISprite* word_consume = (UISprite*)GetUI(E_UIContent::UIContent_Word_Consume);
		word_consume->GetTransform()->SetScale(word_scale);
		word_consume->ShowUI(true, -1, true);
		_card->RegisterChild(word_2_id, word_consume, word_2_offset);

		std::stringstream ss;
		int value = _data->param;
		if (value >= 0) {
			ss << "+";
		}
		else {
			//ss << "-";
		}
		ss << value;
		//ss << "%";
		std::string valueStr;
		ss >> valueStr;

		//num
		UIWord* word_num = (UIWord*)GetUI(E_UIContent::UIContent_Num);
		word_num->SetAlignment(E_UIWordAlignment::UIWordAlignment_Left);
		word_num->SetWord(valueStr);
		word_num->ShowUI(true, -1, true);
		_card->RegisterChild(word_3_id, word_num, word_3_offset);
	}
	else if (cardTypeIndex > 20 && cardTypeIndex <= 30) {//ベースダメージ
		if (_data->cardType == E_CardType::CardType_Hp_Dmg) {
			UISprite* word_hp = (UISprite*)GetUI(E_UIContent::UIContent_HP);
			word_hp->GetTransform()->SetScale(status_img_word_scale);
			word_hp->ShowUI(true, -1, true);
			_card->RegisterChild(word_1_id, word_hp, word_1_offset);
		}
		else if (_data->cardType == E_CardType::CardType_Atk_Dmg) {
			UISprite* word_atk = (UISprite*)GetUI(E_UIContent::UIContent_Atk);
			word_atk->GetTransform()->SetScale(status_img_word_scale);
			word_atk->ShowUI(true, -1, true);
			_card->RegisterChild(word_1_id, word_atk, word_1_offset);
		}
		else if (_data->cardType == E_CardType::CardType_Def_Dmg) {
			UISprite* word_def = (UISprite*)GetUI(E_UIContent::UIContent_Def);
			word_def->GetTransform()->SetScale(status_img_word_scale);
			word_def->ShowUI(true, -1, true);
			_card->RegisterChild(word_1_id, word_def, word_1_offset);
		}
		else if (_data->cardType == E_CardType::CardType_Exp_Dmg) {
			UISprite* word_exp = (UISprite*)GetUI(E_UIContent::UIContent_Exp);
			word_exp->GetTransform()->SetScale(status_img_word_scale);
			word_exp->ShowUI(true, -1, true);
			_card->RegisterChild(word_1_id, word_exp, word_1_offset);
		}

		UISprite* word_damage = (UISprite*)GetUI(E_UIContent::UIContent_Word_Damage);
		word_damage->GetTransform()->SetScale(word_scale);
		word_damage->ShowUI(true, -1, true);
		_card->RegisterChild(word_2_id, word_damage, word_2_offset);

		std::stringstream ss;
		int value = _data->param;
		if (value >= 0) {
			ss << "+";
		}
		else {
			//ss << "-";
		}
		ss << value;
		std::string valueStr;
		ss >> valueStr;

		//num
		UIWord* word_num = (UIWord*)GetUI(E_UIContent::UIContent_Num);
		word_num->SetAlignment(E_UIWordAlignment::UIWordAlignment_Left);
		word_num->SetWord(valueStr);
		word_num->ShowUI(true, -1, true);
		_card->RegisterChild(word_3_id, word_num, word_3_offset);
	}
	else if (cardTypeIndex > 50 && cardTypeIndex <= 60) {//HPエキストラカード
		if (_data->cardType == E_CardType::CardType_Hp_Life_Time) {
			UISprite* word_hp = (UISprite*)GetUI(E_UIContent::UIContent_HP);
			word_hp->GetTransform()->SetScale(status_img_word_scale);
			word_hp->ShowUI(true, -1, true);
			_card->RegisterChild(word_1_id, word_hp, word_1_offset);

			UISprite* word_time = (UISprite*)GetUI(E_UIContent::UIContent_Word_Time);
			word_time->GetTransform()->SetScale(word_scale);
			word_time->ShowUI(true, -1, true);
			_card->RegisterChild(word_2_id, word_time, word_2_offset);

			std::stringstream ss;// format = +-NUM%
			int value = (fabs(_data->param) * 100.0f + 0.5f);
			value = value % 100;
			if (value >= 0) {
				ss << "+";
			}
			else {
				ss << "-";
			}

			ss << abs(value);
			ss << "%";
			std::string valueStr;
			ss >> valueStr;

			UIWord* word_num = (UIWord*)GetUI(E_UIContent::UIContent_Num);
			word_num->SetAlignment(E_UIWordAlignment::UIWordAlignment_Left);
			word_num->SetWord(valueStr);
			word_num->ShowUI(true, -1, true);
			_card->RegisterChild(word_3_id, word_num, word_3_offset);

		}
		else if (_data->cardType == E_CardType::CardType_Hp_Bullet_Reflect) {
			UISprite* word_hp = (UISprite*)GetUI(E_UIContent::UIContent_HP);
			word_hp->GetTransform()->SetScale(status_img_word_scale);
			word_hp->ShowUI(true, -1, true);
			_card->RegisterChild(word_1_id, word_hp, word_1_offset);

			UISprite* word_bullet = (UISprite*)GetUI(E_UIContent::UIContent_Word_Bullet);
			word_bullet->GetTransform()->SetScale(word_scale);
			word_bullet->ShowUI(true, -1, true);
			_card->RegisterChild(word_2_id, word_bullet, word_2_offset);

			UISprite* word_reflect = (UISprite*)GetUI(E_UIContent::UIContent_Word_Reflect);
			word_reflect->GetTransform()->SetScale(word_scale);
			word_reflect->ShowUI(true, -1, true);
			_card->RegisterChild(word_3_id, word_reflect, word_3_offset);
		}
	}
	else if (cardTypeIndex > 60 && cardTypeIndex <= 70) {//Atkエキストラカード
		if (_data->cardType == E_CardType::CardType_Atk_Bullet_Count) {
			UISprite* word_atk = (UISprite*)GetUI(E_UIContent::UIContent_Atk);
			word_atk->GetTransform()->SetScale(status_img_word_scale);
			word_atk->ShowUI(true, -1, true);
			_card->RegisterChild(word_1_id, word_atk, word_1_offset);

			UISprite* word_count = (UISprite*)GetUI(E_UIContent::UIContent_Word_Count);
			word_count->GetTransform()->SetScale(word_scale);
			word_count->ShowUI(true, -1, true);
			_card->RegisterChild(word_2_id, word_count, word_2_offset);

			std::stringstream ss;
			int value = _data->param;
			if (value >= 0) {
				ss << "+";
			}
			else {
				//ss << "-";
			}
			ss << value;
			std::string valueStr;
			ss >> valueStr;

			UIWord* word_num = (UIWord*)GetUI(E_UIContent::UIContent_Num);
			word_num->SetAlignment(E_UIWordAlignment::UIWordAlignment_Left);
			word_num->SetWord(valueStr);
			word_num->ShowUI(true, -1, true);
			_card->RegisterChild(word_3_id, word_num, word_3_offset);
		}
		else if (_data->cardType == E_CardType::CardType_Atk_Bullet_Range) {
			UISprite* word_atk = (UISprite*)GetUI(E_UIContent::UIContent_Atk);
			word_atk->GetTransform()->SetScale(status_img_word_scale);
			word_atk->ShowUI(true, -1, true);
			_card->RegisterChild(word_1_id, word_atk, word_1_offset);

			UISprite* word_range = (UISprite*)GetUI(E_UIContent::UIContent_Word_Range);
			word_range->GetTransform()->SetScale(word_scale);
			word_range->ShowUI(true, -1, true);
			_card->RegisterChild(word_2_id, word_range, word_2_offset);

			std::stringstream ss;// format = +-NUM%
			int value = (fabs(_data->param) * 100.0f + 0.5f);
			value = value % 100;
			if (value >= 0) {
				ss << "+";
			}
			else {
				ss << "-";
			}

			ss << abs(value);
			ss << "%";
			std::string valueStr;
			ss >> valueStr;

			UIWord* word_num = (UIWord*)GetUI(E_UIContent::UIContent_Num);
			word_num->SetAlignment(E_UIWordAlignment::UIWordAlignment_Left);
			word_num->SetWord(valueStr);
			word_num->ShowUI(true, -1, true);
			_card->RegisterChild(word_3_id, word_num, word_3_offset);
		}
	}
	else if (cardTypeIndex > 70 && cardTypeIndex <= 80) {//DEFエキストラカード
		if (_data->cardType == E_CardType::CardType_Def_Bullet_Count) {
			UISprite* word_def = (UISprite*)GetUI(E_UIContent::UIContent_Def);
			word_def->GetTransform()->SetScale(status_img_word_scale);
			word_def->ShowUI(true, -1, true);
			_card->RegisterChild(word_1_id, word_def, word_1_offset);

			UISprite* word_count = (UISprite*)GetUI(E_UIContent::UIContent_Word_Count);
			word_count->GetTransform()->SetScale(word_scale);
			word_count->ShowUI(true, -1, true);
			_card->RegisterChild(word_2_id, word_count, word_2_offset);

			std::stringstream ss;
			int value = _data->param;
			if (value >= 0) {
				ss << "+";
			}
			else {
				//ss << "-";
			}
			ss << value;
			std::string valueStr;
			ss >> valueStr;

			UIWord* word_num = (UIWord*)GetUI(E_UIContent::UIContent_Num);
			word_num->SetAlignment(E_UIWordAlignment::UIWordAlignment_Left);
			word_num->SetWord(valueStr);
			word_num->ShowUI(true, -1, true);
			_card->RegisterChild(word_3_id, word_num, word_3_offset);
		}
		else if (_data->cardType == E_CardType::CardType_Def_Recovery_Cap) {
			UISprite* word_def = (UISprite*)GetUI(E_UIContent::UIContent_Def);
			word_def->GetTransform()->SetScale(status_img_word_scale);
			word_def->ShowUI(true, -1, true);
			_card->RegisterChild(word_1_id, word_def, word_1_offset);

			UISprite* word_recovery = (UISprite*)GetUI(E_UIContent::UIContent_Word_Recovery);
			word_recovery->GetTransform()->SetScale(word_scale);
			word_recovery->ShowUI(true, -1, true);
			_card->RegisterChild(word_2_id, word_recovery, word_2_offset);

			std::stringstream ss;// format = +-NUM%
			int value = (fabs(_data->param) * 100.0f + 0.5f);
			value = value % 100;
			if (value >= 0) {
				ss << "+";
			}
			else {
				ss << "-";
			}

			ss << abs(value);
			ss << "%";
			std::string valueStr;
			ss >> valueStr;

			UIWord* word_num = (UIWord*)GetUI(E_UIContent::UIContent_Num);
			word_num->SetAlignment(E_UIWordAlignment::UIWordAlignment_Left);
			word_num->SetWord(valueStr);
			word_num->ShowUI(true, -1, true);
			_card->RegisterChild(word_3_id, word_num, word_3_offset);
		}
		else if (_data->cardType == E_CardType::CardType_Def_Bullet_Reflect_Count) {
			UISprite* word_def = (UISprite*)GetUI(E_UIContent::UIContent_Def);
			word_def->GetTransform()->SetScale(status_img_word_scale);
			word_def->ShowUI(true, -1, true);
			_card->RegisterChild(word_1_id, word_def, word_1_offset);

			UISprite* word_count = (UISprite*)GetUI(E_UIContent::UIContent_Word_Count);
			word_count->GetTransform()->SetScale(word_scale);
			word_count->ShowUI(true, -1, true);
			_card->RegisterChild(word_2_id, word_count, word_2_offset);

			//TODO
		}
	}
	else if (cardTypeIndex > 80 && cardTypeIndex <= 90) {//EXPエキストラカード
		if (_data->cardType == E_CardType::CardType_Exp_Gain) {
			UISprite* word_exp = (UISprite*)GetUI(E_UIContent::UIContent_Exp);
			word_exp->GetTransform()->SetScale(status_img_word_scale);
			word_exp->ShowUI(true, -1, true);
			_card->RegisterChild(word_1_id, word_exp, word_1_offset);

			UISprite* word_gain = (UISprite*)GetUI(E_UIContent::UIContent_Word_Gain);
			word_gain->GetTransform()->SetScale(word_scale);
			word_gain->ShowUI(true, -1, true);
			_card->RegisterChild(word_2_id, word_gain, word_2_offset);

			std::stringstream ss;// format = +-NUM%
			int value = (fabs(_data->param) * 100.0f + 0.5f);
			value = value % 100;
			if (value >= 0) {
				ss << "+";
			}
			else {
				ss << "-";
			}

			ss << abs(value);
			ss << "%";
			std::string valueStr;
			ss >> valueStr;

			UIWord* word_num = (UIWord*)GetUI(E_UIContent::UIContent_Num);
			word_num->SetAlignment(E_UIWordAlignment::UIWordAlignment_Left);
			word_num->SetWord(valueStr);
			word_num->ShowUI(true, -1, true);
			_card->RegisterChild(word_3_id, word_num, word_3_offset);
		}
		else if (_data->cardType == E_CardType::CardType_Exp_Bullet_Count) {
			UISprite* word_exp = (UISprite*)GetUI(E_UIContent::UIContent_Exp);
			word_exp->GetTransform()->SetScale(status_img_word_scale);
			word_exp->ShowUI(true, -1, true);
			_card->RegisterChild(word_1_id, word_exp, word_1_offset);

			UISprite* word_count = (UISprite*)GetUI(E_UIContent::UIContent_Word_Count);
			word_count->GetTransform()->SetScale(word_scale);
			word_count->ShowUI(true, -1, true);
			_card->RegisterChild(word_2_id, word_count, word_2_offset);

			std::stringstream ss;
			int value = _data->param;
			if (value >= 0) {
				ss << "+";
			}
			else {
				//ss << "-";
			}
			ss << value;
			std::string valueStr;
			ss >> valueStr;

			UIWord* word_num = (UIWord*)GetUI(E_UIContent::UIContent_Num);
			word_num->SetAlignment(E_UIWordAlignment::UIWordAlignment_Left);
			word_num->SetWord(valueStr);
			word_num->ShowUI(true, -1, true);
			_card->RegisterChild(word_3_id, word_num, word_3_offset);
		}
		else if (_data->cardType == E_CardType::CardType_Exp_Bullet_Reflect_Count) {
			UISprite* word_exp = (UISprite*)GetUI(E_UIContent::UIContent_Exp);
			word_exp->GetTransform()->SetScale(status_img_word_scale);
			word_exp->ShowUI(true, -1, true);
			_card->RegisterChild(word_1_id, word_exp, word_1_offset);

			UISprite* word_count = (UISprite*)GetUI(E_UIContent::UIContent_Word_Count);
			word_count->GetTransform()->SetScale(word_scale);
			word_count->ShowUI(true, -1, true);
			_card->RegisterChild(word_2_id, word_count, word_2_offset);

			//TODO
		}
	}
	else if (cardTypeIndex > 100 && cardTypeIndex <= 110) {//スペシャルエキストラカード
		if (_data->cardType == E_CardType::CardType_Attack_Speed_Up) {
			UISprite* word_attack = (UISprite*)GetUI(E_UIContent::UIContent_Word_Attack);
			word_attack->GetTransform()->SetScale(word_scale);
			word_attack->ShowUI(true, -1, true);
			_card->RegisterChild(word_1_id, word_attack, word_1_offset);

			UISprite* word_speed = (UISprite*)GetUI(E_UIContent::UIContent_Word_Speed);
			word_speed->GetTransform()->SetScale(word_scale);
			word_speed->ShowUI(true, -1, true);
			_card->RegisterChild(word_2_id, word_speed, word_2_offset);

			UISprite* word_up = (UISprite*)GetUI(E_UIContent::UIContent_Word_Up);
			word_up->GetTransform()->SetScale(word_scale);
			word_up->ShowUI(true, -1, true);
			_card->RegisterChild(word_3_id, word_up, word_3_offset);
		}
		else if (_data->cardType == E_CardType::CardType_Move_Speed_Up) {
			UISprite* word_move = (UISprite*)GetUI(E_UIContent::UIContent_Word_Move);
			word_move->GetTransform()->SetScale(word_scale);
			word_move->ShowUI(true, -1, true);
			_card->RegisterChild(word_1_id, word_move, word_1_offset);

			UISprite* word_speed = (UISprite*)GetUI(E_UIContent::UIContent_Word_Speed);
			word_speed->GetTransform()->SetScale(word_scale);
			word_speed->ShowUI(true, -1, true);
			_card->RegisterChild(word_2_id, word_speed, word_2_offset);

			UISprite* word_up = (UISprite*)GetUI(E_UIContent::UIContent_Word_Up);
			word_up->GetTransform()->SetScale(word_scale);
			word_up->ShowUI(true, -1, true);
			_card->RegisterChild(word_3_id, word_up, word_3_offset);
		}
		else if (_data->cardType == E_CardType::CardType_Reflect_Enemy_Bullet) {
			UISprite* word_reflect = (UISprite*)GetUI(E_UIContent::UIContent_Word_Reflect);
			word_reflect->GetTransform()->SetScale(word_scale);
			word_reflect->ShowUI(true, -1, true);
			_card->RegisterChild(word_1_id, word_reflect, word_1_offset);

			UISprite* word_enemy = (UISprite*)GetUI(E_UIContent::UIContent_Word_Enemy);
			word_enemy->GetTransform()->SetScale(word_scale);
			word_enemy->ShowUI(true, -1, true);
			_card->RegisterChild(word_2_id, word_enemy, word_2_offset);

			UISprite* word_bullet = (UISprite*)GetUI(E_UIContent::UIContent_Word_Bullet);
			word_bullet->GetTransform()->SetScale(word_scale);
			word_bullet->ShowUI(true, -1, true);
			_card->RegisterChild(word_3_id, word_bullet, word_3_offset);
		}
	}

}

UIMgr::UIMgr()
{
	curSelectIndex = 0;

	card_1 = nullptr;
	card_2 = nullptr;
	card_3 = nullptr;
	card_4 = nullptr;
	lv_panel = nullptr;
	hud_status_frame = nullptr;

	num_sprite_shaderParam = nullptr;
	num_red_sprite_shaderParam = nullptr;
	num_outline_sprite_shaderParam = nullptr;
	word_content_cell_shaderParam = nullptr;
	sprite_cell_bg_shaderParam = nullptr;
	sprite_extra_cell_bg_shaderParam = nullptr;

	guideUI = nullptr;
	crosshair = nullptr;

	title_sprite = nullptr;
	title_bg = nullptr;

	lv_up_sprite = nullptr;

	word_pause_sprite = nullptr;
	word_you_survive_sprite = nullptr;
	word_thank_sprite = nullptr;
	word_you_lose_sprite = nullptr;
	word_start_sprite = nullptr;
	word_exit_sprite = nullptr;
	word_title_sprite = nullptr;
	word_retry_sprite = nullptr;
	word_continue_sprite = nullptr;

	statusSelector = nullptr;
	uiSelector = nullptr;

	word_condition_1_offset = vector3s(CARD_CONDITION_WORD_OFFSET_1_X, CARD_CONDITION_WORD_OFFSET_1_Y, -1);
	word_condition_2_offset = vector3s(CARD_CONDITION_WORD_OFFSET_2_X, CARD_CONDITION_WORD_OFFSET_2_Y, -1);
	word_condition_3_offset = vector3s(CARD_CONDITION_WORD_OFFSET_3_X, CARD_CONDITION_WORD_OFFSET_3_Y, -1);
	word_effect_1_offset = vector3s(CARD_EFFECT_WORD_OFFSET_1_X, CARD_EFFECT_WORD_OFFSET_1_Y, -1);
	word_effect_2_offset = vector3s(CARD_EFFECT_WORD_OFFSET_2_X, CARD_EFFECT_WORD_OFFSET_2_Y, -1);
	word_effect_3_offset = vector3s(CARD_EFFECT_WORD_OFFSET_3_X, CARD_EFFECT_WORD_OFFSET_3_Y, -1);

	selector_pos_status_hp = vector3s(STATUS_SELECTOR_HP_POS_X, STATUS_SELECTOR_HP_POS_Y, 0);
	selector_pos_status_atk = vector3s(STATUS_SELECTOR_ATK_POS_X, STATUS_SELECTOR_ATK_POS_Y, 0);
	selector_pos_status_def = vector3s(STATUS_SELECTOR_DEF_POS_X, STATUS_SELECTOR_DEF_POS_Y, 0);
	selector_pos_status_exp = vector3s(STATUS_SELECTOR_EXP_POS_X, STATUS_SELECTOR_EXP_POS_Y, 0);

	//menu img pos
	title_default_pos = vector3s(0.0f, SCREEN_HEIGHT * 0.27f, -20.0f);

	pause_img_pos = vector3s(0.0f, SCREEN_HEIGHT * 0.27f, -20.0f);
	lose_img_pos = vector3s(0.0f, SCREEN_HEIGHT * 0.27f, -20.0f);
	win_img_pos = vector3s(0.0f, SCREEN_HEIGHT * 0.27f, -20.0f);
	thanks_img_pos = vector3s(0.0f, SCREEN_HEIGHT * 0.1f, -20.0f);

	//item pos
	title_item_start_default_pos = vector3s(0.0f, -SCREEN_HEIGHT * 0.1f, -20.0f);
	title_item_exit_default_pos = vector3s(0.0f, -SCREEN_HEIGHT * 0.22f, -20.0f);

	menu_pause_continue_pos = vector3s(0.0f, -SCREEN_HEIGHT * 0.1f, -20.0f);
	menu_pause_retry_pos = vector3s(0.0f, -SCREEN_HEIGHT * 0.2f, -20.0f);
	menu_pause_title_pos = vector3s(0.0f, -SCREEN_HEIGHT * 0.3f, -20.0f);

	lose_menu_retry_pos = vector3s(0.0f, -SCREEN_HEIGHT * 0.1f, -20.0f);
	lose_menu_title_pos = vector3s(0.0f, -SCREEN_HEIGHT * 0.2f, -20.0f);

	win_menu_title_pos = vector3s(0.0f, -SCREEN_HEIGHT * 0.2f, -20.0f);

	hud_default_pos = vector3s(SCREEN_WIDTH * 0.5f + 200.0f, -350.0f, -20.0f);
	hud_target_pos = vector3s(450.0f, -350.0f, -20.0f);

	lv_up_defaultPos = vector3s(-250.0f, -SCREEN_HEIGHT * 0.5f, -20.0f);
	lv_up_targetPos = vector3s(-250.0f, -300.0f, -20.0f);

	uiPools.clear();
}

UIMgr::~UIMgr() {
	uiPools.clear();

	if (num_sprite_shaderParam != nullptr) {
		delete num_sprite_shaderParam;
	}
	num_sprite_shaderParam = nullptr;

	if (num_red_sprite_shaderParam != nullptr) {
		delete num_red_sprite_shaderParam;
	}
	num_red_sprite_shaderParam = nullptr;

	if (num_outline_sprite_shaderParam != nullptr) {
		delete num_outline_sprite_shaderParam;
	}
	num_outline_sprite_shaderParam = nullptr;

	if (word_content_cell_shaderParam != nullptr) {
		delete word_content_cell_shaderParam;
	}
	word_content_cell_shaderParam = nullptr;

	if (sprite_cell_bg_shaderParam != nullptr) {
		delete sprite_cell_bg_shaderParam;
	}
	sprite_cell_bg_shaderParam = nullptr;
	if (sprite_extra_cell_bg_shaderParam != nullptr) {
		delete sprite_extra_cell_bg_shaderParam;
	}
	sprite_extra_cell_bg_shaderParam = nullptr;

	guideUI = nullptr;
	crosshair = nullptr;
	//if (lv_up_sprite != nullptr) {
	//	delete lv_up_sprite;
	//}
	//if (word_you_lose_sprite != nullptr) {
	//	delete word_you_lose_sprite;
	//}
	//if (word_start_sprite != nullptr) {
	//	delete word_start_sprite;
	//}
	//if (word_exit_sprite != nullptr) {
	//	delete word_exit_sprite;
	//}
	//if (word_title_sprite != nullptr) {
	//	delete word_title_sprite;
	//}

	title_sprite = nullptr;
	title_bg = nullptr;

	word_pause_sprite = nullptr;
	word_you_survive_sprite = nullptr;
	word_thank_sprite = nullptr;
	word_you_lose_sprite = nullptr;
	word_start_sprite = nullptr;
	word_exit_sprite = nullptr;
	word_title_sprite = nullptr;
	word_retry_sprite = nullptr;
	word_continue_sprite = nullptr;

	statusSelector = nullptr;
	uiSelector = nullptr;

	card_1 = nullptr;
	card_2 = nullptr;
	card_3 = nullptr;
	card_4 = nullptr;
	hud_status_frame = nullptr;

}

void UIMgr::DoInit()
{
	//=======================================================================文字描画データを作る
	ShaderParam_Normal* temp_num_sprite_shaderParam = new ShaderParam_Normal();
	temp_num_sprite_shaderParam->shaderProgram = GameMgr::GetInstance()->shaderMgr->GetShaderProgram(E_Shader::Shader_Normal);
	temp_num_sprite_shaderParam->texNo = GameMgr::GetInstance()->textureMgr->GetTextureID(E_Texture::Texture_UI_HUD_Package_Test_2);
	temp_num_sprite_shaderParam->texIndex = vector2s(NUM_WHITE_TEX_START_INDEX_X, NUM_WHITE_TEX_START_INDEX_Y);
	temp_num_sprite_shaderParam->tex_w_count = NUM_TEX_W_COUNT;
	temp_num_sprite_shaderParam->tex_texel_size = num_tex_size;
	num_sprite_shaderParam = temp_num_sprite_shaderParam;

	ShaderParam_Normal* temp_num_red_sprite_shaderParam = new ShaderParam_Normal();
	temp_num_red_sprite_shaderParam->shaderProgram = GameMgr::GetInstance()->shaderMgr->GetShaderProgram(E_Shader::Shader_Normal);
	temp_num_red_sprite_shaderParam->texNo = GameMgr::GetInstance()->textureMgr->GetTextureID(E_Texture::Texture_UI_HUD_Package_Test_2);
	temp_num_red_sprite_shaderParam->texIndex = vector2s(NUM_RED_TEX_START_INDEX_X, NUM_RED_TEX_START_INDEX_Y);
	temp_num_red_sprite_shaderParam->tex_w_count = NUM_TEX_W_COUNT;
	temp_num_red_sprite_shaderParam->tex_texel_size = num_tex_size;
	num_red_sprite_shaderParam = temp_num_red_sprite_shaderParam;

	ShaderParam_Normal* temp_num_outline_sprite_shaderParam = new ShaderParam_Normal();
	temp_num_outline_sprite_shaderParam->shaderProgram = GameMgr::GetInstance()->shaderMgr->GetShaderProgram(E_Shader::Shader_Normal);
	temp_num_outline_sprite_shaderParam->texNo = GameMgr::GetInstance()->textureMgr->GetTextureID(E_Texture::Texture_UI_HUD_Package_Test_2);
	temp_num_outline_sprite_shaderParam->texIndex = vector2s(NUM_WHITE_OUTLINE_TEX_START_INDEX_X, NUM_WHITE_OUTLINE_TEX_START_INDEX_Y);
	temp_num_outline_sprite_shaderParam->tex_w_count = NUM_TEX_W_COUNT;
	temp_num_outline_sprite_shaderParam->tex_texel_size = num_tex_size;
	num_outline_sprite_shaderParam = temp_num_outline_sprite_shaderParam;

	//word
	ShaderParam_Normal* temp_word_content_cell_shaderParma = new ShaderParam_Normal();
	temp_word_content_cell_shaderParma->shaderProgram = GameMgr::GetInstance()->shaderMgr->GetShaderProgram(E_Shader::Shader_Normal);
	temp_word_content_cell_shaderParma->texNo = GameMgr::GetInstance()->textureMgr->GetTextureID(E_Texture::Texture_Colony_Green_Normal);
	temp_word_content_cell_shaderParma->texIndex = vector2s(0, 0);
	temp_word_content_cell_shaderParma->tex_w_count = 1;
	temp_word_content_cell_shaderParma->tex_texel_size = vector2s(1, 1);
	word_content_cell_shaderParam = temp_word_content_cell_shaderParma;

	//card bg
	ShaderParam_Normal* temp_bg_content_cell_shaderParma = new ShaderParam_Normal();
	temp_bg_content_cell_shaderParma->shaderProgram = GameMgr::GetInstance()->shaderMgr->GetShaderProgram(E_Shader::Shader_Normal);
	temp_bg_content_cell_shaderParma->texNo = GameMgr::GetInstance()->textureMgr->GetTextureID(E_Texture::Texture_UI_HUD_Package_Test_2);
	temp_bg_content_cell_shaderParma->usePreComputeUV = true;
	temp_bg_content_cell_shaderParma->preComputeUV = vector2s(CARD_2_IMG_TEX_START_X, CARD_2_IMG_TEX_START_Y);
	temp_bg_content_cell_shaderParma->preComputeUVSize = vector2s(CARD_2_IMG_TEX_SIZE_X, CARD_2_IMG_TEX_SIZE_Y);
	temp_bg_content_cell_shaderParma->tex_texel_size = ui_tex_size;
	sprite_cell_bg_shaderParam = temp_bg_content_cell_shaderParma;

	//extra card bg
	ShaderParam_Normal* temp_extra_bg_content_cell_shaderParma = new ShaderParam_Normal();
	temp_extra_bg_content_cell_shaderParma->shaderProgram = GameMgr::GetInstance()->shaderMgr->GetShaderProgram(E_Shader::Shader_Normal);
	temp_extra_bg_content_cell_shaderParma->texNo = GameMgr::GetInstance()->textureMgr->GetTextureID(E_Texture::Texture_UI_HUD_Package_Test_2);
	temp_extra_bg_content_cell_shaderParma->usePreComputeUV = true;
	temp_extra_bg_content_cell_shaderParma->preComputeUV = vector2s(CARD_3_IMG_TEX_START_X, CARD_3_IMG_TEX_START_Y);
	temp_extra_bg_content_cell_shaderParma->preComputeUVSize = vector2s(CARD_3_IMG_TEX_SIZE_X, CARD_3_IMG_TEX_SIZE_Y);
	temp_extra_bg_content_cell_shaderParma->tex_texel_size = ui_tex_size;
	sprite_extra_cell_bg_shaderParam = temp_extra_bg_content_cell_shaderParma;
	//crosshair


	// guide
	ShaderParam_Normal* temp_guide_content_cell_shaderParma = new ShaderParam_Normal();
	temp_guide_content_cell_shaderParma->shaderProgram = GameMgr::GetInstance()->shaderMgr->GetShaderProgram(E_Shader::Shader_Normal);
	temp_guide_content_cell_shaderParma->texNo = GameMgr::GetInstance()->textureMgr->GetTextureID(E_Texture::Texture_UI_HUD_Package_Test_2);
	temp_guide_content_cell_shaderParma->usePreComputeUV = true;
	temp_guide_content_cell_shaderParma->preComputeUV = vector2s(0, 0);
	temp_guide_content_cell_shaderParma->preComputeUVSize = vector2s(GUIDE_TEX_SIZE_X, GUIDE_TEX_SIZE_X);
	temp_guide_content_cell_shaderParma->tex_texel_size = ui_tex_size;



	guideUI = new UISprite();
	guideUI->SetID(GameMgr::GetInstance()->sceneObjMgr->GetID());
	guideUI->SetPos(vector3s(0, 0, 0), true);

	DrawData* guideSelector_drawData = RenderMgr::GetNewDrawData(nullptr, false, E_BatchingType::BatchingType_UI, false);
	guideSelector_drawData->SetSize(vector2s(GUIDE_TEX_SIZE_X, GUIDE_TEX_SIZE_X));
	guideSelector_drawData->SetShaderParam(temp_guide_content_cell_shaderParma);

	guideUI->SetDrawData(guideSelector_drawData);

	guideUI->ShowUI(false, -1, true);

	GameMgr::GetInstance()->sceneObjMgr->RegisterCell(guideUI);

	if (temp_guide_content_cell_shaderParma != nullptr) {
		delete temp_guide_content_cell_shaderParma;
	}

#pragma region selector

	//status selector
	ShaderParam_Normal* statusSelector_shaderParam = new ShaderParam_Normal();
	statusSelector_shaderParam->shaderProgram = GameMgr::GetInstance()->shaderMgr->GetShaderProgram(E_Shader::Shader_Normal);
	statusSelector_shaderParam->texNo = GameMgr::GetInstance()->textureMgr->GetTextureID(E_Texture::Texture_UI_HUD_Package_Test_2);
	statusSelector_shaderParam->usePreComputeUV = true;
	statusSelector_shaderParam->preComputeUV = vector2s(STATUS_SELECTOR_START_X, STATUS_SELECTOR_START_Y);
	statusSelector_shaderParam->preComputeUVSize = vector2s(STATUS_SELECTOR_TEX_SIZE_X, STATUS_SELECTOR_TEX_SIZE_Y);
	statusSelector_shaderParam->tex_texel_size = ui_tex_size;

	statusSelector = new UISprite();
	statusSelector->SetID(GameMgr::GetInstance()->sceneObjMgr->GetID());
	statusSelector->SetScale(vector3s(2.0f, 2.0f, 1.0f));
	statusSelector->SetPos(vector3s(0, 0, 0), true);
	DrawData* statusSelector_drawData = RenderMgr::GetNewDrawData(nullptr, true, E_BatchingType::BatchingType_UI, false);
	statusSelector_drawData->SetSize(vector2s(STATUS_SELECTOR_TEX_SIZE_X, STATUS_SELECTOR_TEX_SIZE_Y));
	statusSelector_drawData->SetShaderParam(statusSelector_shaderParam);

	statusSelector->SetDrawData(statusSelector_drawData);

	statusSelector->ShowUI(false, -1, true);

	if (statusSelector_shaderParam != nullptr) {
		delete statusSelector_shaderParam;
		statusSelector_shaderParam = nullptr;
	}

	//ui selector
	ShaderParam_Normal* selector_shaderParam = new ShaderParam_Normal();
	selector_shaderParam->shaderProgram = GameMgr::GetInstance()->shaderMgr->GetShaderProgram(E_Shader::Shader_Normal);
	selector_shaderParam->texNo = GameMgr::GetInstance()->textureMgr->GetTextureID(E_Texture::Texture_UI_HUD_Package_Test_2);
	selector_shaderParam->usePreComputeUV = true;
	selector_shaderParam->preComputeUV = vector2s(CARD_SELECTOR_LT_START_X, CARD_SELECTOR_LT_START_Y);
	selector_shaderParam->preComputeUVSize = vector2s(CARD_SELECTOR_TEX_SIZE_X, CARD_SELECTOR_TEX_SIZE_Y);
	selector_shaderParam->tex_texel_size = ui_tex_size;

	uiSelector = new UIGroup();
	uiSelector->SetUIContentType(E_UIContent::UIContent_Img_Title_Selecotr);
	uiSelector->deleteChild = true;
	uiSelector->SetID(GameMgr::GetInstance()->sceneObjMgr->GetID());
	uiSelector->SetPos(vector3s(450, -350, -20), true);

	//lt
	DrawData* selector_lt_drawData = RenderMgr::GetNewDrawData(nullptr, false, E_BatchingType::BatchingType_UI, false);
	selector_lt_drawData->SetSize(vector2s(CARD_SELECTOR_TEX_SIZE_X, CARD_SELECTOR_TEX_SIZE_Y));
	selector_lt_drawData->SetShaderParam(selector_shaderParam);

	UISprite* selector_lt = new UISprite();
	selector_lt->SetDrawData(selector_lt_drawData);

	//rt
	selector_shaderParam->preComputeUV = vector2s(CARD_SELECTOR_RT_START_X, CARD_SELECTOR_RT_START_Y);
	DrawData* selector_rt_drawData = RenderMgr::GetNewDrawData(nullptr, false, E_BatchingType::BatchingType_UI, false);
	selector_rt_drawData->SetSize(vector2s(CARD_SELECTOR_TEX_SIZE_X, CARD_SELECTOR_TEX_SIZE_Y));
	selector_rt_drawData->SetShaderParam(selector_shaderParam);

	UISprite* selector_rt = new UISprite();
	selector_rt->SetDrawData(selector_rt_drawData);

	//lb
	selector_shaderParam->preComputeUV = vector2s(CARD_SELECTOR_LB_START_X, CARD_SELECTOR_LB_START_Y);
	DrawData* selector_lb_drawData = RenderMgr::GetNewDrawData(nullptr, false, E_BatchingType::BatchingType_UI, false);
	selector_lb_drawData->SetSize(vector2s(CARD_SELECTOR_TEX_SIZE_X, CARD_SELECTOR_TEX_SIZE_Y));
	selector_lb_drawData->SetShaderParam(selector_shaderParam);

	UISprite* selector_lb = new UISprite();
	selector_lb->SetDrawData(selector_lb_drawData);

	//rb
	selector_shaderParam->preComputeUV = vector2s(CARD_SELECTOR_RB_START_X, CARD_SELECTOR_RB_START_Y);
	DrawData* selector_rb_drawData = RenderMgr::GetNewDrawData(nullptr, false, E_BatchingType::BatchingType_UI, false);
	selector_rb_drawData->SetSize(vector2s(CARD_SELECTOR_TEX_SIZE_X, CARD_SELECTOR_TEX_SIZE_Y));
	selector_rb_drawData->SetShaderParam(selector_shaderParam);

	UISprite* selector_rb = new UISprite();
	selector_rb->SetDrawData(selector_rb_drawData);


	if (selector_shaderParam != nullptr) {
		delete selector_shaderParam;
	}

	uiSelector->RegisterChild(UI_SELECTOR_LT_ID, selector_lt, vector3s(0, 0, 0));
	uiSelector->RegisterChild(UI_SELECTOR_RT_ID, selector_rt, vector3s(0, 0, 0));
	uiSelector->RegisterChild(UI_SELECTOR_LB_ID, selector_lb, vector3s(0, 0, 0));
	uiSelector->RegisterChild(UI_SELECTOR_RB_ID, selector_rb, vector3s(0, 0, 0));

	uiSelector->ShowUI(false, -1, true);

#pragma endregion selector

#pragma region crosshair

	// guide
	ShaderParam_Normal* temp_cross_cell_shaderParma = new ShaderParam_Normal();
	temp_cross_cell_shaderParma->shaderProgram = GameMgr::GetInstance()->shaderMgr->GetShaderProgram(E_Shader::Shader_Normal);
	temp_cross_cell_shaderParma->texNo = GameMgr::GetInstance()->textureMgr->GetTextureID(E_Texture::Texture_UI_HUD_Package_Test_2);
	temp_cross_cell_shaderParma->usePreComputeUV = true;
	temp_cross_cell_shaderParma->preComputeUV = vector2s(CROSSHAIR_TEX_START_X, CROSSHAIR_TEX_START_Y);
	temp_cross_cell_shaderParma->preComputeUVSize = vector2s(CROSSHAIR_TEX_SIZE_X, CROSSHAIR_TEX_SIZE_Y);
	temp_cross_cell_shaderParma->tex_texel_size = ui_tex_size;



	crosshair = new UISprite();
	crosshair->SetID(GameMgr::GetInstance()->sceneObjMgr->GetID());
	crosshair->SetPos(vector3s(0, 0, 0), true);

	DrawData* crosshair_drawData = RenderMgr::GetNewDrawData(nullptr, false, E_BatchingType::BatchingType_UI, false);
	crosshair_drawData->SetSize(vector2s(CROSSHAIR_TEX_SIZE_X, CROSSHAIR_TEX_SIZE_Y));
	crosshair_drawData->SetShaderParam(temp_cross_cell_shaderParma);

	crosshair->SetDrawData(crosshair_drawData);

	crosshair->ShowUI(false, -1, true);

	GameMgr::GetInstance()->sceneObjMgr->RegisterCell(crosshair);

	if (temp_cross_cell_shaderParma != nullptr) {
		delete temp_cross_cell_shaderParma;
	}

#pragma endregion crosshair

	GameMgr::GetInstance()->sceneObjMgr->RegisterCell(statusSelector);
	GameMgr::GetInstance()->sceneObjMgr->RegisterCell(uiSelector);

#pragma region word_sprite

	//lv up
	lv_up_sprite = new UISprite();
	lv_up_sprite->SetUIContentType(E_UIContent::UIContent_Word_Lv_Up);
	lv_up_sprite->SetID(GameMgr::GetInstance()->sceneObjMgr->GetID());
	lv_up_sprite->SetPos(vector3s(0, 0, -11), true);

	DrawData* lv_up_drawData = RenderMgr::GetNewDrawData(nullptr, true, E_BatchingType::BatchingType_UI, false);
	lv_up_drawData->SetSize(vector2s(LV_UP_IMG_TEX_SIZE_X, LV_UP_IMG_TEX_SIZE_Y));
	ShaderParam_Normal* tempWord_shaderParam = new ShaderParam_Normal();
	tempWord_shaderParam->shaderProgram = GameMgr::GetInstance()->shaderMgr->GetShaderProgram(E_Shader::Shader_Normal);
	tempWord_shaderParam->texNo = GameMgr::GetInstance()->textureMgr->GetTextureID(E_Texture::Texture_UI_HUD_Package_Test_2);
	tempWord_shaderParam->usePreComputeUV = true;
	tempWord_shaderParam->preComputeUV = vector2s(LV_UP_IMG_TEX_START_X, LV_UP_IMG_TEX_START_Y);
	tempWord_shaderParam->preComputeUVSize = vector2s(LV_UP_IMG_TEX_SIZE_X, LV_UP_IMG_TEX_SIZE_Y);
	tempWord_shaderParam->tex_texel_size = ui_tex_size;

	lv_up_drawData->SetShaderParam(tempWord_shaderParam);

	lv_up_sprite->SetDrawData(lv_up_drawData);

	lv_up_sprite->ShowUI(false, -1, true);

	GameMgr::GetInstance()->sceneObjMgr->RegisterCell(lv_up_sprite);

	//pause
	word_pause_sprite = new UISprite();
	word_pause_sprite->SetUIContentType(E_UIContent::UIContent_Word_Pause);
	word_pause_sprite->SetID(GameMgr::GetInstance()->sceneObjMgr->GetID());
	word_pause_sprite->SetPos(pause_img_pos, true);
	word_pause_sprite->SetScale(vector3s(MENU_IMG_SCALE, MENU_IMG_SCALE, 1.0f));

	DrawData* word_pause_drawData = RenderMgr::GetNewDrawData(nullptr, true, E_BatchingType::BatchingType_UI, false);
	word_pause_drawData->SetSize(vector2s(WORD_PAUSE_TEX_SIZE_X, WORD_PAUSE_TEX_SIZE_Y));
	tempWord_shaderParam->usePreComputeUV = true;
	tempWord_shaderParam->preComputeUV = vector2s(WORD_PAUSE_TEX_START_X, WORD_PAUSE_TEX_START_Y);
	tempWord_shaderParam->preComputeUVSize = vector2s(WORD_PAUSE_TEX_SIZE_X, WORD_PAUSE_TEX_SIZE_Y);
	tempWord_shaderParam->tex_texel_size = ui_tex_size;

	word_pause_drawData->SetShaderParam(tempWord_shaderParam);

	word_pause_sprite->SetDrawData(word_pause_drawData);

	word_pause_sprite->ShowUI(false, -1, true);

	GameMgr::GetInstance()->sceneObjMgr->RegisterCell(word_pause_sprite);

	//you_lose
	word_you_lose_sprite = new UISprite();
	word_you_lose_sprite->SetUIContentType(E_UIContent::UIContent_Word_You_Lose);
	word_you_lose_sprite->SetID(GameMgr::GetInstance()->sceneObjMgr->GetID());
	word_you_lose_sprite->SetPos(lose_img_pos, true);
	word_you_lose_sprite->SetScale(vector3s(MENU_IMG_SCALE, MENU_IMG_SCALE, 1.0f));

	DrawData* word_you_lose_drawData = RenderMgr::GetNewDrawData(nullptr, true, E_BatchingType::BatchingType_UI, false);
	word_you_lose_drawData->SetSize(vector2s(WORD_YOU_LOSE_TEX_SIZE_X, WORD_YOU_LOSE_TEX_SIZE_Y));
	tempWord_shaderParam->usePreComputeUV = true;
	tempWord_shaderParam->preComputeUV = vector2s(WORD_YOU_LOSE_TEX_START_X, WORD_YOU_LOSE_TEX_START_Y);
	tempWord_shaderParam->preComputeUVSize = vector2s(WORD_YOU_LOSE_TEX_SIZE_X, WORD_YOU_LOSE_TEX_SIZE_Y);
	tempWord_shaderParam->tex_texel_size = ui_tex_size;

	word_you_lose_drawData->SetShaderParam(tempWord_shaderParam);

	word_you_lose_sprite->SetDrawData(word_you_lose_drawData);

	word_you_lose_sprite->ShowUI(false, -1, true);

	GameMgr::GetInstance()->sceneObjMgr->RegisterCell(word_you_lose_sprite);

	//you survive
	word_you_survive_sprite = new UISprite();
	word_you_survive_sprite->SetUIContentType(E_UIContent::UIContent_Word_You_Survive);
	word_you_survive_sprite->SetID(GameMgr::GetInstance()->sceneObjMgr->GetID());
	word_you_survive_sprite->SetPos(win_img_pos, true);
	word_you_survive_sprite->SetScale(vector3s(MENU_IMG_SCALE, MENU_IMG_SCALE, 1.0f));

	DrawData* word_you_survive_drawData = RenderMgr::GetNewDrawData(nullptr, true, E_BatchingType::BatchingType_UI, false);
	word_you_survive_drawData->SetSize(vector2s(WORD_YOU_SURVIVE_TEX_SIZE_X, WORD_YOU_SURVIVE_TEX_SIZE_Y));
	tempWord_shaderParam->usePreComputeUV = true;
	tempWord_shaderParam->preComputeUV = vector2s(WORD_YOU_SURVIVE_TEX_START_X, WORD_YOU_SURVIVE_TEX_START_Y);
	tempWord_shaderParam->preComputeUVSize = vector2s(WORD_YOU_SURVIVE_TEX_SIZE_X, WORD_YOU_SURVIVE_TEX_SIZE_Y);
	tempWord_shaderParam->tex_texel_size = ui_tex_size;

	word_you_survive_drawData->SetShaderParam(tempWord_shaderParam);

	word_you_survive_sprite->SetDrawData(word_you_survive_drawData);

	word_you_survive_sprite->ShowUI(false, -1, true);

	GameMgr::GetInstance()->sceneObjMgr->RegisterCell(word_you_survive_sprite);

	//thank
	word_thank_sprite = new UISprite();
	word_thank_sprite->SetUIContentType(E_UIContent::UIContent_Word_Thank);
	word_thank_sprite->SetID(GameMgr::GetInstance()->sceneObjMgr->GetID());
	word_thank_sprite->SetPos(thanks_img_pos, true);
	word_thank_sprite->SetScale(vector3s(MENU_IMG_SCALE, MENU_IMG_SCALE, 1.0f));

	DrawData* word_thank_drawData = RenderMgr::GetNewDrawData(nullptr, true, E_BatchingType::BatchingType_UI, false);
	word_thank_drawData->SetSize(vector2s(WORD_THANK_TEX_SIZE_X, WORD_THANK_TEX_SIZE_Y));
	tempWord_shaderParam->usePreComputeUV = true;
	tempWord_shaderParam->preComputeUV = vector2s(WORD_THANK_TEX_START_X, WORD_THANK_TEX_START_Y);
	tempWord_shaderParam->preComputeUVSize = vector2s(WORD_THANK_TEX_SIZE_X, WORD_THANK_TEX_SIZE_Y);
	tempWord_shaderParam->tex_texel_size = ui_tex_size;

	word_thank_drawData->SetShaderParam(tempWord_shaderParam);

	word_thank_sprite->SetDrawData(word_thank_drawData);

	word_thank_sprite->ShowUI(false, -1, true);

	GameMgr::GetInstance()->sceneObjMgr->RegisterCell(word_thank_sprite);


	//retry
	word_retry_sprite = new UISprite();
	word_retry_sprite->SetUIContentType(E_UIContent::UIContent_Word_Retry);
	word_retry_sprite->SetID(GameMgr::GetInstance()->sceneObjMgr->GetID());
	word_retry_sprite->SetPos(menu_pause_retry_pos, true);
	word_retry_sprite->SetScale(vector3s(TITLE_ITEM_SCALE, TITLE_ITEM_SCALE, 1.0f));

	DrawData* word_retry_drawData = RenderMgr::GetNewDrawData(nullptr, true, E_BatchingType::BatchingType_UI, false);
	word_retry_drawData->SetSize(vector2s(WORD_RETRY_TEX_SIZE_X, WORD_RETRY_TEX_SIZE_Y));
	tempWord_shaderParam->usePreComputeUV = true;
	tempWord_shaderParam->preComputeUV = vector2s(WORD_RETRY_TEX_START_X, WORD_RETRY_TEX_START_Y);
	tempWord_shaderParam->preComputeUVSize = vector2s(WORD_RETRY_TEX_SIZE_X, WORD_RETRY_TEX_SIZE_Y);
	tempWord_shaderParam->tex_texel_size = ui_tex_size;

	word_retry_drawData->SetShaderParam(tempWord_shaderParam);

	word_retry_sprite->SetDrawData(word_retry_drawData);

	word_retry_sprite->ShowUI(false, -1, true);

	GameMgr::GetInstance()->sceneObjMgr->RegisterCell(word_retry_sprite);


	//continue
	word_continue_sprite = new UISprite();
	word_continue_sprite->SetUIContentType(E_UIContent::UIContent_Word_Continue);
	word_continue_sprite->SetID(GameMgr::GetInstance()->sceneObjMgr->GetID());
	word_continue_sprite->SetPos(menu_pause_continue_pos, true);
	word_continue_sprite->SetScale(vector3s(TITLE_ITEM_SCALE, TITLE_ITEM_SCALE, 1.0f));

	DrawData* word_continue_drawData = RenderMgr::GetNewDrawData(nullptr, true, E_BatchingType::BatchingType_UI, false);
	word_continue_drawData->SetSize(vector2s(WORD_CONTINUE_TEX_SIZE_X, WORD_CONTINUE_TEX_SIZE_Y));
	tempWord_shaderParam->usePreComputeUV = true;
	tempWord_shaderParam->preComputeUV = vector2s(WORD_CONTINUE_TEX_START_X, WORD_CONTINUE_TEX_START_Y);
	tempWord_shaderParam->preComputeUVSize = vector2s(WORD_CONTINUE_TEX_SIZE_X, WORD_CONTINUE_TEX_SIZE_Y);
	tempWord_shaderParam->tex_texel_size = ui_tex_size;

	word_continue_drawData->SetShaderParam(tempWord_shaderParam);

	word_continue_sprite->SetDrawData(word_continue_drawData);

	word_continue_sprite->ShowUI(false, -1, true);

	GameMgr::GetInstance()->sceneObjMgr->RegisterCell(word_continue_sprite);

	//start
	word_start_sprite = new UISprite();
	word_start_sprite->SetUIContentType(E_UIContent::UIContent_Word_Start);
	word_start_sprite->SetID(GameMgr::GetInstance()->sceneObjMgr->GetID());
	word_start_sprite->SetPos(title_item_start_default_pos, true);
	word_start_sprite->SetScale(vector3s(TITLE_ITEM_SCALE, TITLE_ITEM_SCALE, 1.0f));

	DrawData* word_start_drawData = RenderMgr::GetNewDrawData(nullptr, true, E_BatchingType::BatchingType_UI, false);
	word_start_drawData->SetSize(vector2s(WORD_START_TEX_SIZE_X, WORD_START_TEX_SIZE_Y));
	tempWord_shaderParam->usePreComputeUV = true;
	tempWord_shaderParam->preComputeUV = vector2s(WORD_START_TEX_START_X, WORD_START_TEX_START_Y);
	tempWord_shaderParam->preComputeUVSize = vector2s(WORD_START_TEX_SIZE_X, WORD_START_TEX_SIZE_Y);
	tempWord_shaderParam->tex_texel_size = ui_tex_size;

	word_start_drawData->SetShaderParam(tempWord_shaderParam);

	word_start_sprite->SetDrawData(word_start_drawData);

	word_start_sprite->ShowUI(false, -1, true);

	GameMgr::GetInstance()->sceneObjMgr->RegisterCell(word_start_sprite);

	//exit
	word_exit_sprite = new UISprite();
	word_exit_sprite->SetUIContentType(E_UIContent::UIContent_Word_Exit);
	word_exit_sprite->SetID(GameMgr::GetInstance()->sceneObjMgr->GetID());
	word_exit_sprite->SetPos(title_item_exit_default_pos, true);
	word_exit_sprite->SetScale(vector3s(TITLE_ITEM_SCALE, TITLE_ITEM_SCALE, 1.0f));

	DrawData* word_exit_drawData = RenderMgr::GetNewDrawData(nullptr, true, E_BatchingType::BatchingType_UI, false);
	word_exit_drawData->SetSize(vector2s(WORD_EXIT_TEX_SIZE_X, WORD_EXIT_TEX_SIZE_Y));
	tempWord_shaderParam->usePreComputeUV = true;
	tempWord_shaderParam->preComputeUV = vector2s(WORD_EXIT_TEX_START_X, WORD_EXIT_TEX_START_Y);
	tempWord_shaderParam->preComputeUVSize = vector2s(WORD_EXIT_TEX_SIZE_X, WORD_EXIT_TEX_SIZE_Y);
	tempWord_shaderParam->tex_texel_size = ui_tex_size;

	word_exit_drawData->SetShaderParam(tempWord_shaderParam);

	word_exit_sprite->SetDrawData(word_exit_drawData);

	word_exit_sprite->ShowUI(false, -1, true);

	GameMgr::GetInstance()->sceneObjMgr->RegisterCell(word_exit_sprite);

	//title
	word_title_sprite = new UISprite();
	word_title_sprite->SetUIContentType(E_UIContent::UIContent_Word_Title);
	word_title_sprite->SetID(GameMgr::GetInstance()->sceneObjMgr->GetID());
	word_title_sprite->SetPos(vector3s(0, 0, -11), true);
	word_title_sprite->SetScale(vector3s(TITLE_ITEM_SCALE, TITLE_ITEM_SCALE, 1.0f));

	DrawData* word_title_drawData = RenderMgr::GetNewDrawData(nullptr, true, E_BatchingType::BatchingType_UI, false);
	word_title_drawData->SetSize(vector2s(WORD_TITLE_TEX_SIZE_X, WORD_TITLE_TEX_SIZE_Y));
	tempWord_shaderParam->usePreComputeUV = true;
	tempWord_shaderParam->preComputeUV = vector2s(WORD_TITLE_TEX_START_X, WORD_TITLE_TEX_START_Y);
	tempWord_shaderParam->preComputeUVSize = vector2s(WORD_TITLE_TEX_SIZE_X, WORD_TITLE_TEX_SIZE_Y);
	tempWord_shaderParam->tex_texel_size = ui_tex_size;

	word_title_drawData->SetShaderParam(tempWord_shaderParam);

	word_title_sprite->SetDrawData(word_title_drawData);

	word_title_sprite->ShowUI(false, -1, true);

	GameMgr::GetInstance()->sceneObjMgr->RegisterCell(word_title_sprite);

#pragma endregion word_sprite

#pragma region card_1

	//create const ui
	card_1 = new UIGroup();
	card_1->SetUIContentType(E_UIContent::UIContent_Card);
	card_1->deleteChild = false;
	card_1->SetID(GameMgr::GetInstance()->sceneObjMgr->GetID());
	card_1->SetPos(vector3s(-600.0f, 150.0f, -11.0f), true);
	//card_1->SetRot(vector3s(0.0f, 0.0f, 3.0f));
	card_1->SetScale(vector3s(1.75f, 1.75f, 1.0f));

	card_1->ShowUI(false, -1, true);

#pragma endregion card_1

#pragma region card_2

	//create const ui
	card_2 = new UIGroup();
	card_2->SetUIContentType(E_UIContent::UIContent_Card);
	card_2->deleteChild = false;
	card_2->SetID(GameMgr::GetInstance()->sceneObjMgr->GetID());
	card_2->SetPos(vector3s(-200.0f, 150.0f, -11.0f), true);
	//card_2->SetRot(vector3s(0.0f, 0.0f, -3.0f));
	card_2->SetScale(vector3s(1.75f, 1.75f, 1.0f));


	card_2->ShowUI(false, -1, true);

#pragma endregion card_2

#pragma region card_3

	//create const ui
	card_3 = new UIGroup();
	card_3->SetUIContentType(E_UIContent::UIContent_Card);
	card_3->deleteChild = false;
	card_3->SetID(GameMgr::GetInstance()->sceneObjMgr->GetID());
	card_3->SetPos(vector3s(200.0f, 150.0f, -11.0f), true);
	//card_3->SetRot(vector3s(0.0f, 0.0f, -3.0f));
	card_3->SetScale(vector3s(1.75f, 1.75f, 1.0f));


	card_3->ShowUI(false, -1, true);

#pragma endregion card_3

#pragma region card_4

	//create const ui
	card_4 = new UIGroup();
	card_4->SetUIContentType(E_UIContent::UIContent_Card);
	card_4->deleteChild = false;
	card_4->SetID(GameMgr::GetInstance()->sceneObjMgr->GetID());
	card_4->SetPos(vector3s(600.0f, 150.0f, -11.0f), true);
	//card_4->SetRot(vector3s(0.0f, 0.0f, -3.0f));
	card_4->SetScale(vector3s(1.75f, 1.75f, 1.0f));


	card_4->ShowUI(false, -1, true);

#pragma endregion card_4

	GameMgr::GetInstance()->sceneObjMgr->RegisterCell(card_1);
	GameMgr::GetInstance()->sceneObjMgr->RegisterCell(card_2);
	GameMgr::GetInstance()->sceneObjMgr->RegisterCell(card_3);
	GameMgr::GetInstance()->sceneObjMgr->RegisterCell(card_4);

#pragma region lv_up_state

	lv_panel = new UIGroup();
	lv_panel->deleteChild = true;
	lv_panel->SetID(GameMgr::GetInstance()->sceneObjMgr->GetID());
	lv_panel->SetPos(lv_up_defaultPos, true);
	lv_panel->SetScale(vector3s(2.0f, 2.0f, 1.0f));
	lv_panel->RecordCurState();
	//movement_axis
	Movement_Axis* lv_panel_movement = new Movement_Axis(lv_panel, false, true, false, false, 0.15f, lv_up_defaultPos, lv_up_defaultPos, lv_up_targetPos);
	lv_panel->AddMovement(lv_panel_movement);

	ShaderParam_Normal* lv_panel_status_shaderParam = new ShaderParam_Normal();
	lv_panel_status_shaderParam->shaderProgram = GameMgr::GetInstance()->shaderMgr->GetShaderProgram(E_Shader::Shader_Normal);
	lv_panel_status_shaderParam->texNo = GameMgr::GetInstance()->textureMgr->GetTextureID(E_Texture::Texture_UI_HUD_Package_Test_2);
	lv_panel_status_shaderParam->usePreComputeUV = true;
	lv_panel_status_shaderParam->preComputeUV = vector2s(LV_PANEL_STATUS_IMG_TEX_START_X, LV_PANEL_STATUS_IMG_TEX_START_Y);
	lv_panel_status_shaderParam->preComputeUVSize = vector2s(LV_PANEL_STATUS_IMG_TEX_SIZE_X, LV_PANEL_STATUS_IMG_TEX_SIZE_Y);
	lv_panel_status_shaderParam->tex_texel_size = ui_tex_size;

	DrawData* lv_panel_status_drawData = RenderMgr::GetNewDrawData(nullptr, true, E_BatchingType::BatchingType_UI, false);
	lv_panel_status_drawData->SetSize(vector2s(LV_PANEL_STATUS_IMG_TEX_SIZE_X, LV_PANEL_STATUS_IMG_TEX_SIZE_Y));
	lv_panel_status_drawData->SetShaderParam(lv_panel_status_shaderParam);

	//status img
	UISprite* lv_panel_status_sprite_cell = new UISprite();
	lv_panel_status_sprite_cell->SetDrawData(lv_panel_status_drawData);

	lv_panel->RegisterChild(UI_LV_PANEL_STATUS_IMG, lv_panel_status_sprite_cell, vector3s(0, 0, 0));
	lv_panel_status_sprite_cell->ShowUI(true, -1, true);

	vector2s offset_0 = lv_panel_status_sprite_cell->GetSize(true);

	//lv pre
	UIWord* lv_pre_word = new UIWord();
	lv_pre_word->SetScale(vector3s(1.8f, 1.8f, 1.0f));
	lv_pre_word->SetWordInterval(DEFAULT_WORD_INTERVAL);
	lv_pre_word->SetAlignment(E_UIWordAlignment::UIWordAlignment_Right);

	UISprite* childArray = lv_pre_word->GetSpriteArray();
	if (childArray != nullptr) {
		for (int index = 0; index < UI_WORD_CHILD_COUNT; index++) {
			DrawData* childDrawData = RenderMgr::GetNewDrawData(nullptr, true, E_BatchingType::BatchingType_UI, false);
			if (childDrawData == nullptr)continue;//error
			childDrawData->SetShaderParam(num_sprite_shaderParam);
			childDrawData->SetSize(vector2s(WORD_SIZE_X, WORD_SIZE_Y));
			childArray[index].SetDrawData(childDrawData);
			//childArray[index].SetScale(vector3s(2.0f, 2.0f, 1.0f));
		}
	}

	lv_pre_word->SetWord("010");

	lv_panel->RegisterChild(UI_LV_PANEL_STATUS_LV_PRE, lv_pre_word, vector3s(70.0f, offset_0.y * 0.5f - 16, 0.0f));
	lv_pre_word->ShowUI(true, -1, true);

	//lv cur
	UIWord* lv_target_word = new UIWord();
	lv_target_word->SetScale(vector3s(1.8f, 1.8f, 1.0f));
	lv_target_word->SetWordInterval(DEFAULT_WORD_INTERVAL);
	lv_target_word->SetAlignment(E_UIWordAlignment::UIWordAlignment_Left);

	childArray = lv_target_word->GetSpriteArray();
	if (childArray != nullptr) {
		for (int index = 0; index < UI_WORD_CHILD_COUNT; index++) {
			DrawData* childDrawData = RenderMgr::GetNewDrawData(nullptr, true, E_BatchingType::BatchingType_UI, false);
			if (childDrawData == nullptr)continue;//error
			childDrawData->SetShaderParam(num_sprite_shaderParam);
			childDrawData->SetSize(vector2s(WORD_SIZE_X, WORD_SIZE_Y));
			childArray[index].SetDrawData(childDrawData);
			//childArray[index].SetScale(vector3s(2.0f, 2.0f, 1.0f));
		}
	}

	lv_target_word->SetWord("010");

	lv_panel->RegisterChild(UI_LV_PANEL_STATUS_LV_CUR, lv_target_word, vector3s(180.0f, offset_0.y * 0.5f - 16, 0.0f));
	lv_target_word->ShowUI(true, -1, true);

	//Hp pre
	UIWord* hp_pre_word = new UIWord();
	hp_pre_word->SetScale(vector3s(1.8f, 1.8f, 1.0f));
	hp_pre_word->SetWordInterval(DEFAULT_WORD_INTERVAL);
	hp_pre_word->SetAlignment(E_UIWordAlignment::UIWordAlignment_Right);

	childArray = hp_pre_word->GetSpriteArray();
	if (childArray != nullptr) {
		for (int index = 0; index < UI_WORD_CHILD_COUNT; index++) {
			DrawData* childDrawData = RenderMgr::GetNewDrawData(nullptr, true, E_BatchingType::BatchingType_UI, false);
			if (childDrawData == nullptr)continue;//error
			childDrawData->SetShaderParam(num_sprite_shaderParam);
			childDrawData->SetSize(vector2s(WORD_SIZE_X, WORD_SIZE_Y));
			childArray[index].SetDrawData(childDrawData);
			//childArray[index].SetScale(vector3s(2.0f, 2.0f, 1.0f));
		}
	}

	hp_pre_word->SetWord("010");

	lv_panel->RegisterChild(UI_LV_PANEL_STATUS_HP_PRE, hp_pre_word, vector3s(70.0f, offset_0.y * 0.5f - 16 - 32 * 1, 0.0f));
	hp_pre_word->ShowUI(true, -1, true);

	//hp cur
	UIWord* hp_target_word = new UIWord();
	hp_target_word->SetScale(vector3s(1.8f, 1.8f, 1.0f));
	hp_target_word->SetWordInterval(DEFAULT_WORD_INTERVAL);
	hp_target_word->SetAlignment(E_UIWordAlignment::UIWordAlignment_Left);

	childArray = hp_target_word->GetSpriteArray();
	if (childArray != nullptr) {
		for (int index = 0; index < UI_WORD_CHILD_COUNT; index++) {
			DrawData* childDrawData = RenderMgr::GetNewDrawData(nullptr, true, E_BatchingType::BatchingType_UI, false);
			if (childDrawData == nullptr)continue;//error
			childDrawData->SetShaderParam(num_sprite_shaderParam);
			childDrawData->SetSize(vector2s(WORD_SIZE_X, WORD_SIZE_Y));
			childArray[index].SetDrawData(childDrawData);
			//childArray[index].SetScale(vector3s(2.0f, 2.0f, 1.0f));
		}
	}

	hp_target_word->SetWord("100");

	lv_panel->RegisterChild(UI_LV_PANEL_STATUS_HP_CUR, hp_target_word, vector3s(180.0f, offset_0.y * 0.5f - 16 - 32 * 1, 0.0f));
	hp_target_word->ShowUI(true, -1, true);


	//atk pre
	UIWord* atk_pre_word = new UIWord();
	atk_pre_word->SetScale(vector3s(1.8f, 1.8f, 1.0f));
	atk_pre_word->SetWordInterval(DEFAULT_WORD_INTERVAL);
	atk_pre_word->SetAlignment(E_UIWordAlignment::UIWordAlignment_Right);

	childArray = atk_pre_word->GetSpriteArray();
	if (childArray != nullptr) {
		for (int index = 0; index < UI_WORD_CHILD_COUNT; index++) {
			DrawData* childDrawData = RenderMgr::GetNewDrawData(nullptr, true, E_BatchingType::BatchingType_UI, false);
			if (childDrawData == nullptr)continue;//error
			childDrawData->SetShaderParam(num_sprite_shaderParam);
			childDrawData->SetSize(vector2s(WORD_SIZE_X, WORD_SIZE_Y));
			childArray[index].SetDrawData(childDrawData);
			//childArray[index].SetScale(vector3s(2.0f, 2.0f, 1.0f));
		}
	}

	atk_pre_word->SetWord("99");

	lv_panel->RegisterChild(UI_LV_PANEL_STATUS_ATK_PRE, atk_pre_word, vector3s(70.0f, offset_0.y * 0.5f - 16 - 32 * 2, 0.0f));
	atk_pre_word->ShowUI(true, -1, true);

	//atk cur
	UIWord* atk_target_word = new UIWord();
	atk_target_word->SetScale(vector3s(1.8f, 1.8f, 1.0f));
	atk_target_word->SetWordInterval(DEFAULT_WORD_INTERVAL);
	atk_target_word->SetAlignment(E_UIWordAlignment::UIWordAlignment_Left);

	childArray = atk_target_word->GetSpriteArray();
	if (childArray != nullptr) {
		for (int index = 0; index < UI_WORD_CHILD_COUNT; index++) {
			DrawData* childDrawData = RenderMgr::GetNewDrawData(nullptr, true, E_BatchingType::BatchingType_UI, false);
			if (childDrawData == nullptr)continue;//error
			childDrawData->SetShaderParam(num_sprite_shaderParam);
			childDrawData->SetSize(vector2s(WORD_SIZE_X, WORD_SIZE_Y));
			childArray[index].SetDrawData(childDrawData);
			//childArray[index].SetScale(vector3s(2.0f, 2.0f, 1.0f));
		}
	}

	atk_target_word->SetWord("101");

	lv_panel->RegisterChild(UI_LV_PANEL_STATUS_ATK_CUR, atk_target_word, vector3s(180.0f, offset_0.y * 0.5f - 16 - 32 * 2, 0.0f));
	atk_target_word->ShowUI(true, -1, true);

	//def pre
	UIWord* def_pre_word = new UIWord();
	def_pre_word->SetScale(vector3s(1.8f, 1.8f, 1.0f));
	def_pre_word->SetWordInterval(DEFAULT_WORD_INTERVAL);
	def_pre_word->SetAlignment(E_UIWordAlignment::UIWordAlignment_Right);

	childArray = def_pre_word->GetSpriteArray();
	if (childArray != nullptr) {
		for (int index = 0; index < UI_WORD_CHILD_COUNT; index++) {
			DrawData* childDrawData = RenderMgr::GetNewDrawData(nullptr, true, E_BatchingType::BatchingType_UI, false);
			if (childDrawData == nullptr)continue;//error
			childDrawData->SetShaderParam(num_sprite_shaderParam);
			childDrawData->SetSize(vector2s(WORD_SIZE_X, WORD_SIZE_Y));
			childArray[index].SetDrawData(childDrawData);
			//childArray[index].SetScale(vector3s(2.0f, 2.0f, 1.0f));
		}
	}

	def_pre_word->SetWord("51");

	lv_panel->RegisterChild(UI_LV_PANEL_STATUS_DEF_PRE, def_pre_word, vector3s(70.0f, offset_0.y * 0.5f - 16 - 32 * 3, 0.0f));
	def_pre_word->ShowUI(true, -1, true);

	//def cur
	UIWord* def_target_word = new UIWord();
	def_target_word->SetScale(vector3s(1.8f, 1.8f, 1.0f));
	def_target_word->SetWordInterval(DEFAULT_WORD_INTERVAL);
	def_target_word->SetAlignment(E_UIWordAlignment::UIWordAlignment_Left);

	childArray = def_target_word->GetSpriteArray();
	if (childArray != nullptr) {
		for (int index = 0; index < UI_WORD_CHILD_COUNT; index++) {
			DrawData* childDrawData = RenderMgr::GetNewDrawData(nullptr, true, E_BatchingType::BatchingType_UI, false);
			if (childDrawData == nullptr)continue;//error
			childDrawData->SetShaderParam(num_sprite_shaderParam);
			childDrawData->SetSize(vector2s(WORD_SIZE_X, WORD_SIZE_Y));
			childArray[index].SetDrawData(childDrawData);
			//childArray[index].SetScale(vector3s(2.0f, 2.0f, 1.0f));
		}
	}

	def_target_word->SetWord("42");

	lv_panel->RegisterChild(UI_LV_PANEL_STATUS_DEF_CUR, def_target_word, vector3s(180.0f, offset_0.y * 0.5f - 16 - 32 * 3, 0.0f));
	def_target_word->ShowUI(true, -1, true);


	//arrow
	ShaderParam_Normal* arrow_shaderParam = new ShaderParam_Normal();
	arrow_shaderParam->shaderProgram = GameMgr::GetInstance()->shaderMgr->GetShaderProgram(E_Shader::Shader_Normal);
	arrow_shaderParam->texNo = GameMgr::GetInstance()->textureMgr->GetTextureID(E_Texture::Texture_UI_HUD_Package_Test_2);
	arrow_shaderParam->usePreComputeUV = true;
	arrow_shaderParam->preComputeUV = vector2s(SIGN_ARROW_WHITE_TEX_START_X, SIGN_ARROW_WHITE_TEX_START_Y);
	arrow_shaderParam->preComputeUVSize = vector2s(SIGN_ARROW_WHITE_TEX_SIZE_X, SIGN_ARROW_WHITE_TEX_SIZE_Y);
	arrow_shaderParam->tex_texel_size = ui_tex_size;

	//arrow_1
	UISprite* arrow_1 = new UISprite();
	arrow_1->SetUIContentType(E_UIContent::UIContent_Sign);
	arrow_1->SetID(GameMgr::GetInstance()->sceneObjMgr->GetID());
	arrow_1->SetScale(vector3s(0.7f, 0.7f, 1.0f));

	DrawData* arrow_1_drawData = RenderMgr::GetNewDrawData(nullptr, true, E_BatchingType::BatchingType_UI, false);
	arrow_1_drawData->SetSize(vector2s(SIGN_ARROW_WHITE_TEX_SIZE_X, SIGN_ARROW_WHITE_TEX_SIZE_Y));
	arrow_1_drawData->SetShaderParam(arrow_shaderParam);

	arrow_1->SetDrawData(arrow_1_drawData);

	lv_panel->RegisterChild(UI_LV_PANEL_STATUS_ARROW_1, arrow_1, vector3s(140.0f, offset_0.y * 0.5f - 16, 0.0f));
	arrow_1->ShowUI(true, -1, true);


	//arrow_2
	UISprite* arrow_2 = new UISprite();
	arrow_2->SetUIContentType(E_UIContent::UIContent_Sign);
	arrow_2->SetID(GameMgr::GetInstance()->sceneObjMgr->GetID());
	arrow_2->SetScale(vector3s(0.7f, 0.7f, 1.0f));

	DrawData* arrow_2_drawData = RenderMgr::GetNewDrawData(nullptr, true, E_BatchingType::BatchingType_UI, false);
	arrow_2_drawData->SetSize(vector2s(SIGN_ARROW_WHITE_TEX_SIZE_X, SIGN_ARROW_WHITE_TEX_SIZE_Y));
	arrow_2_drawData->SetShaderParam(arrow_shaderParam);

	arrow_2->SetDrawData(arrow_2_drawData);

	lv_panel->RegisterChild(UI_LV_PANEL_STATUS_ARROW_2, arrow_2, vector3s(140.0f, offset_0.y * 0.5f - 16 - 32 * 1, 0.0f));
	arrow_2->ShowUI(true, -1, true);

	//arrow_3
	UISprite* arrow_3 = new UISprite();
	arrow_3->SetUIContentType(E_UIContent::UIContent_Sign);
	arrow_3->SetID(GameMgr::GetInstance()->sceneObjMgr->GetID());
	arrow_3->SetScale(vector3s(0.7f, 0.7f, 1.0f));

	DrawData* arrow_3_drawData = RenderMgr::GetNewDrawData(nullptr, true, E_BatchingType::BatchingType_UI, false);
	arrow_3_drawData->SetSize(vector2s(SIGN_ARROW_WHITE_TEX_SIZE_X, SIGN_ARROW_WHITE_TEX_SIZE_Y));
	arrow_3_drawData->SetShaderParam(arrow_shaderParam);

	arrow_3->SetDrawData(arrow_3_drawData);

	lv_panel->RegisterChild(UI_LV_PANEL_STATUS_ARROW_3, arrow_3, vector3s(140.0f, offset_0.y * 0.5f - 16 - 32 * 2, 0.0f));
	arrow_3->ShowUI(true, -1, true);

	//arrow_4
	UISprite* arrow_4 = new UISprite();
	arrow_4->SetUIContentType(E_UIContent::UIContent_Sign);
	arrow_4->SetID(GameMgr::GetInstance()->sceneObjMgr->GetID());
	arrow_4->SetScale(vector3s(0.7f, 0.7f, 1.0f));

	DrawData* arrow_4_drawData = RenderMgr::GetNewDrawData(nullptr, true, E_BatchingType::BatchingType_UI, false);
	arrow_4_drawData->SetSize(vector2s(SIGN_ARROW_WHITE_TEX_SIZE_X, SIGN_ARROW_WHITE_TEX_SIZE_Y));
	arrow_4_drawData->SetShaderParam(arrow_shaderParam);

	arrow_4->SetDrawData(arrow_4_drawData);

	lv_panel->RegisterChild(UI_LV_PANEL_STATUS_ARROW_4, arrow_4, vector3s(140.0f, offset_0.y * 0.5f - 16 - 32 * 3, 0.0f));
	arrow_4->ShowUI(true, -1, true);

	lv_panel->ShowUI(false, -1, true);

	if (lv_panel_status_shaderParam != nullptr) {
		delete lv_panel_status_shaderParam;
		lv_panel_status_shaderParam = nullptr;
	}

	if (arrow_shaderParam != nullptr) {
		delete arrow_shaderParam;
		arrow_shaderParam = nullptr;
	}

#pragma endregion lv_up_state
	GameMgr::GetInstance()->sceneObjMgr->RegisterCell(lv_panel);

#pragma region hud

	ShaderParam_Normal* hud_frame_shaderParam = new ShaderParam_Normal();
	hud_frame_shaderParam->shaderProgram = GameMgr::GetInstance()->shaderMgr->GetShaderProgram(E_Shader::Shader_Normal);
	hud_frame_shaderParam->texNo = GameMgr::GetInstance()->textureMgr->GetTextureID(E_Texture::Texture_UI_HUD_Package_Test_2);

	hud_frame_shaderParam->usePreComputeUV = true;
	hud_frame_shaderParam->preComputeUV = vector2s(STATUS_IMG_TEX_START_X, STATUS_IMG_TEX_START_Y);
	hud_frame_shaderParam->preComputeUVSize = vector2s(STATUS_IMG_TEX_SIZE_X, STATUS_IMG_TEX_SIZE_Y);
	hud_frame_shaderParam->tex_texel_size = ui_tex_size;


	hud_status_frame = new UIGroup();
	hud_status_frame->deleteChild = true;
	hud_status_frame->SetID(GameMgr::GetInstance()->sceneObjMgr->GetID());
	hud_status_frame->SetPos(hud_default_pos, true);
	hud_status_frame->SetScale(vector3s(2.0f, 2.0f, 1.0f));
	hud_status_frame->RecordCurState();

	DrawData* hud_frame_DrawData = RenderMgr::GetNewDrawData(nullptr, false, E_BatchingType::BatchingType_UI, false);
	hud_frame_DrawData->SetSize(vector2s(STATUS_IMG_TEX_SIZE_X, STATUS_IMG_TEX_SIZE_Y));
	hud_frame_DrawData->SetShaderParam(hud_frame_shaderParam);


	Movement_Axis* hud_AppearMovement = new Movement_Axis(hud_status_frame, false, true, false, false, 0.35f, hud_default_pos, hud_default_pos, hud_target_pos);
	hud_status_frame->AddMovement(hud_AppearMovement);

	//status img
	UISprite* hud_frame_status_sprite_cell = new UISprite();
	hud_frame_status_sprite_cell->SetDrawData(hud_frame_DrawData);

	hud_status_frame->RegisterChild(UI_BG_ID, hud_frame_status_sprite_cell, vector3s(0, 0, 0));

	vector2s offset = hud_frame_status_sprite_cell->GetSize(true);

	if (hud_frame_shaderParam != nullptr) {
		delete hud_frame_shaderParam;
	}

	UIWord* lv_cur_word = new UIWord();
	lv_cur_word->SetScale(vector3s(1.8f, 1.8f, 1.0f));
	lv_cur_word->SetWordInterval(DEFAULT_WORD_INTERVAL);
	lv_cur_word->SetAlignment(E_UIWordAlignment::UIWordAlignment_Center);

	childArray = lv_cur_word->GetSpriteArray();
	if (childArray != nullptr) {
		for (int index = 0; index < UI_WORD_CHILD_COUNT; index++) {
			DrawData* childDrawData = RenderMgr::GetNewDrawData(nullptr, true, E_BatchingType::BatchingType_UI, false);
			if (childDrawData == nullptr)continue;//error
			childDrawData->SetShaderParam(num_sprite_shaderParam);
			childDrawData->SetSize(vector2s(WORD_SIZE_X, WORD_SIZE_Y));
			childArray[index].SetDrawData(childDrawData);
			//childArray[index].SetScale(vector3s(2.0f, 2.0f, 1.0f));
		}
	}

	lv_cur_word->SetWord("010");

	hud_status_frame->RegisterChild(UI_HUD_STATUS_LV_CUR, lv_cur_word, vector3s(HUD_LV_DATA_DEFAULT_OFFSET_X, offset.y * 0.5f - 16, 0.0f));
	lv_cur_word->ShowUI(true, -1, true);


	//Hp consume
	UIWord* hp_consume_word = new UIWord();
	hp_consume_word->SetScale(vector3s(1.0f, 1.0f, 1.0f));
	hp_consume_word->SetWordInterval(DEFAULT_WORD_INTERVAL);
	hp_consume_word->SetAlignment(E_UIWordAlignment::UIWordAlignment_Left);

	childArray = hp_consume_word->GetSpriteArray();
	if (childArray != nullptr) {
		for (int index = 0; index < UI_WORD_CHILD_COUNT; index++) {
			DrawData* childDrawData = RenderMgr::GetNewDrawData(nullptr, true, E_BatchingType::BatchingType_UI, false);
			if (childDrawData == nullptr)continue;//error
			childDrawData->SetShaderParam(num_red_sprite_shaderParam);
			childDrawData->SetSize(vector2s(WORD_SIZE_X, WORD_SIZE_Y));
			childArray[index].SetDrawData(childDrawData);
			//childArray[index].SetScale(vector3s(2.0f, 2.0f, 1.0f));
		}
	}

	hp_consume_word->SetWord("010");

	hud_status_frame->RegisterChild(UI_HUD_STATUS_HP_CONSUME, hp_consume_word, vector3s(HUD_CONSUME_DATA_DEFAULT_OFFSET_X, offset.y * 0.5f - 16 - 32 * 1, 0.0f));
	hp_consume_word->ShowUI(true, -1, true);

	//Hp cur
	UIWord* hp_cur_word = new UIWord();
	hp_cur_word->SetScale(vector3s(1.8f, 1.8f, 1.0f));
	hp_cur_word->SetWordInterval(DEFAULT_WORD_INTERVAL);
	hp_cur_word->SetAlignment(E_UIWordAlignment::UIWordAlignment_Right);

	childArray = hp_cur_word->GetSpriteArray();
	if (childArray != nullptr) {
		for (int index = 0; index < UI_WORD_CHILD_COUNT; index++) {
			DrawData* childDrawData = RenderMgr::GetNewDrawData(nullptr, true, E_BatchingType::BatchingType_UI, false);
			if (childDrawData == nullptr)continue;//error
			childDrawData->SetShaderParam(num_sprite_shaderParam);
			childDrawData->SetSize(vector2s(WORD_SIZE_X, WORD_SIZE_Y));
			childArray[index].SetDrawData(childDrawData);
			//childArray[index].SetScale(vector3s(2.0f, 2.0f, 1.0f));
		}
	}

	hp_cur_word->SetWord("010");

	hud_status_frame->RegisterChild(UI_HUD_STATUS_CUR_HP, hp_cur_word, vector3s(HUD_CUR_DATA_DEFAULT_OFFSET_X, offset.y * 0.5f - 16 - 32 * 1, 0.0f));
	hp_cur_word->ShowUI(true, -1, true);

	//hp max
	UIWord* hp_max_word = new UIWord();
	hp_max_word->SetScale(vector3s(1.8f, 1.8f, 1.0f));
	hp_max_word->SetWordInterval(DEFAULT_WORD_INTERVAL);
	hp_max_word->SetAlignment(E_UIWordAlignment::UIWordAlignment_Left);

	childArray = hp_max_word->GetSpriteArray();
	if (childArray != nullptr) {
		for (int index = 0; index < UI_WORD_CHILD_COUNT; index++) {
			DrawData* childDrawData = RenderMgr::GetNewDrawData(nullptr, true, E_BatchingType::BatchingType_UI, false);
			if (childDrawData == nullptr)continue;//error
			childDrawData->SetShaderParam(num_sprite_shaderParam);
			childDrawData->SetSize(vector2s(WORD_SIZE_X, WORD_SIZE_Y));
			childArray[index].SetDrawData(childDrawData);
			//childArray[index].SetScale(vector3s(2.0f, 2.0f, 1.0f));
		}
	}

	hp_max_word->SetWord("100");

	hud_status_frame->RegisterChild(UI_HUD_STATUS_MAX_HP, hp_max_word, vector3s(HUD_MAX_DATA_DEFAULT_OFFSET_X, offset.y * 0.5f - 16 - 32 * 1, 0.0f));
	hp_max_word->ShowUI(true, -1, true);

	//atk consume
	UIWord* atk_consume_word = new UIWord();
	atk_consume_word->SetScale(vector3s(1.0f, 1.0f, 1.0f));
	atk_consume_word->SetWordInterval(DEFAULT_WORD_INTERVAL);
	atk_consume_word->SetAlignment(E_UIWordAlignment::UIWordAlignment_Left);

	childArray = atk_consume_word->GetSpriteArray();
	if (childArray != nullptr) {
		for (int index = 0; index < UI_WORD_CHILD_COUNT; index++) {
			DrawData* childDrawData = RenderMgr::GetNewDrawData(nullptr, true, E_BatchingType::BatchingType_UI, false);
			if (childDrawData == nullptr)continue;//error
			childDrawData->SetShaderParam(num_red_sprite_shaderParam);
			childDrawData->SetSize(vector2s(WORD_SIZE_X, WORD_SIZE_Y));
			childArray[index].SetDrawData(childDrawData);
			//childArray[index].SetScale(vector3s(2.0f, 2.0f, 1.0f));
		}
	}

	atk_consume_word->SetWord("010");

	hud_status_frame->RegisterChild(UI_HUD_STATUS_ATK_CONSUME, atk_consume_word, vector3s(HUD_CONSUME_DATA_DEFAULT_OFFSET_X, offset.y * 0.5f - 16 - 32 * 2, 0.0f));
	atk_consume_word->ShowUI(true, -1, true);

	//atk cur
	UIWord* atk_cur_word = new UIWord();
	atk_cur_word->SetScale(vector3s(1.8f, 1.8f, 1.0f));
	atk_cur_word->SetWordInterval(DEFAULT_WORD_INTERVAL);
	atk_cur_word->SetAlignment(E_UIWordAlignment::UIWordAlignment_Right);

	childArray = atk_cur_word->GetSpriteArray();
	if (childArray != nullptr) {
		for (int index = 0; index < UI_WORD_CHILD_COUNT; index++) {
			DrawData* childDrawData = RenderMgr::GetNewDrawData(nullptr, true, E_BatchingType::BatchingType_UI, false);
			if (childDrawData == nullptr)continue;//error
			childDrawData->SetShaderParam(num_sprite_shaderParam);
			childDrawData->SetSize(vector2s(WORD_SIZE_X, WORD_SIZE_Y));
			childArray[index].SetDrawData(childDrawData);
			//childArray[index].SetScale(vector3s(2.0f, 2.0f, 1.0f));
		}
	}

	atk_cur_word->SetWord("99");

	hud_status_frame->RegisterChild(UI_HUD_STATUS_CUR_ATK, atk_cur_word, vector3s(HUD_CUR_DATA_DEFAULT_OFFSET_X, offset.y * 0.5f - 16 - 32 * 2, 0.0f));
	atk_cur_word->ShowUI(true, -1, true);

	//atk max
	UIWord* atk_max_word = new UIWord();
	atk_max_word->SetScale(vector3s(1.8f, 1.8f, 1.0f));
	atk_max_word->SetWordInterval(DEFAULT_WORD_INTERVAL);
	atk_max_word->SetAlignment(E_UIWordAlignment::UIWordAlignment_Left);

	childArray = atk_max_word->GetSpriteArray();
	if (childArray != nullptr) {
		for (int index = 0; index < UI_WORD_CHILD_COUNT; index++) {
			DrawData* childDrawData = RenderMgr::GetNewDrawData(nullptr, true, E_BatchingType::BatchingType_UI, false);
			if (childDrawData == nullptr)continue;//error
			childDrawData->SetShaderParam(num_sprite_shaderParam);
			childDrawData->SetSize(vector2s(WORD_SIZE_X, WORD_SIZE_Y));
			childArray[index].SetDrawData(childDrawData);
			//childArray[index].SetScale(vector3s(2.0f, 2.0f, 1.0f));
		}
	}

	atk_max_word->SetWord("101");

	hud_status_frame->RegisterChild(UI_HUD_STATUS_MAX_ATK, atk_max_word, vector3s(HUD_MAX_DATA_DEFAULT_OFFSET_X, offset.y * 0.5f - 16 - 32 * 2, 0.0f));
	atk_max_word->ShowUI(true, -1, true);

	//def consume
	UIWord* def_consume_word = new UIWord();
	def_consume_word->SetScale(vector3s(1.0f, 1.0f, 1.0f));
	def_consume_word->SetWordInterval(DEFAULT_WORD_INTERVAL);
	def_consume_word->SetAlignment(E_UIWordAlignment::UIWordAlignment_Left);

	childArray = def_consume_word->GetSpriteArray();
	if (childArray != nullptr) {
		for (int index = 0; index < UI_WORD_CHILD_COUNT; index++) {
			DrawData* childDrawData = RenderMgr::GetNewDrawData(nullptr, true, E_BatchingType::BatchingType_UI, false);
			if (childDrawData == nullptr)continue;//error
			childDrawData->SetShaderParam(num_red_sprite_shaderParam);
			childDrawData->SetSize(vector2s(WORD_SIZE_X, WORD_SIZE_Y));
			childArray[index].SetDrawData(childDrawData);
			//childArray[index].SetScale(vector3s(2.0f, 2.0f, 1.0f));
		}
	}

	def_consume_word->SetWord("010");

	hud_status_frame->RegisterChild(UI_HUD_STATUS_DEF_CONSUME, def_consume_word, vector3s(HUD_CONSUME_DATA_DEFAULT_OFFSET_X, offset.y * 0.5f - 16 - 32 * 3, 0.0f));
	def_consume_word->ShowUI(true, -1, true);

	//def cur
	UIWord* def_cur_word = new UIWord();
	def_cur_word->SetScale(vector3s(1.8f, 1.8f, 1.0f));
	def_cur_word->SetWordInterval(DEFAULT_WORD_INTERVAL);
	def_cur_word->SetAlignment(E_UIWordAlignment::UIWordAlignment_Right);

	childArray = def_cur_word->GetSpriteArray();
	if (childArray != nullptr) {
		for (int index = 0; index < UI_WORD_CHILD_COUNT; index++) {
			DrawData* childDrawData = RenderMgr::GetNewDrawData(nullptr, true, E_BatchingType::BatchingType_UI, false);
			if (childDrawData == nullptr)continue;//error
			childDrawData->SetShaderParam(num_sprite_shaderParam);
			childDrawData->SetSize(vector2s(WORD_SIZE_X, WORD_SIZE_Y));
			childArray[index].SetDrawData(childDrawData);
			//childArray[index].SetScale(vector3s(2.0f, 2.0f, 1.0f));
		}
	}

	def_cur_word->SetWord("51");

	hud_status_frame->RegisterChild(UI_HUD_STATUS_CUR_DEF, def_cur_word, vector3s(HUD_CUR_DATA_DEFAULT_OFFSET_X, offset.y * 0.5f - 16 - 32 * 3, 0.0f));
	def_cur_word->ShowUI(true, -1, true);

	//def max
	UIWord* def_max_word = new UIWord();
	def_max_word->SetScale(vector3s(1.8f, 1.8f, 1.0f));
	def_max_word->SetWordInterval(DEFAULT_WORD_INTERVAL);
	def_max_word->SetAlignment(E_UIWordAlignment::UIWordAlignment_Left);

	childArray = def_max_word->GetSpriteArray();
	if (childArray != nullptr) {
		for (int index = 0; index < UI_WORD_CHILD_COUNT; index++) {
			DrawData* childDrawData = RenderMgr::GetNewDrawData(nullptr, true, E_BatchingType::BatchingType_UI, false);
			if (childDrawData == nullptr)continue;//error
			childDrawData->SetShaderParam(num_sprite_shaderParam);
			childDrawData->SetSize(vector2s(WORD_SIZE_X, WORD_SIZE_Y));
			childArray[index].SetDrawData(childDrawData);
			//childArray[index].SetScale(vector3s(2.0f, 2.0f, 1.0f));
		}
	}

	def_max_word->SetWord("42");

	hud_status_frame->RegisterChild(UI_HUD_STATUS_MAX_DEF, def_max_word, vector3s(HUD_MAX_DATA_DEFAULT_OFFSET_X, offset.y * 0.5f - 16 - 32 * 3, 0.0f));
	def_max_word->ShowUI(true, -1, true);

	//exp consume
	UIWord* exp_consume_word = new UIWord();
	exp_consume_word->SetScale(vector3s(1.0f, 1.0f, 1.0f));
	exp_consume_word->SetWordInterval(DEFAULT_WORD_INTERVAL);
	exp_consume_word->SetAlignment(E_UIWordAlignment::UIWordAlignment_Left);

	childArray = exp_consume_word->GetSpriteArray();
	if (childArray != nullptr) {
		for (int index = 0; index < UI_WORD_CHILD_COUNT; index++) {
			DrawData* childDrawData = RenderMgr::GetNewDrawData(nullptr, true, E_BatchingType::BatchingType_UI, false);
			if (childDrawData == nullptr)continue;//error
			childDrawData->SetShaderParam(num_red_sprite_shaderParam);
			childDrawData->SetSize(vector2s(WORD_SIZE_X, WORD_SIZE_Y));
			childArray[index].SetDrawData(childDrawData);
			//childArray[index].SetScale(vector3s(2.0f, 2.0f, 1.0f));
		}
	}

	exp_consume_word->SetWord("010");

	hud_status_frame->RegisterChild(UI_HUD_STATUS_EXP_CONSUME, exp_consume_word, vector3s(HUD_CONSUME_DATA_DEFAULT_OFFSET_X, offset.y * 0.5f - 16 - 32 * 4, 0.0f));
	exp_consume_word->ShowUI(true, -1, true);

	//exp cur
	UIWord* exp_cur_word = new UIWord();
	exp_cur_word->SetScale(vector3s(1.8f, 1.8f, 1.0f));
	exp_cur_word->SetWordInterval(DEFAULT_WORD_INTERVAL);
	exp_cur_word->SetAlignment(E_UIWordAlignment::UIWordAlignment_Right);

	childArray = exp_cur_word->GetSpriteArray();
	if (childArray != nullptr) {
		for (int index = 0; index < UI_WORD_CHILD_COUNT; index++) {
			DrawData* childDrawData = RenderMgr::GetNewDrawData(nullptr, true, E_BatchingType::BatchingType_UI, false);
			if (childDrawData == nullptr)continue;//error
			childDrawData->SetShaderParam(num_sprite_shaderParam);
			childDrawData->SetSize(vector2s(WORD_SIZE_X, WORD_SIZE_Y));
			childArray[index].SetDrawData(childDrawData);
			//childArray[index].SetScale(vector3s(2.0f, 2.0f, 1.0f));
		}
	}

	exp_cur_word->SetWord("0001");

	hud_status_frame->RegisterChild(UI_HUD_STATUS_CUR_EXP, exp_cur_word, vector3s(HUD_CUR_DATA_DEFAULT_OFFSET_X, offset.y * 0.5f - 16 - 32 * 4, 0.0f));
	exp_cur_word->ShowUI(true, -1, true);

	//exp max
	UIWord* exp_max_word = new UIWord();
	exp_max_word->SetScale(vector3s(1.8f, 1.8f, 1.0f));
	exp_max_word->SetWordInterval(DEFAULT_WORD_INTERVAL);
	exp_max_word->SetAlignment(E_UIWordAlignment::UIWordAlignment_Left);

	childArray = exp_max_word->GetSpriteArray();
	if (childArray != nullptr) {
		for (int index = 0; index < UI_WORD_CHILD_COUNT; index++) {
			DrawData* childDrawData = RenderMgr::GetNewDrawData(nullptr, true, E_BatchingType::BatchingType_UI, false);
			if (childDrawData == nullptr)continue;//error
			childDrawData->SetShaderParam(num_sprite_shaderParam);
			childDrawData->SetSize(vector2s(WORD_SIZE_X, WORD_SIZE_Y));
			childArray[index].SetDrawData(childDrawData);
			//childArray[index].SetScale(vector3s(2.0f, 2.0f, 1.0f));
		}
	}

	exp_max_word->SetWord("1000");

	hud_status_frame->RegisterChild(UI_HUD_STATUS_MAX_EXP, exp_max_word, vector3s(HUD_MAX_DATA_DEFAULT_OFFSET_X, offset.y * 0.5f - 16 - 32 * 4, 0.0f));
	exp_max_word->ShowUI(true, -1, true);

	//slash
	ShaderParam_Normal* slash_shaderParam = new ShaderParam_Normal();
	slash_shaderParam->shaderProgram = GameMgr::GetInstance()->shaderMgr->GetShaderProgram(E_Shader::Shader_Normal);
	slash_shaderParam->texNo = GameMgr::GetInstance()->textureMgr->GetTextureID(E_Texture::Texture_UI_HUD_Package_Test_2);
	slash_shaderParam->usePreComputeUV = true;
	slash_shaderParam->preComputeUVSize = vector2s(SIGN_SLASH_WHITE_TEX_SIZE_X, SIGN_SLASH_WHITE_TEX_SIZE_Y);
	slash_shaderParam->preComputeUV = vector2s(SIGN_SLASH_WHITE_TEX_START_X, SIGN_SLASH_WHITE_TEX_START_Y);
	slash_shaderParam->tex_texel_size = ui_tex_size;

	//slash_1
	UISprite* slash_1 = new UISprite();
	slash_1->SetUIContentType(E_UIContent::UIContent_Sign);
	slash_1->SetID(GameMgr::GetInstance()->sceneObjMgr->GetID());
	slash_1->SetScale(vector3s(0.8f, 0.8f, 1.0f));

	DrawData* slash_1_drawData = RenderMgr::GetNewDrawData(nullptr, true, E_BatchingType::BatchingType_UI, false);
	slash_1_drawData->SetSize(vector2s(SIGN_SLASH_WHITE_TEX_SIZE_X, SIGN_SLASH_WHITE_TEX_SIZE_Y));
	slash_1_drawData->SetShaderParam(slash_shaderParam);

	slash_1->SetDrawData(slash_1_drawData);

	hud_status_frame->RegisterChild(UI_HUD_STATUS_SLASH_1, slash_1, vector3s(HUD_SLASH_DEFAULT_OFFSET_X, offset.y * 0.5f - 16 - 32 * 1, 0.0f));
	slash_1->ShowUI(true, -1, true);

	//slash_2
	UISprite* slash_2 = new UISprite();
	slash_2->SetUIContentType(E_UIContent::UIContent_Sign);
	slash_2->SetID(GameMgr::GetInstance()->sceneObjMgr->GetID());
	slash_2->SetScale(vector3s(0.8f, 0.8f, 1.0f));

	DrawData* slash_2_drawData = RenderMgr::GetNewDrawData(nullptr, true, E_BatchingType::BatchingType_UI, false);
	slash_2_drawData->SetSize(vector2s(SIGN_SLASH_WHITE_TEX_SIZE_X, SIGN_SLASH_WHITE_TEX_SIZE_Y));
	slash_2_drawData->SetShaderParam(slash_shaderParam);

	slash_2->SetDrawData(slash_2_drawData);

	hud_status_frame->RegisterChild(UI_HUD_STATUS_SLASH_2, slash_2, vector3s(HUD_SLASH_DEFAULT_OFFSET_X, offset.y * 0.5f - 16 - 32 * 2, 0.0f));
	slash_2->ShowUI(true, -1, true);

	//slash_3
	UISprite* slash_3 = new UISprite();
	slash_3->SetUIContentType(E_UIContent::UIContent_Sign);
	slash_3->SetID(GameMgr::GetInstance()->sceneObjMgr->GetID());
	slash_3->SetScale(vector3s(0.8f, 0.8f, 1.0f));

	DrawData* slash_3_drawData = RenderMgr::GetNewDrawData(nullptr, true, E_BatchingType::BatchingType_UI, false);
	slash_3_drawData->SetSize(vector2s(SIGN_SLASH_WHITE_TEX_SIZE_X, SIGN_SLASH_WHITE_TEX_SIZE_Y));
	slash_3_drawData->SetShaderParam(slash_shaderParam);

	slash_3->SetDrawData(slash_3_drawData);

	hud_status_frame->RegisterChild(UI_HUD_STATUS_SLASH_3, slash_3, vector3s(HUD_SLASH_DEFAULT_OFFSET_X, offset.y * 0.5f - 16 - 32 * 3, 0.0f));
	slash_3->ShowUI(true, -1, true);

	//slash_4
	UISprite* slash_4 = new UISprite();
	slash_4->SetUIContentType(E_UIContent::UIContent_Sign);
	slash_4->SetID(GameMgr::GetInstance()->sceneObjMgr->GetID());
	slash_4->SetScale(vector3s(0.8f, 0.8f, 1.0f));

	DrawData* slash_4_drawData = RenderMgr::GetNewDrawData(nullptr, true, E_BatchingType::BatchingType_UI, false);
	slash_4_drawData->SetSize(vector2s(SIGN_SLASH_WHITE_TEX_SIZE_X, SIGN_SLASH_WHITE_TEX_SIZE_Y));
	slash_4_drawData->SetShaderParam(slash_shaderParam);

	slash_4->SetDrawData(slash_4_drawData);

	hud_status_frame->RegisterChild(UI_HUD_STATUS_SLASH_4, slash_4, vector3s(HUD_SLASH_DEFAULT_OFFSET_X, offset.y * 0.5f - 16 - 32 * 4, 0.0f));
	slash_4->ShowUI(true, -1, true);

	hud_status_frame->ShowUI(false, -1, true);

	if (slash_shaderParam != nullptr) {
		delete slash_shaderParam;
	}
	slash_shaderParam = nullptr;

	GameMgr::GetInstance()->sceneObjMgr->RegisterCell(hud_status_frame);

#pragma endregion hud

#pragma region hud_game_timer

	hud_game_timer = new UIGroup();
	hud_game_timer->SetID(GameMgr::GetInstance()->sceneObjMgr->GetID());
	hud_game_timer->deleteChild = true;
	hud_game_timer->SetPos(vector3s(HUD_GAME_TIMER_POS_X, HUD_GAME_TIMER_POS_Y, -20), true);
	hud_game_timer->SetScale(vector3s(3.0f, 3.0f, 1.0f));

	//minute
	UIWord* minute = new UIWord();
	minute->SetScale(vector3s(1.8f, 1.8f, 1.0f));
	minute->SetWordInterval(TIME_WORD_INTERVAL);
	minute->SetAlignment(E_UIWordAlignment::UIWordAlignment_Right);

	childArray = minute->GetSpriteArray();
	if (childArray != nullptr) {
		for (int index = 0; index < UI_WORD_CHILD_COUNT; index++) {
			DrawData* childDrawData = RenderMgr::GetNewDrawData(nullptr, true, E_BatchingType::BatchingType_UI, false);
			if (childDrawData == nullptr)continue;//error
			childDrawData->SetShaderParam(num_sprite_shaderParam);
			childDrawData->SetSize(vector2s(WORD_SIZE_X, WORD_SIZE_Y));
			childArray[index].SetDrawData(childDrawData);
			//childArray[index].SetScale(vector3s(2.0f, 2.0f, 1.0f));
		}
	}

	minute->SetWord("00");

	hud_game_timer->RegisterChild(UI_GAME_TIMER_MINUTE, minute, vector3s(HUD_GAME_TIMER_MINUTE_OFFSET_X, 0.0f, 0.0f));
	minute->ShowUI(true, -1, true);

	//colon
	ShaderParam_Normal* colon_shaderParam = new ShaderParam_Normal();
	colon_shaderParam->shaderProgram = GameMgr::GetInstance()->shaderMgr->GetShaderProgram(E_Shader::Shader_Normal);
	colon_shaderParam->texNo = GameMgr::GetInstance()->textureMgr->GetTextureID(E_Texture::Texture_UI_HUD_Package_Test_2);
	colon_shaderParam->usePreComputeUV = true;
	colon_shaderParam->preComputeUVSize = vector2s(SIGN_COLON_OUTLINE_TEX_SIZE_X, SIGN_COLON_OUTLINE_TEX_SIZE_Y);
	colon_shaderParam->preComputeUV = vector2s(SIGN_COLON_OUTLINE_TEX_START_X, SIGN_COLON_OUTLINE_TEX_START_Y);
	colon_shaderParam->tex_texel_size = ui_tex_size;

	UISprite* colon = new UISprite();
	colon->SetUIContentType(E_UIContent::UIContent_Sign);
	colon->SetID(GameMgr::GetInstance()->sceneObjMgr->GetID());
	colon->SetScale(vector3s(0.8f, 0.8f, 1.0f));

	DrawData* colon_drawData = RenderMgr::GetNewDrawData(nullptr, true, E_BatchingType::BatchingType_UI, false);
	colon_drawData->SetSize(vector2s(SIGN_COLON_OUTLINE_TEX_SIZE_X, SIGN_COLON_OUTLINE_TEX_SIZE_Y));
	colon_drawData->SetShaderParam(colon_shaderParam);

	colon->SetDrawData(colon_drawData);

	hud_game_timer->RegisterChild(UI_GAME_TIMER_COLON, colon, vector3s(0.0f, 0.0f, 0.0f));
	colon->ShowUI(true, -1, true);


	//second
	UIWord* second = new UIWord();
	second->SetScale(vector3s(1.8f, 1.8f, 1.0f));
	second->SetWordInterval(TIME_WORD_INTERVAL);
	second->SetAlignment(E_UIWordAlignment::UIWordAlignment_Left);

	childArray = second->GetSpriteArray();
	if (childArray != nullptr) {
		for (int index = 0; index < UI_WORD_CHILD_COUNT; index++) {
			DrawData* childDrawData = RenderMgr::GetNewDrawData(nullptr, true, E_BatchingType::BatchingType_UI, false);
			if (childDrawData == nullptr)continue;//error
			childDrawData->SetShaderParam(num_sprite_shaderParam);
			childDrawData->SetSize(vector2s(WORD_SIZE_X, WORD_SIZE_Y));
			childArray[index].SetDrawData(childDrawData);
			//childArray[index].SetScale(vector3s(2.0f, 2.0f, 1.0f));
		}
	}

	second->SetWord("11");

	hud_game_timer->RegisterChild(UI_GAME_TIMER_SECOND, second, vector3s(HUD_GAME_TIMER_SECOND_OFFSET_X, 0.0f, 0.0f));
	second->ShowUI(true, -1, true);

	GameMgr::GetInstance()->sceneObjMgr->RegisterCell(hud_game_timer);

	hud_game_timer->ShowUI(false, -1, true);

#pragma endregion hud_game_timer

	if (colon_shaderParam != nullptr) {
		delete colon_shaderParam;
	}

	if (tempWord_shaderParam != nullptr) {
		delete tempWord_shaderParam;
	}

#pragma region title

	ShaderParam_Normal* title_img_shaderParam = new ShaderParam_Normal();
	title_img_shaderParam->shaderProgram = GameMgr::GetInstance()->shaderMgr->GetShaderProgram(E_Shader::Shader_Normal);
	title_img_shaderParam->texNo = GameMgr::GetInstance()->textureMgr->GetTextureID(E_Texture::Texture_UI_HUD_Package_Test_2);
	title_img_shaderParam->usePreComputeUV = true;
	title_img_shaderParam->preComputeUV = vector2s(TITLE_TEX_START_X, TITLE_TEX_START_Y);
	title_img_shaderParam->preComputeUVSize = vector2s(TITLE_TEX_SIZE_X, TITLE_TEX_SIZE_Y);
	title_img_shaderParam->tex_texel_size = ui_tex_size;

	//title img
	title_sprite = new UISprite();
	title_sprite->SetUIContentType(E_UIContent::UIContent_Img_Title);
	title_sprite->SetPos(title_default_pos, true);
	title_sprite->SetID(GameMgr::GetInstance()->sceneObjMgr->GetID());
	title_sprite->RecordCurState();
	title_sprite->SetScale(vector3s(TITLE_SCALE, TITLE_SCALE, 1.0f));

	DrawData* title_sprite_drawData = RenderMgr::GetNewDrawData(nullptr, true, E_BatchingType::BatchingType_UI, false);
	title_sprite_drawData->SetSize(vector2s(TITLE_TEX_SIZE_X, TITLE_TEX_SIZE_Y));
	title_sprite_drawData->SetShaderParam(title_img_shaderParam);

	title_sprite->SetDrawData(title_sprite_drawData);
	title_sprite->ShowUI(false, -1, true);

	GameMgr::GetInstance()->sceneObjMgr->RegisterCell(title_sprite);

	//title item
	title_bg = new UISprite();
	title_bg->SetUIContentType(E_UIContent::UIContent_Bg);
	title_bg->SetPos(vector3s(0, 0, 10), true);
	title_bg->SetID(GameMgr::GetInstance()->sceneObjMgr->GetID());

	ShaderParam_Mask* bg_mask_shaderParam = new ShaderParam_Mask();
	bg_mask_shaderParam->shaderProgram = GameMgr::GetInstance()->shaderMgr->GetShaderProgram(E_Shader::Shader_Mask);
	bg_mask_shaderParam->circleCenter = vector3s(0, 0, 0);
	bg_mask_shaderParam->circleRadius = 0.0f;
	bg_mask_shaderParam->color = vector4s(0.1f, 0.1f, 0.1f, 1.0f);

	DrawData* title_bg_drawData = RenderMgr::GetNewDrawData(nullptr, false, E_BatchingType::BatchingType_None, false);
	title_bg_drawData->SetShaderParam(bg_mask_shaderParam);
	title_bg_drawData->SetSize(vector2s(SCREEN_WIDTH, SCREEN_HEIGHT));
	title_bg->SetDrawData(title_bg_drawData);

	title_bg->ShowUI(false, -1, true);

	GameMgr::GetInstance()->sceneObjMgr->RegisterCell(title_bg);

	if (bg_mask_shaderParam != nullptr) {
		delete bg_mask_shaderParam;
	}

	if (title_img_shaderParam != nullptr) {
		delete title_img_shaderParam;
	}

#pragma endregion title

}

UI* UIMgr::CreateUIEntity(E_UIContent uiContentType)
{
	if (uiContentType == E_UIContent::UIContent_Dmg_Num) {
		UIWord* newUI = new UIWord();
		newUI->SetWordInterval(DMG_UI_WORD_INTERVAL);
		newUI->SetID(GameMgr::GetInstance()->sceneObjMgr->GetID());
		newUI->SetUIContentType(E_UIContent::UIContent_Dmg_Num);
		UISprite* childArray = newUI->GetSpriteArray();
		if (childArray != nullptr) {
			for (int index = 0; index < UI_WORD_CHILD_COUNT; index++) {
				DrawData* childDrawData = RenderMgr::GetNewDrawData(nullptr, true, E_BatchingType::BatchingType_UI, false);
				if (childDrawData == nullptr)continue;//error
				childDrawData->SetShaderParam(num_sprite_shaderParam);
				childDrawData->SetSize(vector2s(WORD_SIZE_X * 2, WORD_SIZE_Y * 2));
				childArray[index].SetDrawData(childDrawData);
				childArray[index].SetScale(vector3s(DMG_UI_SCALE, DMG_UI_SCALE, 1.0f));
			}
		}
		newUI->SetState(false);
		//register ui
		RegisterUI(newUI);
		GameMgr::GetInstance()->sceneObjMgr->RegisterCell(newUI);

		return newUI;
	}
	else if (uiContentType == E_UIContent::UIContent_Num)
	{
		UIWord* num_word = new UIWord();
		num_word->SetID(GameMgr::GetInstance()->sceneObjMgr->GetID());
		num_word->SetUIContentType(E_UIContent::UIContent_Num);
		num_word->SetScale(vector3s(1.8f, 1.8f, 1.0f));
		num_word->SetWordInterval(DEFAULT_WORD_INTERVAL);
		num_word->SetAlignment(E_UIWordAlignment::UIWordAlignment_Right);

		UISprite* childArray = num_word->GetSpriteArray();
		if (childArray != nullptr) {
			for (int index = 0; index < UI_WORD_CHILD_COUNT; index++) {
				DrawData* childDrawData = RenderMgr::GetNewDrawData(nullptr, true, E_BatchingType::BatchingType_UI, false);
				if (childDrawData == nullptr)continue;//error
				childDrawData->SetShaderParam(num_sprite_shaderParam);
				childDrawData->SetSize(vector2s(WORD_SIZE_X, WORD_SIZE_Y));
				childArray[index].SetDrawData(childDrawData);
			}
		}

		RegisterUI(num_word);
		GameMgr::GetInstance()->sceneObjMgr->RegisterCell(num_word);

		return num_word;
	}
	else if (uiContentType == E_UIContent::UIContent_Sign) {
		UISprite* newUI = new UISprite();
		newUI->SetID(GameMgr::GetInstance()->sceneObjMgr->GetID());
		newUI->SetUIContentType(E_UIContent::UIContent_Sign);

		DrawData* uiDrawData = RenderMgr::GetNewDrawData(nullptr, false, E_BatchingType::BatchingType_UI, false);
		if (uiDrawData == nullptr) {
			delete newUI;
			return nullptr;
		}

		uiDrawData->SetShaderParam(num_sprite_shaderParam);

		newUI->SetDrawData(uiDrawData);
		newUI->SetState(false);
		//register ui
		RegisterUI(newUI);
		GameMgr::GetInstance()->sceneObjMgr->RegisterCell(newUI);

		return newUI;
	}
	else if (uiContentType == E_UIContent::UIContent_HP) {
		UISprite* word_hp_sprite = new UISprite();
		word_hp_sprite->SetUIContentType(E_UIContent::UIContent_HP);
		word_hp_sprite->SetID(GameMgr::GetInstance()->sceneObjMgr->GetID());
		word_hp_sprite->SetPos(vector3s(0, 0, -11), true);

		DrawData* word_hp_drawData = RenderMgr::GetNewDrawData(nullptr, true, E_BatchingType::BatchingType_UI, false);
		word_hp_drawData->SetSize(vector2s(STATUS_SINGLE_IMG_TEX_SIZE_X, STATUS_SINGLE_IMG_TEX_SIZE_Y));
		ShaderParam_Normal* tempWord_shaderParam = new ShaderParam_Normal();
		tempWord_shaderParam->shaderProgram = GameMgr::GetInstance()->shaderMgr->GetShaderProgram(E_Shader::Shader_Normal);
		tempWord_shaderParam->texNo = GameMgr::GetInstance()->textureMgr->GetTextureID(E_Texture::Texture_UI_HUD_Package_Test_2);
		tempWord_shaderParam->usePreComputeUV = true;
		tempWord_shaderParam->preComputeUV = vector2s(STATUS_HP_IMG_TEX_START_X, STATUS_HP_IMG_TEX_START_Y);
		tempWord_shaderParam->preComputeUVSize = vector2s(STATUS_SINGLE_IMG_TEX_SIZE_X, STATUS_SINGLE_IMG_TEX_SIZE_Y);
		tempWord_shaderParam->tex_texel_size = ui_tex_size;

		word_hp_drawData->SetShaderParam(tempWord_shaderParam);

		word_hp_sprite->SetDrawData(word_hp_drawData);

		word_hp_sprite->ShowUI(false, -1, true);

		RegisterUI(word_hp_sprite);
		GameMgr::GetInstance()->sceneObjMgr->RegisterCell(word_hp_sprite);

		delete tempWord_shaderParam;

		return word_hp_sprite;
	}
	else if (uiContentType == E_UIContent::UIContent_Atk) {
		UISprite* word_atk_sprite = new UISprite();
		word_atk_sprite->SetUIContentType(E_UIContent::UIContent_Atk);
		word_atk_sprite->SetID(GameMgr::GetInstance()->sceneObjMgr->GetID());
		word_atk_sprite->SetPos(vector3s(0, 0, -11), true);

		DrawData* word_atk_drawData = RenderMgr::GetNewDrawData(nullptr, true, E_BatchingType::BatchingType_UI, false);
		word_atk_drawData->SetSize(vector2s(STATUS_SINGLE_IMG_TEX_SIZE_X, STATUS_SINGLE_IMG_TEX_SIZE_Y));
		ShaderParam_Normal* tempWord_shaderParam = new ShaderParam_Normal();
		tempWord_shaderParam->shaderProgram = GameMgr::GetInstance()->shaderMgr->GetShaderProgram(E_Shader::Shader_Normal);
		tempWord_shaderParam->texNo = GameMgr::GetInstance()->textureMgr->GetTextureID(E_Texture::Texture_UI_HUD_Package_Test_2);
		tempWord_shaderParam->usePreComputeUV = true;
		tempWord_shaderParam->preComputeUV = vector2s(STATUS_ATK_IMG_TEX_START_X, STATUS_ATK_IMG_TEX_START_Y);
		tempWord_shaderParam->preComputeUVSize = vector2s(STATUS_SINGLE_IMG_TEX_SIZE_X, STATUS_SINGLE_IMG_TEX_SIZE_Y);
		tempWord_shaderParam->tex_texel_size = ui_tex_size;

		word_atk_drawData->SetShaderParam(tempWord_shaderParam);

		word_atk_sprite->SetDrawData(word_atk_drawData);

		word_atk_sprite->ShowUI(false, -1, true);

		RegisterUI(word_atk_sprite);
		GameMgr::GetInstance()->sceneObjMgr->RegisterCell(word_atk_sprite);

		delete tempWord_shaderParam;

		return word_atk_sprite;
	}
	else if (uiContentType == E_UIContent::UIContent_Def) {
		//def
		UISprite* word_def_sprite = new UISprite();
		word_def_sprite->SetUIContentType(E_UIContent::UIContent_Def);
		word_def_sprite->SetID(GameMgr::GetInstance()->sceneObjMgr->GetID());
		word_def_sprite->SetPos(vector3s(0, 0, -11), true);

		DrawData* word_def_drawData = RenderMgr::GetNewDrawData(nullptr, true, E_BatchingType::BatchingType_UI, false);
		word_def_drawData->SetSize(vector2s(STATUS_SINGLE_IMG_TEX_SIZE_X, STATUS_SINGLE_IMG_TEX_SIZE_Y));
		ShaderParam_Normal* tempWord_shaderParam = new ShaderParam_Normal();
		tempWord_shaderParam->shaderProgram = GameMgr::GetInstance()->shaderMgr->GetShaderProgram(E_Shader::Shader_Normal);
		tempWord_shaderParam->texNo = GameMgr::GetInstance()->textureMgr->GetTextureID(E_Texture::Texture_UI_HUD_Package_Test_2);
		tempWord_shaderParam->usePreComputeUV = true;
		tempWord_shaderParam->preComputeUV = vector2s(STATUS_DEF_IMG_TEX_START_X, STATUS_DEF_IMG_TEX_START_Y);
		tempWord_shaderParam->preComputeUVSize = vector2s(STATUS_SINGLE_IMG_TEX_SIZE_X, STATUS_SINGLE_IMG_TEX_SIZE_Y);
		tempWord_shaderParam->tex_texel_size = ui_tex_size;

		word_def_drawData->SetShaderParam(tempWord_shaderParam);

		word_def_sprite->SetDrawData(word_def_drawData);

		word_def_sprite->ShowUI(false, -1, true);

		RegisterUI(word_def_sprite);
		GameMgr::GetInstance()->sceneObjMgr->RegisterCell(word_def_sprite);

		delete tempWord_shaderParam;

		return word_def_sprite;
	}
	else if (uiContentType == E_UIContent::UIContent_Exp) {

		UISprite* word_exp_sprite = new UISprite();
		word_exp_sprite->SetUIContentType(E_UIContent::UIContent_Exp);
		word_exp_sprite->SetID(GameMgr::GetInstance()->sceneObjMgr->GetID());
		word_exp_sprite->SetPos(vector3s(0, 0, -11), true);

		DrawData* word_exp_drawData = RenderMgr::GetNewDrawData(nullptr, true, E_BatchingType::BatchingType_UI, false);
		word_exp_drawData->SetSize(vector2s(STATUS_SINGLE_IMG_TEX_SIZE_X, STATUS_SINGLE_IMG_TEX_SIZE_Y));
		ShaderParam_Normal* tempWord_shaderParam = new ShaderParam_Normal();
		tempWord_shaderParam->shaderProgram = GameMgr::GetInstance()->shaderMgr->GetShaderProgram(E_Shader::Shader_Normal);
		tempWord_shaderParam->texNo = GameMgr::GetInstance()->textureMgr->GetTextureID(E_Texture::Texture_UI_HUD_Package_Test_2);
		tempWord_shaderParam->usePreComputeUV = true;
		tempWord_shaderParam->preComputeUV = vector2s(STATUS_EXP_IMG_TEX_START_X, STATUS_EXP_IMG_TEX_START_Y);
		tempWord_shaderParam->preComputeUVSize = vector2s(STATUS_SINGLE_IMG_TEX_SIZE_X, STATUS_SINGLE_IMG_TEX_SIZE_Y);
		tempWord_shaderParam->tex_texel_size = ui_tex_size;

		word_exp_drawData->SetShaderParam(tempWord_shaderParam);

		word_exp_sprite->SetDrawData(word_exp_drawData);

		word_exp_sprite->ShowUI(false, -1, true);

		RegisterUI(word_exp_sprite);
		GameMgr::GetInstance()->sceneObjMgr->RegisterCell(word_exp_sprite);

		return word_exp_sprite;

	}
	else if (uiContentType == E_UIContent::UIContent_Word_Max) {

		UISprite* word_max_sprite = new UISprite();
		word_max_sprite->SetUIContentType(E_UIContent::UIContent_Word_Max);
		word_max_sprite->SetID(GameMgr::GetInstance()->sceneObjMgr->GetID());
		word_max_sprite->SetPos(vector3s(0, 0, -11), true);

		DrawData* word_max_drawData = RenderMgr::GetNewDrawData(nullptr, true, E_BatchingType::BatchingType_UI, false);
		word_max_drawData->SetSize(vector2s(WORD_MAX_TEX_SIZE_X, WORD_MAX_TEX_SIZE_Y));
		ShaderParam_Normal* tempWord_shaderParam = new ShaderParam_Normal();
		tempWord_shaderParam->shaderProgram = GameMgr::GetInstance()->shaderMgr->GetShaderProgram(E_Shader::Shader_Normal);
		tempWord_shaderParam->texNo = GameMgr::GetInstance()->textureMgr->GetTextureID(E_Texture::Texture_UI_HUD_Package_Test_2);
		tempWord_shaderParam->usePreComputeUV = true;
		tempWord_shaderParam->preComputeUV = vector2s(WORD_MAX_TEX_START_X, WORD_MAX_TEX_START_Y);
		tempWord_shaderParam->preComputeUVSize = vector2s(WORD_MAX_TEX_SIZE_X, WORD_MAX_TEX_SIZE_Y);
		tempWord_shaderParam->tex_texel_size = ui_tex_size;

		word_max_drawData->SetShaderParam(tempWord_shaderParam);

		word_max_sprite->SetDrawData(word_max_drawData);

		word_max_sprite->ShowUI(false, -1, true);

		RegisterUI(word_max_sprite);
		GameMgr::GetInstance()->sceneObjMgr->RegisterCell(word_max_sprite);

		delete tempWord_shaderParam;

		return word_max_sprite;
	}
	else if (uiContentType == E_UIContent::UIContent_Word_Up) {
		UISprite* word_up_sprite = new UISprite();
		word_up_sprite->SetUIContentType(E_UIContent::UIContent_Word_Up);
		word_up_sprite->SetID(GameMgr::GetInstance()->sceneObjMgr->GetID());
		word_up_sprite->SetPos(vector3s(0, 0, -11), true);

		DrawData* word_up_drawData = RenderMgr::GetNewDrawData(nullptr, true, E_BatchingType::BatchingType_UI, false);
		word_up_drawData->SetSize(vector2s(WORD_UP_TEX_SIZE_X, WORD_UP_TEX_SIZE_Y));
		ShaderParam_Normal* tempWord_shaderParam = new ShaderParam_Normal();
		tempWord_shaderParam->shaderProgram = GameMgr::GetInstance()->shaderMgr->GetShaderProgram(E_Shader::Shader_Normal);
		tempWord_shaderParam->texNo = GameMgr::GetInstance()->textureMgr->GetTextureID(E_Texture::Texture_UI_HUD_Package_Test_2);
		tempWord_shaderParam->usePreComputeUV = true;
		tempWord_shaderParam->preComputeUV = vector2s(WORD_UP_TEX_START_X, WORD_UP_TEX_START_Y);
		tempWord_shaderParam->preComputeUVSize = vector2s(WORD_UP_TEX_SIZE_X, WORD_UP_TEX_SIZE_Y);
		tempWord_shaderParam->tex_texel_size = ui_tex_size;

		word_up_drawData->SetShaderParam(tempWord_shaderParam);

		word_up_sprite->SetDrawData(word_up_drawData);

		word_up_sprite->ShowUI(false, -1, true);

		RegisterUI(word_up_sprite);
		GameMgr::GetInstance()->sceneObjMgr->RegisterCell(word_up_sprite);

		delete tempWord_shaderParam;

		return word_up_sprite;
	}
	else if (uiContentType == E_UIContent::UIContent_Word_Attack) {
		UISprite* word_attack_sprite = new UISprite();
		word_attack_sprite->SetUIContentType(E_UIContent::UIContent_Word_Attack);
		word_attack_sprite->SetID(GameMgr::GetInstance()->sceneObjMgr->GetID());
		word_attack_sprite->SetPos(vector3s(0, 0, -11), true);

		DrawData* word_attack_drawData = RenderMgr::GetNewDrawData(nullptr, true, E_BatchingType::BatchingType_UI, false);
		word_attack_drawData->SetSize(vector2s(WORD_ATTACK_TEX_SIZE_X, WORD_ATTACK_TEX_SIZE_Y));
		ShaderParam_Normal* tempWord_shaderParam = new ShaderParam_Normal();
		tempWord_shaderParam->shaderProgram = GameMgr::GetInstance()->shaderMgr->GetShaderProgram(E_Shader::Shader_Normal);
		tempWord_shaderParam->texNo = GameMgr::GetInstance()->textureMgr->GetTextureID(E_Texture::Texture_UI_HUD_Package_Test_2);
		tempWord_shaderParam->usePreComputeUV = true;
		tempWord_shaderParam->preComputeUV = vector2s(WORD_ATTACK_TEX_START_X, WORD_ATTACK_TEX_START_Y);
		tempWord_shaderParam->preComputeUVSize = vector2s(WORD_ATTACK_TEX_SIZE_X, WORD_ATTACK_TEX_SIZE_Y);
		tempWord_shaderParam->tex_texel_size = ui_tex_size;

		word_attack_drawData->SetShaderParam(tempWord_shaderParam);

		word_attack_sprite->SetDrawData(word_attack_drawData);

		word_attack_sprite->ShowUI(false, -1, true);

		RegisterUI(word_attack_sprite);
		GameMgr::GetInstance()->sceneObjMgr->RegisterCell(word_attack_sprite);

		delete tempWord_shaderParam;

		return word_attack_sprite;
	}
	else if (uiContentType == E_UIContent::UIContent_Word_Speed) {
		UISprite* word_speed_sprite = new UISprite();
		word_speed_sprite->SetUIContentType(E_UIContent::UIContent_Word_Speed);
		word_speed_sprite->SetID(GameMgr::GetInstance()->sceneObjMgr->GetID());
		word_speed_sprite->SetPos(vector3s(0, 0, -11), true);

		DrawData* word_speed_drawData = RenderMgr::GetNewDrawData(nullptr, true, E_BatchingType::BatchingType_UI, false);
		word_speed_drawData->SetSize(vector2s(WORD_SPEED_TEX_SIZE_X, WORD_SPEED_TEX_SIZE_Y));
		ShaderParam_Normal* tempWord_shaderParam = new ShaderParam_Normal();
		tempWord_shaderParam->shaderProgram = GameMgr::GetInstance()->shaderMgr->GetShaderProgram(E_Shader::Shader_Normal);
		tempWord_shaderParam->texNo = GameMgr::GetInstance()->textureMgr->GetTextureID(E_Texture::Texture_UI_HUD_Package_Test_2);
		tempWord_shaderParam->usePreComputeUV = true;
		tempWord_shaderParam->preComputeUV = vector2s(WORD_SPEED_TEX_START_X, WORD_SPEED_TEX_START_Y);
		tempWord_shaderParam->preComputeUVSize = vector2s(WORD_SPEED_TEX_SIZE_X, WORD_SPEED_TEX_SIZE_Y);
		tempWord_shaderParam->tex_texel_size = ui_tex_size;

		word_speed_drawData->SetShaderParam(tempWord_shaderParam);

		word_speed_sprite->SetDrawData(word_speed_drawData);

		word_speed_sprite->ShowUI(false, -1, true);

		RegisterUI(word_speed_sprite);
		GameMgr::GetInstance()->sceneObjMgr->RegisterCell(word_speed_sprite);

		delete tempWord_shaderParam;

		return word_speed_sprite;
	}
	else if (uiContentType == E_UIContent::UIContent_Word_Bullet) {
		UISprite* word_bullet_sprite = new UISprite();
		word_bullet_sprite->SetUIContentType(E_UIContent::UIContent_Word_Bullet);
		word_bullet_sprite->SetID(GameMgr::GetInstance()->sceneObjMgr->GetID());
		word_bullet_sprite->SetPos(vector3s(0, 0, -11), true);

		DrawData* word_bullet_drawData = RenderMgr::GetNewDrawData(nullptr, true, E_BatchingType::BatchingType_UI, false);
		word_bullet_drawData->SetSize(vector2s(WORD_BULLET_TEX_SIZE_X, WORD_BULLET_TEX_SIZE_Y));
		ShaderParam_Normal* tempWord_shaderParam = new ShaderParam_Normal();
		tempWord_shaderParam->shaderProgram = GameMgr::GetInstance()->shaderMgr->GetShaderProgram(E_Shader::Shader_Normal);
		tempWord_shaderParam->texNo = GameMgr::GetInstance()->textureMgr->GetTextureID(E_Texture::Texture_UI_HUD_Package_Test_2);
		tempWord_shaderParam->usePreComputeUV = true;
		tempWord_shaderParam->preComputeUV = vector2s(WORD_BULLET_TEX_START_X, WORD_BULLET_TEX_START_Y);
		tempWord_shaderParam->preComputeUVSize = vector2s(WORD_BULLET_TEX_SIZE_X, WORD_BULLET_TEX_SIZE_Y);
		tempWord_shaderParam->tex_texel_size = ui_tex_size;

		word_bullet_drawData->SetShaderParam(tempWord_shaderParam);

		word_bullet_sprite->SetDrawData(word_bullet_drawData);

		word_bullet_sprite->ShowUI(false, -1, true);

		RegisterUI(word_bullet_sprite);
		GameMgr::GetInstance()->sceneObjMgr->RegisterCell(word_bullet_sprite);

		delete tempWord_shaderParam;

		return word_bullet_sprite;
	}
	else if (uiContentType == E_UIContent::UIContent_Word_Consume) {
		UISprite* word_consume_sprite = new UISprite();
		word_consume_sprite->SetUIContentType(E_UIContent::UIContent_Word_Consume);
		word_consume_sprite->SetID(GameMgr::GetInstance()->sceneObjMgr->GetID());
		word_consume_sprite->SetPos(vector3s(0, 0, -11), true);

		DrawData* word_consume_drawData = RenderMgr::GetNewDrawData(nullptr, true, E_BatchingType::BatchingType_UI, false);
		word_consume_drawData->SetSize(vector2s(WORD_CONSUME_TEX_SIZE_X, WORD_CONSUME_TEX_SIZE_Y));
		ShaderParam_Normal* tempWord_shaderParam = new ShaderParam_Normal();
		tempWord_shaderParam->shaderProgram = GameMgr::GetInstance()->shaderMgr->GetShaderProgram(E_Shader::Shader_Normal);
		tempWord_shaderParam->texNo = GameMgr::GetInstance()->textureMgr->GetTextureID(E_Texture::Texture_UI_HUD_Package_Test_2);
		tempWord_shaderParam->usePreComputeUV = true;
		tempWord_shaderParam->preComputeUV = vector2s(WORD_CONSUME_TEX_START_X, WORD_CONSUME_TEX_START_Y);
		tempWord_shaderParam->preComputeUVSize = vector2s(WORD_CONSUME_TEX_SIZE_X, WORD_CONSUME_TEX_SIZE_Y);
		tempWord_shaderParam->tex_texel_size = ui_tex_size;

		word_consume_drawData->SetShaderParam(tempWord_shaderParam);

		word_consume_sprite->SetDrawData(word_consume_drawData);

		word_consume_sprite->ShowUI(false, -1, true);

		RegisterUI(word_consume_sprite);
		GameMgr::GetInstance()->sceneObjMgr->RegisterCell(word_consume_sprite);

		delete tempWord_shaderParam;

		return word_consume_sprite;
	}
	else if (uiContentType == E_UIContent::UIContent_Word_Gain) {
		UISprite* word_gain_sprite = new UISprite();
		word_gain_sprite->SetUIContentType(E_UIContent::UIContent_Word_Consume);
		word_gain_sprite->SetID(GameMgr::GetInstance()->sceneObjMgr->GetID());
		word_gain_sprite->SetPos(vector3s(0, 0, -11), true);

		DrawData* word_gain_drawData = RenderMgr::GetNewDrawData(nullptr, true, E_BatchingType::BatchingType_UI, false);
		word_gain_drawData->SetSize(vector2s(WORD_GAIN_TEX_SIZE_X, WORD_GAIN_TEX_SIZE_Y));
		ShaderParam_Normal* tempWord_shaderParam = new ShaderParam_Normal();
		tempWord_shaderParam->shaderProgram = GameMgr::GetInstance()->shaderMgr->GetShaderProgram(E_Shader::Shader_Normal);
		tempWord_shaderParam->texNo = GameMgr::GetInstance()->textureMgr->GetTextureID(E_Texture::Texture_UI_HUD_Package_Test_2);
		tempWord_shaderParam->usePreComputeUV = true;
		tempWord_shaderParam->preComputeUV = vector2s(WORD_GAIN_TEX_START_X, WORD_GAIN_TEX_START_Y);
		tempWord_shaderParam->preComputeUVSize = vector2s(WORD_GAIN_TEX_SIZE_X, WORD_GAIN_TEX_SIZE_Y);
		tempWord_shaderParam->tex_texel_size = ui_tex_size;

		word_gain_drawData->SetShaderParam(tempWord_shaderParam);

		word_gain_sprite->SetDrawData(word_gain_drawData);

		word_gain_sprite->ShowUI(false, -1, true);

		RegisterUI(word_gain_sprite);
		GameMgr::GetInstance()->sceneObjMgr->RegisterCell(word_gain_sprite);

		delete tempWord_shaderParam;

		return word_gain_sprite;
	}
	else if (uiContentType == E_UIContent::UIContent_Word_Recovery) {
		UISprite* word_recovery_sprite = new UISprite();
		word_recovery_sprite->SetUIContentType(E_UIContent::UIContent_Word_Recovery);
		word_recovery_sprite->SetID(GameMgr::GetInstance()->sceneObjMgr->GetID());
		word_recovery_sprite->SetPos(vector3s(0, 0, -11), true);

		DrawData* word_recovery_drawData = RenderMgr::GetNewDrawData(nullptr, true, E_BatchingType::BatchingType_UI, false);
		word_recovery_drawData->SetSize(vector2s(WORD_RECOVERY_TEX_SIZE_X, WORD_RECOVERY_TEX_SIZE_Y));
		ShaderParam_Normal* tempWord_shaderParam = new ShaderParam_Normal();
		tempWord_shaderParam->shaderProgram = GameMgr::GetInstance()->shaderMgr->GetShaderProgram(E_Shader::Shader_Normal);
		tempWord_shaderParam->texNo = GameMgr::GetInstance()->textureMgr->GetTextureID(E_Texture::Texture_UI_HUD_Package_Test_2);
		tempWord_shaderParam->usePreComputeUV = true;
		tempWord_shaderParam->preComputeUV = vector2s(WORD_RECOVERY_TEX_START_X, WORD_RECOVERY_TEX_START_Y);
		tempWord_shaderParam->preComputeUVSize = vector2s(WORD_RECOVERY_TEX_SIZE_X, WORD_RECOVERY_TEX_SIZE_Y);
		tempWord_shaderParam->tex_texel_size = ui_tex_size;

		word_recovery_drawData->SetShaderParam(tempWord_shaderParam);

		word_recovery_sprite->SetDrawData(word_recovery_drawData);

		word_recovery_sprite->ShowUI(false, -1, true);

		RegisterUI(word_recovery_sprite);
		GameMgr::GetInstance()->sceneObjMgr->RegisterCell(word_recovery_sprite);

		delete tempWord_shaderParam;

		return word_recovery_sprite;
	}
	else if (uiContentType == E_UIContent::UIContent_Word_Cap) {
		UISprite* word_cap_sprite = new UISprite();
		word_cap_sprite->SetUIContentType(E_UIContent::UIContent_Word_Cap);
		word_cap_sprite->SetID(GameMgr::GetInstance()->sceneObjMgr->GetID());
		word_cap_sprite->SetPos(vector3s(0, 0, -11), true);

		DrawData* word_cap_drawData = RenderMgr::GetNewDrawData(nullptr, true, E_BatchingType::BatchingType_UI, false);
		word_cap_drawData->SetSize(vector2s(WORD_CAP_TEX_SIZE_X, WORD_CAP_TEX_SIZE_Y));
		ShaderParam_Normal* tempWord_shaderParam = new ShaderParam_Normal();
		tempWord_shaderParam->shaderProgram = GameMgr::GetInstance()->shaderMgr->GetShaderProgram(E_Shader::Shader_Normal);
		tempWord_shaderParam->texNo = GameMgr::GetInstance()->textureMgr->GetTextureID(E_Texture::Texture_UI_HUD_Package_Test_2);
		tempWord_shaderParam->usePreComputeUV = true;
		tempWord_shaderParam->preComputeUV = vector2s(WORD_CAP_TEX_START_X, WORD_CAP_TEX_START_Y);
		tempWord_shaderParam->preComputeUVSize = vector2s(WORD_CAP_TEX_SIZE_X, WORD_CAP_TEX_SIZE_Y);
		tempWord_shaderParam->tex_texel_size = ui_tex_size;

		word_cap_drawData->SetShaderParam(tempWord_shaderParam);

		word_cap_sprite->SetDrawData(word_cap_drawData);

		word_cap_sprite->ShowUI(false, -1, true);

		RegisterUI(word_cap_sprite);
		GameMgr::GetInstance()->sceneObjMgr->RegisterCell(word_cap_sprite);

		delete tempWord_shaderParam;

		return word_cap_sprite;
	}
	else if (uiContentType == E_UIContent::UIContent_Word_Damage) {
		UISprite* word_damage_sprite = new UISprite();
		word_damage_sprite->SetUIContentType(E_UIContent::UIContent_Word_Damage);
		word_damage_sprite->SetID(GameMgr::GetInstance()->sceneObjMgr->GetID());
		word_damage_sprite->SetPos(vector3s(0, 0, -11), true);

		DrawData* word_damage_drawData = RenderMgr::GetNewDrawData(nullptr, true, E_BatchingType::BatchingType_UI, false);
		word_damage_drawData->SetSize(vector2s(WORD_DAMAGE_TEX_SIZE_X, WORD_DAMAGE_TEX_SIZE_Y));
		ShaderParam_Normal* tempWord_shaderParam = new ShaderParam_Normal();
		tempWord_shaderParam->shaderProgram = GameMgr::GetInstance()->shaderMgr->GetShaderProgram(E_Shader::Shader_Normal);
		tempWord_shaderParam->texNo = GameMgr::GetInstance()->textureMgr->GetTextureID(E_Texture::Texture_UI_HUD_Package_Test_2);
		tempWord_shaderParam->usePreComputeUV = true;
		tempWord_shaderParam->preComputeUV = vector2s(WORD_DAMAGE_TEX_START_X, WORD_DAMAGE_TEX_START_Y);
		tempWord_shaderParam->preComputeUVSize = vector2s(WORD_DAMAGE_TEX_SIZE_X, WORD_DAMAGE_TEX_SIZE_Y);
		tempWord_shaderParam->tex_texel_size = ui_tex_size;

		word_damage_drawData->SetShaderParam(tempWord_shaderParam);

		word_damage_sprite->SetDrawData(word_damage_drawData);

		word_damage_sprite->ShowUI(false, -1, true);

		RegisterUI(word_damage_sprite);
		GameMgr::GetInstance()->sceneObjMgr->RegisterCell(word_damage_sprite);

		delete tempWord_shaderParam;

		return word_damage_sprite;
	}
	else if (uiContentType == E_UIContent::UIContent_Word_Reflect) {
		UISprite* word_reflect_sprite = new UISprite();
		word_reflect_sprite->SetUIContentType(E_UIContent::UIContent_Word_Reflect);
		word_reflect_sprite->SetID(GameMgr::GetInstance()->sceneObjMgr->GetID());
		word_reflect_sprite->SetPos(vector3s(0, 0, -11), true);

		DrawData* word_reflect_drawData = RenderMgr::GetNewDrawData(nullptr, true, E_BatchingType::BatchingType_UI, false);
		word_reflect_drawData->SetSize(vector2s(WORD_REFLECT_TEX_SIZE_X, WORD_REFLECT_TEX_SIZE_Y));
		ShaderParam_Normal* tempWord_shaderParam = new ShaderParam_Normal();
		tempWord_shaderParam->shaderProgram = GameMgr::GetInstance()->shaderMgr->GetShaderProgram(E_Shader::Shader_Normal);
		tempWord_shaderParam->texNo = GameMgr::GetInstance()->textureMgr->GetTextureID(E_Texture::Texture_UI_HUD_Package_Test_2);
		tempWord_shaderParam->usePreComputeUV = true;
		tempWord_shaderParam->preComputeUV = vector2s(WORD_REFLECT_TEX_START_X, WORD_REFLECT_TEX_START_Y);
		tempWord_shaderParam->preComputeUVSize = vector2s(WORD_REFLECT_TEX_SIZE_X, WORD_REFLECT_TEX_SIZE_Y);
		tempWord_shaderParam->tex_texel_size = ui_tex_size;

		word_reflect_drawData->SetShaderParam(tempWord_shaderParam);

		word_reflect_sprite->SetDrawData(word_reflect_drawData);

		word_reflect_sprite->ShowUI(false, -1, true);

		RegisterUI(word_reflect_sprite);
		GameMgr::GetInstance()->sceneObjMgr->RegisterCell(word_reflect_sprite);

		delete tempWord_shaderParam;

		return word_reflect_sprite;
	}
	else if (uiContentType == E_UIContent::UIContent_Word_Enemy) {
		UISprite* word_enemy_sprite = new UISprite();
		word_enemy_sprite->SetUIContentType(E_UIContent::UIContent_Word_Enemy);
		word_enemy_sprite->SetID(GameMgr::GetInstance()->sceneObjMgr->GetID());
		word_enemy_sprite->SetPos(vector3s(0, 0, -11), true);

		DrawData* word_enemy_drawData = RenderMgr::GetNewDrawData(nullptr, true, E_BatchingType::BatchingType_UI, false);
		word_enemy_drawData->SetSize(vector2s(WORD_ENEMY_TEX_SIZE_X, WORD_ENEMY_TEX_SIZE_Y));
		ShaderParam_Normal* tempWord_shaderParam = new ShaderParam_Normal();
		tempWord_shaderParam->shaderProgram = GameMgr::GetInstance()->shaderMgr->GetShaderProgram(E_Shader::Shader_Normal);
		tempWord_shaderParam->texNo = GameMgr::GetInstance()->textureMgr->GetTextureID(E_Texture::Texture_UI_HUD_Package_Test_2);
		tempWord_shaderParam->usePreComputeUV = true;
		tempWord_shaderParam->preComputeUV = vector2s(WORD_ENEMY_TEX_START_X, WORD_ENEMY_TEX_START_Y);
		tempWord_shaderParam->preComputeUVSize = vector2s(WORD_ENEMY_TEX_SIZE_X, WORD_ENEMY_TEX_SIZE_Y);
		tempWord_shaderParam->tex_texel_size = ui_tex_size;

		word_enemy_drawData->SetShaderParam(tempWord_shaderParam);

		word_enemy_sprite->SetDrawData(word_enemy_drawData);

		word_enemy_sprite->ShowUI(false, -1, true);

		RegisterUI(word_enemy_sprite);
		GameMgr::GetInstance()->sceneObjMgr->RegisterCell(word_enemy_sprite);

		delete tempWord_shaderParam;

		return word_enemy_sprite;
	}
	else if (uiContentType == E_UIContent::UIContent_Word_Lv_Up) {

	}
	else if (uiContentType == E_UIContent::UIContent_Word_You_Lose) {

	}
	else if (uiContentType == E_UIContent::UIContent_Word_Start) {

	}
	else if (uiContentType == E_UIContent::UIContent_Word_Exit) {

	}
	else if (uiContentType == E_UIContent::UIContent_Word_Title) {

	}
	else if (uiContentType == E_UIContent::UIContent_Word_Move) {
		UISprite* word_move_sprite = new UISprite();
		word_move_sprite->SetUIContentType(E_UIContent::UIContent_Word_Move);
		word_move_sprite->SetID(GameMgr::GetInstance()->sceneObjMgr->GetID());
		word_move_sprite->SetPos(vector3s(0, 0, -11), true);

		DrawData* word_move_drawData = RenderMgr::GetNewDrawData(nullptr, true, E_BatchingType::BatchingType_UI, false);
		word_move_drawData->SetSize(vector2s(WORD_MOVE_TEX_SIZE_X, WORD_MOVE_TEX_SIZE_Y));
		ShaderParam_Normal* tempWord_shaderParam = new ShaderParam_Normal();
		tempWord_shaderParam->shaderProgram = GameMgr::GetInstance()->shaderMgr->GetShaderProgram(E_Shader::Shader_Normal);
		tempWord_shaderParam->texNo = GameMgr::GetInstance()->textureMgr->GetTextureID(E_Texture::Texture_UI_HUD_Package_Test_2);
		tempWord_shaderParam->usePreComputeUV = true;
		tempWord_shaderParam->preComputeUV = vector2s(WORD_MOVE_TEX_START_X, WORD_MOVE_TEX_START_Y);
		tempWord_shaderParam->preComputeUVSize = vector2s(WORD_MOVE_TEX_SIZE_X, WORD_MOVE_TEX_SIZE_Y);
		tempWord_shaderParam->tex_texel_size = ui_tex_size;

		word_move_drawData->SetShaderParam(tempWord_shaderParam);

		word_move_sprite->SetDrawData(word_move_drawData);

		word_move_sprite->ShowUI(false, -1, true);

		RegisterUI(word_move_sprite);
		GameMgr::GetInstance()->sceneObjMgr->RegisterCell(word_move_sprite);

		delete tempWord_shaderParam;

		return word_move_sprite;
	}
	else if (uiContentType == E_UIContent::UIContent_Word_Count) {
		UISprite* word_count_sprite = new UISprite();
		word_count_sprite->SetUIContentType(E_UIContent::UIContent_Word_Count);
		word_count_sprite->SetID(GameMgr::GetInstance()->sceneObjMgr->GetID());
		word_count_sprite->SetPos(vector3s(0, 0, -11), true);

		DrawData* word_count_drawData = RenderMgr::GetNewDrawData(nullptr, true, E_BatchingType::BatchingType_UI, false);
		word_count_drawData->SetSize(vector2s(WORD_COUNT_TEX_SIZE_X, WORD_COUNT_TEX_SIZE_Y));
		ShaderParam_Normal* tempWord_shaderParam = new ShaderParam_Normal();
		tempWord_shaderParam->shaderProgram = GameMgr::GetInstance()->shaderMgr->GetShaderProgram(E_Shader::Shader_Normal);
		tempWord_shaderParam->texNo = GameMgr::GetInstance()->textureMgr->GetTextureID(E_Texture::Texture_UI_HUD_Package_Test_2);
		tempWord_shaderParam->usePreComputeUV = true;
		tempWord_shaderParam->preComputeUV = vector2s(WORD_COUNT_TEX_START_X, WORD_COUNT_TEX_START_Y);
		tempWord_shaderParam->preComputeUVSize = vector2s(WORD_COUNT_TEX_SIZE_X, WORD_COUNT_TEX_SIZE_Y);
		tempWord_shaderParam->tex_texel_size = ui_tex_size;

		word_count_drawData->SetShaderParam(tempWord_shaderParam);

		word_count_sprite->SetDrawData(word_count_drawData);

		word_count_sprite->ShowUI(false, -1, true);

		RegisterUI(word_count_sprite);
		GameMgr::GetInstance()->sceneObjMgr->RegisterCell(word_count_sprite);

		delete tempWord_shaderParam;

		return word_count_sprite;
	}
	else if (uiContentType == E_UIContent::UIContent_Word_Create) {
		UISprite* word_create_sprite = new UISprite();
		word_create_sprite->SetUIContentType(E_UIContent::UIContent_Word_Create);
		word_create_sprite->SetID(GameMgr::GetInstance()->sceneObjMgr->GetID());
		word_create_sprite->SetPos(vector3s(0, 0, -11), true);

		DrawData* word_create_drawData = RenderMgr::GetNewDrawData(nullptr, true, E_BatchingType::BatchingType_UI, false);
		word_create_drawData->SetSize(vector2s(WORD_CREATE_TEX_SIZE_X, WORD_CREATE_TEX_SIZE_Y));
		ShaderParam_Normal* tempWord_shaderParam = new ShaderParam_Normal();
		tempWord_shaderParam->shaderProgram = GameMgr::GetInstance()->shaderMgr->GetShaderProgram(E_Shader::Shader_Normal);
		tempWord_shaderParam->texNo = GameMgr::GetInstance()->textureMgr->GetTextureID(E_Texture::Texture_UI_HUD_Package_Test_2);
		tempWord_shaderParam->usePreComputeUV = true;
		tempWord_shaderParam->preComputeUV = vector2s(WORD_CREATE_TEX_START_X, WORD_CREATE_TEX_START_Y);
		tempWord_shaderParam->preComputeUVSize = vector2s(WORD_CREATE_TEX_SIZE_X, WORD_CREATE_TEX_SIZE_Y);
		tempWord_shaderParam->tex_texel_size = ui_tex_size;

		word_create_drawData->SetShaderParam(tempWord_shaderParam);

		word_create_sprite->SetDrawData(word_create_drawData);

		word_create_sprite->ShowUI(false, -1, true);

		RegisterUI(word_create_sprite);
		GameMgr::GetInstance()->sceneObjMgr->RegisterCell(word_create_sprite);

		delete tempWord_shaderParam;

		return word_create_sprite;
	}
	else if (uiContentType == E_UIContent::UIContent_Word_Every) {
		UISprite* word_every_sprite = new UISprite();
		word_every_sprite->SetUIContentType(E_UIContent::UIContent_Word_Every);
		word_every_sprite->SetID(GameMgr::GetInstance()->sceneObjMgr->GetID());
		word_every_sprite->SetPos(vector3s(0, 0, -11), true);

		DrawData* word_every_drawData = RenderMgr::GetNewDrawData(nullptr, true, E_BatchingType::BatchingType_UI, false);
		word_every_drawData->SetSize(vector2s(WORD_EVERY_TEX_SIZE_X, WORD_EVERY_TEX_SIZE_Y));
		ShaderParam_Normal* tempWord_shaderParam = new ShaderParam_Normal();
		tempWord_shaderParam->shaderProgram = GameMgr::GetInstance()->shaderMgr->GetShaderProgram(E_Shader::Shader_Normal);
		tempWord_shaderParam->texNo = GameMgr::GetInstance()->textureMgr->GetTextureID(E_Texture::Texture_UI_HUD_Package_Test_2);
		tempWord_shaderParam->usePreComputeUV = true;
		tempWord_shaderParam->preComputeUV = vector2s(WORD_EVERY_TEX_START_X, WORD_EVERY_TEX_START_Y);
		tempWord_shaderParam->preComputeUVSize = vector2s(WORD_EVERY_TEX_SIZE_X, WORD_EVERY_TEX_SIZE_Y);
		tempWord_shaderParam->tex_texel_size = ui_tex_size;

		word_every_drawData->SetShaderParam(tempWord_shaderParam);

		word_every_sprite->SetDrawData(word_every_drawData);

		word_every_sprite->ShowUI(false, -1, true);

		RegisterUI(word_every_sprite);
		GameMgr::GetInstance()->sceneObjMgr->RegisterCell(word_every_sprite);

		delete tempWord_shaderParam;

		return word_every_sprite;
	}
	else if (uiContentType == E_UIContent::UIContent_Word_Die) {
		UISprite* word_die_sprite = new UISprite();
		word_die_sprite->SetUIContentType(E_UIContent::UIContent_Word_Die);
		word_die_sprite->SetID(GameMgr::GetInstance()->sceneObjMgr->GetID());
		word_die_sprite->SetPos(vector3s(0, 0, -11), true);

		DrawData* word_die_drawData = RenderMgr::GetNewDrawData(nullptr, true, E_BatchingType::BatchingType_UI, false);
		word_die_drawData->SetSize(vector2s(WORD_DIE_TEX_SIZE_X, WORD_DIE_TEX_SIZE_Y));
		ShaderParam_Normal* tempWord_shaderParam = new ShaderParam_Normal();
		tempWord_shaderParam->shaderProgram = GameMgr::GetInstance()->shaderMgr->GetShaderProgram(E_Shader::Shader_Normal);
		tempWord_shaderParam->texNo = GameMgr::GetInstance()->textureMgr->GetTextureID(E_Texture::Texture_UI_HUD_Package_Test_2);
		tempWord_shaderParam->usePreComputeUV = true;
		tempWord_shaderParam->preComputeUV = vector2s(WORD_DIE_TEX_START_X, WORD_DIE_TEX_START_Y);
		tempWord_shaderParam->preComputeUVSize = vector2s(WORD_DIE_TEX_SIZE_X, WORD_DIE_TEX_SIZE_Y);
		tempWord_shaderParam->tex_texel_size = ui_tex_size;

		word_die_drawData->SetShaderParam(tempWord_shaderParam);

		word_die_sprite->SetDrawData(word_die_drawData);

		word_die_sprite->ShowUI(false, -1, true);

		RegisterUI(word_die_sprite);
		GameMgr::GetInstance()->sceneObjMgr->RegisterCell(word_die_sprite);

		delete tempWord_shaderParam;

		return word_die_sprite;
	}
	else if (uiContentType == E_UIContent::UIContent_Word_Hit) {
		UISprite* word_hit_sprite = new UISprite();
		word_hit_sprite->SetUIContentType(E_UIContent::UIContent_Word_Hit);
		word_hit_sprite->SetID(GameMgr::GetInstance()->sceneObjMgr->GetID());
		word_hit_sprite->SetPos(vector3s(0, 0, -11), true);

		DrawData* word_hit_drawData = RenderMgr::GetNewDrawData(nullptr, true, E_BatchingType::BatchingType_UI, false);
		word_hit_drawData->SetSize(vector2s(WORD_HIT_TEX_SIZE_X, WORD_HIT_TEX_SIZE_Y));
		ShaderParam_Normal* tempWord_shaderParam = new ShaderParam_Normal();
		tempWord_shaderParam->shaderProgram = GameMgr::GetInstance()->shaderMgr->GetShaderProgram(E_Shader::Shader_Normal);
		tempWord_shaderParam->texNo = GameMgr::GetInstance()->textureMgr->GetTextureID(E_Texture::Texture_UI_HUD_Package_Test_2);
		tempWord_shaderParam->usePreComputeUV = true;
		tempWord_shaderParam->preComputeUV = vector2s(WORD_HIT_TEX_START_X, WORD_HIT_TEX_START_Y);
		tempWord_shaderParam->preComputeUVSize = vector2s(WORD_HIT_TEX_SIZE_X, WORD_HIT_TEX_SIZE_Y);
		tempWord_shaderParam->tex_texel_size = ui_tex_size;

		word_hit_drawData->SetShaderParam(tempWord_shaderParam);

		word_hit_sprite->SetDrawData(word_hit_drawData);

		word_hit_sprite->ShowUI(false, -1, true);

		RegisterUI(word_hit_sprite);
		GameMgr::GetInstance()->sceneObjMgr->RegisterCell(word_hit_sprite);

		delete tempWord_shaderParam;

		return word_hit_sprite;
	}
	else if (uiContentType == E_UIContent::UIContent_Word_Second) {
		UISprite* word_second_sprite = new UISprite();
		word_second_sprite->SetUIContentType(E_UIContent::UIContent_Word_Second);
		word_second_sprite->SetID(GameMgr::GetInstance()->sceneObjMgr->GetID());
		word_second_sprite->SetPos(vector3s(0, 0, -11), true);

		DrawData* word_second_drawData = RenderMgr::GetNewDrawData(nullptr, true, E_BatchingType::BatchingType_UI, false);
		word_second_drawData->SetSize(vector2s(WORD_SECOND_TEX_SIZE_X, WORD_SECOND_TEX_SIZE_Y));
		ShaderParam_Normal* tempWord_shaderParam = new ShaderParam_Normal();
		tempWord_shaderParam->shaderProgram = GameMgr::GetInstance()->shaderMgr->GetShaderProgram(E_Shader::Shader_Normal);
		tempWord_shaderParam->texNo = GameMgr::GetInstance()->textureMgr->GetTextureID(E_Texture::Texture_UI_HUD_Package_Test_2);
		tempWord_shaderParam->usePreComputeUV = true;
		tempWord_shaderParam->preComputeUV = vector2s(WORD_SECOND_TEX_START_X, WORD_SECOND_TEX_START_Y);
		tempWord_shaderParam->preComputeUVSize = vector2s(WORD_SECOND_TEX_SIZE_X, WORD_SECOND_TEX_SIZE_Y);
		tempWord_shaderParam->tex_texel_size = ui_tex_size;

		word_second_drawData->SetShaderParam(tempWord_shaderParam);

		word_second_sprite->SetDrawData(word_second_drawData);

		word_second_sprite->ShowUI(false, -1, true);

		RegisterUI(word_second_sprite);
		GameMgr::GetInstance()->sceneObjMgr->RegisterCell(word_second_sprite);

		delete tempWord_shaderParam;

		return word_second_sprite;
	}
	else if (uiContentType == E_UIContent::UIContent_Word_Boost) {
		UISprite* word_boost_sprite = new UISprite();
		word_boost_sprite->SetUIContentType(E_UIContent::UIContent_Word_Boost);
		word_boost_sprite->SetID(GameMgr::GetInstance()->sceneObjMgr->GetID());
		word_boost_sprite->SetPos(vector3s(0, 0, -11), true);

		DrawData* word_boost_drawData = RenderMgr::GetNewDrawData(nullptr, true, E_BatchingType::BatchingType_UI, false);
		word_boost_drawData->SetSize(vector2s(WORD_BOOST_TEX_SIZE_X, WORD_BOOST_TEX_SIZE_Y));
		ShaderParam_Normal* tempWord_shaderParam = new ShaderParam_Normal();
		tempWord_shaderParam->shaderProgram = GameMgr::GetInstance()->shaderMgr->GetShaderProgram(E_Shader::Shader_Normal);
		tempWord_shaderParam->texNo = GameMgr::GetInstance()->textureMgr->GetTextureID(E_Texture::Texture_UI_HUD_Package_Test_2);
		tempWord_shaderParam->usePreComputeUV = true;
		tempWord_shaderParam->preComputeUV = vector2s(WORD_BOOST_TEX_START_X, WORD_BOOST_TEX_START_Y);
		tempWord_shaderParam->preComputeUVSize = vector2s(WORD_BOOST_TEX_SIZE_X, WORD_BOOST_TEX_SIZE_Y);
		tempWord_shaderParam->tex_texel_size = ui_tex_size;

		word_boost_drawData->SetShaderParam(tempWord_shaderParam);

		word_boost_sprite->SetDrawData(word_boost_drawData);

		word_boost_sprite->ShowUI(false, -1, true);

		RegisterUI(word_boost_sprite);
		GameMgr::GetInstance()->sceneObjMgr->RegisterCell(word_boost_sprite);

		delete tempWord_shaderParam;

		return word_boost_sprite;
	}
	else if (uiContentType == E_UIContent::UIContent_Word_Auto) {
		UISprite* word_stop_sprite = new UISprite();
		word_stop_sprite->SetUIContentType(E_UIContent::UIContent_Word_Boost);
		word_stop_sprite->SetID(GameMgr::GetInstance()->sceneObjMgr->GetID());
		word_stop_sprite->SetPos(vector3s(0, 0, -11), true);

		DrawData* word_stop_drawData = RenderMgr::GetNewDrawData(nullptr, true, E_BatchingType::BatchingType_UI, false);
		word_stop_drawData->SetSize(vector2s(WORD_STOP_TEX_SIZE_X, WORD_STOP_TEX_SIZE_Y));
		ShaderParam_Normal* tempWord_shaderParam = new ShaderParam_Normal();
		tempWord_shaderParam->shaderProgram = GameMgr::GetInstance()->shaderMgr->GetShaderProgram(E_Shader::Shader_Normal);
		tempWord_shaderParam->texNo = GameMgr::GetInstance()->textureMgr->GetTextureID(E_Texture::Texture_UI_HUD_Package_Test_2);
		tempWord_shaderParam->usePreComputeUV = true;
		tempWord_shaderParam->preComputeUV = vector2s(WORD_STOP_TEX_START_X, WORD_STOP_TEX_START_Y);
		tempWord_shaderParam->preComputeUVSize = vector2s(WORD_STOP_TEX_SIZE_X, WORD_STOP_TEX_SIZE_Y);
		tempWord_shaderParam->tex_texel_size = ui_tex_size;

		word_stop_drawData->SetShaderParam(tempWord_shaderParam);

		word_stop_sprite->SetDrawData(word_stop_drawData);

		word_stop_sprite->ShowUI(false, -1, true);

		RegisterUI(word_stop_sprite);
		GameMgr::GetInstance()->sceneObjMgr->RegisterCell(word_stop_sprite);

		delete tempWord_shaderParam;

		return word_stop_sprite;
	}
	else if (uiContentType == E_UIContent::UIContent_Word_Stop) {
		UISprite* word_stop_sprite = new UISprite();
		word_stop_sprite->SetUIContentType(E_UIContent::UIContent_Word_Stop);
		word_stop_sprite->SetID(GameMgr::GetInstance()->sceneObjMgr->GetID());
		word_stop_sprite->SetPos(vector3s(0, 0, -11), true);

		DrawData* word_stop_drawData = RenderMgr::GetNewDrawData(nullptr, true, E_BatchingType::BatchingType_UI, false);
		word_stop_drawData->SetSize(vector2s(WORD_STOP_TEX_SIZE_X, WORD_STOP_TEX_SIZE_Y));
		ShaderParam_Normal* tempWord_shaderParam = new ShaderParam_Normal();
		tempWord_shaderParam->shaderProgram = GameMgr::GetInstance()->shaderMgr->GetShaderProgram(E_Shader::Shader_Normal);
		tempWord_shaderParam->texNo = GameMgr::GetInstance()->textureMgr->GetTextureID(E_Texture::Texture_UI_HUD_Package_Test_2);
		tempWord_shaderParam->usePreComputeUV = true;
		tempWord_shaderParam->preComputeUV = vector2s(WORD_STOP_TEX_START_X, WORD_STOP_TEX_START_Y);
		tempWord_shaderParam->preComputeUVSize = vector2s(WORD_STOP_TEX_SIZE_X, WORD_STOP_TEX_SIZE_Y);
		tempWord_shaderParam->tex_texel_size = ui_tex_size;

		word_stop_drawData->SetShaderParam(tempWord_shaderParam);

		word_stop_sprite->SetDrawData(word_stop_drawData);

		word_stop_sprite->ShowUI(false, -1, true);

		RegisterUI(word_stop_sprite);
		GameMgr::GetInstance()->sceneObjMgr->RegisterCell(word_stop_sprite);

		delete tempWord_shaderParam;

		return word_stop_sprite;
	}
	else if (uiContentType == E_UIContent::UIContent_Word_Attacking) {
		UISprite* word_attacking_sprite = new UISprite();
		word_attacking_sprite->SetUIContentType(E_UIContent::UIContent_Word_Attacking);
		word_attacking_sprite->SetID(GameMgr::GetInstance()->sceneObjMgr->GetID());
		word_attacking_sprite->SetPos(vector3s(0, 0, -11), true);

		DrawData* word_attacking_drawData = RenderMgr::GetNewDrawData(nullptr, true, E_BatchingType::BatchingType_UI, false);
		word_attacking_drawData->SetSize(vector2s(WORD_ATTACKING_TEX_SIZE_X, WORD_ATTACKING_TEX_SIZE_Y));
		ShaderParam_Normal* tempWord_shaderParam = new ShaderParam_Normal();
		tempWord_shaderParam->shaderProgram = GameMgr::GetInstance()->shaderMgr->GetShaderProgram(E_Shader::Shader_Normal);
		tempWord_shaderParam->texNo = GameMgr::GetInstance()->textureMgr->GetTextureID(E_Texture::Texture_UI_HUD_Package_Test_2);
		tempWord_shaderParam->usePreComputeUV = true;
		tempWord_shaderParam->preComputeUV = vector2s(WORD_ATTACKING_TEX_START_X, WORD_ATTACKING_TEX_START_Y);
		tempWord_shaderParam->preComputeUVSize = vector2s(WORD_ATTACKING_TEX_SIZE_X, WORD_ATTACKING_TEX_SIZE_Y);
		tempWord_shaderParam->tex_texel_size = ui_tex_size;

		word_attacking_drawData->SetShaderParam(tempWord_shaderParam);

		word_attacking_sprite->SetDrawData(word_attacking_drawData);

		word_attacking_sprite->ShowUI(false, -1, true);

		RegisterUI(word_attacking_sprite);
		GameMgr::GetInstance()->sceneObjMgr->RegisterCell(word_attacking_sprite);

		delete tempWord_shaderParam;

		return word_attacking_sprite;
	}
	else if (uiContentType == E_UIContent::UIContent_Word_Range) {
		UISprite* word_range_sprite = new UISprite();
		word_range_sprite->SetUIContentType(E_UIContent::UIContent_Word_Attacking);
		word_range_sprite->SetID(GameMgr::GetInstance()->sceneObjMgr->GetID());
		word_range_sprite->SetPos(vector3s(0, 0, -11), true);

		DrawData* word_range_drawData = RenderMgr::GetNewDrawData(nullptr, true, E_BatchingType::BatchingType_UI, false);
		word_range_drawData->SetSize(vector2s(WORD_RANGE_TEX_SIZE_X, WORD_RANGE_TEX_SIZE_Y));
		ShaderParam_Normal* tempWord_shaderParam = new ShaderParam_Normal();
		tempWord_shaderParam->shaderProgram = GameMgr::GetInstance()->shaderMgr->GetShaderProgram(E_Shader::Shader_Normal);
		tempWord_shaderParam->texNo = GameMgr::GetInstance()->textureMgr->GetTextureID(E_Texture::Texture_UI_HUD_Package_Test_2);
		tempWord_shaderParam->usePreComputeUV = true;
		tempWord_shaderParam->preComputeUV = vector2s(WORD_RANGE_TEX_START_X, WORD_RANGE_TEX_START_Y);
		tempWord_shaderParam->preComputeUVSize = vector2s(WORD_RANGE_TEX_SIZE_X, WORD_RANGE_TEX_SIZE_Y);
		tempWord_shaderParam->tex_texel_size = ui_tex_size;

		word_range_drawData->SetShaderParam(tempWord_shaderParam);

		word_range_sprite->SetDrawData(word_range_drawData);

		word_range_sprite->ShowUI(false, -1, true);

		RegisterUI(word_range_sprite);
		GameMgr::GetInstance()->sceneObjMgr->RegisterCell(word_range_sprite);

		delete tempWord_shaderParam;

		return word_range_sprite;
	}
	else if (uiContentType == E_UIContent::UIContent_Word_Time) {
		UISprite* word_time_sprite = new UISprite();
		word_time_sprite->SetUIContentType(E_UIContent::UIContent_Word_Attacking);
		word_time_sprite->SetID(GameMgr::GetInstance()->sceneObjMgr->GetID());
		word_time_sprite->SetPos(vector3s(0, 0, -11), true);

		DrawData* word_time_drawData = RenderMgr::GetNewDrawData(nullptr, true, E_BatchingType::BatchingType_UI, false);
		word_time_drawData->SetSize(vector2s(WORD_TIME_TEX_SIZE_X, WORD_TIME_TEX_SIZE_Y));
		ShaderParam_Normal* tempWord_shaderParam = new ShaderParam_Normal();
		tempWord_shaderParam->shaderProgram = GameMgr::GetInstance()->shaderMgr->GetShaderProgram(E_Shader::Shader_Normal);
		tempWord_shaderParam->texNo = GameMgr::GetInstance()->textureMgr->GetTextureID(E_Texture::Texture_UI_HUD_Package_Test_2);
		tempWord_shaderParam->usePreComputeUV = true;
		tempWord_shaderParam->preComputeUV = vector2s(WORD_TIME_TEX_START_X, WORD_TIME_TEX_START_Y);
		tempWord_shaderParam->preComputeUVSize = vector2s(WORD_TIME_TEX_SIZE_X, WORD_TIME_TEX_SIZE_Y);
		tempWord_shaderParam->tex_texel_size = ui_tex_size;

		word_time_drawData->SetShaderParam(tempWord_shaderParam);

		word_time_sprite->SetDrawData(word_time_drawData);

		word_time_sprite->ShowUI(false, -1, true);

		RegisterUI(word_time_sprite);
		GameMgr::GetInstance()->sceneObjMgr->RegisterCell(word_time_sprite);

		delete tempWord_shaderParam;

		return word_time_sprite;
	}
	else if (uiContentType == E_UIContent::UIContent_Card_Bg) {
		//bg
		DrawData* card_bg_DrawData = RenderMgr::GetNewDrawData(nullptr, true, E_BatchingType::BatchingType_UI, false);

		card_bg_DrawData->SetSize(vector2s(CARD_2_IMG_TEX_SIZE_X, CARD_2_IMG_TEX_SIZE_Y));
		card_bg_DrawData->SetShaderParam(sprite_cell_bg_shaderParam);

		UISprite* card_bg = new UISprite();
		card_bg->SetID(GameMgr::GetInstance()->sceneObjMgr->GetID());
		card_bg->SetUIContentType(E_UIContent::UIContent_Card_Bg);
		card_bg->SetDrawData(card_bg_DrawData);
		card_bg->SetScale(vector3s(1.0f, 1.0f, 1.0f));

		RegisterUI(card_bg);
		GameMgr::GetInstance()->sceneObjMgr->RegisterCell(card_bg);

		return card_bg;
	}
	else if (uiContentType == E_UIContent::UIContent_Card_Extra_Bg) {
		//bg_extra
		DrawData* card_bg_DrawData = RenderMgr::GetNewDrawData(nullptr, true, E_BatchingType::BatchingType_UI, false);

		card_bg_DrawData->SetSize(vector2s(CARD_3_IMG_TEX_SIZE_X, CARD_3_IMG_TEX_SIZE_Y));
		card_bg_DrawData->SetShaderParam(sprite_extra_cell_bg_shaderParam);

		UISprite* card_bg = new UISprite();
		card_bg->SetID(GameMgr::GetInstance()->sceneObjMgr->GetID());
		card_bg->SetUIContentType(E_UIContent::UIContent_Card_Extra_Bg);
		card_bg->SetDrawData(card_bg_DrawData);
		card_bg->SetScale(vector3s(1.0f, 1.0f, 1.0f));

		RegisterUI(card_bg);
		GameMgr::GetInstance()->sceneObjMgr->RegisterCell(card_bg);

		return card_bg;
	}

	return nullptr;
}

UI* UIMgr::GetUI(E_UIContent uiContentType)
{
	if (uiPools.find(uiContentType) == uiPools.end()) {
		//create new one
		UI* newUI = CreateUIEntity(uiContentType);
		return newUI;
	}

	std::vector<UI*>::iterator iter;
	for (iter = uiPools[uiContentType].begin(); iter != uiPools[uiContentType].end(); iter++) {
		if ((*iter) == nullptr)continue;

		if ((*iter)->CheckState() == false) {
			(*iter)->Reset();
			return (*iter);
		}
	}

	UI* newUI = CreateUIEntity(uiContentType);
	return newUI;
}

void UIMgr::ShowUI(E_UIContent uiContentType, bool state)
{

}

void UIMgr::ShowTitle(bool show)
{
	if (title_bg != nullptr) {
		DrawData* tempDrawData = title_bg->GetDrawData();
		if (tempDrawData != nullptr) {
			if (show == true) {
				tempDrawData->SetShaderFloatParam(1, 0.0f);
			}
			else {
				tempDrawData->SetShaderFloatParam(1, SCREEN_WIDTH);
			}
		}
		//title_bg->ShowUI(show, -1, true);
	}

	if (title_sprite != nullptr) {
		title_sprite->ShowUI(show, -1, true);
	}

	if (word_start_sprite != nullptr) {
		word_start_sprite->ShowUI(show, -1, true);
	}
	if (word_exit_sprite != nullptr) {
		word_exit_sprite->ShowUI(show, -1, true);
	}
}

void UIMgr::ShowMenu(E_GameState stateMenuType, bool show)
{
	if (title_bg != nullptr) {
		//title_bg->ShowUI(show, -1, true);
	}

	if (stateMenuType == E_GameState::GameState_Game_Menu) {
		//pause
		//item_continue
		//item_retry
		//item_title
		//init pos
		if (word_pause_sprite != nullptr) {
			word_pause_sprite->ShowUI(show, -1, true);
		}

		if (word_continue_sprite != nullptr) {
			word_continue_sprite->SetPos(menu_pause_continue_pos, true);
			word_continue_sprite->ShowUI(show, -1, true);
		}

		if (word_retry_sprite != nullptr) {
			word_retry_sprite->SetPos(menu_pause_retry_pos, true);
			word_retry_sprite->ShowUI(show, -1, true);
		}

		if (word_title_sprite != nullptr) {
			word_title_sprite->SetPos(menu_pause_title_pos, true);
			word_title_sprite->ShowUI(show, -1, true);
		}

	}
	else if (stateMenuType == E_GameState::GameState_Game_Lose) {
		//you lose
		//item_retry
		//item_title

		if (word_you_lose_sprite != nullptr) {
			word_you_lose_sprite->ShowUI(show, -1, true);
		}

		if (word_retry_sprite != nullptr) {
			word_retry_sprite->SetPos(lose_menu_retry_pos, true);
			word_retry_sprite->ShowUI(show, -1, true);
		}

		if (word_title_sprite != nullptr) {
			word_title_sprite->SetPos(lose_menu_title_pos, true);
			word_title_sprite->ShowUI(show, -1, true);
		}

	}
	else if (stateMenuType == E_GameState::GameState_Game_End) {
		//you survivy
		//thanks
		//item_title

		if (word_you_survive_sprite != nullptr) {
			word_you_survive_sprite->ShowUI(show, -1, true);
		}

		if (word_thank_sprite != nullptr) {
			word_thank_sprite = nullptr;
			word_thank_sprite->ShowUI(show, -1, true);
		}

		if (word_title_sprite != nullptr) {
			word_title_sprite->SetPos(win_menu_title_pos, true);
			word_title_sprite->ShowUI(show, -1, true);
		}
	}
}

void UIMgr::ShowCard(int curPlayerLv, int cardIndex, bool show)
{
	bool isExtraCard = false;
	if (cardIndex == 1 && card_1 != nullptr) {

		if (show == true) {
			CardEffectData* card_1_data = GameMgr::GetInstance()->battleMgr->GetCardData(cardIndex, curPlayerLv, isExtraCard);
			ComposeCard(card_1, card_1_data, isExtraCard);
			card_1->ShowUI(show, -1, true);
		}
		else {
			card_1->ClearChild();
			card_1->ShowUI(false, -1, true);
		}
	}
	else if (cardIndex == 2 && card_2 != nullptr) {
		if (show == true) {
			CardEffectData* card_2_data = GameMgr::GetInstance()->battleMgr->GetCardData(cardIndex, curPlayerLv, isExtraCard);
			ComposeCard(card_2, card_2_data, isExtraCard);
			card_2->ShowUI(show, -1, true);
		}
		else {
			card_2->ClearChild();
			card_2->ShowUI(false, -1, true);
		}
	}
	else if (cardIndex == 3 && card_3 != nullptr) {
		if (show == true) {
			CardEffectData* card_3_data = GameMgr::GetInstance()->battleMgr->GetCardData(cardIndex, curPlayerLv, isExtraCard);
			ComposeCard(card_3, card_3_data, isExtraCard);
			card_3->ShowUI(show, -1, true);
		}
		else {
			card_3->ClearChild();
			card_3->ShowUI(false, -1, true);
		}
	}
	else if (cardIndex == 4 && card_4 != nullptr) {
		if (show == true) {
			CardEffectData* card_4_data = GameMgr::GetInstance()->battleMgr->GetCardData(cardIndex, curPlayerLv, isExtraCard);
			ComposeCard(card_4, card_4_data, isExtraCard);
			card_4->ShowUI(show, -1, true);
		}
		else {
			card_4->ClearChild();
			card_4->ShowUI(false, -1, true);
		}
	}
}

void UIMgr::UpdateCard(int cardIndex)
{
	//TODO
}

void UIMgr::CardCollision(vector3s mousePos)
{
	if (card_1 != nullptr) {
		std::map<int, UI*>& childDic = card_1->GetChildDic();
		if (childDic.find(UI_BG_ID) != childDic.end() && childDic[UI_BG_ID] != nullptr) {
			vector3s pos = card_1->GetTransform()->GetPos();
			vector3s tempScale = card_1->GetTransform()->GetScale();
			vector2s size = vector2s(CARD_2_IMG_TEX_SIZE_X, CARD_2_IMG_TEX_SIZE_Y);
			size.x *= tempScale.x;
			size.y *= tempScale.y;
			if (Collider::IsRectContainPoint(pos, size, vector3s(0, 0, 0), mousePos)) {
				curSelectIndex = 0;
				UpdateSelector();
				ShowSelector(E_GameState::GameState_Game_Lv_Up, true);
				return;
			}
		}
	}
	if (card_2 != nullptr) {
		std::map<int, UI*>& childDic = card_2->GetChildDic();
		if (childDic.find(UI_BG_ID) != childDic.end() && childDic[UI_BG_ID] != nullptr) {
			vector3s pos = card_2->GetTransform()->GetPos();
			vector3s tempScale = card_2->GetTransform()->GetScale();
			vector2s size = vector2s(CARD_2_IMG_TEX_SIZE_X, CARD_2_IMG_TEX_SIZE_Y);
			size.x *= tempScale.x;
			size.y *= tempScale.y;
			if (Collider::IsRectContainPoint(pos, size, vector3s(0, 0, 0), mousePos)) {
				curSelectIndex = 1;
				UpdateSelector();
				ShowSelector(E_GameState::GameState_Game_Lv_Up, true);
				return;
			}
		}
	}

	curSelectIndex = -1;
	ShowSelector(E_GameState::GameState_Game_Lv_Up, false);
}

void UIMgr::ShowStatusSelector(bool show)
{
	if (show) {
		//update
		Player* player = GameMgr::GetInstance()->sceneObjMgr->GetPlayer();
		if (player != nullptr) {
			UpdateStatusSelector(player->GetCurBulletType());
		}
		else {
			UpdateStatusSelector(E_Status::Status_Exp);
		}
	}

	if (statusSelector != nullptr) {
		statusSelector->ShowUI(show, -1, true);
	}
}

void UIMgr::UpdateStatusSelector(E_Status curStatus)
{
	if (statusSelector == nullptr)return;

	switch (curStatus) {
	case E_Status::Status_Hp:
		statusSelector->SetPos(selector_pos_status_hp, true);
		break;
	case E_Status::Status_Atk:
		statusSelector->SetPos(selector_pos_status_atk, true);
		break;
	case E_Status::Status_Def:
		statusSelector->SetPos(selector_pos_status_def, true);
		break;
	case E_Status::Status_Exp:
		statusSelector->SetPos(selector_pos_status_exp, true);
		break;
	default:
		break;
	}

}

void UIMgr::SetCrosshairPos(vector3s _pos)
{
	if (crosshair == nullptr)return;
	crosshair->SetPos(_pos, true);
}

void UIMgr::ShowCrosshair(bool _show)
{
	if (crosshair == nullptr)return;
	crosshair->ShowUI(_show, -1, true);
}

void UIMgr::ShowHUD(bool show)
{
	if (hud_status_frame != nullptr) {
		if (show == true) {
			hud_status_frame->Reset();
			hud_status_frame->StartMovement(true);
		}
		else {
			hud_status_frame->Reset();
		}
		hud_status_frame->ShowUI(show, -1, true);
	}

	ShowStatusSelector(show);
}

void UIMgr::ShowGuide(bool show)
{
	return;
	if (guideUI == nullptr) {
		return;
	}

	guideUI->ShowUI(show, -1, true);
}

void UIMgr::UpdateHUD(Status_Player* playerStatus)
{
	if (hud_status_frame == nullptr || playerStatus == nullptr) {
		return;
	}

	std::map<int, UI*>& tempChildDic = hud_status_frame->GetChildDic();
	std::list<E_Status>& changingStatusQueue = playerStatus->GetChangingStatusQueue();
	if (changingStatusQueue.size() == 0)return;

	for (auto& statusType : changingStatusQueue) {

		//cur hp
		if (statusType == E_Status::Status_Hp && tempChildDic.find(UI_HUD_STATUS_CUR_HP) != tempChildDic.end() && tempChildDic[UI_HUD_STATUS_CUR_HP] != nullptr) {
			UIWord* temp = (UIWord*)tempChildDic[UI_HUD_STATUS_CUR_HP];
			if (temp != nullptr) {
				temp->SetWord(playerStatus->GetValue(E_Status::Status_Hp));
			}
		}
		else if (statusType == E_Status::Status_Atk && tempChildDic.find(UI_HUD_STATUS_CUR_ATK) != tempChildDic.end() && tempChildDic[UI_HUD_STATUS_CUR_ATK] != nullptr) {
			UIWord* temp = (UIWord*)tempChildDic[UI_HUD_STATUS_CUR_ATK];
			if (temp != nullptr) {
				temp->SetWord(playerStatus->GetValue(E_Status::Status_Atk));
			}
		}
		else if (statusType == E_Status::Status_Def && tempChildDic.find(UI_HUD_STATUS_CUR_DEF) != tempChildDic.end() && tempChildDic[UI_HUD_STATUS_CUR_DEF] != nullptr) {
			UIWord* temp = (UIWord*)tempChildDic[UI_HUD_STATUS_CUR_DEF];
			if (temp != nullptr) {
				temp->SetWord(playerStatus->GetValue(E_Status::Status_Def));
			}
		}
		else if (statusType == E_Status::Status_Exp && tempChildDic.find(UI_HUD_STATUS_CUR_EXP) != tempChildDic.end() && tempChildDic[UI_HUD_STATUS_CUR_EXP] != nullptr) {
			UIWord* temp = (UIWord*)tempChildDic[UI_HUD_STATUS_CUR_EXP];
			if (temp != nullptr) {
				temp->SetWord(playerStatus->GetValue(E_Status::Status_Exp));
			}
		}
		else if (statusType == E_Status::Status_Max_Hp && tempChildDic.find(UI_HUD_STATUS_MAX_HP) != tempChildDic.end() && tempChildDic[UI_HUD_STATUS_MAX_HP] != nullptr) {
			UIWord* temp = (UIWord*)tempChildDic[UI_HUD_STATUS_MAX_HP];
			if (temp != nullptr) {
				temp->SetWord(playerStatus->GetValue(E_Status::Status_Max_Hp));
			}
		}
		else if (statusType == E_Status::Status_Max_Atk && tempChildDic.find(UI_HUD_STATUS_MAX_ATK) != tempChildDic.end() && tempChildDic[UI_HUD_STATUS_MAX_ATK] != nullptr) {
			UIWord* temp = (UIWord*)tempChildDic[UI_HUD_STATUS_MAX_ATK];
			if (temp != nullptr) {
				temp->SetWord(playerStatus->GetValue(E_Status::Status_Max_Atk));
			}
		}
		else if (statusType == E_Status::Status_Max_Def && tempChildDic.find(UI_HUD_STATUS_MAX_DEF) != tempChildDic.end() && tempChildDic[UI_HUD_STATUS_MAX_DEF] != nullptr) {
			UIWord* temp = (UIWord*)tempChildDic[UI_HUD_STATUS_MAX_DEF];
			if (temp != nullptr) {
				temp->SetWord(playerStatus->GetValue(E_Status::Status_Max_Def));
			}
		}
		else if (statusType == E_Status::Status_Max_Exp && tempChildDic.find(UI_HUD_STATUS_MAX_EXP) != tempChildDic.end() && tempChildDic[UI_HUD_STATUS_MAX_EXP] != nullptr) {
			UIWord* temp = (UIWord*)tempChildDic[UI_HUD_STATUS_MAX_EXP];
			if (temp != nullptr) {
				temp->SetWord(playerStatus->GetValue(E_Status::Status_Max_Exp));
			}
		}
		else if (statusType == E_Status::Status_Level && tempChildDic.find(UI_HUD_STATUS_LV_CUR) != tempChildDic.end() && tempChildDic[UI_HUD_STATUS_LV_CUR] != nullptr) {
			UIWord* temp = (UIWord*)tempChildDic[UI_HUD_STATUS_LV_CUR];
			if (temp != nullptr) {
				temp->SetWord(playerStatus->GetValue(E_Status::Status_Level));
			}
		}
		else if (statusType == E_Status::Status_Consume_Hp && tempChildDic.find(UI_HUD_STATUS_HP_CONSUME) != tempChildDic.end() && tempChildDic[UI_HUD_STATUS_HP_CONSUME] != nullptr) {
			UIWord* temp = (UIWord*)tempChildDic[UI_HUD_STATUS_HP_CONSUME];
			if (temp != nullptr) {
				std::stringstream ss;
				int consumeValue = playerStatus->GetValue(E_Status::Status_Consume_Hp);
				ss << "-";
				ss << consumeValue;
				std::string valueStr;
				ss >> valueStr;

				temp->SetWord(valueStr);
			}
		}
		else if (statusType == E_Status::Status_Consume_Atk && tempChildDic.find(UI_HUD_STATUS_ATK_CONSUME) != tempChildDic.end() && tempChildDic[UI_HUD_STATUS_ATK_CONSUME] != nullptr) {
			UIWord* temp = (UIWord*)tempChildDic[UI_HUD_STATUS_ATK_CONSUME];
			if (temp != nullptr) {
				std::stringstream ss;
				int consumeValue = playerStatus->GetValue(E_Status::Status_Consume_Atk);
				ss << "-";
				ss << consumeValue;
				std::string valueStr;
				ss >> valueStr;

				temp->SetWord(valueStr);
			}
		}
		else if (statusType == E_Status::Status_Consume_Def && tempChildDic.find(UI_HUD_STATUS_DEF_CONSUME) != tempChildDic.end() && tempChildDic[UI_HUD_STATUS_DEF_CONSUME] != nullptr) {
			UIWord* temp = (UIWord*)tempChildDic[UI_HUD_STATUS_DEF_CONSUME];
			if (temp != nullptr) {
				std::stringstream ss;
				int consumeValue = playerStatus->GetValue(E_Status::Status_Consume_Def);
				ss << "-";
				ss << consumeValue;
				std::string valueStr;
				ss >> valueStr;

				temp->SetWord(valueStr);
			}
		}
		else if (statusType == E_Status::Status_Consume_Exp && tempChildDic.find(UI_HUD_STATUS_EXP_CONSUME) != tempChildDic.end() && tempChildDic[UI_HUD_STATUS_EXP_CONSUME] != nullptr) {
			UIWord* temp = (UIWord*)tempChildDic[UI_HUD_STATUS_EXP_CONSUME];
			if (temp != nullptr) {

				std::stringstream ss;
				int consumeValue = playerStatus->GetValue(E_Status::Status_Consume_Exp);
				ss << "-";
				ss << consumeValue;
				std::string valueStr;
				ss >> valueStr;

				temp->SetWord(valueStr);
			}
		}
	}
}

void UIMgr::UpdateHUD(E_Status statusType, int value) {
	if (hud_status_frame == nullptr)return;
	std::map<int, UI*> childDic = hud_status_frame->GetChildDic();
	if (statusType == E_Status::Status_Hp
		&& childDic.find(UI_HUD_STATUS_CUR_HP) != childDic.end() && childDic[UI_HUD_STATUS_CUR_HP] != nullptr) {
		UIWord* temp = (UIWord*)childDic[UI_HUD_STATUS_CUR_HP];
		if (temp != nullptr) {
			temp->SetWord(value);
		}
	}
	else if (statusType == E_Status::Status_Atk
		&& childDic.find(UI_HUD_STATUS_CUR_ATK) != childDic.end() && childDic[UI_HUD_STATUS_CUR_ATK] != nullptr) {
		UIWord* temp = (UIWord*)childDic[UI_HUD_STATUS_CUR_ATK];
		if (temp != nullptr) {
			temp->SetWord(value);
		}
	}
	else if (statusType == E_Status::Status_Def
		&& childDic.find(UI_HUD_STATUS_CUR_DEF) != childDic.end() && childDic[UI_HUD_STATUS_CUR_DEF] != nullptr) {
		UIWord* temp = (UIWord*)childDic[UI_HUD_STATUS_CUR_DEF];
		if (temp != nullptr) {
			temp->SetWord(value);
		}
	}
	else if (statusType == E_Status::Status_Exp
		&& childDic.find(UI_HUD_STATUS_CUR_EXP) != childDic.end() && childDic[UI_HUD_STATUS_CUR_EXP] != nullptr) {
		UIWord* temp = (UIWord*)childDic[UI_HUD_STATUS_CUR_EXP];
		if (temp != nullptr) {
			temp->SetWord(value);
		}
	}
	else if (statusType == E_Status::Status_Max_Hp
		&& childDic.find(UI_HUD_STATUS_MAX_HP) != childDic.end() && childDic[UI_HUD_STATUS_MAX_HP] != nullptr) {
		UIWord* temp = (UIWord*)childDic[UI_HUD_STATUS_MAX_HP];
		if (temp != nullptr) {
			temp->SetWord(value);
		}
	}
	else if (statusType == E_Status::Status_Max_Atk
		&& childDic.find(UI_HUD_STATUS_MAX_ATK) != childDic.end() && childDic[UI_HUD_STATUS_MAX_ATK] != nullptr) {
		UIWord* temp = (UIWord*)childDic[UI_HUD_STATUS_MAX_ATK];
		if (temp != nullptr) {
			temp->SetWord(value);
		}
	}
	else if (statusType == E_Status::Status_Max_Def
		&& childDic.find(UI_HUD_STATUS_MAX_DEF) != childDic.end() && childDic[UI_HUD_STATUS_MAX_DEF] != nullptr) {
		UIWord* temp = (UIWord*)childDic[UI_HUD_STATUS_MAX_DEF];
		if (temp != nullptr) {
			temp->SetWord(value);
		}
	}
	else if (statusType == E_Status::Status_Max_Exp
		&& childDic.find(UI_HUD_STATUS_MAX_EXP) != childDic.end() && childDic[UI_HUD_STATUS_MAX_EXP] != nullptr) {
		UIWord* temp = (UIWord*)childDic[UI_HUD_STATUS_MAX_EXP];
		if (temp != nullptr) {
			temp->SetWord(value);
		}
	}
	else if (statusType == E_Status::Status_Consume_Hp
		&& childDic.find(UI_HUD_STATUS_MAX_EXP) != childDic.end() && childDic[UI_HUD_STATUS_MAX_EXP] != nullptr) {

	}
	else if (statusType == E_Status::Status_Consume_Atk
		&& childDic.find(UI_HUD_STATUS_MAX_EXP) != childDic.end() && childDic[UI_HUD_STATUS_MAX_EXP] != nullptr) {

	}
	else if (statusType == E_Status::Status_Consume_Def
		&& childDic.find(UI_HUD_STATUS_MAX_EXP) != childDic.end() && childDic[UI_HUD_STATUS_MAX_EXP] != nullptr) {

	}
	else if (statusType == E_Status::Status_Consume_Exp
		&& childDic.find(UI_HUD_STATUS_MAX_EXP) != childDic.end() && childDic[UI_HUD_STATUS_MAX_EXP] != nullptr) {

	}
}

void UIMgr::ShowGameTimer(bool show)
{
	if (hud_game_timer == nullptr)return;
	if (show) {
		UpdateGameTimer();
	}
	hud_game_timer->ShowUI(show, -1, true);
}

void UIMgr::UpdateGameTimer()
{
	if (hud_game_timer == nullptr || hud_game_timer->CheckState() == false) {
		return;
	}

	int minute = GameMgr::GetInstance()->timer->GetCountDownMinute();
	int second = GameMgr::GetInstance()->timer->GetCountDownSecond();

	std::stringstream ss;
	std::map<int, UI*>& tempChildDic = hud_game_timer->GetChildDic();
	if (tempChildDic.find(UI_GAME_TIMER_MINUTE) != tempChildDic.end() && tempChildDic[UI_GAME_TIMER_MINUTE] != nullptr) {
		if (minute < 10) {
			ss.clear();
			std::string strValue;
			ss << "0";
			ss << minute;
			ss >> strValue;

			((UIWord*)tempChildDic[UI_GAME_TIMER_MINUTE])->SetWord(strValue);
		}
		else {
			((UIWord*)tempChildDic[UI_GAME_TIMER_MINUTE])->SetWord(minute);
		}
	}
	if (tempChildDic.find(UI_GAME_TIMER_SECOND) != tempChildDic.end() && tempChildDic[UI_GAME_TIMER_SECOND] != nullptr) {
		if (second < 10) {
			ss.clear();
			std::string strValue;
			ss << "0";
			ss << second;
			ss >> strValue;

			((UIWord*)tempChildDic[UI_GAME_TIMER_SECOND])->SetWord(strValue);
		}
		else {
			((UIWord*)tempChildDic[UI_GAME_TIMER_SECOND])->SetWord(second);
		}
	}

}

void UIMgr::ShowLevelUpImg(bool show, vector3s pos)
{
	if (lv_up_sprite != nullptr) {
		if (show) {
			lv_up_sprite->SetPos(pos + vector3s(0.0f, 60.0f, 0.0f), true);
			lv_up_sprite->ShowUI(true, 5.0f, true);
		}
	}
}

void UIMgr::ShowLvUpPanel(bool show, int playerCurLv)
{
	//TODO
	//show card:card_1 card_2
	//show lv status panel
	this->ShowCard(playerCurLv, 1, show);
	this->ShowCard(playerCurLv, 2, show);
	this->ShowCard(playerCurLv, 3, show);
	this->ShowCard(playerCurLv, 4, show);
	//show selector
	//if (card_1 != nullptr && show == true ) {
	//	vector3s defaultSelectorPos = card_1->GetTransform()->GetPos();
	//	vector2s defaultSelectorSize = vector2s(CARD_2_IMG_TEX_SIZE_X * 0.75f, CARD_2_IMG_TEX_SIZE_Y* 0.9f );
	//	SetSelectorParam(defaultSelectorPos, vector3s(2, 2, 1), defaultSelectorSize);
	//}
	//ShowSelector(show);
	if (show) {
		curStateSelect = E_GameState::GameState_Game_Lv_Up;
	}
	else {
		curStateSelect = E_GameState::GameState_None;
	}

	if (lv_panel != nullptr) {
		if (show == true) {
			lv_panel->Reset();
			lv_panel->StartMovement(true);
		}
		else {
			lv_panel->Reset();
		}
		lv_panel->ShowUI(show, -1, true);
	}


}

void UIMgr::UpdateLvPanel(Status_Player* playerStatus, bool preStatusData)
{
	if (playerStatus == nullptr || lv_panel == nullptr)return;

	std::map<int, UI*>& tempChildDic = lv_panel->GetChildDic();

	int lvID = preStatusData ? UI_LV_PANEL_STATUS_LV_PRE : UI_LV_PANEL_STATUS_LV_CUR;
	int hpID = preStatusData ? UI_LV_PANEL_STATUS_HP_PRE : UI_LV_PANEL_STATUS_HP_CUR;
	int atkID = preStatusData ? UI_LV_PANEL_STATUS_ATK_PRE : UI_LV_PANEL_STATUS_ATK_CUR;
	int defID = preStatusData ? UI_LV_PANEL_STATUS_DEF_PRE : UI_LV_PANEL_STATUS_DEF_CUR;


	if (tempChildDic.find(lvID) != tempChildDic.end() && tempChildDic[lvID] != nullptr) {
		UIWord* temp = (UIWord*)tempChildDic[lvID];
		if (temp != nullptr) {
			temp->SetWord(playerStatus->GetValue(E_Status::Status_Level));
		}
	}
	if (tempChildDic.find(hpID) != tempChildDic.end() && tempChildDic[hpID] != nullptr) {
		UIWord* temp = (UIWord*)tempChildDic[hpID];
		if (temp != nullptr) {
			temp->SetWord(playerStatus->GetValue(E_Status::Status_Max_Hp));
		}
	}
	if (tempChildDic.find(atkID) != tempChildDic.end() && tempChildDic[atkID] != nullptr) {
		UIWord* temp = (UIWord*)tempChildDic[atkID];
		if (temp != nullptr) {
			temp->SetWord(playerStatus->GetValue(E_Status::Status_Max_Atk));
		}
	}
	if (tempChildDic.find(defID) != tempChildDic.end() && tempChildDic[defID] != nullptr) {
		UIWord* temp = (UIWord*)tempChildDic[defID];
		if (temp != nullptr) {
			temp->SetWord(playerStatus->GetValue(E_Status::Status_Max_Def));
		}
	}
}

void UIMgr::InitSelectorParam(E_GameState gameStateItemType)
{
	if (gameStateItemType == E_GameState::GameState_Game_Title) {
		curSelectIndex = DEFAULT_TITLE_ITEM_INDEX;
	}
	else if (gameStateItemType == E_GameState::GameState_Game_Lv_Up) {
		curSelectIndex = DEFAULT_CARD_INDEX;
	}
	else if (gameStateItemType == E_GameState::GameState_Game_Menu) {
		curSelectIndex = DEFAULT_MENU_INDEX;
	}
	else if (gameStateItemType == E_GameState::GameState_Game_Lose) {
		curSelectIndex = DEFAULT_LOSE_MENU_INDEX;
	}
	else if (gameStateItemType == E_GameState::GameState_Game_End) {
		curSelectIndex = DEFAULT_WIN_MENU_INDEX;
	}

	curStateSelect = gameStateItemType;

	UpdateSelector();
}

void UIMgr::UpdateSelector()
{
	UI* curItem = GetItem(curStateSelect, curSelectIndex);
	if (curItem == nullptr)return;

	if (curStateSelect == E_GameState::GameState_Game_Title) {
		vector3s defaultSelectorPos = curItem->GetTransform()->GetPos();
		vector2s tempSize = curItem->GetSize(true);
		vector2s defaultSelectorSize = vector2s(SELECTOR_TITLE_CONST_SIZE_X, tempSize.y * 0.25f);
		SetSelectorParam(defaultSelectorPos, vector3s(2, 2, 1), defaultSelectorSize);
	}
	else if (curStateSelect == E_GameState::GameState_Game_Lv_Up) {
		vector3s defaultSelectorPos = curItem->GetTransform()->GetPos();
		defaultSelectorPos.z -= 10.0f;
		vector2s defaultSelectorSize = vector2s(CARD_2_IMG_TEX_SIZE_X * 0.75f, CARD_2_IMG_TEX_SIZE_Y * 0.9f);
		SetSelectorParam(defaultSelectorPos, vector3s(2, 2, 1), defaultSelectorSize);
	}
	else if (curStateSelect == E_GameState::GameState_Game_Menu) {
		vector3s defaultSelectorPos = curItem->GetTransform()->GetPos();
		vector2s tempSize = curItem->GetSize(true);
		vector2s defaultSelectorSize = vector2s(SELECTOR_MENU_CONST_SIZE_X, tempSize.y * 0.25f);
		SetSelectorParam(defaultSelectorPos, vector3s(2, 2, 1), defaultSelectorSize);
	}
	else if (curStateSelect == E_GameState::GameState_Game_Lose) {
		vector3s defaultSelectorPos = curItem->GetTransform()->GetPos();
		vector2s tempSize = curItem->GetSize(true);
		vector2s defaultSelectorSize = vector2s(SELECTOR_MENU_CONST_SIZE_X, tempSize.y * 0.25f);
		SetSelectorParam(defaultSelectorPos, vector3s(2, 2, 1), defaultSelectorSize);
	}
	else if (curStateSelect == E_GameState::GameState_Game_End) {
		vector3s defaultSelectorPos = curItem->GetTransform()->GetPos();
		vector2s tempSize = curItem->GetSize(true);
		vector2s defaultSelectorSize = vector2s(SELECTOR_MENU_CONST_SIZE_X, tempSize.y * 0.25f);
		SetSelectorParam(defaultSelectorPos, vector3s(2, 2, 1), defaultSelectorSize);
	}
}

void UIMgr::SetSelectorParam(vector3s pos, vector3s scale, vector2s sizeOffset)
{
	if (uiSelector == nullptr)return;
	uiSelector->SetPos(pos, true);
	uiSelector->SetScale(scale);

	vector3s offset = vector3s(0, 0, -1);
	//lt
	offset.x = -sizeOffset.x * 0.5f;
	offset.y = sizeOffset.y * 0.5f;
	uiSelector->SetChildOffset(UI_SELECTOR_LT_ID, offset);

	//rt
	offset.x = sizeOffset.x * 0.5f;
	offset.y = sizeOffset.y * 0.5f;
	uiSelector->SetChildOffset(UI_SELECTOR_RT_ID, offset);

	//lb
	offset.x = -sizeOffset.x * 0.5f;
	offset.y = -sizeOffset.y * 0.5f;
	uiSelector->SetChildOffset(UI_SELECTOR_LB_ID, offset);

	//rb
	offset.x = sizeOffset.x * 0.5f;
	offset.y = -sizeOffset.y * 0.5f;
	uiSelector->SetChildOffset(UI_SELECTOR_RB_ID, offset);
}

void UIMgr::SwitchNextItem(bool forward)
{
	if (curStateSelect == E_GameState::GameState_Game_Title) {
		curSelectIndex = (forward ? curSelectIndex + 1 : curSelectIndex + TITLE_ITEM_COUNT - 1) % TITLE_ITEM_COUNT;
	}
	else if (curStateSelect == E_GameState::GameState_Game_Lv_Up) {
		curSelectIndex = (forward ? curSelectIndex + 1 : curSelectIndex + CARD_COUNT - 1) % CARD_COUNT;
	}
	else if (curStateSelect == E_GameState::GameState_Game_Menu) {
		curSelectIndex = (forward ? curSelectIndex + 1 : curSelectIndex + MENU_ITEM_COUNT - 1) % MENU_ITEM_COUNT;
	}
	else if (curStateSelect == E_GameState::GameState_Game_Lose) {
		curSelectIndex = (forward ? curSelectIndex + 1 : curSelectIndex + LOSE_MENU_ITEM_COUNT - 1) % LOSE_MENU_ITEM_COUNT;
	}
	else if (curStateSelect == E_GameState::GameState_Game_End) {
		curSelectIndex = (forward ? curSelectIndex + 1 : curSelectIndex + WIN_MENU_ITEM_COUNT - 1) % WIN_MENU_ITEM_COUNT;
	}

	UpdateSelector();
}

int UIMgr::GetCurSelectItem()
{
	return curSelectIndex;
}

void UIMgr::SelectCurItem()
{
	if (curStateSelect == E_GameState::GameState_Game_Title) {
		return;//empty
	}
	else if (curStateSelect == E_GameState::GameState_Game_Lv_Up) {
		if (curSelectIndex == 0) {//card 1
			GameMgr::GetInstance()->battleMgr->SelectCard(1);
		}
		else if (curSelectIndex == 1) {//card 2
			GameMgr::GetInstance()->battleMgr->SelectCard(2);
		}
		else if (curSelectIndex == 2) {//card 3
			GameMgr::GetInstance()->battleMgr->SelectCard(3);
		}
		else if (curSelectIndex == 3) {//card 4
			GameMgr::GetInstance()->battleMgr->SelectCard(4);
		}
	}
	else if (curStateSelect == E_GameState::GameState_Game_Menu) {
		return;//empty
	}
	else if (curStateSelect == E_GameState::GameState_Game_Lose) {
		return;//empty
	}
	else if (curStateSelect == E_GameState::GameState_Game_End) {
		return;//empty
	}


}

void UIMgr::ShowSelector(E_GameState stateSelectType, bool show)
{
	if (uiSelector == nullptr)return;
	if (show == true) {
		//init
		InitSelectorParam(stateSelectType);
	}
	uiSelector->ShowUI(show, -1, true);
}

UI* UIMgr::GetItem(E_GameState stateSelectType, int index)
{
	if (stateSelectType == E_GameState::GameState_Game_Title) {
		if (index == 0) {
			//start
			return word_start_sprite;
		}
		else if (index == 1) {
			//exit
			return word_exit_sprite;
		}
	}
	else if (stateSelectType == E_GameState::GameState_Game_Lv_Up) {
		if (index == 0) {
			return card_1;
		}
		else if (index == 1) {
			return card_2;
		}
		else if (index == 2) {
			return card_3;
		}
		else if (index == 3) {
			return card_4;
		}
	}
	else if (stateSelectType == E_GameState::GameState_Game_Menu) {
		if (index == 0) {
			return word_continue_sprite;
		}
		else if (index == 1) {
			return word_retry_sprite;
		}
		else if (index == 2) {
			return word_title_sprite;
		}
	}
	else if (stateSelectType == E_GameState::GameState_Game_Lose) {
		if (index == 0) {
			return word_retry_sprite;
		}
		else if (index == 1) {
			return word_title_sprite;
		}
	}
	else if (stateSelectType == E_GameState::GameState_Game_End) {
		if (index == 0) {
			return word_title_sprite;
		}
	}

	return nullptr;
}

void UIMgr::RegisterUI(UI* ui)
{
	if (ui == nullptr)return;

	E_UIContent uiContentType = ui->GetUIContentType();

	uiPools[uiContentType].push_back(ui);

	//if(uiPools.find(uiContentType)==uiPools.end())
}

UI* UIMgr::ShowDmgUI(int dmgValue, float appearTime, vector3s pos)
{
	UI* dmgUI = GetUI(E_UIContent::UIContent_Dmg_Num);
	if (dmgUI == nullptr)return nullptr;
	((UIWord*)dmgUI)->SetWord(dmgValue);
	dmgUI->SetPos(pos, true);
	dmgUI->ShowUI(true, appearTime, true);

	return dmgUI;
}

void UIMgr::ShowDmgUI(UI* ui, int dmgValue, float appearTime, vector3s pos)
{
	if (ui == nullptr)return;
	((UIWord*)ui)->SetWord(dmgValue);
	ui->SetPos(pos, true);
	ui->ShowUI(true, appearTime, true);
}

#pragma endregion ui_manager