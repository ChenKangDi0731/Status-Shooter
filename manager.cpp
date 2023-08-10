#include "manager.h"
#include "sceneManager.h"
#include "inputManager.h"
#include "sceneObjManager.h"
#include "effectManager.h"
#include "cellManager.h"
#include "cellSpawnManager.h"
#include "battleManager.h"
#include "dataManager.h"
#include "gameStates.h"
#include "gameObject.h"
#include "uiManager.h"
#include "player.h"
#include "enemy.h"
#include "scene.h"
#include "UI.h"
#include <sstream>

#pragma region game_manager

GameMgr::GameMgr() :curGameStateType(E_GameState::GameState_None), defaultGameStateType(E_GameState::GameState_None),
defaultSubGameStateType(E_GameState::GameState_None), curSubGameStateType(E_GameState::GameState_None),
curGameState(nullptr), curSubGameState(nullptr) {
	//マネージャーをインスタンス化
	dataMgr = new DataManager();

	sceneMgr = new SceneMgr();
	textureMgr = new TextureMgr();
	shaderMgr = new ShaderMgr();
	inputMgr = new InputMgr();
	sceneObjMgr = new SceneObjMgr();
	effectMgr = new EffectMgr();
	cellMgr = new CellMgr();
	cellSpawnerMgr = new CellSpawnerMgr();
	uiMgr = new UIMgr();
	battleMgr = new BattleMgr();
	timer = new Timer();

	gameStateDic.clear();
	subGameStateDic.clear();
}

GameMgr::~GameMgr() {
	if (dataMgr != nullptr) {
		delete dataMgr;
	}
	if (sceneMgr != nullptr) {
		delete sceneMgr;
	}
	if (textureMgr != nullptr) {
		delete textureMgr;
	}
	if (shaderMgr != nullptr) {
		delete shaderMgr;
	}
	if (inputMgr != nullptr) {
		delete inputMgr;
	}
	if (sceneObjMgr != nullptr) {
		delete sceneObjMgr;
	}
	if (effectMgr != nullptr) {
		delete effectMgr;
	}
	if (cellMgr != nullptr) {
		delete cellMgr;
	}
	if (cellSpawnerMgr != nullptr) {
		delete cellSpawnerMgr;
	}
	if (uiMgr != nullptr) {
		delete uiMgr;
	}
	if (battleMgr != nullptr) {
		delete battleMgr;
	}
	if (timer != nullptr) {
		delete timer;
	}
	dataMgr = nullptr;
	sceneMgr = nullptr;
	textureMgr = nullptr;
	shaderMgr = nullptr;
	inputMgr = nullptr;
	sceneObjMgr = nullptr;
	effectMgr = nullptr;
	cellMgr = nullptr;
	cellSpawnerMgr = nullptr;
	uiMgr = nullptr;
	battleMgr = nullptr;
	timer = nullptr;

	std::map<E_GameState, GameState*>::iterator iter;
	for (iter = gameStateDic.begin(); iter != gameStateDic.end(); iter++) {
		if (iter->second == nullptr)continue;
		delete iter->second;
	}
	for (iter = subGameStateDic.begin(); iter != subGameStateDic.end(); iter++) {
		if (iter->second == nullptr)continue;
		delete iter->second;
	}

	gameStateDic.clear();
	subGameStateDic.clear();
}

void GameMgr::DoInit() {

	//ゲームステートをリストに追加
	RegisterGameState(E_GameState::GameState_Game_Title, new GameState_Title(), true);//タイトル
	RegisterGameState(E_GameState::GameState_Game_Normal, new GameState_Game(), false);//ゲームちゅう
	//サブステート（メニューやゲームクリア、ゲームオーバーなど
	RegisterSubGameState(E_GameState::GameState_Game_Lv_Up, new GameState_Lv_Up(), false);//レベルアップ
	RegisterSubGameState(E_GameState::GameState_Game_Menu, new GameState_Menu(), false);//メニュー
	RegisterSubGameState(E_GameState::GameState_Game_Lose, new GameState_Lose(), false);//ゲームオーバー
	RegisterSubGameState(E_GameState::GameState_Game_End, new GameState_End(), false);//ゲームクリア

	//マネージャー初期化
	dataMgr->DoInit();

	textureMgr->DoInit();
	shaderMgr->DoInit();
	sceneObjMgr->DoInit();

	uiMgr->DoInit();
	effectMgr->DoInit();

	cellMgr->DoInit();
	sceneMgr->DoInit();

	//デフォルトステートに切り替え
	SwitchGameState(defaultGameStateType, true);//メインゲームステート
	SwitchSubGameState(defaultSubGameStateType, true);//サブゲームステート
}

