/*==============================================================================

   共通ヘッダー [main.h]
                                                         Author : 
                                                         Date   : 
--------------------------------------------------------------------------------

==============================================================================*/
#pragma once
#ifndef _MAIN_H_
#define _MAIN_H_

#pragma warning(push)
#pragma warning(disable:4005)
#pragma warning(disable:4244)
#pragma warning(disable:26812)


#define _CRT_SECURE_NO_WARNINGS			// scanf のwarning防止
#include <stdio.h>
#include <string>
#include <vector>
#include <list>
#include <map>
//#include <list>

#include <d3d11.h>
#include <d3dx11.h>
#include <d3dcompiler.h>
#include <d3dx9.h>

//using namespace std;

#define DIRECTINPUT_VERSION 0x0800		// 警告対処
//#define SCREEN_WIDTH	(1280)				// ウインドウの幅
//#define SCREEN_HEIGHT	(720)				// ウインドウの高さ
typedef D3DXVECTOR3 vector3s;
typedef D3DXVECTOR2 vector2s;
typedef D3DXVECTOR4 vector4s;

#pragma region tool_method

inline float F_Clamp(float src, float min, float max);
inline int I_Lerp(float src, float dest, float progress);
inline float VecDistS(vector3s src, vector3s dest);
inline float VecDist(vector3s src, vector3s dest);
inline float VecLengthS(vector3s src);
inline float VecLengthS(vector2s src);
inline float VecLength(vector3s src);
inline float VecLength(vector2s src);
inline float VecDot(vector3s src, vector3s dest);
inline float VecDot(vector2s src, vector2s dest);
inline vector3s VecClamp(vector3s src, float minV, float maxV);
inline vector2s VecNormalize(const vector2s& src);
inline vector3s VecNormalize(const vector3s& src);
inline vector3s VecLerp(const vector3s& src, const vector3s& dest, float progress);
inline float s_max(float v1, float v2);
inline float s_min(float v1, float v2);
bool AdjustScreenPoint(vector3s& pos, vector2s size);
inline vector3s GetReflectDir2D(const vector3s& src, const vector3s& normal);

inline float F_Clamp(float src, float min, float max) {
	float result = src;
	if (src < min) {
		result = min;
	}
	else if (src > max) {
		result = max;
	}

	return result;
}

inline int I_Lerp(float src, float dest, float progress) {
	if (progress >= 1) {
		return dest;
	}
	else if (progress <= 0) {
		return src;
	}

	return src + (dest - src) * progress;
}

inline float VecDistS(vector3s src, vector3s dest) {
	vector3s temp = src - dest;
	return temp.x * temp.x + temp.y * temp.y;
}

inline float VecDist(vector3s src, vector3s dest) {
	vector3s temp = src - dest;
	return sqrtf(temp.x * temp.x + temp.y * temp.y);
}

inline float VecLengthS(vector3s src) {
	return src.x * src.x + src.y * src.y + src.z * src.z;
}

inline float VecLengthS(vector2s src) {
	return src.x * src.x + src.y * src.y;
}

inline float VecLength(vector3s src) {
	return sqrtf(src.x * src.x + src.y * src.y + src.z * src.z);
}

inline float VecLength(vector2s src) {
	return sqrtf(src.x * src.x + src.y * src.y);
}

inline float VecDot(vector3s src, vector3s dest) {
	return src.x * dest.x + src.y * dest.y + src.z * dest.z;
}

inline float VecDot(vector2s src, vector2s dest) {
	return src.x * dest.x + src.y * dest.y;
}

inline vector2s VecNormalize(const vector2s& src) {
	float length = VecLength(src);
	vector2s result;

	if (length <= 0.0001f) {
		result.x = 0;
		result.y = 0;
	}
	else {
		result = src / length;
	}
	return result;
}

inline vector3s VecNormalize(const vector3s& src) {
	float length = VecLength(src);
	vector3s result;

	if (length <= 0.0001f) {
		result.x = 0;
		result.y = 0;
		result.z = 0;
	}
	else {
		result = src / length;
	}
	return result;
}

inline vector3s operator*(vector3s& src, const float factor) {
	vector3s result;
	result.x = src.x * factor;
	result.y = src.y * factor;
	result.z = src.z * factor;
	return result;
}

inline vector3s VecClamp(vector3s src, float minV, float maxV) {
	vector3s temp = src;
	float curLengthS = VecLengthS(temp);
	if (curLengthS < minV * minV) {
		vector3s tempNormalize = VecNormalize(temp);
		return tempNormalize * minV;
	}
	else if (curLengthS > maxV * maxV) {
		return VecNormalize(temp) * maxV;
	}

	return temp;
}

inline vector3s VecLerp(const vector3s& src, const vector3s& dest, float progress) {
	vector3s result;
	if (progress >= 1) {
		result = dest;
	}
	else if (progress <= 0) {
		result = src;
	}

	result = src + progress * (dest - src);
	return result;
}

inline vector3s GetReflectDir2D(const vector3s& src, const vector3s& normal) {
	vector3s result = vector3s(0, 0, 0);
	vector3s n = normal;
	n.z = 0.0f;
	n = VecNormalize(n);

	float num = src.x * n.x + src.y * n.y;

	result.x = src.x - ((2.0f * num) * n.x);
	result.y = src.y - ((2.0f * num) * n.y);

	return VecNormalize(result);
}

inline float s_max(float v1, float v2) {
	if (v1 > v2) {
		return v1;
	}
	else {
		return v2;
	}
}

inline float s_min(float v1, float v2) {
	if (v1 > v2) {
		return v2;
	}
	else {
		return v1;
	}
}


inline vector2s VecTransformTToD(const vector3s& src) {
	vector2s result = vector2s(src.x, src.y);
	return result;
}

inline vector3s VecTransformDToT(const vector2s& src)
{
	vector3s result = vector3s(0.0f, 0.0f, 0.0f);
	result.x = src.x;
	result.y = src.y;
	return result;
}

inline vector2s operator*(const vector2s& src, const vector2s& dest) {
	vector2s result;
	result.x = src.x * dest.x;
	result.y = src.y * dest.y;
	return result;
}

inline int GetRandomRangeValueI(int _range) {
	if (_range == 0)return 0;
	int range = abs(_range);
	//[0 , range ] mapping to [ -range , range]
	return (rand() % range * 2 - range);
}

inline int GetAbsRandomRangeValueI(int _range) {
	if (_range == 0)return 0;

	return rand() % _range;
}

#pragma endregion tool_method

#include "dinput.h"
#include "mmsystem.h"
#include "sprite.h"
#include "texture.h"
#include "sound.h"
#include "renderer.h"
#include "input.h"
#include "game.h"

#include "manager.h"
#include "sceneManager.h"
#include "component.h"

#pragma warning(pop)



//*****************************************************************************
// マクロ定義
//*****************************************************************************

// Window dimensions
#define SCREEN_WIDTH 1920
#define SCREEN_HEIGHT 1080

#define SCREEN_UNIT_SIZE_X 0.08f
#define SCREEN_UNIT_SIZE_Y 0.125f

#define MOUSE_SENSITY 0.05f

