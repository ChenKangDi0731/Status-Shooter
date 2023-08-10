//===========================================================================
// [�G�t�F�N�g�}�l�[�W��
// 1�D�U�����󂯂�G�t�F�N�g
// 2�D�J�����h�����
//===========================================================================
#ifndef _EFFECTMANAGER_H_
#define _EFFECTMANAGER_H_

class EffectMgr {
private:
	std::map<E_Effect, std::list<Effect*>> effectPools;

	//�`��f�[�^�̃e���v���[�g
	BaseShaderParam* enemyHitEffect_ShaderParam;
	BaseShaderParam* playerHitEffect_ShaderParam;
	BaseShaderParam* explosionEffect_ShaderParam;
	BaseShaderParam* shootLight_ShaderParam;
private:
	//�G�t�F�N�g�I�����̃R�[���o�b�N
	Anim_Effect_Normal_CB* hitEffect_end_cb;

private:
	//�J�����h��
	float cameraShakeFactor;
	float cameraShakeTime;
	float cameraShakeTimePass;
	vector3s shakeTargetPos;

public:
	bool cameraShakeSwitch;
	vector3s shakePosOffset;

public:
	EffectMgr();
	~EffectMgr();

	void DoInit();
	void UnInit();

	void DoUpdate(float deltatime);

	Effect* CreateEffectEntity(E_Effect effectType);//�G�t�F�N�g�����
	Effect* GetEffect(E_Effect effectType);
	Effect* CreateEffect(E_Effect effectType, BaseCell* followObj, vector3s pos, vector3s dir);

	void RecycleEffect(Effect* effect);

	void RegisterEffect(Effect* effect);

	//�J�����h���ݒ�
	void SetCameraShake(float shakeTime, float shakeFactor);
};



#endif