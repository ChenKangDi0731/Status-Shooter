#include "sceneObjManager.h"
#include "gameLogic.h"
#include "effectManager.h"
#include "gameObject.h"


#pragma region effect_manager

EffectMgr::EffectMgr()
{
	enemyHitEffect_ShaderParam = nullptr;
	explosionEffect_ShaderParam = nullptr;
	shootLight_ShaderParam = nullptr;

	effectPools[E_Effect::Effect_EnemyHit].clear();
}

EffectMgr::~EffectMgr()
{
	effectPools.clear();

	if (enemyHitEffect_ShaderParam != nullptr) {
		delete enemyHitEffect_ShaderParam;
	}
	enemyHitEffect_ShaderParam = nullptr;

	if (playerHitEffect_ShaderParam != nullptr) {
		delete playerHitEffect_ShaderParam;
	}
	playerHitEffect_ShaderParam = nullptr;

	if (hitEffect_end_cb != nullptr) {
		delete hitEffect_end_cb;
	}
	hitEffect_end_cb = nullptr;

	if (explosionEffect_ShaderParam != nullptr) {
		delete explosionEffect_ShaderParam;
	}
	explosionEffect_ShaderParam = nullptr;

	if (shootLight_ShaderParam != nullptr) {
		delete shootLight_ShaderParam;
	}
	shootLight_ShaderParam = nullptr;
}

void EffectMgr::DoInit()
{
	//create effect data
	ShaderParam_Normal* tempHitEffect_shaderParam = new ShaderParam_Normal();

	tempHitEffect_shaderParam->shaderProgram = GameMgr::GetInstance()->shaderMgr->GetShaderProgram(E_Shader::Shader_Normal);
	tempHitEffect_shaderParam->texNo = GameMgr::GetInstance()->textureMgr->GetTextureID(E_Texture::Texture_Effect_Package);
	tempHitEffect_shaderParam->texIndex = vector2s(EFFECT_HIT_START_INDEX_X, EFFECT_HIT_START_INDEX_Y);
	tempHitEffect_shaderParam->tex_w_count = EFFECT_TEX_W_COUNT;
	tempHitEffect_shaderParam->tex_texel_size = effect_hit_tex_size;
	enemyHitEffect_ShaderParam = tempHitEffect_shaderParam;

	//create effect data
	ShaderParam_Normal* tempPlayerHitEffect_shaderParam = new ShaderParam_Normal();

	tempPlayerHitEffect_shaderParam->shaderProgram = GameMgr::GetInstance()->shaderMgr->GetShaderProgram(E_Shader::Shader_Normal);
	tempPlayerHitEffect_shaderParam->texNo = GameMgr::GetInstance()->textureMgr->GetTextureID(E_Texture::Texture_Effect_Package);
	tempPlayerHitEffect_shaderParam->texIndex = vector2s(EFFECT_HIT_RED_START_INDEX_X, EFFECT_HIT_RED_START_INDEX_Y);
	tempPlayerHitEffect_shaderParam->tex_w_count = EFFECT_TEX_W_COUNT;
	tempPlayerHitEffect_shaderParam->tex_texel_size = effect_hit_tex_size;
	playerHitEffect_ShaderParam = tempPlayerHitEffect_shaderParam;

	hitEffect_end_cb = new Anim_Effect_Normal_CB();

	ShaderParam_Normal* tempExplosionEffect_shaderParam = new ShaderParam_Normal();

	tempExplosionEffect_shaderParam->shaderProgram = GameMgr::GetInstance()->shaderMgr->GetShaderProgram(E_Shader::Shader_Normal);
	tempExplosionEffect_shaderParam->texNo = GameMgr::GetInstance()->textureMgr->GetTextureID(E_Texture::Texture_Effect_Package);
	tempExplosionEffect_shaderParam->texIndex = vector2s(EFFECT_EXPLOSION_START_INDEX_X, EFFECT_EXPLOSION_START_INDEX_Y);
	tempExplosionEffect_shaderParam->tex_w_count = EFFECT_EXPLOSION_ANIM_INDEX_COUNT;
	tempExplosionEffect_shaderParam->tex_texel_size = effect_explosion_tex_cell_size;
	explosionEffect_ShaderParam = tempExplosionEffect_shaderParam;

	ShaderParam_Normal* tempShootLightEffect_shaderParam = new ShaderParam_Normal();

	tempShootLightEffect_shaderParam->shaderProgram = GameMgr::GetInstance()->shaderMgr->GetShaderProgram(E_Shader::Shader_Normal);
	tempShootLightEffect_shaderParam->texNo = GameMgr::GetInstance()->textureMgr->GetTextureID(E_Texture::Texture_Effect_Package);
	tempShootLightEffect_shaderParam->texIndex = vector2s(EFFECT_SHOOT_LIGHT_START_INDEX_X, EFFECT_SHOOT_LIGHT_START_INDEX_Y);
	tempShootLightEffect_shaderParam->tex_w_count = EFFECT_TEX_W_COUNT;
	tempShootLightEffect_shaderParam->tex_texel_size = effect_hit_tex_size;
	shootLight_ShaderParam = tempShootLightEffect_shaderParam;
}

