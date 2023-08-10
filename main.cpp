/*==============================================================================

   DirectXの初期化 [main.cpp]
                                                         Author : 
                                                         Date   : 
--------------------------------------------------------------------------------

==============================================================================*/
#include "main.h"
//*****************************************************************************
// ライブラリのリンク0.
//*****************************************************************************
#pragma comment (lib, "d3d11.lib")		
#pragma comment (lib, "d3dcompiler.lib")
#pragma comment (lib, "d3dx11.lib")	
#pragma comment (lib, "d3dx9.lib")	
#pragma comment (lib, "winmm.lib")
#pragma comment (lib, "dxerr.lib")
#pragma comment (lib, "dxguid.lib")
#pragma comment (lib, "dinput8.lib")

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define CLASS_NAME			"GameWindow"				// ウインドウのクラス名
#define WINDOW_CAPTION		"ステータスシューター"			// ウインドウのキャプション名

//*****************************************************************************
// 構造体定義
//*****************************************************************************

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

HRESULT Init(HINSTANCE hInstance, HWND hWnd, BOOL bWindow);
void Uninit(void);
void Update(void);
void Draw(void);


//*****************************************************************************
// グローバル変数:
//*****************************************************************************

#ifdef _DEBUG
int		g_CountFPS;							// FPSカウンタ
char	g_DebugStr[2048] = WINDOW_CAPTION;	// デバッグ文字表示用

#endif

vector2s preMousePos = vector2s(0.0f, 0.0f);
vector2s curMousePos = vector2s(0.0f, 0.0f);
vector2s curMouseMovement = vector2s(0.0f, 0.0f);
HWND hwndInstance;
											//=============================================================================
// メイン関数
//=============================================================================
int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);	// 無くても良いけど、警告が出る（未使用宣言）
	UNREFERENCED_PARAMETER(lpCmdLine);		// 無くても良いけど、警告が出る（未使用宣言）

	// 時間計測用
	DWORD dwExecLastTime;
	DWORD dwFPSLastTime;
	DWORD dwCurrentTime;
	DWORD dwFrameCount;
	
	WNDCLASS wc = {};
	wc.lpfnWndProc = WndProc;
	wc.lpszClassName = CLASS_NAME;
	wc.hInstance = hInstance;
	wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)(COLOR_BACKGROUND + 1);

	RegisterClass(&wc);
	
	// ウィンドウの作成
	HWND hWnd = CreateWindow(CLASS_NAME,
						WINDOW_CAPTION,
						WS_OVERLAPPEDWINDOW,
						CW_USEDEFAULT,																		// ウィンドウの左座標
						CW_USEDEFAULT,																		// ウィンドウの上座標
						SCREEN_WIDTH + GetSystemMetrics(SM_CXDLGFRAME)*2,									// ウィンドウ横幅
						SCREEN_HEIGHT + GetSystemMetrics(SM_CXDLGFRAME)*2+GetSystemMetrics(SM_CYCAPTION),	// ウィンドウ縦幅
						nullptr,
						nullptr,
						hInstance,
						nullptr);
	// DirectXの初期化(ウィンドウを作成してから行う)
	if(FAILED(Init(hInstance, hWnd, true)))
	{
		return -1;
	}

	// フレームカウント初期化
	timeBeginPeriod(1);	// 分解能を設定
	dwExecLastTime = dwFPSLastTime = timeGetTime();	// システム時刻をミリ秒単位で取得
	dwCurrentTime = dwFrameCount = 0;
	
	// ウインドウの表示(Init()の後に呼ばないと駄目)
	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	MSG			msg;

	hwndInstance = hWnd;

	// メッセージループ
	while(1)
	{
		if(PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
		{
			if(msg.message == WM_QUIT)
			{// PostQuitMessage()が呼ばれたらループ終了
				break;
			}
			else
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}
		else
		{
			dwCurrentTime = timeGetTime();					// システム時刻を取得

			if ((dwCurrentTime - dwFPSLastTime) >= 1000)	// 1秒ごとに実行
			{
#ifdef _DEBUG
				g_CountFPS = dwFrameCount;
#endif
				dwFPSLastTime = dwCurrentTime;				// FPSを測定した時刻を保存
				dwFrameCount = 0;							// カウントをクリア
			}

			if ((dwCurrentTime - dwExecLastTime) >= (1000 / 60))	// 1/60秒ごとに実行
			{
				dwExecLastTime = dwCurrentTime;	// 処理した時刻を保存

#ifdef _DEBUG	// デバッグ版の時だけFPSを表示する
				wsprintf(g_DebugStr, WINDOW_CAPTION);
				wsprintf(&g_DebugStr[strlen(g_DebugStr)], " FPS:%d", g_CountFPS);
				SetWindowText(hWnd, g_DebugStr);
#endif

				Update();			// 更新処理
				Draw();				// 描画処理

				dwFrameCount++;		// 処理回数のカウントを加算
			}
		}
	}
	
	timeEndPeriod(1);				// 分解能を戻す

	// 終了処理
	Uninit();

	return (int)msg.wParam;
}