#define DEG2RAD 3.1415926f/180.0f
#define RAD2DEG 180.0f/3.1415926f

const float SCREEN_TEXEL_WIDTH = 1.0f / SCREEN_WIDTH;
const float SCREEN_TEXEL_HEIGHT = 1.0f / SCREEN_HEIGHT;

#define Z_AXIS_OFFSET_BASE_GUIDE -0.30f
#define Z_AXIS_OFFSET_TITLE_PLAYER -0.21f
#define Z_AXIS_OFFSET_MENU_ITEM -0.20f
#define Z_AXIS_OFFSET_SELECTOR -0.11f
#define Z_AXIS_OFFSET_BG -0.10f
#define Z_AXIS_OFFSET_SCENE_MENU_ITEM -0.07f
#define Z_AXIS_OFFSET_CARD -0.03f
#define Z_AXIS_OFFSET_SCENE_SPRITE -0.02f
#define Z_AXIS_OFSFET_PLAYER -0.01f
#define Z_AXIS_OFFSET_SCENE_ENEMY 0.0f
#define Z_AXIS_OFFSET_BG_ITEM 0.1f;

#define TEST_OBSTACLE_SIZE_X 1920
#define TEST_OBSTACLE_SIZE_Y 1080

#define CAMERA_LERP_FACTOR 0.3f

#define DEFAULT_GAME_TIME 301

//collision
#define SECOND_DETECT_OFFSET_X 40
#define SECOND_DETECT_OFFSET_Y 40

//draw
//test param
#define TEST_DRAW_WIDTH 1920
#define TEST_DRAW_HEIGHT 800

#define GUIDE_TEX_SIZE_X 1920
#define GUIDE_TEX_SIZE_Y 1080

//background
	//grass
#define GRASS_TEX_WIDTH 192
#define GRASS_TEX_HEIGHT 64
#define GRASS_TEX_CELL_COUNT_W 3
#define GRASS_TEX_CELL_COUNT_H 1
#define GRASS_TEX_CELL_SIZE_X 64
#define GRASS_TEX_CELL_SIZE_Y 64
#define GRASS_TEX_START_INDEX_X (1 - 1)
#define GRASS_TEX_START_INDEX_Y (1 - 1)

//character(tex
	//player
#define CHARACTER_TEX_WIDTH 640
#define CHARACTER_TEX_HEIGHT 960
#define PLAYER_TEX_SIZE_X 64
#define PLAYER_TEX_SIZE_Y 64
#define PLAYER_TEX_W_COUNT 10
#define PLAYER_TEX_H_COUNT 6
#define PLAYER_IDLE_INDEX_X (1-1)
#define PLAYER_IDLE_INDEX_Y (14 - (15-1))
#define PLAYER_IDLE_FRAME_COUNT 5
#define PLAYER_HITED_INDEX_X (6-1)
#define PLAYER_HITED_INDEX_Y (14 - (15-1))
#define PLAYER_HITED_FRAME_COUNT 1
#define PLAYER_MOVE_INDEX_X (1-1)
#define PLAYER_MOVE_INDEX_Y (14 - (14-1))
#define PLAYER_MOVE_FRAME_COUNT 8
	//enemy
#define ENEMY_TEX_SIZE_X 64
#define ENEMY_TEX_SIZE_Y 64
		//1
#define ENEMY_1_TEX_W_COUNT 10
#define ENEMY_1_TEX_H_COUNT 6
#define ENEMY_1_IDLE_INDEX_X (1-1)
#define ENEMY_1_IDLE_INDEX_Y (14 - (12-1))
#define ENEMY_1_IDLE_FRAME_COUNT 20
#define ENEMY_1_DMG_INDEX_X (1-1)
#define ENEMY_1_DMG_INDEX_Y (14 - (10-1))
#define ENEMY_1_DMG_FRAME_COUNT 1
		//2
#define ENEMY_2_TEX_W_COUNT 10
#define ENEMY_2_TEX_H_COUNT 6
#define ENEMY_2_IDLE_INDEX_X (1-1)
#define ENEMY_2_IDLE_INDEX_Y (14 - (9-1))
#define ENEMY_2_IDLE_FRAME_COUNT 20
#define ENEMY_2_DMG_INDEX_X (1-1)
#define ENEMY_2_DMG_INDEX_Y (14 - (7-1))
#define ENEMY_2_DMG_FRAME_COUNT 1
		//3
#define ENEMY_3_TEX_W_COUNT 10
#define ENEMY_3_TEX_H_COUNT 6
#define ENEMY_3_IDLE_INDEX_X (1-1)
#define ENEMY_3_IDLE_INDEX_Y (14 - (6-1))
#define ENEMY_3_IDLE_FRAME_COUNT 20
#define ENEMY_3_ATTACK_INDEX_X (2-1)
#define ENEMY_3_ATTACK_INDEX_Y (14 - (4-1))
#define ENEMY_3_ATTACK_FRAME_COUNT 9
#define ENEMY_3_DMG_INDEX_X (1-1)
#define ENEMY_3_DMG_INDEX_Y (14 - (4-1))
#define ENEMY_3_DMG_FRAME_COUNT 1

	//gun_light
#define GUN_LIGHT_TEX_SIZE_X 64
#define GUN_LIGHT_TEX_SIZE_Y 64
#define GUN_LIGHT_TEX_W_COUNT 10
#define GUN_LIGHT_TEX_H_COUNT 10
#define GUN_LIGHT_START_INDEX_X (1-1)
#define GUN_LIGHT_START_INDEX_Y (14 - (13-1))
#define GUN_LIGHT_FRAME_COUNT 10

#define DOOR_TEX_INDEX_X (1-1)
#define DOOR_TEX_INDEX_Y (4-1)
//bullet(tex
#define BULLET_TEX_SIZE_X 58
#define BULLET_TEX_SIZE_Y 29
#define BULLET_CELL_TEX_SIZE_X 29
#define BULLET_CELL_TEX_SIZE_Y 29
#define BULLET_ENEMY_CELL_TEX_SIZE_X 42
#define BULLET_ENEMY_CELL_TEX_SIZE_Y 42
#define BULLET_TEX_W_COUNT 2
#define BULLET_TEX_H_COUNT 1

#define BULLET_NEW_TEX_SIZE_X 320
#define BULLET_NEW_TEX_SIZE_Y 64
#define BULLET_NEW_TEX_WIDTH_COUNT 5
#define BULLET_NEW_TEX_HEIGHT_COUNT 1
#define BULLET_NEW_TEX_CELL_SIZE_X 64
#define BULLET_NEW_TEX_CELL_SIZE_Y 64
#define BULLET_NEW_TEX_BULLET_START_INDEX_X (1-1)
#define BULLET_NEW_TEX_BULLET_START_INDEX_Y (1-1)
#define BULLET_NEW_TEX_CANNON_START_INDEX_X (2-1)
#define BULLET_NEW_TEX_CANNON_START_INDEX_Y (1-1)
#define BULLET_NEW_TEX_LASER_START_INDEX_X (3-1)
#define BULLET_NEW_TEX_LASER_START_INDEX_Y (1-1)
#define BULLET_NEW_TEX_ENEMY_NORMAL_BULLET_START_INDEX_X (4-1)
#define BULLET_NEW_TEX_ENEMY_NORMAL_BULLET_START_INDEX_Y (1-1)
#define BULLET_NEW_TEX_ENEMY_NORMAL_BULLET_REFLECTED_START_INDEX_X (5-1)
#define BULLET_NEW_TEX_ENEMY_NORMAL_BULLET_REFLECTED_START_INDEX_Y (1-1)


