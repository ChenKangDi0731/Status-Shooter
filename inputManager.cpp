#include "inputManager.h"
#include "manager.h"
#include "gameLogic.h"
#include "sceneObjManager.h"
#include "uiManager.h"
#include "player.h"

#pragma region input_manager

InputMgr::InputMgr() :curInputMode(E_InputMode::InputMode_None) {

}

InputMgr::~InputMgr() {

}

void InputMgr::DoUpdate(float deltatime) {
	if (inputState == true) {

		if (curInputMode == E_InputMode::InputMode_Title) {//タイトル

			if (GetKeyboardTrigger(DIK_C) == true) {
				GameMgr::GetInstance()->SwitchGameState(E_GameState::GameState_Game_Normal, true);
			}
			else if (GetKeyboardTrigger(DIK_W) == true) {
				GameMgr::GetInstance()->uiMgr->SwitchNextItem(false);
			}
			else if (GetKeyboardTrigger(DIK_S) == true) {
				GameMgr::GetInstance()->uiMgr->SwitchNextItem(true);
			}
			else if (GetKeyboardTrigger(DIK_G) == true) {
				GameMgr::GetInstance()->uiMgr->SelectCurItem();
				int selectIndex = GameMgr::GetInstance()->uiMgr->GetCurSelectItem();
				if (selectIndex == 0) {//start
					GameMgr::GetInstance()->SwitchGameState(E_GameState::GameState_Game_Normal, true);
				}
				else if (selectIndex == 1) {//exit
					//TODO
				}
			}
		}
		else if (curInputMode == E_InputMode::InputMode_Game) {//ゲーム内

			Player* player = GameMgr::GetInstance()->sceneObjMgr->GetPlayer();
			if (player != nullptr) {
				player->Input(deltatime);
			}

			if (GetKeyboardTrigger(DIK_C) == true) {
				GameMgr::GetInstance()->SwitchGameState(E_GameState::GameState_Game_Title, true);
			}
			else if (GetKeyboardTrigger(DIK_RETURN) == true) {
				GameMgr::GetInstance()->SwitchSubGameState(E_GameState::GameState_Game_Menu, true);
			}

			if (GetKeyboardTrigger(DIK_Q) == true)
			{
				if (player != nullptr) {
					player->SwitchBulletType(false);
				}
			}
			else if (GetKeyboardTrigger(DIK_E) == true) {
				if (player != nullptr) {
					player->SwitchBulletType(true);
				}
			}

			if (GetKeyboardTrigger(DIK_F) == true) {
				if (player != nullptr) {
					Status_Player* playerStatus = (Status_Player*)player->GetComponent(E_Component::Component_Status);
					if (playerStatus != nullptr) {
						playerStatus->ResetData(E_Status::Status_All);
					}
				}
			}

			if (IsMouseLeftTriggered()) {
				if (player != nullptr) {
					player->ShootBullet();
				}
			}

		}
		else if (curInputMode == E_InputMode::InputMode_Game_Lv_Up) {//スキルカード

			if (GetKeyboardTrigger(DIK_A) == true) {
				GameMgr::GetInstance()->uiMgr->SwitchNextItem(false);
			}
			else if (GetKeyboardTrigger(DIK_D) == true) {
				GameMgr::GetInstance()->uiMgr->SwitchNextItem(true);
			}
			else if (GetKeyboardTrigger(DIK_G) == true) {
				GameMgr::GetInstance()->uiMgr->SelectCurItem();
				GameMgr::GetInstance()->SwitchSubGameState(E_GameState::GameState_None, true);
			}
			else if (GetKeyboardTrigger(DIK_R) == true) {
				//GameMgr::GetInstance()->uiMgr->ShowLvUpPanel(true);
			}
		}
		else if (curInputMode == E_InputMode::InputMode_Game_Menu) {//一時停止

			if (GetKeyboardTrigger(DIK_W) == true) {
				GameMgr::GetInstance()->uiMgr->SwitchNextItem(false);
			}
			else if (GetKeyboardTrigger(DIK_S) == true) {
				GameMgr::GetInstance()->uiMgr->SwitchNextItem(true);
			}
			else if (GetKeyboardTrigger(DIK_G) == true) {
				GameMgr::GetInstance()->uiMgr->SelectCurItem();
				int selectIndex = GameMgr::GetInstance()->uiMgr->GetCurSelectItem();
				if (selectIndex == 0) {//continue
					GameMgr::GetInstance()->SwitchSubGameState(E_GameState::GameState_None, true);
				}
				else if (selectIndex == 1) {//retry
					GameMgr::GetInstance()->SwitchSubGameState(E_GameState::GameState_None, true);
					GameMgr::GetInstance()->SwitchGameState(E_GameState::GameState_Game_Normal, true);
				}
				else if (selectIndex == 2) {
					GameMgr::GetInstance()->SwitchSubGameState(E_GameState::GameState_None, true);
					GameMgr::GetInstance()->SwitchGameState(E_GameState::GameState_Game_Title, true);
				}
			}
			else if (GetKeyboardTrigger(DIK_ESCAPE) == true) {
				GameMgr::GetInstance()->SwitchSubGameState(E_GameState::GameState_None, true);
			}
		}
		else if (curInputMode == E_InputMode::InputMode_Game_Lose_Menu) {//ゲームオーバー
			if (GetKeyboardTrigger(DIK_W) == true) {
				GameMgr::GetInstance()->uiMgr->SwitchNextItem(false);
			}
			else if (GetKeyboardTrigger(DIK_S) == true) {
				GameMgr::GetInstance()->uiMgr->SwitchNextItem(true);
			}
			else if (GetKeyboardTrigger(DIK_G) == true) {
				GameMgr::GetInstance()->uiMgr->SelectCurItem();
				int selectIndex = GameMgr::GetInstance()->uiMgr->GetCurSelectItem();
				if (selectIndex == 0) {//retry
					GameMgr::GetInstance()->SwitchSubGameState(E_GameState::GameState_None, true);
					GameMgr::GetInstance()->SwitchGameState(E_GameState::GameState_Game_Normal, true);
				}
				else if (selectIndex == 1) {//title
					GameMgr::GetInstance()->SwitchSubGameState(E_GameState::GameState_None, true);
					GameMgr::GetInstance()->SwitchGameState(E_GameState::GameState_Game_Title, true);
				}

			}
		}
		else if (curInputMode == E_InputMode::InputMode_Game_Win_Menu) {//ゲームクリア
			if (GetKeyboardTrigger(DIK_W) == true) {
				GameMgr::GetInstance()->uiMgr->SwitchNextItem(false);
			}
			else if (GetKeyboardTrigger(DIK_S) == true) {
				GameMgr::GetInstance()->uiMgr->SwitchNextItem(true);
			}
			else if (GetKeyboardTrigger(DIK_G) == true) {
				GameMgr::GetInstance()->uiMgr->SelectCurItem();
				int selectIndex = GameMgr::GetInstance()->uiMgr->GetCurSelectItem();
				if (selectIndex == 0) {//title
					GameMgr::GetInstance()->SwitchSubGameState(E_GameState::GameState_None, true);
					GameMgr::GetInstance()->SwitchGameState(E_GameState::GameState_Game_Title, true);
				}
			}
		}


	}
}

void InputMgr::SwitchInputMode(E_InputMode targetInputMode) {
	curInputMode = targetInputMode;
}

void InputMgr::SetInputState(bool state) {
	inputState = state;
}

E_InputMode InputMgr::GetCurInputMode()
{
	return curInputMode;
}

#pragma endregion input_manager
