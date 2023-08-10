//===========================================================================
// [���̓}�l�[�W���[]
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
	void SwitchInputMode(E_InputMode targetInputMode);//���̓��[�h��؂�ւ��i�^�C�g����Q�[�����Ȃ�

	void SetInputState(bool state);
	E_InputMode GetCurInputMode();
};



#endif