#define BULLET_LASER_TEX_SIZE_X 421
#define BULLET_LASER_TEX_SIZE_Y 76
#define BULLET_LASER_TEX_W_COUNT 1
#define BULLET_LASER_TEX_H_COUNT 1

#define BULLET_CANNON_TEX_SIZE_X 160
#define BULLET_CANNON_TEX_SIZE_Y 16
#define BULLET_CANNON_CELL_SIZE_X 16
#define BULLET_CANNON_CELL_SIZE_Y 16
#define BULLET_CANNON_TEX_W_COUNT 10
#define BULLET_CANNON_TEX_H_COUNT 1

//bullet(normal
#define BULLET_NORMAL_MIN_DMG 2.0f
#define BULLET_NORMAL_BASE_DMG 4.0f
#define BULLET_NORMAL_BASE_DMG_MAX 15.0f
#define BULLET_NORMAL_DEFAULT_COUNT 1
#define BULLET_NORMAL_MAX_COUNT 2
#define BULLET_NORMAL_OFFSET_ANGLE_HALF 15.0f
#define BULLET_NORMAL_OFFSET_DIST 10.0f

#define BULLET_NORMAL_BASE_RECOVERY_CAP 3
#define BULLET_NORMAL_DEFEAT_RECOVERY_CAP 10

#define BULLET_NORMAL_DEFAULT_REFLECT_COUNT 0
#define BULLET_NORMAL_MAX_REFLECT_COUNT 5

//bullet(shotgun
#define BULLET_SHOTGUN_BASE_DMG 1.0f
#define BULLET_SHOTGUN_BASE_DMG_MAX 10.0
#define BULLET_SHOTGUN_DEFAULT_BULLET_COUNT 3
#define BULLET_SHOTGUN_MAX_BULLET_COUNT 6
#define BULLET_SHOOTPOS_OFFSET_ANGLE 4.0f
#define BULLET_SHOTGUN_SPEED_FACTOR 0.7f
#define BULLET_SHOTGUN_BULLET_LIFETIME_DEFAULT 0.6f
#define BULLET_SHOTGUN_BULLET_LIFETIME_MAX 0.9f

#define BULLET_SHOTGUN_BASE_RECOVERY_CAP 0
#define BULLET_SHOTGUN_DEFEAT_RECOVERY_CAP 15

#define BULLET_SHOTGUN_DEFAULT_REFLECT_COUNT 0
#define BULLET_SHOTGUN_MAX_REFLECT_COUNT 5

//bullet(cannon
#define BULLET_CANNON_BASE_DMG 10.0f
#define BULLET_CANNON_BASE_DMG_MAX 30.0f
#define BULLET_CANNON_SPEED_FACTOR 0.6f
#define BULLET_CANNON_DMG_REDUCE_FACTOR 0.5f
#define CANNON_LIFE_TIME 1.3f

#define BULLET_CANNON_BASE_RECOVERY_CAP 5
#define BULLET_CANNON_DEFEAT_RECOVERY_CAP 10
#define BULLET_CANNON_DEFAULT_EXPLODE_RANGE 150.0f
#define BULLET_CANNON_MAX_EXPLODE_RANGE 300.0f
#define BULLET_CANNON_DEFAULT_EXPLODE_EFFECT_SIZE 4.0f
#define BULLET_CANNON_MAX_EXPLODE_EFFECT_SIZE 8.0f

//bullet(laser
#define BULLET_LASER_BASE_DMG 5.0f
#define BULLET_LASER_BASE_DMG_MAX 10.0f
#define BULLET_LASER_DEFAULT_SCALE_X 6.0f
#define BULLET_LASER_DEFAULT_SCALE_Y 0.7f
#define BULLET_LASER_LIFETIME_DEFAULT 0.5f
#define BULLET_LASER_LIFETIME_MAX 3.0f

#define BULLET_LASER_BASE_RECOVERY_CAP 10
#define BULLET_LASER_DEFEAT_RECOVERY_CAP 15

//bullet(enemy_normal
#define BULLET_ENEMY_NORMAL_MOVE_SPEED 500.0f

//bullet status param
#define DEF_BULLET_RECOVERY_CAP_DEFAULT 1.0f
#define DEF_BULLET_RECOVERY_CAP_MAX 3.0f

#define EXP_BULLET_GAIN_DEFAULT 1.0f
#define EXP_BULLET_GAIN_MAX 3.0f

#define BULLET_REFLECT_COUNT 0


#define LASER_LIFE_TIME 0.5f //temp
#define LASER_MAX_LIFE_TIME 2.0f

//effect
#define EFFECT_TEX_SIZE_X 768
#define EFFECT_TEX_SIZE_Y 256
#define EFFECT_TEX_W_COUNT 12
#define EFFECT_TEX_H_COUNT 4

#define EFFECT_HIT_CELL_TEX_SIZE_X 64//hit
#define EFFECT_HIT_CELL_TEX_SIZE_Y 64
#define EFFECT_HIT_START_INDEX_X 0 
#define EFFECT_HIT_START_INDEX_Y (3-(4-1))
#define EFFECT_HIT_ANIM_INDEX_COUNT 6

#define EFFECT_HIT_RED_CELL_TEX_SIZE_X 64//hit
#define EFFECT_HIT_RED_CELL_TEX_SIZE_Y 64
#define EFFECT_HIT_RED_START_INDEX_X (7-1)
#define EFFECT_HIT_RED_START_INDEX_Y (3-(4-1))
#define EFFECT_HIT_RED_ANIM_INDEX_COUNT 6

#define EFFECT_SHOOT_LIGHT_START_INDEX_X 0//shoot light
#define EFFECT_SHOOT_LIGHT_START_INDEX_Y (3-(3-1))
#define EFFECT_SHOOT_LIGHT_ANIM_INDEX_COUNT 6

#define EFFECT_EXPLOSION_CELL_TEX_SIZE_X 128//explosion
#define EFFECT_EXPLOSION_CELL_TEX_SIZE_Y 128
#define EFFECT_EXPLOSION_START_INDEX_X 0
#define EFFECT_EXPLOSION_START_INDEX_Y (1-(1-1))
#define EFFECT_EXPLOSION_ANIM_INDEX_COUNT 6

#define EFFECT_HIT_TEX_SIZE_X 320
#define EFFECT_HIT_TEX_SIZE_Y 128
#define EFFECT_HIT_TEX_W_COUNT 5
#define EFFECT_HIT_TEX_H_COUNT 2


//movement(player
#define PLAYER_REPULSE_TIME 0.8f
#define PLAYER_REPULSE_FORCE 1000.0f
#define PLAYER_REPULSE_REDUCE_FACTOR 0.9f
#define PLAYER_BE_HIT_INVINCIBLE_TIME 3.0f

