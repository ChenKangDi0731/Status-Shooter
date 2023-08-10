/*==============================================================================

   DirectX�̏����� [main.cpp]
                                                         Author : 
                                                         Date   : 
--------------------------------------------------------------------------------

==============================================================================*/
#include "main.h"
//*****************************************************************************
// ���C�u�����̃����N0.
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
// �}�N����`
//*****************************************************************************
#define CLASS_NAME			"GameWindow"				// �E�C���h�E�̃N���X��
#define WINDOW_CAPTION		"�X�e�[�^�X�V���[�^�["			// �E�C���h�E�̃L���v�V������

//*****************************************************************************
// �\���̒�`
//*****************************************************************************

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

HRESULT Init(HINSTANCE hInstance, HWND hWnd, BOOL bWindow);
void Uninit(void);
void Update(void);
void Draw(void);


//*****************************************************************************
// �O���[�o���ϐ�:
//*****************************************************************************

#ifdef _DEBUG
int		g_CountFPS;							// FPS�J�E���^
char	g_DebugStr[2048] = WINDOW_CAPTION;	// �f�o�b�O�����\���p

#endif

vector2s preMousePos = vector2s(0.0f, 0.0f);
vector2s curMousePos = vector2s(0.0f, 0.0f);
vector2s curMouseMovement = vector2s(0.0f, 0.0f);
HWND hwndInstance;
											//=============================================================================
// ���C���֐�
//=============================================================================
int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);	// �����Ă��ǂ����ǁA�x�����o��i���g�p�錾�j
	UNREFERENCED_PARAMETER(lpCmdLine);		// �����Ă��ǂ����ǁA�x�����o��i���g�p�錾�j

	// ���Ԍv���p
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
	
	// �E�B���h�E�̍쐬
	HWND hWnd = CreateWindow(CLASS_NAME,
						WINDOW_CAPTION,
						WS_OVERLAPPEDWINDOW,
						CW_USEDEFAULT,																		// �E�B���h�E�̍����W
						CW_USEDEFAULT,																		// �E�B���h�E�̏���W
						SCREEN_WIDTH + GetSystemMetrics(SM_CXDLGFRAME)*2,									// �E�B���h�E����
						SCREEN_HEIGHT + GetSystemMetrics(SM_CXDLGFRAME)*2+GetSystemMetrics(SM_CYCAPTION),	// �E�B���h�E�c��
						nullptr,
						nullptr,
						hInstance,
						nullptr);
	// DirectX�̏�����(�E�B���h�E���쐬���Ă���s��)
	if(FAILED(Init(hInstance, hWnd, true)))
	{
		return -1;
	}

	// �t���[���J�E���g������
	timeBeginPeriod(1);	// ����\��ݒ�
	dwExecLastTime = dwFPSLastTime = timeGetTime();	// �V�X�e���������~���b�P�ʂŎ擾
	dwCurrentTime = dwFrameCount = 0;
	
	// �E�C���h�E�̕\��(Init()�̌�ɌĂ΂Ȃ��Ƒʖ�)
	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	MSG			msg;

	hwndInstance = hWnd;

	// ���b�Z�[�W���[�v
	while(1)
	{
		if(PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
		{
			if(msg.message == WM_QUIT)
			{// PostQuitMessage()���Ă΂ꂽ�烋�[�v�I��
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
			dwCurrentTime = timeGetTime();					// �V�X�e���������擾

			if ((dwCurrentTime - dwFPSLastTime) >= 1000)	// 1�b���ƂɎ��s
			{
#ifdef _DEBUG
				g_CountFPS = dwFrameCount;
#endif
				dwFPSLastTime = dwCurrentTime;				// FPS�𑪒肵��������ۑ�
				dwFrameCount = 0;							// �J�E���g���N���A
			}

			if ((dwCurrentTime - dwExecLastTime) >= (1000 / 60))	// 1/60�b���ƂɎ��s
			{
				dwExecLastTime = dwCurrentTime;	// ��������������ۑ�

#ifdef _DEBUG	// �f�o�b�O�ł̎�����FPS��\������
				wsprintf(g_DebugStr, WINDOW_CAPTION);
				wsprintf(&g_DebugStr[strlen(g_DebugStr)], " FPS:%d", g_CountFPS);
				SetWindowText(hWnd, g_DebugStr);
#endif

				Update();			// �X�V����
				Draw();				// �`�揈��

				dwFrameCount++;		// �����񐔂̃J�E���g�����Z
			}
		}
	}
	
	timeEndPeriod(1);				// ����\��߂�

	// �I������
	Uninit();

	return (int)msg.wParam;
}

//=============================================================================
// �v���V�[�W��
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
		if (MessageBox(hWnd, "�{���ɏI�����Ă�낵���ł����H", "�m�F", MB_OKCANCEL | MB_DEFBUTTON2) == IDOK) {
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
// ����������
//=============================================================================
HRESULT Init(HINSTANCE hInstance, HWND hWnd, BOOL bWindow)
{
	// �����_�����O�����̏�����
	InitRenderer(hInstance, hWnd, bWindow);
	//���͏����̏�����
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
// �I������
//=============================================================================
void Uninit(void)
{
	//���͏����̏I��
	UninitInput();
	UninitSound();
	UninitTexture();
	UninitGame();
}

//=============================================================================
// �X�V����
//=============================================================================
void Update(void)
{
	//���͏����̍X�V
	UpdateInput();

	preMousePos = curMousePos;
	curMousePos.x = GetMouseX()* MOUSE_SENSITY;
	curMousePos.y = GetMouseY()* MOUSE_SENSITY;
	UpdateGame();

}

//=============================================================================
// �`�揈��
//=============================================================================
void Draw(void)
{
	// �o�b�N�o�b�t�@�N���A
	Clear();

	// 2D�`��Ȃ̂Ő[�x����
	SetDepthEnable(true);
	DrawGame();
	// �o�b�N�o�b�t�@�A�t�����g�o�b�t�@����ւ�
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


