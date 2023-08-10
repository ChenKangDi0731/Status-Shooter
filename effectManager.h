//===========================================================================
// [エフェクトマネージャ
// 1．攻撃を受けるエフェクト
// 2．カメラ揺れ効果
//===========================================================================
#ifndef _EFFECTMANAGER_H_
#define _EFFECTMANAGER_H_

class EffectMgr {
private:
	std::map<E_Effect, std::list<Effect*>> effectPools;

	//描画データのテンプレート
	BaseShaderParam* enemyHitEffect_ShaderParam;
	BaseShaderParam* playerHitEffect_ShaderParam;
	BaseShaderParam* explosionEffect_ShaderParam;
	BaseShaderParam* shootLight_ShaderParam;
private:
	//エフェクト終了時のコールバック
	Anim_Effect_Normal_CB* hitEffect_end_cb;

private:
	//カメラ揺れ
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

	Effect* CreateEffectEntity(E_Effect effectType);//エフェクトを作る
	Effect* GetEffect(E_Effect effectType);
	Effect* CreateEffect(E_Effect effectType, BaseCell* followObj, vector3s pos, vector3s dir);

	void RecycleEffect(Effect* effect);

	void RegisterEffect(Effect* effect);

	//カメラ揺れを設定
	void SetCameraShake(float shakeTime, float shakeFactor);
};



#endif