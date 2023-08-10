#pragma once
#include "game.h"
#include "main.h"
#include "renderer.h"
#include "sprite.h"

#include "texture.h"

#include "sceneObjManager.h"

GameMgr* GameMgr::instance = nullptr;
GameMgr::Deletor deletor;

void InitGame() {
	//ゲーム関連初期化
	InitSprite();//描画関数初期化
	srand(timeGetTime());	//乱数初期化

	GameMgr::GetInstance()->DoInit();//ゲーム初期化
}

void UninitGame() {
	UninitSprite();
	UninitRenderer();
	GameMgr::GetInstance()->DoUninit();//ゲーム終了処理
}

void UpdateGame() {
	GameMgr::GetInstance()->DoUpdate(0.016f);//ゲーム更新
}

void DrawGame() {
	SetWorldViewProjection2D();
	GameMgr::GetInstance()->sceneObjMgr->DoDraw();//ゲーム描画
}