void GameMgr::DoUninit() {
	sceneMgr->UnInit();
	effectMgr->UnInit();
	sceneObjMgr->UnInit(true);
}

void GameMgr::DoUpdate(float deltatime) {
	inputMgr->DoUpdate(deltatime);
	effectMgr->DoUpdate(deltatime * timer->gameTimer_1);
	sceneObjMgr->DoUpdate(deltatime);
	sceneObjMgr->DoLateUpdate(deltatime * timer->uiTimer);
	sceneMgr->DoUpdate(deltatime * timer->gameTimer_1);
	timer->DoUpdate(deltatime);

	if (curGameState != nullptr) {
		curGameState->DoUpdate(deltatime);
	}
	if (curSubGameState != nullptr) {
		curSubGameState->DoUpdate(deltatime);
	}
}

void GameMgr::DoLateUpdate(float deltatime) {

}

void GameMgr::SwitchGameState(E_GameState gameState, bool reset)
{
	if (gameState == E_GameState::GameState_None) {
		if (curGameState != nullptr) {
			curGameState->Uninit();
			curGameState = nullptr;
		}
		curGameStateType = E_GameState::GameState_None;
		return;
	}

	if (gameState == curGameStateType) {
		if (reset == false) {
			return;
		}
		else {
			if (curGameState != nullptr) {
				curGameState->Uninit();
				curGameState->DoInit();
			}
			return;
		}
	}

	if (curGameState != nullptr) {
		curGameState->Uninit();
		curGameState = nullptr;
	}

	if (gameStateDic.find(gameState) == gameStateDic.end()) {
		return;
	}

	curGameState = gameStateDic[gameState];
	if (curGameState != nullptr) {
		curGameState->DoInit();
		curGameStateType = gameState;
	}
}

void GameMgr::SwitchSubGameState(E_GameState subGameState, bool reset)
{
	if (subGameState == E_GameState::GameState_None) {
		if (curSubGameState != nullptr) {
			curSubGameState->Uninit();
			curSubGameState = nullptr;
		}
		curSubGameStateType = E_GameState::GameState_None;
		return;
	}

	if (subGameState == curSubGameStateType) {
		if (reset == false) {
			return;
		}
		else {
			if (curSubGameState != nullptr) {
				curSubGameState->Uninit();
				curSubGameState->DoInit();
			}
			return;
		}
	}

	if (curSubGameState != nullptr) {
		curSubGameState->Uninit();
		curSubGameState = nullptr;
	}

	if (subGameStateDic.find(subGameState) == subGameStateDic.end()) {
		return;
	}

	curSubGameState = subGameStateDic[subGameState];
	if (curSubGameState != nullptr) {
		curSubGameState->DoInit();
		curSubGameStateType = subGameState;
	}
}

void GameMgr::RegisterGameState(E_GameState gameStateType, GameState* gameState, bool setDefault)
{
	if (gameState == nullptr)return;
	if (gameStateDic.find(gameStateType) == gameStateDic.end()) {
		gameStateDic[gameStateType] = gameState;
		if (setDefault) {
			defaultGameStateType = gameStateType;
		}
	}
	else {
		//error
	}
}

void GameMgr::RegisterSubGameState(E_GameState gameStateType, GameState* gameState, bool setDefault)
{
	if (gameState == nullptr)return;
	if (subGameStateDic.find(gameStateType) == subGameStateDic.end()) {
		subGameStateDic[gameStateType] = gameState;
		if (setDefault) {
			defaultSubGameStateType = gameStateType;
		}
	}
	else {
		//error
	}
}

E_GameState GameMgr::GetCurGameState()
{
	return curGameStateType;
}

E_GameState GameMgr::GetCurSubGameState() {
	return curSubGameStateType;
}

#pragma endregion game_manager

#pragma region render_manager

RenderMgr::RenderMgr() {

}

RenderMgr::~RenderMgr() {

}

