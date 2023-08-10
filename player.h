#pragma once
#ifndef _PLAYER_H_
#define _PLAYER_H_

#include "gameObject.h"

class Player : public BaseCell {

private:
	Status_Player* status;//ステータス
	E_Status curStatusBulletType;//今の武器種類
private:
	Light* playerLight;
	float lightFieldRadius;
	vector3s lightPosOffset;
	vector3s curLightDir;

	vector3s targetLightPos;

	vector3s preMouseMovement;

	//ダメージ受けた時のパラメータ
	float m_beHitedTimePass = 0.0f;
	bool m_beHitedColorSign = false;

public:
	vector3s curLightPos;
	bool inputLock = false;
public:

	Player();
	~Player();

	void DoUpdate(float deltatime);
	void DoLateUpdate(float deltatime);
	void Input(float deltatime);//プレイヤー入力

	void SetPlayerLight(Light* light);

	//武器函数
	void SetCurBulletType(E_Status bulletType);//武器を設定
	E_Status GetCurBulletType();
	void SwitchBulletType(bool forward);//武器の切り替え
	void ShootBullet();//弾を発射

	int GetStatusValue(E_Status statusType);//ステータス値を取得

	vector3s GetCurShootPos();//弾を発射する位置
};

#endif