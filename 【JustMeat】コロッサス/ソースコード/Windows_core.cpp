//�C���N���[�h
#include<string>
#include"ini.h"
#include"Windows_core.h"
#include"gamepad.h"
#include"DX9_sound.h"
#include<thread>


//�v���g�^�C�v�錾
LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
static HWND gHWnd;

//�O���[�o���ϐ�
static std::string caption;
static std::string classname;
static int gWidth;
static int gHeight;

//static std::thread th(BGMLoop);

//�A�N�Z�T
int GetWindowWidth()
{
	return gWidth;
}
int GetWindowHeight()
{
	return gHeight;
}
float GetWindowScaleX()
{
	return (float)gWidth / gDefaultWidth;
}
float GetWindowScaleY()
{
	return (float)gHeight / gDefaultHeight;
}
HWND GetHWND()
{
	return gHWnd;
}
void UninitWINAPI()
{
	//th.join();
}

//�֐�
HWND InitWINAPI(HINSTANCE hInstance, int nCmdShow)
{
	caption = GetConfigString("Windows", "Caption");
	classname = GetConfigString("Windows", "ClassName");
	gWidth = GetConfigInt("Windows", "Width");
	gHeight = GetConfigInt("Windows", "Height");

	// �E�B���h�E�N���X�\���̂̐ݒ�
	WNDCLASS wc = {};
	wc.lpfnWndProc = WndProc;                          // �E�B���h�E�v���V�[�W���̎w��
	wc.lpszClassName = classname.c_str();                     // �N���X���̐ݒ�
	wc.hInstance = hInstance;                          // �C���X�^���X�n���h���̎w��
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);          // �}�E�X�J�[�\�����w��
	wc.hbrBackground = (HBRUSH)(COLOR_BACKGROUND + 1); // �E�C���h�E�̃N���C�A���g�̈�̔w�i�F��ݒ�

	// �N���X�o�^
	RegisterClass(&wc);

	// �E�B���h�E�X�^�C��
	DWORD window_style = WS_OVERLAPPEDWINDOW & ~(WS_MAXIMIZEBOX | WS_THICKFRAME);

	// ��{��`���W
	RECT window_rect = { 0, 0, gWidth, gHeight };

	// �w�肵���N���C�A���g�̈���m�ۂ��邽�߂ɐV���ȋ�`���W���v�Z
	AdjustWindowRect(&window_rect, window_style, FALSE);

	// �V����Window�̋�`���W���畝�ƍ������Z�o
	int window_width = window_rect.right - window_rect.left;
	int window_height = window_rect.bottom - window_rect.top;

	// �v���C�}�����j�^�[�̉�ʉ𑜓x�擾
	int desktop_width = GetSystemMetrics(SM_CXSCREEN);
	int desktop_height = GetSystemMetrics(SM_CYSCREEN);

	// �f�X�N�g�b�v�̐^�񒆂ɃE�B���h�E�����������悤�ɍ��W���v�Z
	// ��������������A�f�X�N�g�b�v���E�B���h�E���傫���ꍇ�͍���ɕ\��
	int window_x = max((desktop_width - window_width) / 2, 0);
	int window_y = max((desktop_height - window_height) / 2, 0);

	// �E�B���h�E�̐���
	gHWnd = CreateWindow(
		classname.c_str(),     // �E�B���h�E�N���X
		caption.c_str(), // �E�B���h�E�e�L�X�g
		window_style,   // �E�B���h�E�X�^�C��
		window_x,       // �E�B���h�E���Wx
		window_y,       // �E�B���h�E���Wy
		window_width,   // �E�B���h�E�̕�
		window_height,  // �E�B���h�E�̍���
		NULL,           // �e�E�B���h�E�n���h��
		NULL,           // ���j���[�n���h��
		hInstance,      // �C���X�^���X�n���h��
		NULL            // �ǉ��̃A�v���P�[�V�����f�[�^
	);

	if (gHWnd == NULL) {
		// �E�B���h�E�n���h�������炩�̗��R�Ő����o���Ȃ�����
		return NULL;
	}

	// �w��̃E�B���h�E�n���h���̃E�B���h�E���w��̕��@�ŕ\��
	ShowWindow(gHWnd, nCmdShow);
	UpdateWindow(gHWnd);
	return gHWnd;
}
bool GameLoopWINAPI()
{
	MSG msg = {}; // msg.message == WM_NULL
	while (WM_QUIT != msg.message) {

		//BGMLoop();
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			// ���b�Z�[�W������ꍇ�̓��b�Z�[�W������D��
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			return true;
		}
	}
	return false;	//WM_QUIT
}

// �E�B���h�E�v���V�[�W��(�R�[���o�b�N�֐�)
LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	//BGMLoop();
	switch (uMsg) {
	case WM_CLOSE:
		if (MessageBox(hWnd, "�{���ɏI�����Ă�낵���ł����H", "�m�F", MB_OKCANCEL | MB_DEFBUTTON2) == IDOK) {
			DestroyWindow(hWnd); // �w��̃E�B���h�E��WM_DESTROY���b�Z�[�W�𑗂�
			UninitXInput();
		}
		return 0; // DefWindowProc�֐��Ƀ��b�Z�[�W�𗬂����I�����邱�Ƃɂ���ĉ����Ȃ��������Ƃɂ���

	case WM_DESTROY: // �E�B���h�E�̔j�����b�Z�[�W
		PostQuitMessage(0); // WM_QUIT���b�Z�[�W�̑��M
		return 0;
	};

	// �ʏ탁�b�Z�[�W�����͂��̊֐��ɔC����
	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}


void MessageErrorOK(const char msg[])
{
	MessageBox(gHWnd, msg, "�G���[", MB_OK);
}