//movement(enemy
#define REPULSE_TIME 1.0f
#define REPULSE_FORCE 200.0f
#define REPULSE_REDUCE_FACTOR 0.9f

#pragma region ui

//ui
#define UI_TEX_SIZE_X 1024	
#define UI_TEX_SIZE_Y 1024

//NUM
#define NUM_TEX_W_COUNT 32 
#define NUM_TEX_H_COUNT 32
#define NUM_WHITE_TEX_START_INDEX_X 0
#define NUM_WHITE_TEX_START_INDEX_Y (31 - (23-1))
#define NUM_WHITE_OUTLINE_TEX_START_INDEX_X 0
#define NUM_WHITE_OUTLINE_TEX_START_INDEX_Y (31 - (22-1))
#define NUM_RED_TEX_START_INDEX_X 0
#define NUM_RED_TEX_START_INDEX_Y (31 - (21-1))


#define WORD_SIZE_X 16
#define WORD_SIZE_Y 16

#define TIME_WORD_INTERVAL 7.0f
#define DEFAULT_WORD_INTERVAL 5.0f

#define DEFAULT_FONT_SIZE 11

#define DMG_UI_WORD_INTERVAL 6.0f
#define DMG_UI_SCALE 2.5f

//img

//title
#define TITLE_TEX_SIZE_X 960
#define TITLE_TEX_SIZE_Y 304
#define TITLE_TEX_START_X 0
#define TITLE_TEX_START_Y 720

	//status img
#define STATUS_IMG_TEX_SIZE_X 64   
#define STATUS_IMG_TEX_SIZE_Y 160
#define STATUS_IMG_TEX_START_X 0  
#define STATUS_IMG_TEX_START_Y (32 * ( 31 - (32-1)))

#define STATUS_SINGLE_IMG_TEX_SIZE_X 64
#define STATUS_SINGLE_IMG_TEX_SIZE_Y 32
#define STATUS_LV_IMG_TEX_START_X 0  
#define STATUS_LV_IMG_TEX_START_Y (32 * ( 31 -(32-1)))
#define STATUS_HP_IMG_TEX_START_X 0  
#define STATUS_HP_IMG_TEX_START_Y (32 * ( 31 -(31-1)))
#define STATUS_ATK_IMG_TEX_START_X 0  
#define STATUS_ATK_IMG_TEX_START_Y (32 * ( 31 -(30-1)))
#define STATUS_DEF_IMG_TEX_START_X 0  
#define STATUS_DEF_IMG_TEX_START_Y (32 * ( 31 -(29-1)))
#define STATUS_EXP_IMG_TEX_START_X 0  
#define STATUS_EXP_IMG_TEX_START_Y (32 * ( 31 -(28-1)))

	//status img(lv_panel
#define LV_PANEL_STATUS_IMG_TEX_SIZE_X 64   
#define LV_PANEL_STATUS_IMG_TEX_SIZE_Y 128
#define LV_PANEL_STATUS_IMG_TEX_START_X 0  
#define LV_PANEL_STATUS_IMG_TEX_START_Y (32 * (31 - (32-1)))

	//status selector
#define STATUS_SELECTOR_TEX_SIZE_X (32* 11)
#define STATUS_SELECTOR_TEX_SIZE_Y 32
#define STATUS_SELECTOR_START_X 0
#define STATUS_SELECTOR_START_Y (32* (31 - (24-1)))

		//pos
#define STATUS_SELECTOR_HP_POS_X 580
#define STATUS_SELECTOR_HP_POS_Y -285
#define STATUS_SELECTOR_ATK_POS_X 580
#define STATUS_SELECTOR_ATK_POS_Y (STATUS_SELECTOR_HP_POS_Y - 64 * 1)
#define STATUS_SELECTOR_DEF_POS_X 580
#define STATUS_SELECTOR_DEF_POS_Y (STATUS_SELECTOR_HP_POS_Y - 64 * 2)
#define STATUS_SELECTOR_EXP_POS_X 580
#define STATUS_SELECTOR_EXP_POS_Y (STATUS_SELECTOR_HP_POS_Y - 64 * 3)

	//card selector(single)
#define CARD_SELECTOR_TEX_SIZE_X 32
#define CARD_SELECTOR_TEX_SIZE_Y 32
#define CARD_SELECTOR_LT_START_X (32 * (32-1))
#define CARD_SELECTOR_LT_START_Y (32 * (31 - (32-1)))
#define CARD_SELECTOR_RT_START_X (32 * (32-1))
#define CARD_SELECTOR_RT_START_Y (32 * (31 - (31-1)))
#define CARD_SELECTOR_LB_START_X (32 * (32-1))
#define CARD_SELECTOR_LB_START_Y (32 * (31 - (30-1)))
#define CARD_SELECTOR_RB_START_X (32 * (32-1))
#define CARD_SELECTOR_RB_START_Y (32 * (31 - (29-1)))

	//crosshair
#define CROSSHAIR_TEX_SIZE_X 64
#define CROSSHAIR_TEX_SIZE_Y 64
#define CROSSHAIR_TEX_START_X (32 * (32-2))
#define CROSSHAIR_TEX_START_Y (32 * (31- (23-1)))

//hud
#define HUD_CONSUME_DATA_DEFAULT_OFFSET_X 55
#define HUD_LV_DATA_DEFAULT_OFFSET_X 150
#define HUD_CUR_DATA_DEFAULT_OFFSET_X 95
#define HUD_MAX_DATA_DEFAULT_OFFSET_X 185
#define HUD_SLASH_DEFAULT_OFFSET_X 153

//timer
#define HUD_GAME_TIMER_POS_X (-SCREEN_WIDTH * 0.5f + 170.0f)
#define HUD_GAME_TIMER_POS_Y (SCREEN_HEIGHT * 0.5f - 120.0f)
#define HUD_GAME_TIMER_MINUTE_OFFSET_X -55
#define HUD_GAME_TIMER_SECOND_OFFSET_X 25

//menu item
#define TITLE_SCALE 1.3f
#define TITLE_ITEM_COUNT 2
#define TITLE_ITEM_SCALE 3.0f

#define MENU_IMG_SCALE 4.0f

#define CARD_COUNT 4

#define MENU_ITEM_COUNT 3 // continue retry title

#define LOSE_MENU_ITEM_COUNT 2 //retry title

#define WIN_MENU_ITEM_COUNT 1 // title

#define DEFAULT_TITLE_ITEM_INDEX 0
#define DEFAULT_CARD_INDEX 0
#define DEFAULT_MENU_INDEX 0
#define DEFAULT_LOSE_MENU_INDEX 0 
#define DEFAULT_WIN_MENU_INDEX 0

#define SELECTOR_TITLE_CONST_SIZE_X 260
#define SELECTOR_TITLE_CONST_SIZE_Y 50
#define SELECTOR_MENU_CONST_SIZE_X 300

	//card
#define CARD_IMG_TEX_SIZE_X 128
#define CARD_IMG_TEX_SIZE_Y 128
#define CARD_IMG_TEX_START_X (32 * (3-1))
#define CARD_IMG_TEX_START_Y (32 * (31 - (29-1)))

	//card 2
