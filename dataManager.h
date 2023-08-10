//===========================================================================
// [CSVファイル読み込み]
// ゲームパラメータはCSVファイルに保存され、プログラムが実行された時ロードされます。
// 1．スキルカードパラメータ
// 2．難易度パラメータ
// ３．プレイヤーレベルアップパラメータ
//===========================================================================

#pragma once
#include "main.h"
#pragma region data

const std::string stage_data_file_path = "data\\table\\DataTest.csv";
const std::string csv_card_rule_file_path = "data\\table\\Csv_Card_Rule.csv";
const std::string csv_card_lv_file_path = "data\\table\\Csv_Level_Range.csv";
const std::string csv_player_lvup_file_path = "data\\table\\Csv_Player_LvUp_Param.csv";

enum class E_Csv_CardRuleItemType {
	CardRuleItem_CardType = 0,
	CardRuleItem_Lv_ID = 1,
	CardRuleItem_Card_ID = 2,
	CardRuleItem_Condition_Type = 3,
	CardRuleItem_Condition_Param = 4,
	CardRuleItem_Effect_Type = 5,
	CardRuleItem_Effect_Param = 6,
	CardRuleItem_Card_Genre_Type = 7,
};

enum class E_Csv_CardLvRangeItem {
	CardLvItem_ID,
	CardLvItem_Range,
};

enum class E_Csv_PlayerParamItem {
	PlayerParamItem_ID = 0 ,
	PlayerParamItem_Range = 1,
	PlayerParamItem_Hp = 2,
	PlayerParamItem_Atk = 3,
	PlayerParamItem_Def = 4,
	PlayerParamItem_Exp = 5,
};

/// <summary>
/// スキルカードデータ
/// </summary>
struct CardRuleRawData {
	int card_type;
	int lv_id;
	int card_id;
	int condition_type;//条件カードタイプ（マイナス効果
	vector2s condition_param_range;
	int effect_type;//効果カードタイプ（プラス効果）
	vector2s effect_param_range;
	int card_genre_type;

	CardRuleRawData() :
		card_type(-1),
		lv_id(-1),
		card_id(-1),
		condition_type(-1),
		condition_param_range(vector2s(0, 0)),
		effect_type(-1),
		effect_param_range(vector2s(0, 0)),
		card_genre_type(-1)
	{

	}

	static CardRuleRawData* GetCardRuleRawData(std::vector<std::string>& _dataVec);
};

/// <summary>
/// 難易度データ
/// </summary>
struct CardLvRawData {
	int lv_id;
	vector2s lv_range;
	CardLvRawData() :
		lv_id(-1),
		lv_range(vector2s(0, 0)) {

	}

	static CardLvRawData* GetCardLvRawData(std::vector<std::string>& _dataVec);
};

/// <summary>
/// レベルアップパラメータデータ
/// </summary>
struct PlayerLvUpRawData {
	int id;
	vector2s lv_range;
	int hp_increasement;
	int atk_increasement;
	int def_increasement;
	int exp_increasement;
	PlayerLvUpRawData() :
		id(-1),
		lv_range(vector2s(0, 0)),
		hp_increasement(0),
		atk_increasement(0),
		def_increasement(0),
		exp_increasement(0) {

	}

	static PlayerLvUpRawData* GetPlayerLvRawData(std::vector<std::string>& _dataVec);
};

#pragma endregion data

class DataManager {
private:
	std::map<int, CardLvRawData*> m_lvDataDic;
	std::map<int, std::list<CardRuleRawData*>> m_normalCardDataDic;
	std::map<E_CardGenreType, std::vector<CardRuleRawData*>> m_extraCardDataDic;
	std::map<int, PlayerLvUpRawData*> m_playerLvUpDataDic;

private:
	//=========================データ読み込み
	void ReadData_CardRule();
	void ReadData_Lv();
	void ReadData_PlayerLvUpParam();
public:
	DataManager();
	~DataManager();

	void DoInit();
	void UnInit();

	void ReloadData();

	std::map<int, CardLvRawData*>& GetLvDataDic();
	std::map<int, std::list<CardRuleRawData*>>& GetNormalCardDataDic();
	std::map<E_CardGenreType, std::vector<CardRuleRawData*>> GetExtraCardDic();
	std::map<int, PlayerLvUpRawData*>& GetPlayerLvUpDataDic();

	//=======================ツール函数
	static std::vector<std::string> Split(std::string& input, char delimiter);//文字列を分割する
	static vector4s GetVec4(std::string& str);//文字列をベクトルに転換する
	static vector3s GetVec3(std::string& str);
	static vector2s GetVec2(std::string& str);
};


