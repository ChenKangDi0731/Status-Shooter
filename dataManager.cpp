#include "gameLogic.h"
#include "dataManager.h"
#include <fstream>
#include <sstream>

#pragma region data_manager

void DataManager::ReadData_CardRule()
{
	if (csv_card_rule_file_path.empty() == true) {
		return;
	}

	std::ifstream ifs(csv_card_rule_file_path);//ファイルを開く

	std::string line;
	getline(ifs, line);//先頭行を切り捨て

	while (getline(ifs, line)) {//一行ずつ読み込む　（1行分の文字列　＝　1オブジェクトのデータ　
		std::vector<std::string> strVec = Split(line, ',');//文字列をカンマで分ける

		CardRuleRawData* data = CardRuleRawData::GetCardRuleRawData(strVec);//文字列をデータに変換する
		if (data == nullptr)continue;
		if (data->card_type == 0) { // normal card
			if (m_normalCardDataDic.find(data->lv_id) == m_normalCardDataDic.end()) {
				m_normalCardDataDic[data->lv_id].clear();
			}

			m_normalCardDataDic[data->lv_id].push_back(data);//リストに保存する
		}
		else if (data->card_type == 1) { // extra card
			E_CardGenreType genreType = (E_CardGenreType)data->card_genre_type;
			if (m_extraCardDataDic.find(genreType) == m_extraCardDataDic.end()) {
				m_extraCardDataDic[genreType].clear();
			}

			m_extraCardDataDic[genreType].push_back(data);//リストに保存する
		}
		else {//error data
			delete data;
		}
	}
}

void DataManager::ReadData_Lv()
{
	if (csv_card_lv_file_path.empty() == true) {
		return;
	}

	std::ifstream ifs(csv_card_lv_file_path);//ファイルを開く

	std::string line;
	getline(ifs, line);//先頭行を切り捨て

	while (getline(ifs, line)) {//一行ずつ読み込む　（1行分の文字列　＝　1オブジェクトのデータ　
		std::vector<std::string> strVec = Split(line, ',');//文字列をカンマで分ける

		CardLvRawData* data = CardLvRawData::GetCardLvRawData(strVec);//文字列をデータに変換する
		if (data == nullptr)continue;
		if (m_lvDataDic.find(data->lv_id) == m_lvDataDic.end()) {
			m_lvDataDic[data->lv_id] = data;
		}
		else {// already had same data
			delete data;
		}
	}
}

void DataManager::ReadData_PlayerLvUpParam()
{
	if (csv_player_lvup_file_path.empty() == true) {
		return;
	}

	std::ifstream ifs(csv_player_lvup_file_path);//ファイルを開く

	std::string line;
	getline(ifs, line);//先頭行を切り捨て

	while (getline(ifs, line)) {//一行ずつ読み込む　（1行分の文字列　＝　1オブジェクトのデータ　
		std::vector<std::string> strVec = Split(line, ',');//文字列をカンマで分ける

		PlayerLvUpRawData* data = PlayerLvUpRawData::GetPlayerLvRawData(strVec);//文字列をデータに変換する
		if (data == nullptr)continue;
		if (m_playerLvUpDataDic.find(data->id) == m_playerLvUpDataDic.end()) {
			m_playerLvUpDataDic[data->id] = data;
		}
		else {
			delete data;
		}
	}
}

/// <summary>
/// 文字列を分割
/// </summary>
/// <param name="input"></param>
/// <param name="delimiter"></param>
/// <returns></returns>
std::vector<std::string> DataManager::Split(std::string& input, char delimiter)
{
	std::vector<std::string> result;
	if (input.empty() == true) {
		return result;
	}
	std::istringstream stream(input);
	std::string field;
	while (getline(stream, field, delimiter)) {//文字列を分割
		result.push_back(field);
	}

	return result;
}

DataManager::DataManager()
{
	m_lvDataDic.clear();
	m_normalCardDataDic.clear();
	m_extraCardDataDic.clear();
	m_playerLvUpDataDic.clear();
}