#define CARD_2_IMG_TEX_SIZE_X 256
#define CARD_2_IMG_TEX_SIZE_Y 256
#define CARD_2_IMG_TEX_START_X (32 * (7 -1))
#define CARD_2_IMG_TEX_START_Y (32 * (31 - (32-1)))
	//card 3(Extra bg)
#define CARD_3_IMG_TEX_SIZE_X 256
#define CARD_3_IMG_TEX_SIZE_Y 256
#define CARD_3_IMG_TEX_START_X (32 * (15 -1))
#define CARD_3_IMG_TEX_START_Y (32 * (31 - (32-1)))

	//card word(condition)
#define CARD_CONDITION_WORD_OFFSET_1_X 0
#define CARD_CONDITION_WORD_OFFSET_2_X 0
#define CARD_CONDITION_WORD_OFFSET_3_X 0
//#define CARD_CONDITION_WORD_OFFSET_1_X -50
//#define CARD_CONDITION_WORD_OFFSET_2_X -20
//#define CARD_CONDITION_WORD_OFFSET_3_X 20
#define CARD_CONDITION_WORD_OFFSET_1_Y 85
#define CARD_CONDITION_WORD_OFFSET_2_Y 55
#define CARD_CONDITION_WORD_OFFSET_3_Y 25
	//card word(effect)
#define CARD_EFFECT_WORD_OFFSET_1_X 0
#define CARD_EFFECT_WORD_OFFSET_2_X 0
#define CARD_EFFECT_WORD_OFFSET_3_X 0
//#define CARD_EFFECT_WORD_OFFSET_1_X -50
//#define CARD_EFFECT_WORD_OFFSET_2_X -20
//#define CARD_EFFECT_WORD_OFFSET_3_X 20
#define CARD_EFFECT_WORD_OFFSET_1_Y -30
#define CARD_EFFECT_WORD_OFFSET_2_Y -60
#define CARD_EFFECT_WORD_OFFSET_3_Y -90

//word
#pragma region word_tex_param 
//level up
#define LV_UP_IMG_APPEAR_TIME 5.0f

#define LV_UP_IMG_TEX_SIZE_X 224
#define LV_UP_IMG_TEX_SIZE_Y 32
#define LV_UP_IMG_TEX_START_X 0
#define LV_UP_IMG_TEX_START_Y (32 * (31 - (20-1)))

//row 7
	//max
#define WORD_MAX_TEX_SIZE_X (16 * 5)
#define WORD_MAX_TEX_SIZE_Y (16 * 2)
#define WORD_MAX_TEX_START_X 0
#define WORD_MAX_TEX_START_Y (32* (31 - (19-1)))
	//up
#define WORD_UP_TEX_SIZE_X (16 * 3)
#define WORD_UP_TEX_SIZE_Y (16 * 2)
#define WORD_UP_TEX_START_X (16*(6-1))
#define WORD_UP_TEX_START_Y (32* (31 - (19-1)))
	//attack
#define WORD_ATTACK_TEX_SIZE_X (16 * 8)
#define WORD_ATTACK_TEX_SIZE_Y (16 * 2)
#define WORD_ATTACK_TEX_START_X (16*(9-1))
#define WORD_ATTACK_TEX_START_Y (32* (31 - (19-1)))
	//speed
#define WORD_SPEED_TEX_SIZE_X (16 * 7)
#define WORD_SPEED_TEX_SIZE_Y (16 * 2)
#define WORD_SPEED_TEX_START_X (16 * (17-1))
#define WORD_SPEED_TEX_START_Y (32 * (31 - (19-1)))
	//bullet
#define WORD_BULLET_TEX_SIZE_X (16 * 8)
#define WORD_BULLET_TEX_SIZE_Y (16 * 2)
#define WORD_BULLET_TEX_START_X (16 * (24-1))
#define WORD_BULLET_TEX_START_Y (32 * (31 - (19-1)))

		//prat 2
	//you survive
#define WORD_YOU_SURVIVE_TEX_SIZE_X	(16 * 17)
#define WORD_YOU_SURVIVE_TEX_SIZE_Y (16 * 2)
#define WORD_YOU_SURVIVE_TEX_START_X (16 * (33 -1))
#define WORD_YOU_SURVIVE_TEX_START_Y (32* (31 - (19 - 1)))
	//retry
#define WORD_RETRY_TEX_SIZE_X (16 * 7)
#define WORD_RETRY_TEX_SIZE_Y (16 * 2)
#define WORD_RETRY_TEX_START_X (16 * (50 - 1))
#define WORD_RETRY_TEX_START_Y (32 * (31 - (19 - 1)))
	//pause
#define WORD_PAUSE_TEX_SIZE_X (16 * 8)
#define WORD_PAUSE_TEX_SIZE_Y (16 * 2)
#define WORD_PAUSE_TEX_START_X (16 * (57 - 1))
#define WORD_PAUSE_TEX_START_Y (32 * (31 - (19 - 1)))


//row 6
	//consume
#define WORD_CONSUME_TEX_SIZE_X (16 * 11)
#define WORD_CONSUME_TEX_SIZE_Y (16 * 2)
#define WORD_CONSUME_TEX_START_X 0
#define WORD_CONSUME_TEX_START_Y (32* (31 - (18-1)))
	//gain
#define WORD_GAIN_TEX_SIZE_X (16 * 5)
#define WORD_GAIN_TEX_SIZE_Y (16 * 2)
#define WORD_GAIN_TEX_START_X (16 * (12-1))
#define WORD_GAIN_TEX_START_Y (32* (31 - (18-1)))
	//recovery
#define WORD_RECOVERY_TEX_SIZE_X (16 * 11)
#define WORD_RECOVERY_TEX_SIZE_Y (16 * 2)
#define WORD_RECOVERY_TEX_START_X (16 * (17-1))
#define WORD_RECOVERY_TEX_START_Y (32* (31 - (18-1)))
	//cap
#define WORD_CAP_TEX_SIZE_X (16 * 4)
#define WORD_CAP_TEX_SIZE_Y (16 * 2)
#define WORD_CAP_TEX_START_X (16 * (28-1))
#define WORD_CAP_TEX_START_Y (32 * (31 - (18-1)))
		//prat 2
	//thank you for playing
#define WORD_THANK_TEX_SIZE_X (16 * 30)
#define WORD_THANK_TEX_SIZE_Y (16 * 2)
#define WORD_THANK_TEX_START_X (16 * (33 -1))
#define WORD_THANK_TEX_START_Y (32 * (31 - (18-1)))

//row 5
	//damage
#define WORD_DAMAGE_TEX_SIZE_X (16 * 10)
#define WORD_DAMAGE_TEX_SIZE_Y (16 * 2)
#define WORD_DAMAGE_TEX_START_X 0
#define WORD_DAMAGE_TEX_START_Y (32* (31 - (17-1)))
	//reflect
#define WORD_REFLECT_TEX_SIZE_X (16 * 9)
#define WORD_REFLECT_TEX_SIZE_Y (16 * 2)
#define WORD_REFLECT_TEX_START_X (16 * (11-1))
#define WORD_REFLECT_TEX_START_Y (32* (31 - (17-1)))
	//enemy
