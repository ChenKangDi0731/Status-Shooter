//===========================================================================
// [UIオブジェクト]
// 1．スプライトUI
// 2．文字UI（最大4桁
// 3。UIグループ（複数のUI
//===========================================================================
#pragma once
#ifndef _UI_H_
#define _UI_H_

//最大桁数
#define UI_WORD_CHILD_COUNT 4

//uiSelector
#define UI_SELECTOR_LT_ID 0
#define UI_SELECTOR_RT_ID 1
#define UI_SELECTOR_LB_ID 2
#define UI_SELECTOR_RB_ID 3

//UIGroup_Normal
#define UI_BG_ID 0 
#define UI_CARD_CONDITION_WORD_1_ID 1
#define UI_CARD_CONDITION_WORD_2_ID 2
#define UI_CARD_CONDITION_WORD_3_ID 3
#define UI_CARD_EFFECT_WORD_1_ID 4
#define UI_CARD_EFFECT_WORD_2_ID 5
#define UI_CARD_EFFECT_WORD_3_ID 6

//UI_HUD
#define UI_HUD_STATUS_IMG 0
#define UI_HUD_STATUS_CUR_HP 1
#define UI_HUD_STATUS_MAX_HP 2
#define UI_HUD_STATUS_CUR_ATK 3
#define UI_HUD_STATUS_MAX_ATK 4
#define UI_HUD_STATUS_CUR_DEF 5
#define UI_HUD_STATUS_MAX_DEF 6
#define UI_HUD_STATUS_CUR_EXP 7
#define UI_HUD_STATUS_MAX_EXP 8
#define UI_HUD_STATUS_LV_CUR 9
#define UI_HUD_STATUS_HP_CONSUME 10
#define UI_HUD_STATUS_ATK_CONSUME 11
#define UI_HUD_STATUS_DEF_CONSUME 12
#define UI_HUD_STATUS_EXP_CONSUME 13
#define UI_HUD_STATUS_SLASH_1 14
#define UI_HUD_STATUS_SLASH_2 15
#define UI_HUD_STATUS_SLASH_3 16
#define UI_HUD_STATUS_SLASH_4 17

//UI_GAME_TIMER
#define UI_GAME_TIMER_MINUTE 0
#define UI_GAME_TIMER_COLON 1
#define UI_GAME_TIMER_SECOND 2

//UI_LV_PANEL
#define UI_LV_PANEL_STATUS_IMG 0
#define UI_LV_PANEL_STATUS_HP_PRE 1
#define UI_LV_PANEL_STATUS_HP_CUR 2
#define UI_LV_PANEL_STATUS_ATK_PRE 3
#define UI_LV_PANEL_STATUS_ATK_CUR 4
#define UI_LV_PANEL_STATUS_DEF_PRE 5
#define UI_LV_PANEL_STATUS_DEF_CUR 6
#define UI_LV_PANEL_STATUS_LV_PRE 7
#define UI_LV_PANEL_STATUS_LV_CUR 8
#define UI_LV_PANEL_STATUS_ARROW_1 9
#define UI_LV_PANEL_STATUS_ARROW_2 10
#define UI_LV_PANEL_STATUS_ARROW_3 11
#define UI_LV_PANEL_STATUS_ARROW_4 12

enum class E_UIType {
	UI_None,
	UI_Sprite,
	UI_Word,
	UI_Group,
};

enum class E_UIContent {
	UIContent_None = 1,
	UIContent_Num = 2,
	UIContent_Sign = 3,
	UIContent_Alphabet = 4,
	UIContent_HP = 5,
	UIContent_Atk = 6,
	UIContent_Def = 7,
	UIContent_Exp = 8,
	UIContent_Dmg_Num = 9,

	//word
	UIContent_Word_Max = 10,
	UIContent_Word_Lv_Up = 11,
	UIContent_Word_Up = 12,
	UIContent_Word_Attack = 13,
	UIContent_Word_Speed = 14,
	UIContent_Word_Bullet = 15,
	UIContent_Word_Consume = 16,
	UIContent_Word_Gain = 17,
	UIContent_Word_Recovery = 18,
	UIContent_Word_Cap = 19,
	UIContent_Word_Damage = 20,
	UIContent_Word_Reflect = 21,
	UIContent_Word_Enemy = 22,