//=============================================================================
// プロシージャ
//=============================================================================
LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg) {
	case WM_KEYDOWN:
		if (wParam == VK_ESCAPE) {
			SendMessage(hWnd, WM_CLOSE, 0, 0);
		}
		break;

	case WM_CLOSE:
		if (MessageBox(hWnd, "本当に終了してよろしいですか？", "確認", MB_OKCANCEL | MB_DEFBUTTON2) == IDOK) {
			DestroyWindow(hWnd);
		}
		return 0;

	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	};

	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}
RECT clipRect;

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT Init(HINSTANCE hInstance, HWND hWnd, BOOL bWindow)
{
	// レンダリング処理の初期化
	InitRenderer(hInstance, hWnd, bWindow);
	//入力処理の初期化
	InitInput(hInstance, hWnd);
	InitSound(hWnd);

	InitGame();

	GetWindowRect(hWnd, &clipRect);
	ShowCursor(true);

	POINT mouseM = POINT();
	mouseM.x = (LONG)(SCREEN_WIDTH * 0.5f);
	mouseM.y = (LONG)(SCREEN_HEIGHT * 0.5f);
	ClientToScreen(hWnd,&mouseM);
	SetCursorPos(mouseM.x, mouseM.y);

	preMousePos.x = (FLOAT)GetMouseX();
	preMousePos.y = (FLOAT)GetMouseY();
	curMousePos = preMousePos;

	vector3s temp_1 = vector3s(1.0f, 1.0f, 1.0f);
	vector3s temp_2 = vector3s(0.985f, 0.0005f, 0.0f) * 20.0f;
	temp_1 = temp_1 + temp_2;
	temp_1 = temp_2 + (temp_2 * 20);
	return S_OK;
}


//=============================================================================
// 終了処理
//=============================================================================
void Uninit(void)
{
	//入力処理の終了
	UninitInput();
	UninitSound();
	UninitTexture();
	UninitGame();
}

//=============================================================================
// 更新処理
//=============================================================================
void Update(void)
{
	//入力処理の更新
	UpdateInput();

	preMousePos = curMousePos;
	curMousePos.x = GetMouseX()* MOUSE_SENSITY;
	curMousePos.y = GetMouseY()* MOUSE_SENSITY;
	UpdateGame();

}

//=============================================================================
// 描画処理
//=============================================================================
void Draw(void)
{
	// バックバッファクリア
	Clear();

	// 2D描画なので深度無効
	SetDepthEnable(true);
	DrawGame();
	// バックバッファ、フロントバッファ入れ替え
	Present();
}


vector2s GetMouseMovement() {
	return curMousePos;
}

vector2s GetMouseMovement(bool _resetSign)
{
	return curMousePos;
}

HWND& GetHWnd()
{
	return hwndInstance;
}

int GetNumWordTexIndex(int num) {
	int result = 0;
	switch (num) {
	case 0:
		result = 9;
		break;
	case 1:
		result = 0;
		break;
	case 2:
		result = 1;
		break;
	case 3:
		result = 2;
		break;
	case 4:
		result = 3;
		break;
	case 5:
		result = 4;
		break;
	case 6:
		result = 5;
		break;
	case 7:
		result = 6;
		break;
	case 8:
		result = 7;
		break;
	case 9:
		result = 8;
		break;
	case 10:
		result = 10;
		break;
	case 11:
		result = 11;
		break;
	case 12:
		result = 12;
		break;
	default:
		result = 9;
		break;
	}

	return result;
}

int GetNumWordTexIndexWithAscCode(int code)
{
	int result = 0;
	switch (code) {
	case 48:
		result = 0;
		break;
	case 49:
		result = 1;
		break;
	case 50:
		result = 2;
		break;
	case 51:
		result = 3;
		break;
	case 52:
		result = 4;
		break;
	case 53:
		result = 5;
		break;
	case 54:
		result = 6;
		break;
	case 55:
		result = 7;
		break;
	case 56:
		result = 8;
		break;
	case 57:
		result = 9;
		break;
	case 45://-
		result = 10;
		break;
	case 43://+
		result = 11;
		break;
	case 37://%
		result = 12;
		break;
	default:
		result = 12;
		break;
	}

	return result;
}


void SetColliderColor(D3DXCOLOR _colliderColor)
{
	//TODO
}


bool AdjustScreenPoint(vector3s& pos, vector2s size){
	if (pos.x + size.x * 0.5f > SCREEN_WIDTH * 0.5f) {
		pos.x = SCREEN_WIDTH * 0.5f - size.x * 0.5f;
	}
	else if (pos.x - size.x * 0.5f < -SCREEN_WIDTH * 0.5f) {
		pos.x = -SCREEN_WIDTH * 0.5f + size.x * 0.5f;
	}

	if (pos.y + size.y * 0.5f > SCREEN_HEIGHT * 0.5f) {
		pos.y = SCREEN_HEIGHT * 0.5f - size.y * 0.5f;
	}
	else if (pos.y - size.y * 0.5f < -SCREEN_HEIGHT * 0.5f) {
		pos.y = -SCREEN_HEIGHT * 0.5f + size.y * 0.5f;
	}

	return true;
}


