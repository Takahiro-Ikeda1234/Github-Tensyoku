#include"DX9_core.h"
#include"DX9_sound.h"
#include"gameProcess.h"
#include <Windows.h>
#include<stdio.h>
#include<stdlib.h>
#pragma comment (lib, "d3d9.lib")
#pragma comment (lib, "d3dx9.lib")
#pragma comment (lib, "dinput8.lib")
#pragma comment (lib, "dxguid.lib")
#pragma comment (lib, "winmm.lib")


// メイン関数
int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	// 使用しない一時変数を明示
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);
	// ゲームの初期化(Direct3Dの初期化)
	if (InitDX9(hInstance, nCmdShow)) {
		// ゲームの初期化に失敗した
		return -1;
	}
	// Windowsゲーム用メインループ
	
	GameProcess* gameProc = new GameProcess;


	while (GameLoopDX9())
	{

		if (gameProc->Update())
		{
			break;
		}
	}

	// ゲームの終了処理(Direct3Dの終了処理)
	delete gameProc;
	UninitDX9();

	return 0;
}