#define WORD_ENEMY_TEX_SIZE_X (16 * 8)
#define WORD_ENEMY_TEX_SIZE_Y (16 * 2)
#define WORD_ENEMY_TEX_START_X (16 * (20-1))
#define WORD_ENEMY_TEX_START_Y (32* (31 - (17-1)))
		//prat 2
	//continue
#define WORD_CONTINUE_TEX_SIZE_X (16 * 11)
#define WORD_CONTINUE_TEX_SIZE_Y (16 * 2)
#define WORD_CONTINUE_TEX_START_X (16 * (33 -1))
#define WORD_CONTINUE_TEX_START_Y (32 * (31 - (17 - 1)))

//row 4
	//you lose
#define WORD_YOU_LOSE_TEX_SIZE_X (16 * 14)
#define WORD_YOU_LOSE_TEX_SIZE_Y (16 * 2)
#define WORD_YOU_LOSE_TEX_START_X 0
#define WORD_YOU_LOSE_TEX_START_Y (32 * (31 - (16-1)))
	//start
#define WORD_START_TEX_SIZE_X (16 * 7)
#define WORD_START_TEX_SIZE_Y (16 * 2)
#define WORD_START_TEX_START_X (16 * (15-1))
#define WORD_START_TEX_START_Y (32 * (31 - (16-1)))
	//exit
#define WORD_EXIT_TEX_SIZE_X (16 * 5)
#define WORD_EXIT_TEX_SIZE_Y (16 * 2)
#define WORD_EXIT_TEX_START_X (16 * (22-1))
#define WORD_EXIT_TEX_START_Y (32 * (31 - (16-1)))
	//title
#define WORD_TITLE_TEX_SIZE_X (16 * 6)
#define WORD_TITLE_TEX_SIZE_Y (16 * 2)
#define WORD_TITLE_TEX_START_X (16 * (27-1))
#define WORD_TITLE_TEX_START_Y (32 * (31 - (16-1)))

//row 3
	//move
#define WORD_MOVE_TEX_SIZE_X (16 * 7)
#define WORD_MOVE_TEX_SIZE_Y (16 * 2)
#define WORD_MOVE_TEX_START_X 0
#define WORD_MOVE_TEX_START_Y (32 * (31 - (15-1)))
	//count
#define WORD_COUNT_TEX_SIZE_X (16 * 8)
#define WORD_COUNT_TEX_SIZE_Y (16 * 2)
#define WORD_COUNT_TEX_START_X (16 * (8-1))
#define WORD_COUNT_TEX_START_Y (32 * (31 - (15-1)))
	//create
#define WORD_CREATE_TEX_SIZE_X (16 * 8)
#define WORD_CREATE_TEX_SIZE_Y (16 * 2)
#define WORD_CREATE_TEX_START_X (16 * (16-1))
#define WORD_CREATE_TEX_START_Y (32 * (31 - (15-1)))
	//every
#define WORD_EVERY_TEX_SIZE_X (16 * 8)
#define WORD_EVERY_TEX_SIZE_Y (16 * 2)
#define WORD_EVERY_TEX_START_X (16 * (16-1))
#define WORD_EVERY_TEX_START_Y (32 * (31 - (15-1)))

//row 2
	//die
#define WORD_DIE_TEX_SIZE_X (16 * 4)
#define WORD_DIE_TEX_SIZE_Y (16 * 2)
#define WORD_DIE_TEX_START_X 0
#define WORD_DIE_TEX_START_Y (32 * (31 - (14-1)))
	//hit
#define WORD_HIT_TEX_SIZE_X (16 * 4)
#define WORD_HIT_TEX_SIZE_Y (16 * 2)
#define WORD_HIT_TEX_START_X (16 * (5-1))
#define WORD_HIT_TEX_START_Y (32 * (31 - (14-1)))
	//second
#define WORD_SECOND_TEX_SIZE_X (16 * 9)
#define WORD_SECOND_TEX_SIZE_Y (16 * 2)
#define WORD_SECOND_TEX_START_X (16 * (9-1))
#define WORD_SECOND_TEX_START_Y (32 * (31 - (14-1)))
	//boost
#define WORD_BOOST_TEX_SIZE_X (16 * 8)
#define WORD_BOOST_TEX_SIZE_Y (16 * 2)
#define WORD_BOOST_TEX_START_X (16 * (18-1)) 
#define WORD_BOOST_TEX_START_Y (32 * (31 - (14-1)))
	//auto
#define WORD_AUTO_TEX_SIZE_X (16 * 6)
#define WORD_AUTO_TEX_SIZE_Y (16 * 2)
#define WORD_AUTO_TEX_START_X (16 * (26-1))
#define WORD_AUTO_TEX_START_Y (32 * (31 - (14-1)))

//row 1
	//stop
#define WORD_STOP_TEX_SIZE_X (16 * 6)
#define WORD_STOP_TEX_SIZE_Y (16 * 2)
#define WORD_STOP_TEX_START_X 0
#define WORD_STOP_TEX_START_Y (32 * (31 - (13-1)))
	//attacking
#define WORD_ATTACKING_TEX_SIZE_X (16 * 12)
#define WORD_ATTACKING_TEX_SIZE_Y (16 * 2)
#define WORD_ATTACKING_TEX_START_X (16 * (7-1))
#define WORD_ATTACKING_TEX_START_Y (32 * (31 - (13-1)))
	//range
#define WORD_RANGE_TEX_SIZE_X (16 * 7)
#define WORD_RANGE_TEX_SIZE_Y (16 * 2)
#define WORD_RANGE_TEX_START_X (16 * (19-1))
#define WORD_RANGE_TEX_START_Y (32 * (31 - (13-1)))
	//time
#define WORD_TIME_TEX_SIZE_X (16 * 6)
#define WORD_TIME_TEX_SIZE_Y (16 * 2)
#define WORD_TIME_TEX_START_X (16 * (26-1))
#define WORD_TIME_TEX_START_Y (32 * (31 - (13-1)))  
//other
	//arrow_white
#define SIGN_ARROW_WHITE_TEX_SIZE_X (16 * 2)
#define SIGN_ARROW_WHITE_TEX_SIZE_Y (16 * 2)
#define SIGN_ARROW_WHITE_TEX_START_X (16 * (27-1))
#define SIGN_ARROW_WHITE_TEX_START_Y (32 * (31 - (23-1)))
	//arrow_outline
#define SIGN_ARROW_OUTLINE_TEX_SIZE_X (16 * 2)
#define SIGN_ARROW_OUTLINE_TEX_SIZE_Y (16 * 2)
#define SIGN_ARROW_OUTLINE_TEX_START_X (16 * (29-1))
#define SIGN_ARROW_OUTLINE_TEX_START_Y (32 * (31 - (23-1)))
	//greater than
#define SIGN_GREATER_THAN_TEX_SIZE_X (16 * 2)
#define SIGN_GREATER_THAN_TEX_SIZE_Y (16 * 2)
#define SIGN_GREATER_THAN_TEX_START_X (16 * (27-1))
#define SIGN_GREATER_THAN_TEX_START_Y (32 * (31 - (22-1)))
	//less than