DrawData* RenderMgr::GetNewDrawData(DrawData* copySrc, bool useBatching, E_BatchingType batchingType, bool shakeWithCamera) {
	int drawDataIndex = -1;
	useBatching = false;
	if (useBatching) {
		drawDataIndex = GetEmptyCellVertexDataIndex(batchingType);
	}
	else {
		drawDataIndex = GetEmptyCellVertexDataIndex(E_BatchingType::BatchingType_None);
	}

	if (drawDataIndex == -1)return (new DrawData());

	DrawData* temp = nullptr;
	if (copySrc == nullptr) {
		temp = new DrawData();
	}
	else {
		temp = new DrawData(*copySrc);
	}
	temp->useBatching = useBatching;
	temp->batchingType = batchingType;
	temp->SetDrawVertexIndex(drawDataIndex);
	if (useBatching) {
		UpdateCellVertexArrayData(batchingType, drawDataIndex, true);
	}
	else {
		UpdateCellVertexArrayData(E_BatchingType::BatchingType_None, drawDataIndex, true);
	}

	temp->shakeWithCamera = shakeWithCamera;

	return temp;
}

void RenderMgr::ReleaseDrawDataVertex(int vertexDataIndex, E_BatchingType batchingType)
{
	UpdateCellVertexArrayData(batchingType, vertexDataIndex, false);
}


#pragma endregion render_manager

#pragma region shader_manager

ShaderMgr::ShaderMgr() {
	shaderDic.clear();
}

ShaderMgr::~ShaderMgr() {
	std::map<E_Shader, Shader*>::iterator iter;
	for (iter = shaderDic.begin(); iter != shaderDic.end(); iter++) {
		if (iter->second != nullptr) {
			delete iter->second;
		}
	}

	shaderDic.clear();
}

void ShaderMgr::DoInit()
{
	RegisterShader(E_Shader::Shader_Normal, new Shader("path/Shader/vertex_normal.vs", "path/Shader/fragment_normal.frag"));
	RegisterShader(E_Shader::Shader_Normal_1, new Shader("path/Shader/vertexShader.vs", "path/Shader/fragmentShader.frag"));
}

void ShaderMgr::RegisterShader(E_Shader shaderType, Shader* shader)
{
	if (shader == nullptr)return;

	if (shaderDic.find(shaderType) == shaderDic.end()) {
		shaderDic[shaderType] = shader;
	}
	else {
		//error;
	}

}

int ShaderMgr::GetShaderProgram(E_Shader shaderType)
{
	if (shaderDic.size() == 0)return 0;

	if (shaderDic.find(shaderType) == shaderDic.end() || shaderDic[shaderType] == nullptr)return 0;
	return shaderDic[shaderType]->Program;
}


#pragma endregion shader_manager

#pragma region texture_manager

TextureMgr::TextureMgr() {
	textureDic.clear();
}

TextureMgr::~TextureMgr()
{
	textureDic.clear();
}

void TextureMgr::DoInit()
{
	//テクスチャー読み込み
	//character
	RegisterTexture(E_Texture::Texture_Character_Package,
		textureController.LoadTextureS(E_Texture::Texture_Character_Package, "data\\texture\\character_package.png", E_TextureType::RGBA));

	//bullet
	RegisterTexture(E_Texture::Texture_Bullet_04,
		textureController.LoadTextureS(E_Texture::Texture_Bullet_04, "data\\texture\\spr_bullet_strip04.png", E_TextureType::RGBA));
	RegisterTexture(E_Texture::Texture_Bullet_RedLaser,
		textureController.LoadTextureS(E_Texture::Texture_Bullet_RedLaser, "data\\texture\\red_laser.png", E_TextureType::RGBA));
	RegisterTexture(E_Texture::Texture_Bullet_Package,
		textureController.LoadTextureS(E_Texture::Texture_Bullet_Package, "data\\texture\\bullet.png", E_TextureType::RGBA));

	//effect
	RegisterTexture(E_Texture::Texture_Effect_Package,
		textureController.LoadTextureS(E_Texture::Texture_Effect_Package, "data\\texture\\effect_package_1.png", E_TextureType::RGBA));

	//UI
	RegisterTexture(E_Texture::Texture_UI_HUD_Package_Test_2,
		textureController.LoadTextureS(E_Texture::Texture_UI_HUD_Package_Test_2, "data\\texture\\HUD_Package_test_9.3.png", E_TextureType::RGBA));
	RegisterTexture(E_Texture::Texture_Grass,
		textureController.LoadTextureS(E_Texture::Texture_Grass, "data\\texture\\grass.png", E_TextureType::RGBA));
}

void TextureMgr::RegisterTexture(E_Texture textureType, int textureID)
{
	if (textureDic.find(textureType) == textureDic.end()) {
		textureDic[textureType] = textureID;
	}
	else {
		//error;
	}

}

int TextureMgr::GetTextureID(E_Texture textureType)
{
	if (textureDic.size() == 0)return 0;
	return textureDic[textureType];
}


#pragma endregion texture_manager