void EffectMgr::UnInit()
{

}

void EffectMgr::DoUpdate(float deltatime)
{

	//カメラ揺れ処理
	if (cameraShakeSwitch == true) {

		cameraShakeFactor -= deltatime * CAMERA_SHAKE_REDUCE_FACTOR;

		shakeTargetPos.x = (rand() % 200 - 100) * 0.01f;
		shakeTargetPos.y = (rand() % 200 - 100) * 0.01f;
		shakeTargetPos.z = 0.0f;
		shakeTargetPos = VecNormalize(shakeTargetPos);
		shakeTargetPos = shakeTargetPos * cameraShakeFactor;
		shakeTargetPos = shakeTargetPos;

		shakePosOffset = VecLerp(shakePosOffset, shakeTargetPos, deltatime * cameraShakeFactor);

		cameraShakeTimePass += deltatime;
		if (cameraShakeTimePass >= cameraShakeTime) {
			cameraShakeSwitch = false;
			cameraShakeFactor = 0.0f;
			cameraShakeTime = 0.0f;
		}
	}
}

/// <summary>
/// エフェクトを作る
/// </summary>
/// <param name="effectType"></param>
/// <returns></returns>
Effect* EffectMgr::CreateEffectEntity(E_Effect effectType) {

	if (effectType == E_Effect::Effect_EnemyHit) {
		Effect* newEffect = new Effect();
		newEffect->SetCellType(E_CellType::CellType_Effect);
		if (newEffect != nullptr) {
			newEffect->SetEffectType(effectType);
			newEffect->SetID(GameMgr::GetInstance()->sceneObjMgr->GetID());
			//newEffect->SetScale(vector3s(1.2f, 1.2f, 1.0f));

			DrawData* effectDrawData = RenderMgr::GetNewDrawData(nullptr, true, E_BatchingType::BatchingType_Effect, true);
			if (effectDrawData == nullptr) {
				delete newEffect;
				return nullptr;
			}
			effectDrawData->SetSize(effect_hit_tex_cell_size);
			effectDrawData->SetShaderParam(enemyHitEffect_ShaderParam);
			newEffect->SetDrawData(effectDrawData);

			Animation* effectAnimComponent = new Animation();
			newEffect->RegisterComponent(E_Component::Component_Animation, effectAnimComponent);
			//hit anim
			AnimClip* effectAnimClip = new AnimClip();
			int startTexIndex = EFFECT_TEX_W_COUNT * EFFECT_HIT_START_INDEX_Y + EFFECT_HIT_START_INDEX_X;
			int endTexIndex = startTexIndex + EFFECT_HIT_ANIM_INDEX_COUNT;
			effectAnimClip->SetAnimParam(E_Anim::Anim_Test_1, vector2s(startTexIndex, endTexIndex), 0.2f, true, false);
			effectAnimComponent->RegisterAnim(E_Anim::Anim_Test_1, effectAnimClip, true);
			//anim callback
			effectAnimClip->animEnd_Callback = this->hitEffect_end_cb;

			newEffect->StopEffect();
			newEffect->SetState(false);

			GameMgr::GetInstance()->sceneObjMgr->RegisterCell(newEffect);
			this->RegisterEffect(newEffect);
		}
		return newEffect;
	}
	else if (effectType == E_Effect::Effect_PlayerHit) {
		Effect* newEffect = new Effect();
		newEffect->SetCellType(E_CellType::CellType_Effect);
		if (newEffect != nullptr) {
			newEffect->SetEffectType(effectType);
			newEffect->SetID(GameMgr::GetInstance()->sceneObjMgr->GetID());
			newEffect->SetScale(vector3s(2.0f, 2.0f, 1.0f));

			DrawData* effectDrawData = RenderMgr::GetNewDrawData(nullptr, true, E_BatchingType::BatchingType_Effect, true);
			if (effectDrawData == nullptr) {
				delete newEffect;
				return nullptr;
			}
			effectDrawData->SetSize(effect_hit_tex_cell_size);
			effectDrawData->SetShaderParam(playerHitEffect_ShaderParam);
			newEffect->SetDrawData(effectDrawData);

			Animation* effectAnimComponent = new Animation();
			newEffect->RegisterComponent(E_Component::Component_Animation, effectAnimComponent);
			//hit anim
			AnimClip* effectAnimClip = new AnimClip();
			int startTexIndex = EFFECT_TEX_W_COUNT * EFFECT_HIT_RED_START_INDEX_Y + EFFECT_HIT_RED_START_INDEX_X;
			int endTexIndex = startTexIndex + EFFECT_HIT_RED_ANIM_INDEX_COUNT;
			effectAnimClip->SetAnimParam(E_Anim::Anim_Test_1, vector2s(startTexIndex, endTexIndex), 0.5f, true, false);
			effectAnimComponent->RegisterAnim(E_Anim::Anim_Test_1, effectAnimClip, true);
			//anim callback
			effectAnimClip->animEnd_Callback = this->hitEffect_end_cb;

			newEffect->StopEffect();
			newEffect->SetState(false);

			GameMgr::GetInstance()->sceneObjMgr->RegisterCell(newEffect);
			this->RegisterEffect(newEffect);
		}
		return newEffect;
	}
	else if (effectType == E_Effect::Effect_Explosion) {
		Effect* newEffect = new Effect();
		newEffect->SetCellType(E_CellType::CellType_Effect);
		if (newEffect != nullptr) {
			newEffect->SetEffectType(effectType);
			newEffect->SetID(GameMgr::GetInstance()->sceneObjMgr->GetID());
			newEffect->SetScale(vector3s(4.0f, 4.0f, 1.0f));

			DrawData* effectDrawData = RenderMgr::GetNewDrawData(nullptr, true, E_BatchingType::BatchingType_Effect, true);
			if (effectDrawData == nullptr) {
				delete newEffect;
				return nullptr;
			}
			effectDrawData->SetSize(effect_hit_tex_cell_size);
			effectDrawData->SetShaderParam(explosionEffect_ShaderParam);
			newEffect->SetDrawData(effectDrawData);

			Animation* effectAnimComponent = new Animation();
			newEffect->RegisterComponent(E_Component::Component_Animation, effectAnimComponent);
			//hit anim
			AnimClip* effectAnimClip = new AnimClip();
			int startTexIndex = EFFECT_EXPLOSION_ANIM_INDEX_COUNT * EFFECT_EXPLOSION_START_INDEX_Y + EFFECT_EXPLOSION_START_INDEX_X;
			int endTexIndex = startTexIndex + EFFECT_EXPLOSION_ANIM_INDEX_COUNT;
			effectAnimClip->SetAnimParam(E_Anim::Anim_Test_1, vector2s(startTexIndex, endTexIndex), 0.3f, true, false);
			effectAnimComponent->RegisterAnim(E_Anim::Anim_Test_1, effectAnimClip, true);
			//anim callback
			effectAnimClip->animEnd_Callback = this->hitEffect_end_cb;

			newEffect->StopEffect();
			newEffect->SetState(false);

			GameMgr::GetInstance()->sceneObjMgr->RegisterCell(newEffect);
			this->RegisterEffect(newEffect);
		}
		return newEffect;
	}
	else if (effectType == E_Effect::Effect_ShootLight) {
		Effect* newEffect = new Effect();
		newEffect->SetCellType(E_CellType::CellType_Effect);
		if (newEffect != nullptr) {
			newEffect->SetEffectType(effectType);
			newEffect->SetID(GameMgr::GetInstance()->sceneObjMgr->GetID());
			newEffect->SetScale(vector3s(0.8f, 0.8f, 1.0f));

			DrawData* effectDrawData = RenderMgr::GetNewDrawData(nullptr, true, E_BatchingType::BatchingType_Effect, true);
			if (effectDrawData == nullptr) {
				delete newEffect;
				return nullptr;
			}
			effectDrawData->SetSize(effect_hit_tex_cell_size);
			effectDrawData->SetShaderParam(shootLight_ShaderParam);
			newEffect->SetDrawData(effectDrawData);

			Animation* effectAnimComponent = new Animation();
			newEffect->RegisterComponent(E_Component::Component_Animation, effectAnimComponent);
			//hit anim
			AnimClip* effectAnimClip = new AnimClip();
			int startTexIndex = EFFECT_TEX_W_COUNT * EFFECT_SHOOT_LIGHT_START_INDEX_Y + EFFECT_SHOOT_LIGHT_START_INDEX_X;
			int endTexIndex = startTexIndex + EFFECT_SHOOT_LIGHT_ANIM_INDEX_COUNT;
			effectAnimClip->SetAnimParam(E_Anim::Anim_Test_1, vector2s(startTexIndex, endTexIndex), 0.1f, true, false);
			effectAnimComponent->RegisterAnim(E_Anim::Anim_Test_1, effectAnimClip, true);
			//anim callback
			effectAnimClip->animEnd_Callback = this->hitEffect_end_cb;

			newEffect->StopEffect();
			newEffect->SetState(false);

			GameMgr::GetInstance()->sceneObjMgr->RegisterCell(newEffect);
			this->RegisterEffect(newEffect);
		}
		return newEffect;
	}

	return nullptr;
}