DataManager::~DataManager()
{
	for (auto temp : m_lvDataDic) {
		if (temp.second != nullptr) {
			delete temp.second;
		}
	}
	m_lvDataDic.clear();

	std::map<int, std::list<CardRuleRawData*>>::iterator normal_iter_1;
	for (normal_iter_1 = m_normalCardDataDic.begin(); normal_iter_1 != m_normalCardDataDic.end(); normal_iter_1++) {
		if (normal_iter_1->second.size() == 0)continue;
		for (auto& temp : normal_iter_1->second) {
			if (temp != nullptr) {
				delete temp;
			}
		}
	}
	m_normalCardDataDic.clear();

	std::map<E_CardGenreType, std::vector<CardRuleRawData*>>::iterator extra_iter_1;
	for (extra_iter_1 = m_extraCardDataDic.begin(); extra_iter_1 != m_extraCardDataDic.end(); extra_iter_1++) {
		if (extra_iter_1->second.size() == 0)continue;
		for (auto& temp : extra_iter_1->second) {
			if (temp != nullptr) {
				delete temp;
			}
		}
	}
	m_extraCardDataDic.clear();

	std::map<int, PlayerLvUpRawData*>::iterator playerData_iter;
	for (playerData_iter = m_playerLvUpDataDic.begin(); playerData_iter != m_playerLvUpDataDic.end(); playerData_iter++) {
		if (playerData_iter->second == nullptr)continue;
				delete playerData_iter->second;
	}
	m_playerLvUpDataDic.clear();

}

void DataManager::DoInit()
{
	//データ読み込み
	ReadData_CardRule();
	ReadData_Lv();
	ReadData_PlayerLvUpParam();
}

void DataManager::UnInit()
{

}

void DataManager::ReloadData()
{

}


std::map<int, CardLvRawData*>& DataManager::GetLvDataDic()
{
	return m_lvDataDic;
}

std::map<int, std::list<CardRuleRawData*>>& DataManager::GetNormalCardDataDic()
{
	return m_normalCardDataDic;
}

std::map<E_CardGenreType, std::vector<CardRuleRawData*>> DataManager::GetExtraCardDic()
{
	return m_extraCardDataDic;
}

std::map<int, PlayerLvUpRawData*>& DataManager::GetPlayerLvUpDataDic()
{
	return m_playerLvUpDataDic;
}

/// <summary>
/// 文字列を4次元ベクトルに変換
/// </summary>
/// <param name="str"></param>
/// <returns></returns>
vector4s DataManager::GetVec4(std::string& str)
{
	vector4s result = vector4s(0, 0, 0, 0);
	if (str.empty() == false) {
		std::vector<std::string> tempVec = Split(str, ';');
		if (tempVec.size() >= 4) {
			if (tempVec[0].empty() == false) {
				result.x = stof(tempVec[0]);
			}
			if (tempVec[1].empty() == false) {
				result.y = stof(tempVec[1]);
			}
			if (tempVec[2].empty() == false) {
				result.z = stof(tempVec[2]);
			}
			if (tempVec[3].empty() == false) {
				result.w = stof(tempVec[3]);
			}
		}
	}
	return result;
}
/// <summary>
/// 文字列を3次元ベクトルに変換
/// </summary>
/// <param name="str"></param>
/// <returns></returns>
vector3s DataManager::GetVec3(std::string& str)
{
	vector3s result = vector3s(0, 0, 0);
	if (str.empty() == false) {
		std::vector<std::string> tempVec = Split(str, ';');
		if (tempVec.size() >= 3) {
			if (tempVec[0].empty() == false) {
				result.x = stof(tempVec[0]);
			}
			if (tempVec[1].empty() == false) {
				result.y = stof(tempVec[1]);
			}
			if (tempVec[2].empty() == false) {
				result.z = stof(tempVec[2]);
			}
		}
	}
	return result;
}

/// <summary>
/// 文字列を2次元ベクトルに変換
/// </summary>
/// <param name="str"></param>
/// <returns></returns>
vector2s DataManager::GetVec2(std::string& str)
{
	vector2s result = vector2s(0, 0);
	if (str.empty() == false) {
		std::vector<std::string> tempVec = Split(str, ';');
		if (tempVec.size() >= 2) {
			if (tempVec[0].empty() == false) {
				result.x = stof(tempVec[0]);
			}
			if (tempVec[1].empty() == false) {
				result.y = stof(tempVec[1]);
			}
		}
	}
	return result;
}

#pragma endregion data_manager