#define SIGN_LESS_THAN_TEX_SIZE_X (16 * 2)
#define SIGN_LESS_THAN_TEX_SIZE_Y (16 * 2)
#define SIGN_LESS_THAN_TEX_START_X (16 * (29-1))
#define SIGN_LESS_THAN_TEX_START_Y (32 * (31 - (22-1)))
	//slash_white
#define SIGN_SLASH_WHITE_TEX_SIZE_X (16 * 2)
#define SIGN_SLASH_WHITE_TEX_SIZE_Y (16 * 2)
#define SIGN_SLASH_WHITE_TEX_START_X (16 * (27-1))
#define SIGN_SLASH_WHITE_TEX_START_Y (32 * (31 - (21-1)))
	//slash_outline
#define SIGN_SLASH_OUTLINE_TEX_SIZE_X (16 * 2)
#define SIGN_SLASH_OUTLINE_TEX_SIZE_Y (16 * 2)
#define SIGN_SLASH_OUTLINE_TEX_START_X (16 * (29-1))
#define SIGN_SLASH_OUTLINE_TEX_START_Y (32 * (31 - (21-1)))

	//colon_outline
#define SIGN_COLON_OUTLINE_TEX_SIZE_X 32
#define SIGN_COLON_OUTLINE_TEX_SIZE_Y 32
#define SIGN_COLON_OUTLINE_TEX_START_X (16 * (15 - 1))
#define SIGN_COLON_OUTLINE_TEX_START_Y (32 * (31 - (20-1)))

#pragma endregion word_tex_param

#pragma endregion ui

#pragma region gamelogic

#define LIGHT_COLOR_LERP_CHANGE_SPPED 10.0f
#define CHECK_LIGHT_ENTER_TIMES 10

#define EXTRA_CARD_MULTIPLE_FACTOR 3
#define EXTRA_CARD_GENRE_COUNT 21
#pragma region status

//be hited param
#define BE_HITED_COLOR_CHANGE_TIME 0.18f

//player status
#define STATUS_COUNT 4

#define STATUS_LIMIT_HP 9999
#define STATUS_LIMIT_ATK 999
#define STATUS_LIMIT_DEF 999
#define STATUS_LIMIT_EXP 9999

#define PLAYER_STATUS_LIMIT_CONSUME_HP 100
#define PLAYER_STATUS_LIMIT_CONSUME_ATK	100
#define PLAYER_STATUS_LIMIT_CONSUME_DEF	100
#define PLAYER_STATUS_LIMIT_CONSUME_EXP	10000

#define PLAYER_DEFAULT_STATUS_MAX_HP 100
#define PLAYER_DEFAULT_STATUS_MAX_ATK 10
#define PLAYER_DEFAULT_STATUS_MAX_DEF 10
#define PLAYER_DEFAULT_STATUS_MAX_EXP 20
#define PLAYER_DEFAULT_STATUS_CONSUME_HP 40
#define PLAYER_DEFAULT_STATUS_CONSUME_ATK 4
#define PLAYER_DEFAULT_STATUS_CONSUME_DEF 2
#define PLAYER_DEFAULT_STATUS_CONSUME_EXP 2

#define PLAYER_DEFAULT_LV 0
#define PLAYER_LIMIT_LV 99

#define LV_UP_INCREASE_HP 10
#define LV_UP_INCREASE_ATK 2
#define LV_UP_INCREASE_DEF 4
#define LV_HP_INCREASE_EXP 1

//enemy status
#define ENEMY_STATUS_BASE_DMG 50
#define ENEMY_STATUS_MIN_DMG 20

#define ENEMY_1_STATUS_HP 20
#define ENEMY_1_STATUS_ATK 20

#define ENEMY_2_STATUS_HP 100
#define ENEMY_2_STATUS_ATK 60

#define ENEMY_3_STATUS_HP 200
#define ENEMY_3_STATUS_ATK 1

#pragma region card_param

//card(default increasement range
#define DEFAULT_STATUS_INCREASE_RANGE_MIN 1
#define DEFAULT_STATUS_INCREASE_RANGE_MAX 10

#define DEFAULT_STATUS_CONSUME_DOWN_MIN 1
#define DEFAULT_STATUS_CONSUME_DOWN_MAX 2

#define DEFAULT_STATUS_EXP_GAIN_MIN 0.2f
#define DEFAULT_STATUS_EXP_GAIN_MAX 0.5f
#define EXP_GAIN_MIN 0.5f
#define EXP_GAIN_MAX 3.0f
#define DEFAULT_EXP_VALUE 10
#define DEFAULT_EXP_2_VALUE 30

//card(extra card param
#define MAX_ATK_DOWN_HP_UP_ATK -10//atk down - hp up
#define MAX_ATK_DOWN_HP_UP_HP 30 
#define MAX_DEF_DOWN_HP_UP_DEF -10//def down -hp up
#define MAX_DEF_DOWN_HP_UP_HP 30

#define MAX_HP_DOWN_ATK_UP_HP -30// hp down - atk up
#define MAX_HP_DOWN_ATK_UP_ATK 5
#define MAX_DEF_DOWN_ATK_UP_DEF -10//def down - atk up
#define MAX_DEF_DOWN_ATK_UP_ATK 10

#define MAX_HP_DOWN_DEF_UP_HP -30// hp down - def up
#define MAX_HP_DOWN_DEF_UP_DEF 10
#define MAX_ATK_DOWN_DEF_UP_ATK -5//atk down - def up
#define MAX_ATK_DOWN_DEF_UP_DEF 10

#define CONSUME_UP_MAX_HP_UP_CONSUME 20 //consume max hp up
#define CONSUME_UP_MAX_HP_UP_HP 50
#define CONSUME_UP_MAX_ATK_UP_CONSUME 3//consume max atk up
#define CONSUME_UP_MAX_ATK_UP_ATK 10
#define CONSUME_UP_MAX_DEF_UP_CONSUME 1//consume max def up
#define CONSUME_UP_MAX_DEF_UP_DEF 20

#define CONSUME_DOWN_MAX_HP_DOWN_CONSUME -20 //consume max hp up
#define CONSUME_DOWN_MAX_HP_DOWN_HP -50
#define CONSUME_DOWN_MAX_ATK_DOWN_CONSUME -3//consume max atk up
#define CONSUME_DOWN_MAX_ATK_DOWN_ATK -10
#define CONSUME_DOWN_MAX_DEF_DOWN_CONSUME -1//consume max def up
#define CONSUME_DOWN_MAX_DEF_DOWN_DEF -20

#define DAMAGE_UP_CONSUME_UP_HP_DAMAGE 3 //consume dmg hp up
#define DAMAGE_UP_CONSUME_UP_HP_CONSUME 50
#define DAMAGE_UP_CONSUME_UP_ATK_DAMAGE 5//consume dmg atk up
#define DAMAGE_UP_CONSUME_UP_ATK_CONSUME 3
#define DAMAGE_UP_CONSUME_UP_DEF_DAMAGE 2//consume dmg def up
#define DAMAGE_UP_CONSUME_UP_DEF_CONSUME 1
#define DAMAGE_UP_CONSUME_UP_EXP_DAMAGE 1//consume dmg exp up
#define DAMAGE_UP_CONSUME_UP_EXP_CONSUME 1

