//===========================================================================
// [UI�}�l�[�W���[]
//===========================================================================
#ifndef _UIMANAGER_H_
#define _UIMANAGER_H_

class UIMgr {
private:

	std::map<E_UIContent, std::vector<UI*>> uiPools;//UI���X�g

	//uiSprite �`��f�[�^
	BaseShaderParam* num_sprite_shaderParam;
	BaseShaderParam* num_red_sprite_shaderParam;
	BaseShaderParam* num_outline_sprite_shaderParam;
	BaseShaderParam* word_content_cell_shaderParam;
	BaseShaderParam* sprite_cell_bg_shaderParam;
	BaseShaderParam* sprite_extra_cell_bg_shaderParam;


private:

	std::map<E_UIContent, UISprite*> wordDic;

	//const ui
	UISprite* guideUI;

	//title
	UISprite* title_sprite;
	UISprite* title_bg;
	vector3s title_default_pos;
	vector3s title_item_start_default_pos;
	vector3s title_item_exit_default_pos;

	//menu
	// lv up
	UISprite* lv_up_sprite;
	vector3s lv_up_defaultPos;
	vector3s lv_up_targetPos;


	//menu_game(card
	UIGroup* card_1;
	UIGroup* card_2;
	UIGroup* card_3;
	UIGroup* card_4;
	vector3s word_condition_1_offset;
	vector3s word_condition_2_offset;
	vector3s word_condition_3_offset;
	vector3s word_effect_1_offset;
	vector3s word_effect_2_offset;
	vector3s word_effect_3_offset;

	//hud
	UIGroup* hud_status_frame;
	vector3s hud_default_pos;
	vector3s hud_target_pos;

	//timer
	UIGroup* hud_game_timer;

	//lv-up panel(status
	UIGroup* lv_panel;

	//menu sprite
	UISprite* word_pause_sprite;
	UISprite* word_you_lose_sprite;
	UISprite* word_you_survive_sprite;
	UISprite* word_thank_sprite;

	vector3s pause_img_pos;
	vector3s lose_img_pos;
	vector3s win_img_pos;
	vector3s thanks_img_pos;

	//menu item
	UISprite* word_start_sprite;
	UISprite* word_exit_sprite;
	UISprite* word_title_sprite;
	UISprite* word_retry_sprite;
	UISprite* word_continue_sprite;

	//menu pos
	vector3s menu_pause_continue_pos;
	vector3s menu_pause_retry_pos;
	vector3s menu_pause_title_pos;

	vector3s lose_menu_retry_pos;
	vector3s lose_menu_title_pos;

	vector3s win_menu_title_pos;

private:
	//status selector
	UISprite* statusSelector;
	vector3s selector_pos_status_hp;
	vector3s selector_pos_status_atk;
	vector3s selector_pos_status_def;
	vector3s selector_pos_status_exp;

	//card selector
	UIGroup* uiSelector;
	E_GameState curStateSelect;
	int curSelectIndex;

	//crosshair
	UISprite* crosshair;

private:
	//========================================�X�L���J�[�h����
	void ComposeCard(UIGroup* card, CardEffectData* data, bool isExtraCard);//�J�[�h��g�ݗ���
	void ComposeCardPart(UIGroup* _card, CardEffectData* _data, bool _cardSkillType /*,int _wordCount*/);
public:
	UIMgr();
	~UIMgr();

	void DoInit();

	//UI�����
	UI* CreateUIEntity(E_UIContent uiContentType);
	//UI���擾
	UI* GetUI(E_UIContent uiContentType);
	//UI�����X�g�ɓ����
	void RegisterUI(UI* ui);
	//�_���[�WUI��\��
	UI* ShowDmgUI(int dmgValue, float appearTime, vector3s pos);
	void ShowDmgUI(UI* ui, int dmgValue, float appearTime, vector3s pos);
	//UI��\������
	void ShowUI(E_UIContent uiContentType, bool show);//const ui

	//title
	//�^�C�g����\��
	void ShowTitle(bool show);

	//menu
	//�Q�[�����j���[��\��
	void ShowMenu(E_GameState stateMenuType, bool show);
	//�Q�[���`���[�g���A��UI��\��
	void ShowGuide(bool show);

	//hud��\��
	void ShowHUD(bool show);
	void UpdateHUD(Status_Player* playerStatus);
	void UpdateHUD(E_Status statusType, int value);

	//���Ԃ�\��
	void ShowGameTimer(bool show);
	void UpdateGameTimer();

	//scene ui
	//���x���A�b�vUI
	void ShowLevelUpImg(bool show, vector3s pos);
	void ShowLvUpPanel(bool show, int playerCurLv);
	void UpdateLvPanel(Status_Player* playerStatus, bool preStatusData);

	//�X�L���J�[�hUI
	void ShowCard(int curPlayerLv, int cardIndex, bool show);
	void UpdateCard(int cardIndex);
	void CardCollision(vector3s mousePos);

	//selector(status
	//�����I��
	void ShowStatusSelector(bool show);
	void UpdateStatusSelector(E_Status curStatus);

	//crosshair
	//�Ɛ���\��
	void SetCrosshairPos(vector3s _pos);
	void ShowCrosshair(bool _show);

	//selector(card
	//�J�[�h��I��
	void InitSelectorParam(E_GameState gameStateItemType);
	void UpdateSelector();
	void SetSelectorParam(vector3s pos, vector3s scale, vector2s sizeOffset);
	void SwitchNextItem(bool forward);
	int GetCurSelectItem();
	void SelectCurItem();
	void ShowSelector(E_GameState stateSelectType, bool show);

	UI* GetItem(E_GameState stateSelectType, int index);
};


#endif
