#pragma once
#pragma region game_state

enum class E_GameState;

class GameState {
protected:
	E_GameState gameStateType;
public:
	GameState();
	GameState(E_GameState state);
	virtual ~GameState();

	virtual void DoInit();
	virtual void Uninit();

	virtual void DoUpdate(float deltatime);
};

class GameState_Title :public GameState {
private:

public:
	GameState_Title();
	~GameState_Title();

	void DoUpdate(float deltatime);

	void DoInit();
	void Uninit();
};

class GameState_Game :public GameState {
private:

public:
	GameState_Game();
	~GameState_Game();

	void DoUpdate(float deltatime);

	void DoInit();
	void Uninit();
};

class GameState_Lv_Up :public GameState {
private:

public:
	GameState_Lv_Up();
	~GameState_Lv_Up();

	void DoUpdate(float deltatime);

	void DoInit();
	void Uninit();
};

class GameState_Menu :public GameState {
private:

public:
	GameState_Menu();
	~GameState_Menu();

	void DoUpdate(float deltatime);

	void DoInit();
	void Uninit();
};

class GameState_Lose :public GameState {
private:

public:
	GameState_Lose();
	~GameState_Lose();

	void DoUpdate(float deltatime);

	void DoInit();
	void Uninit();
};

class GameState_End :public GameState {
private:

public:
	GameState_End();
	~GameState_End();

	void DoUpdate(float deltatime);

	void DoInit();
	void Uninit();
};

#pragma endregion game_state