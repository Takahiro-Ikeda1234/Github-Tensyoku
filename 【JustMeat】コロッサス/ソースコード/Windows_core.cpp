//インクルード
#include<string>
#include"ini.h"
#include"Windows_core.h"
#include"gamepad.h"
#include"DX9_sound.h"
#include<thread>


//プロトタイプ宣言
LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
static HWND gHWnd;

//グローバル変数
static std::string caption;
static std::string classname;
static int gWidth;
static int gHeight;

//static std::thread th(BGMLoop);

//アクセサ
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

//関数
HWND InitWINAPI(HINSTANCE hInstance, int nCmdShow)
{
	caption = GetConfigString("Windows", "Caption");
	classname = GetConfigString("Windows", "ClassName");
	gWidth = GetConfigInt("Windows", "Width");
	gHeight = GetConfigInt("Windows", "Height");

	// ウィンドウクラス構造体の設定
	WNDCLASS wc = {};
	wc.lpfnWndProc = WndProc;                          // ウィンドウプロシージャの指定
	wc.lpszClassName = classname.c_str();                     // クラス名の設定
	wc.hInstance = hInstance;                          // インスタンスハンドルの指定
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);          // マウスカーソルを指定
	wc.hbrBackground = (HBRUSH)(COLOR_BACKGROUND + 1); // ウインドウのクライアント領域の背景色を設定

	// クラス登録
	RegisterClass(&wc);

	// ウィンドウスタイル
	DWORD window_style = WS_OVERLAPPEDWINDOW & ~(WS_MAXIMIZEBOX | WS_THICKFRAME);

	// 基本矩形座標
	RECT window_rect = { 0, 0, gWidth, gHeight };

	// 指定したクライアント領域を確保するために新たな矩形座標を計算
	AdjustWindowRect(&window_rect, window_style, FALSE);

	// 新たなWindowの矩形座標から幅と高さを算出
	int window_width = window_rect.right - window_rect.left;
	int window_height = window_rect.bottom - window_rect.top;

	// プライマリモニターの画面解像度取得
	int desktop_width = GetSystemMetrics(SM_CXSCREEN);
	int desktop_height = GetSystemMetrics(SM_CYSCREEN);

	// デスクトップの真ん中にウィンドウが生成されるように座標を計算
	// ※ただし万が一、デスクトップよりウィンドウが大きい場合は左上に表示
	int window_x = max((desktop_width - window_width) / 2, 0);
	int window_y = max((desktop_height - window_height) / 2, 0);

	// ウィンドウの生成
	gHWnd = CreateWindow(
		classname.c_str(),     // ウィンドウクラス
		caption.c_str(), // ウィンドウテキスト
		window_style,   // ウィンドウスタイル
		window_x,       // ウィンドウ座標x
		window_y,       // ウィンドウ座標y
		window_width,   // ウィンドウの幅
		window_height,  // ウィンドウの高さ
		NULL,           // 親ウィンドウハンドル
		NULL,           // メニューハンドル
		hInstance,      // インスタンスハンドル
		NULL            // 追加のアプリケーションデータ
	);

	if (gHWnd == NULL) {
		// ウィンドウハンドルが何らかの理由で生成出来なかった
		return NULL;
	}

	// 指定のウィンドウハンドルのウィンドウを指定の方法で表示
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
			// メッセージがある場合はメッセージ処理を優先
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

// ウィンドウプロシージャ(コールバック関数)
LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	//BGMLoop();
	switch (uMsg) {
	case WM_CLOSE:
		if (MessageBox(hWnd, "本当に終了してよろしいですか？", "確認", MB_OKCANCEL | MB_DEFBUTTON2) == IDOK) {
			DestroyWindow(hWnd); // 指定のウィンドウにWM_DESTROYメッセージを送る
			UninitXInput();
		}
		return 0; // DefWindowProc関数にメッセージを流さず終了することによって何もなかったことにする

	case WM_DESTROY: // ウィンドウの破棄メッセージ
		PostQuitMessage(0); // WM_QUITメッセージの送信
		return 0;
	};

	// 通常メッセージ処理はこの関数に任せる
	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}


void MessageErrorOK(const char msg[])
{
	MessageBox(gHWnd, msg, "エラー", MB_OK);
}