	//menu
	UIContent_Word_Pause = 30,
	UIContent_Word_You_Lose = 31,
	UIContent_Word_You_Survive = 32,
	UIContent_Word_Retry = 33,
	UIContent_Word_Continue = 34,
	UIContent_Word_Thank = 35,
	UIContent_Word_Start = 36,
	UIContent_Word_Exit = 37,
	UIContent_Word_Title = 38,
	UIContent_Img_Title = 39,
	UIContent_Img_Title_Selecotr = 41,

	UIContent_Word_Count = 60,
	UIContent_Word_Create = 61,
	UIContent_Word_Every = 62,

	UIContent_Word_Die = 63,
	UIContent_Word_Hit = 64,
	UIContent_Word_Second = 65,
	UIContent_Word_Boost = 66,
	UIContent_Word_Auto = 67,

	UIContent_Word_Stop = 68,
	UIContent_Word_Attacking = 69,
	UIContent_Word_Move = 70,
	UIContent_Word_Range = 71,
	UIContent_Word_Time = 72,

	//other
	UIContent_Card_Bg = 51,
	UIContent_Card = 52,
	UIContent_Bg = 53,
	UIContent_Card_Extra_Bg = 54,
};

enum class E_UIGroupCell {
	UIGroupCell_BG = 0,
	UIGroupCell_Word_1 = 1,
	UIGroupCell_Word_2 = 2,
	UIGroupCell_Word_3 = 3,
};

enum class E_UIWordAlignment {
	UIWordAlignment_Center,
	UIWordAlignment_Right,
	UIWordAlignment_Left,
};

class BaseCell;
#pragma region UI

class UI :public BaseCell {
protected:
	E_UIType uiType;
	E_UIContent uiContentType;

	float appearTime;
	float appearTimePass;

public:
	UI();
	UI(E_UIType uiT);
	~UI();

	virtual void DoUpdate(float deltatime);

	virtual void ShowUI(bool show, float appearT, bool resetTimer);

	virtual void UpdateState();

	void SetUIContentType(E_UIContent contentType);

	E_UIType GetUIType();
	E_UIContent GetUIContentType();
};

/// <summary>
/// UIグループ（複数のUI
/// </summary>
class UIGroup :public UI {
private:
	std::map<int, UI*> childDic;
	std::map<int, vector3s> childOffsets;//子UIのオフセット値
public:
	bool deleteChild;
private:
	void UpdateChildPos();
public:
	UIGroup();
	~UIGroup();

	bool RegisterChild(int id, UI* child, vector3s offset);
	void ClearChild();

	void DoUpdate(float deltatime);
	void UpdateState();
	void ShowUI(bool show, float appearT, bool resetTimer);

	std::map<int, UI*>& GetChildDic();
	void SetChildOffset(int childID, vector3s offset);
};

/// <summary>
/// スプライトUI
/// </summary>
class UISprite :public UI {
private:

public:
	UISprite();
	~UISprite();

	void SetSprite(E_UIContent content);
};

/// <summary>
/// 文字（最大4桁
/// </summary>
class UIWord :public UI {
private:
	E_UIWordAlignment alignmentMode;//揃えモード

	UISprite wordArray[UI_WORD_CHILD_COUNT];
	float wordInterval;//文字の間の距離
	float fontSize;//文字サイズ

	int curWordCount;

	int wordCodes[UI_WORD_CHILD_COUNT];

private:
	void UpdateWordPos();
	void UpdateWord(bool setEnable);
public:
	UIWord();
	~UIWord();

	//文字設定
	void SetWord(const std::string& words);
	void SetWord(int numWord);

	void DoUpdate(float deltatime);
	void UpdateState();

	void ShowUI(bool show, float appearT, bool resetTimer);

	void SetWordInterval(float interval);
	void SetAlignment(E_UIWordAlignment alignmentT);//揃えモードを切り替え

	UISprite* GetSpriteArray();

	void Clear();
};

#pragma endregion UI


#endif