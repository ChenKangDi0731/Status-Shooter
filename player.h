#pragma once
#ifndef _PLAYER_H_
#define _PLAYER_H_

#include "gameObject.h"

class Player : public BaseCell {

private:
	Status_Player* status;//�X�e�[�^�X
	E_Status curStatusBulletType;//���̕�����
private:
	Light* playerLight;
	float lightFieldRadius;
	vector3s lightPosOffset;
	vector3s curLightDir;

	vector3s targetLightPos;

	vector3s preMouseMovement;

	//�_���[�W�󂯂����̃p�����[�^
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
	void Input(float deltatime);//�v���C���[����

	void SetPlayerLight(Light* light);

	//���픟��
	void SetCurBulletType(E_Status bulletType);//�����ݒ�
	E_Status GetCurBulletType();
	void SwitchBulletType(bool forward);//����̐؂�ւ�
	void ShootBullet();//�e�𔭎�

	int GetStatusValue(E_Status statusType);//�X�e�[�^�X�l���擾

	vector3s GetCurShootPos();//�e�𔭎˂���ʒu
};

#endif