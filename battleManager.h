//===========================================================================
// [�o�g���}�l�[�W���[]
// �]����̃p�����[�^���Ǘ�����i�U���͂���՗ʂƂ�
// �]�_���[�W�����i�_���[�W�v�Z�Ƃ�
//===========================================================================
#pragma once
#ifndef _BATTLEMANAGER_H_
#define _BATTLEMANAGER_H_
#pragma warning(disable:4244)

struct CardLvRawData;

class BattleMgr {
private://card
	bool m_initCardDataOnce;

	std::list<CardRuleData*> cardRuleDataDic;
	CardRuleData* curRuleData;

	CardEffectData* card_1_data;
	CardEffectData* card_2_data;
	CardEffectData* card_3_data;
	CardEffectData* card_4_data;

	//	lvId	lvRange
	std::map<int, CardLvRawData*> m_cardLvDic;
	//  lvId    cardList
	std::map<int, CardRuleData*> m_cardRuleDataDic;
	//  genre				extraCardList
	std::map<E_CardGenreType, std::vector<CardRuleRawData*>> m_extraCardRuleDataDic;//vector

	std::map<E_CardGenreType, std::list<CardRuleRawData*>> m_gotExtraCardDic;//EXTRA�J�[�h�̂�
	std::map<int, CardRuleRawData*> m_tempCardDataDic;//�I�񂾃G�L�X�g���J�[�h�����X�g�ɕۑ����邽�߂Ɏg���ꎞ�I�ȃ��X�g
	std::list<int> m_tempNormalCardVec;//�m�[�}���J�[�h���J��Ԃ��ďo�����Ȃ��悤�Ɏg���郊�X�g�i�}�X�N�AID��ۑ�

	std::map<int, PlayerLvUpRawData*> m_playerLvUpParamDataDic;

	//�G�L�X�g���J�[�h�̎�ނ�ۑ����郊�X�g
	std::vector<E_CardGenreType> m_extraCardTypeList;

private://bullet

	//hp
	int hpBaseDmg;
	float hpLifeTime;

	//atk
	int atkBaseDmg;
	float atkRange;
	float atkExplodeSize;

	//def
	int defBaseDmg;
	int defBulletCount;
	float defRecoveryCap;
	int defBulletReflectCount;
	//float def lifeTime

	//exp
	int expBaseDmg;
	int expBulletCount;
	float expGain;
	int expBulletReflectCount;

	//recovery counter
	float hpRecoveryCounter;
	float atkRecoveryCounter;
	float defRecoveryCounter;
	float expRecoveryCounter;

public:
	//���˂ł��邩�ǂ����i�X�L���J�[�h����
	bool bulletReflectSign = false;

public:
	BattleMgr();
	~BattleMgr();

	void DoInit();
	void DoUpdate(float deltatime);

	//card
	void SwitchCardRuleData(int lv);
	CardEffectData* GetCardData(int cardIndex, int playerCurLv, bool& isExtraCard);
	void SelectCard(int cardIndex);
	void ResetCardData();
	void ResetGotData();

	//�_���[�W����
	static void BulletHit(Bullet* bullet, BaseCell* target);
	static void Explosion(Bullet* bullet);

	//bullet method
	void ResetBulletParam();

	void SetBaseDmg(E_Status status, int value);//base dmg
	void ChangeBaseDmg(E_Status status, int increasement);
	int GetBaseDmg(E_Status status);

	void SetBulletCount(E_Status status, int _value);//bullet count
	int GetBulletCount(E_Status status);
	void ChangeBulletCount(E_Status status, int increasement);

	void SetDefRecoveryCap(float capValue);//def recovery cap
	void ChangeDefRecoveryCap(float increasement);
	float GetDefRecoveryCap();
	void ShootRecovery(Status_Player* playerStatus, E_Status bulletTypeStatus, E_Status recoveryStatus);

	void SetExpGain(float gain);
	void ChangeExpGain(float increasement);
	float GetExpGain();

	void SetHpBulletTime(float _value);
	void ChangeHpBulletTime(float _increasementFactor);
	float GetHpBulletTime();

	void SetAtkBulletRange(float _value);
	void ChangeAtkBulletRange(float _increasementFactor);
	float GetAtkBulletRange();
	float GetAtkBulletExplodeEffectSize();

	void SetReflectCount(E_Status _status, int _value);
	void ChangeReflectCount(E_Status _status, int _increasement);
	int GetReflectCount(E_Status _status);

	int GetLvUpStatusParam(E_Status _status, int _curPlayerLv);

	void SetReflectEnemyBullet(bool _state);

	//�_���[�W�����擾
	int GetDmg(Player* player, Status_Player* playerStatus, Bullet* bullet);
	int GetHurtDmg(Player* player, Enemy* hurter);
	int GetExp(Status_Player* playerStatus, Enemy* target);//�o���l���v�Z
	void Recovery(Status_Player* playerStatus, E_AttackType attackType);//�񕜗ʂ��v�Z

	static void ApplyCard(Status_Player* _playerStatus, CardEffectData* _cardData);//�J�[�h���ʂ��擾
	static void RecoveryCard(Status_Player* _playerStatus, CardEffectData* _cardData);//�J�[�h���ʂ�������
};




#endif