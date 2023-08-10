#include "gameLogic.h"
#include "sceneObjManager.h"
#include "battleManager.h"
#include "cellSpawnManager.h"
#include "inputManager.h"
#include "uiManager.h"
#include "gameStates.h"
#include "manager.h"
#include "player.h"

#pragma region game_state

GameState::GameState() :gameStateType(E_GameState::GameState_None)
{

}

GameState::GameState(E_GameState state) : gameStateType(state) {

}

GameState::~GameState() {

}

void GameState::DoInit()
{
}

void GameState::Uninit()
{
}

void GameState::DoUpdate(float deltatime) {

}


#pragma region game_state_title

GameState_Title::GameState_Title() :GameState(E_GameState::GameState_Game_Title)
{

}

GameState_Title::~GameState_Title() {

}

void GameState_Title::DoUpdate(float deltatime)
{

}

void GameState_Title::DoInit()
{

	GameMgr::GetInstance()->uiMgr->ShowTitle(true);
	GameMgr::GetInstance()->inputMgr->SwitchInputMode(E_InputMode::InputMode_Title);
	GameMgr::GetInstance()->inputMgr->SetInputState(true);

	GameMgr::GetInstance()->uiMgr->ShowSelector(E_GameState::GameState_Game_Title, true);

	GameMgr::GetInstance()->sceneObjMgr->InitPlayerTitleState();

	GameMgr::GetInstance()->uiMgr->ShowGuide(true);

}

void GameState_Title::Uninit()
{
	GameMgr::GetInstance()->uiMgr->ShowTitle(false);
	GameMgr::GetInstance()->uiMgr->ShowSelector(E_GameState::GameState_Game_Title, false);

}

#pragma endregion game_state_title

#pragma region game_state_game

GameState_Game::GameState_Game() {

}

GameState_Game::~GameState_Game() {

}

void GameState_Game::DoUpdate(float deltatime) {
	GameMgr::GetInstance()->battleMgr->DoUpdate(deltatime);
	GameMgr::GetInstance()->uiMgr->UpdateGameTimer();
	GameMgr::GetInstance()->cellSpawnerMgr->DoUpdate(deltatime * GameMgr::GetInstance()->timer->gameTimer_1);

	if (GameMgr::GetInstance()->timer->CheckCountDownDone() == true) {
		//win
		GameMgr::GetInstance()->SwitchSubGameState(E_GameState::GameState_Game_End, true);
	}

	vector3s mouseMovement = vector3s();
	POINT mouseM = POINT();
	mouseM.x = 0;
	mouseM.y = 0;
	GetCursorPos(&mouseM);
	ScreenToClient(GetHWnd(), &mouseM);
	mouseMovement.x = mouseM.x;
	mouseMovement.y = mouseM.y;

	mouseMovement.x -= SCREEN_WIDTH * 0.5f;
	mouseMovement.y = -(mouseMovement.y - SCREEN_HEIGHT * 0.5f);
	mouseMovement.z = 30.0f;
	GameMgr::GetInstance()->uiMgr->SetCrosshairPos(mouseMovement);
}

void GameState_Game::DoInit() {
	//load game scene
	//active spawner
	//switch input state

	GameMgr::GetInstance()->sceneMgr->SwitchScene(E_Scene::Scene_Stage3, true);//game scene

	GameMgr::GetInstance()->inputMgr->SwitchInputMode(E_InputMode::InputMode_Game);//game input
	GameMgr::GetInstance()->inputMgr->SetInputState(true);//active input

	GameMgr::GetInstance()->cellSpawnerMgr->DoInit();//reset spawn data
	GameMgr::GetInstance()->cellSpawnerMgr->StartSpawn();//start spawn enemy

	GameMgr::GetInstance()->battleMgr->DoInit();//card data init

	GameMgr::GetInstance()->uiMgr->ShowHUD(true);//hud show
	GameMgr::GetInstance()->uiMgr->ShowGameTimer(true);//game count down show
	GameMgr::GetInstance()->uiMgr->ShowCrosshair(true);

	GameMgr::GetInstance()->timer->ResetCountDownParam();
	GameMgr::GetInstance()->timer->SetGameCountDownState(true);

	GameMgr::GetInstance()->battleMgr->ResetGotData();
	GameMgr::GetInstance()->uiMgr->ShowGuide(true);
}

void GameState_Game::Uninit() {
	//unload all unit(unload scene
	//inactive spawner
	GameMgr::GetInstance()->sceneMgr->SwitchScene(E_Scene::Scene_None, true);//unload game scece
	GameMgr::GetInstance()->inputMgr->SwitchInputMode(E_InputMode::InputMode_None);//siwtch input mode
	GameMgr::GetInstance()->cellSpawnerMgr->StopSpawn();//stop spawn enemy

	GameMgr::GetInstance()->uiMgr->ShowHUD(false);//hide hud
	GameMgr::GetInstance()->uiMgr->ShowGameTimer(false);
	GameMgr::GetInstance()->uiMgr->ShowCrosshair(false);

	GameMgr::GetInstance()->timer->SetGameCountDownState(false);

	//scene obj cell
	//
}

#pragma endregion game_state_game

#pragma region game_state_lv_up

GameState_Lv_Up::GameState_Lv_Up() {

}

GameState_Lv_Up::~GameState_Lv_Up() {

}

void GameState_Lv_Up::DoUpdate(float deltatime) {
	//GameMgr::GetInstance()->uiMgr->CardCollision(GetMousePos());
}

