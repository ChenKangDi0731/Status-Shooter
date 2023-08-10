//===========================================================================
// [入力マネージャー]
//===========================================================================
#ifndef _INPUT_MANAGER_H_
#define _INPUT_MANAGER_H_

enum class E_InputMode;

class InputMgr {
private:
	bool inputState;
	E_InputMode curInputMode;

public:

	InputMgr();
	~InputMgr();

	void DoUpdate(float deltatime);
	void SwitchInputMode(E_InputMode targetInputMode);//入力モードを切り替え（タイトルやゲーム内など

	void SetInputState(bool state);
	E_InputMode GetCurInputMode();
};



#endif