#define DAMAGE_DOWN_CONSUME_DOWN_HP_DAMAGE -2 //consume dmg hp down
#define DAMAGE_DOWN_CONSUME_DOWN_HP_CONSUME -20
#define DAMAGE_DOWN_CONSUME_DOWN_ATK_DAMAGE -2//consume dmg atk down
#define DAMAGE_DOWN_CONSUME_DOWN_ATK_CONSUME -2
#define DAMAGE_DOWN_CONSUME_DOWN_DEF_DAMAGE -2//consume dmg def down
#define DAMAGE_DOWN_CONSUME_DOWN_DEF_CONSUME -1
#define DAMAGE_DOWN_CONSUME_DOWN_EXP_DAMAGE -1//consume dmg exp down
#define DAMAGE_DOWN_CONSUME_DOWN_EXP_CONSUME -1

#pragma endregion card_param

#pragma endregion status

//enemy

//ai(movement)

#define ENEMY_SKIRMISHER_IDLE_TIME 1.0f
#define ENEMY_SKIRMISHER_MOVE_SPEED 50.0f
#define ENEMY_SKIRMISHER_DIST_TO_PLAYER 10.0f

#define ENEMY_DRAGONFLY_IDLE_TIME 3.0f
#define ENEMY_DRAGONFLY_MOVE_SPEED 100.0f
#define ENEMY_DRAGONFLY_DIST_TO_PLAYER 600.0f
#define ENEMY_DRAGONFLY_ATTACK_RANGE 800.0f
#define ENEMY_DRAGONFLY_ATTACK_DELAY_TIME 1.0f
#define ENEMY_DRAGONFLY_ATTACK_TIME 3.0f


//other 
#define CAMERA_SHAKE_REDUCE_FACTOR 10.0f

#define CAMERA_SHAKE_EXPLOSION_FORCE 25.0f
#define CAMERA_SHAKE_EXPLOSION_TIME 0.6f
#define CAMERA_SHAKE_PLAYER_SHOOT_FORCE 11.0f
#define CAMERA_SHAKE_PLAYER_SHOOT_TIME 0.1f
#define CAMERA_SHAKE_PLAYER_HURT_FORCE 25.0f
#define CAMERA_SHAKE_PLAYER_HURT_TIME 0.2f

#pragma endregion gamelogic

#pragma region enum_definition

enum class E_DrawLayer {
	DrawLayer_Bottom = 0,
	DrawLayer_Scene_3 = 1,
	DrawLayer_Scene_2 = 2,
	DrawLayer_Scene_1 = 3,
	DrawLayer_UI = 4,
	DrawLayer_Top = 5,
};


enum class E_SceneLayer {
	SceneLayer_None,
	SceneLayer_1,
	SceneLayer_2,
	SceneLayer_3,
};

enum class E_Dir {
	Dir_None = 0,
	Dir_Up = 1,
	Dir_Left = 2,
	Dir_Down = -1,
	Dir_Right = -2,
};

#pragma endregion enum_definition

const vector2s upVector = vector2s(0.0f, 1.0f);
const vector2s downVector = vector2s(0.0f, -1.0f);
const vector2s leftVector = vector2s(-1.0f, 0.0f);
const vector2s rightVector = vector2s(1.0f, 0.0f);

const vector3s screen_size = vector3s(SCREEN_TEXEL_WIDTH, SCREEN_TEXEL_HEIGHT, 0.0f);
const float aspect = (float)SCREEN_HEIGHT / SCREEN_WIDTH;
const vector3s aspect_factor = vector3s(1.0f, 1.0f, 1.0f);

const vector2s player_tex_size = vector2s((float)1.0f / PLAYER_TEX_W_COUNT, (float)1.0f / PLAYER_TEX_H_COUNT);
const vector2s character_cell_tex_size = vector2s((float)PLAYER_TEX_SIZE_X / CHARACTER_TEX_WIDTH, (float)PLAYER_TEX_SIZE_Y / CHARACTER_TEX_HEIGHT);

//const vector2s player_size = vector2s(1080.0f, 1080.0f);
//bullet
const vector2s bullet_tex_size = vector2s((float)BULLET_NEW_TEX_CELL_SIZE_X / BULLET_NEW_TEX_SIZE_X, (float)BULLET_NEW_TEX_CELL_SIZE_Y / BULLET_NEW_TEX_SIZE_Y);
const vector2s bullet_laser_tex_size = vector2s(1.0f, 1.0f);
const vector2s bullet_cannon_tex_size = vector2s((float)1.0f / BULLET_CANNON_TEX_W_COUNT, (float)1.0f / BULLET_CANNON_TEX_H_COUNT);

//effect
const vector2s effect_tex_size = vector2s((float)1.0f / EFFECT_TEX_SIZE_X, (float)1.0f / EFFECT_TEX_SIZE_Y);
const vector2s effect_hit_tex_cell_size = vector2s((float)EFFECT_HIT_TEX_SIZE_X / EFFECT_HIT_TEX_W_COUNT, (float)EFFECT_HIT_TEX_SIZE_Y / EFFECT_HIT_TEX_H_COUNT);
const vector2s effect_hit_tex_size = vector2s((float)EFFECT_HIT_CELL_TEX_SIZE_X / EFFECT_TEX_SIZE_X, (float)EFFECT_HIT_CELL_TEX_SIZE_Y / EFFECT_TEX_SIZE_Y);
const vector2s effect_explosion_tex_cell_size = vector2s((float)EFFECT_EXPLOSION_CELL_TEX_SIZE_X / EFFECT_TEX_SIZE_X, (float)EFFECT_EXPLOSION_CELL_TEX_SIZE_Y / EFFECT_TEX_SIZE_Y);

//ui
const vector2s ui_tex_size = vector2s((float)1.0f / UI_TEX_SIZE_X, (float)1.0f / UI_TEX_SIZE_Y);
const vector2s num_tex_size = vector2s((float)1.0f / NUM_TEX_W_COUNT, (float)1.0f / NUM_TEX_H_COUNT);
const vector2s lv_up_img_tex_size = vector2s((float)LV_UP_IMG_TEX_SIZE_X / UI_TEX_SIZE_X, (float)LV_UP_IMG_TEX_SIZE_Y / UI_TEX_SIZE_Y);
const vector3s word_scale = vector3s(0.6f, 0.6f, 1.0f);
const vector3s status_img_word_scale = vector3s(0.9f, 0.9f, 1.0f);


//other
//background
const vector2s grass_tex_size = vector2s((float)1.0f / GRASS_TEX_CELL_COUNT_W, (float)1.0f / GRASS_TEX_CELL_COUNT_H);

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************

const vector2s defaultRaySize = vector2s(100.0f, 20.0f);


vector2s GetMouseMovement();
vector2s GetMouseMovement(bool _resetSign);
HWND& GetHWnd();

int GetNumWordTexIndex(int _wordIndex);
int GetNumWordTexIndexWithAscCode(int _wordIndex);

void SetColliderColor(D3DXCOLOR _colliderColor);

#endif