Effect* EffectMgr::GetEffect(E_Effect effectType)
{
	if (effectPools.find(effectType) != effectPools.end()) {
		if (effectPools[effectType].size() != 0) {

			std::list<Effect*>::iterator iter;
			for (iter = effectPools[effectType].begin(); iter != effectPools[effectType].end(); iter++) {
				if ((*iter) == nullptr)continue;

				if ((*iter)->CheckState() == false) {
					return *iter;
				}
			}
		}
	}

	return  CreateEffectEntity(effectType);
}

Effect* EffectMgr::CreateEffect(E_Effect effectType, BaseCell* followObj, vector3s pos, vector3s dir)
{
	Effect* effect = GetEffect(effectType);
	if (effect == nullptr)return nullptr;

	vector3s directoin = VecNormalize(dir);

	float angle = (atan2(rightVector.x, rightVector.y) - atan2(dir.x, dir.y)) * RAD2DEG;
	effect->GetTransform()->SetRot(vector3s(0, 0, angle));

	pos.z = -10;
	effect->SetPos(pos, true);
	effect->StartEffect();
	effect->SetState(true);

	return effect;
}

void EffectMgr::RecycleEffect(Effect* effect)
{
	if (effect == nullptr)return;
	effect->SetState(false);
}

void EffectMgr::RegisterEffect(Effect* effect)
{
	if (effect == nullptr)return;
	E_Effect effectType = effect->GetEffectType();
	if (effectPools.find(effectType) == effectPools.end()) {
		effectPools[effectType].clear();
	}

	effectPools[effectType].push_back(effect);
}

void EffectMgr::SetCameraShake(float shakeTime, float shakeFactor)
{
	if (cameraShakeSwitch == false) {
		shakePosOffset = vector3s(0.0f, 0.0f, 0.0f);
	}
	else {
		if (shakeFactor < cameraShakeFactor) { 
			return;
		}
	}

	cameraShakeSwitch = true;
	cameraShakeTime = shakeTime;
	cameraShakeFactor = shakeFactor;
	cameraShakeTimePass = 0.0f;
}


#pragma endregion effect_manager
