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
	//�Q�[���֘A������
	InitSprite();//�`��֐�������
	srand(timeGetTime());	//����������

	GameMgr::GetInstance()->DoInit();//�Q�[��������
}

void UninitGame() {
	UninitSprite();
	UninitRenderer();
	GameMgr::GetInstance()->DoUninit();//�Q�[���I������
}

void UpdateGame() {
	GameMgr::GetInstance()->DoUpdate(0.016f);//�Q�[���X�V
}

void DrawGame() {
	SetWorldViewProjection2D();
	GameMgr::GetInstance()->sceneObjMgr->DoDraw();//�Q�[���`��
}