void GameState_Lv_Up::DoInit() {
	//load level up panel
	//switch input state
	//set timer
	//glfwSetInputMode(GetWindow(), GLFW_CURSOR, GLFW_CURSOR_NORMAL);
	GameMgr::GetInstance()->uiMgr->ShowHUD(false);
	//panel
		//update card data

	Player* player = GameMgr::GetInstance()->sceneObjMgr->GetPlayer();
	if (player != nullptr) {
		int curLv = player->GetStatusValue(E_Status::Status_Level);
		GameMgr::GetInstance()->uiMgr->ShowLvUpPanel(true, curLv);
	}
	//show panel
	GameMgr::GetInstance()->uiMgr->ShowSelector(E_GameState::GameState_Game_Lv_Up, true);
	GameMgr::GetInstance()->inputMgr->SwitchInputMode(E_InputMode::InputMode_Game_Lv_Up);
	//timer
	GameMgr::GetInstance()->timer->SetTimer(E_Timer::Timer_Game_1, 0.0f);

	GameMgr::GetInstance()->uiMgr->ShowGuide(true);
}

void GameState_Lv_Up::Uninit() {
	//unload level up panel
	//reset timer
	GameMgr::GetInstance()->uiMgr->ShowLvUpPanel(false, 0);
	GameMgr::GetInstance()->uiMgr->ShowSelector(E_GameState::GameState_Game_Lv_Up, false);
	GameMgr::GetInstance()->inputMgr->SwitchInputMode(E_InputMode::InputMode_Game);
	GameMgr::GetInstance()->timer->SetTimer(E_Timer::Timer_Game_1, 1.0f);

	GameMgr::GetInstance()->uiMgr->ShowHUD(true);

	GameMgr::GetInstance()->battleMgr->ResetCardData();
}

#pragma endregion game_state_lv_up

#pragma region game_state_menu


GameState_Menu::GameState_Menu() {

}

GameState_Menu::~GameState_Menu() {

}

void GameState_Menu::DoUpdate(float deltatime) {

}

void GameState_Menu::DoInit() {
	//load menu
	//set timer
	//set input state

	GameMgr::GetInstance()->uiMgr->ShowMenu(E_GameState::GameState_Game_Menu, true);
	GameMgr::GetInstance()->uiMgr->ShowSelector(E_GameState::GameState_Game_Menu, true);
	GameMgr::GetInstance()->inputMgr->SwitchInputMode(E_InputMode::InputMode_Game_Menu);
	GameMgr::GetInstance()->inputMgr->SetInputState(true);

	GameMgr::GetInstance()->timer->SetTimer(E_Timer::Timer_Game_1, 0.0f);

	GameMgr::GetInstance()->uiMgr->ShowGuide(true);
}

void GameState_Menu::Uninit() {
	//unload menu
	//reset timer
	//reset input state
	GameMgr::GetInstance()->uiMgr->ShowMenu(E_GameState::GameState_Game_Menu, false);
	GameMgr::GetInstance()->uiMgr->ShowSelector(E_GameState::GameState_Game_Menu, false);

	//return to game input
	GameMgr::GetInstance()->inputMgr->SwitchInputMode(E_InputMode::InputMode_Game);

	GameMgr::GetInstance()->timer->SetTimer(E_Timer::Timer_Game_1, 1.0f);
}

#pragma endregion game_state_menu

#pragma region game_state_lose

GameState_Lose::GameState_Lose() {

}

GameState_Lose::~GameState_Lose() {

}

void GameState_Lose::DoUpdate(float deltatime) {

}

void GameState_Lose::DoInit() {
	//set timer
	//load lose panel
	//set input state
	GameMgr::GetInstance()->uiMgr->ShowMenu(E_GameState::GameState_Game_Lose, true);
	GameMgr::GetInstance()->uiMgr->ShowSelector(E_GameState::GameState_Game_Lose, true);
	GameMgr::GetInstance()->inputMgr->SwitchInputMode(E_InputMode::InputMode_Game_Lose_Menu);
	GameMgr::GetInstance()->inputMgr->SetInputState(true);

	GameMgr::GetInstance()->timer->SetTimer(E_Timer::Timer_Game_1, 0.0f);

	GameMgr::GetInstance()->uiMgr->ShowGuide(true);
}

void GameState_Lose::Uninit() {
	GameMgr::GetInstance()->uiMgr->ShowMenu(E_GameState::GameState_Game_Lose, false);
	GameMgr::GetInstance()->uiMgr->ShowSelector(E_GameState::GameState_Game_Lose, false);

	GameMgr::GetInstance()->timer->SetTimer(E_Timer::Timer_Game_1, 1.0f);
}

#pragma endregion game_state_lose

#pragma region game_state_end

GameState_End::GameState_End() {

}

GameState_End::~GameState_End() {

}

void GameState_End::DoUpdate(float deltatime) {

}

void GameState_End::DoInit() {
	GameMgr::GetInstance()->uiMgr->ShowMenu(E_GameState::GameState_Game_End, true);
	GameMgr::GetInstance()->uiMgr->ShowSelector(E_GameState::GameState_Game_End, true);
	GameMgr::GetInstance()->inputMgr->SwitchInputMode(E_InputMode::InputMode_Game_Win_Menu);
	GameMgr::GetInstance()->inputMgr->SetInputState(true);

	GameMgr::GetInstance()->timer->SetTimer(E_Timer::Timer_Game_1, 0.0f);

	GameMgr::GetInstance()->uiMgr->ShowGuide(true);
}

void GameState_End::Uninit() {
	GameMgr::GetInstance()->uiMgr->ShowMenu(E_GameState::GameState_Game_Lose, false);
	GameMgr::GetInstance()->uiMgr->ShowSelector(E_GameState::GameState_Game_Lose, false);

	GameMgr::GetInstance()->timer->SetTimer(E_Timer::Timer_Game_1, 1.0f);
}

#pragma endregion game_state_end

#pragma endregion game_state