CardRuleRawData* CardRuleRawData::GetCardRuleRawData(std::vector<std::string>& _dataVec)
{
	CardRuleRawData* resultData = new CardRuleRawData();
	for (int index = 0; index < _dataVec.size(); index++) {
		if (_dataVec[index].empty() == true || _dataVec[index] == "/") {
			if (index == 0) {
				//data error
				delete resultData;
				return nullptr;
			}
			continue;
		}
		try {
			switch ((E_Csv_CardRuleItemType)index) {
			case E_Csv_CardRuleItemType::CardRuleItem_CardType:
				resultData->card_type = stoi(_dataVec[index]);
				break;
			case E_Csv_CardRuleItemType::CardRuleItem_Lv_ID:
				resultData->lv_id = stoi(_dataVec[index]);
				break;
			case E_Csv_CardRuleItemType::CardRuleItem_Card_ID:
				resultData->card_id = stoi(_dataVec[index]);
				break;
			case E_Csv_CardRuleItemType::CardRuleItem_Condition_Type:
				resultData->condition_type = stoi(_dataVec[index]);
				break;
			case E_Csv_CardRuleItemType::CardRuleItem_Condition_Param:
				resultData->condition_param_range = DataManager::GetVec2(_dataVec[index]);
				break;
			case E_Csv_CardRuleItemType::CardRuleItem_Effect_Type:
				resultData->effect_type = stoi(_dataVec[index]);
				break;
			case E_Csv_CardRuleItemType::CardRuleItem_Effect_Param:
				resultData->effect_param_range = DataManager::GetVec2(_dataVec[index]);
				break;
			case E_Csv_CardRuleItemType::CardRuleItem_Card_Genre_Type:
				resultData->card_genre_type = stoi(_dataVec[index]);
				break;
			default:
				break;
			}
		}
		catch (std::exception e) {
			//output error data (row,line)
		}
	}

	return resultData;
}

CardLvRawData* CardLvRawData::GetCardLvRawData(std::vector<std::string>& _dataVec)
{
	CardLvRawData* resultData = new CardLvRawData();
	for (int index = 0; index < _dataVec.size(); index++) {
		if (_dataVec[index].empty() == true || _dataVec[index] == "/") {
			if (index == 0) {
				//data error
				delete resultData;
				return nullptr;
			}
			continue;
		}
		try {
			switch ((E_Csv_CardLvRangeItem)index) {
			case E_Csv_CardLvRangeItem::CardLvItem_ID:
				resultData->lv_id = stoi(_dataVec[index]);
				break;
			case E_Csv_CardLvRangeItem::CardLvItem_Range:
				resultData->lv_range = DataManager::GetVec2(_dataVec[index]);
				break;
			default:
				break;
			}
		}
		catch (std::exception e) {
			//output error data (row,line)
		}
	}

	return resultData;
}

PlayerLvUpRawData* PlayerLvUpRawData::GetPlayerLvRawData(std::vector<std::string>& _dataVec)
{
	PlayerLvUpRawData* resultData = new PlayerLvUpRawData();
	for (int index = 0; index < _dataVec.size(); index++) {
		if (_dataVec[index].empty() == true || _dataVec[index] == "/") {
			if (index == 0) {
				//data error
				delete resultData;
				return nullptr;
			}
			continue;
		}
		try {
			switch ((E_Csv_PlayerParamItem)index) {
			case E_Csv_PlayerParamItem::PlayerParamItem_ID:
				resultData->id = stoi(_dataVec[index]);
				break;
			case E_Csv_PlayerParamItem::PlayerParamItem_Range:
				resultData->lv_range = DataManager::GetVec2(_dataVec[index]);
				break;
			case E_Csv_PlayerParamItem::PlayerParamItem_Hp:
				resultData->hp_increasement = stoi(_dataVec[index]);
				break;
			case E_Csv_PlayerParamItem::PlayerParamItem_Atk:
				resultData->atk_increasement = stoi(_dataVec[index]);
				break;
			case E_Csv_PlayerParamItem::PlayerParamItem_Def:
				resultData->def_increasement = stoi(_dataVec[index]);
				break;
			case E_Csv_PlayerParamItem::PlayerParamItem_Exp:
				resultData->exp_increasement = stoi(_dataVec[index]);
				break;
			default:
				break;
			}
		}
		catch (std::exception e) {
			//output error data (row,line)
		}
	}

	return resultData;
}
