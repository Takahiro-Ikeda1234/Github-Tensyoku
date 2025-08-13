//=============================================================================
//
// [mouse.cpp] マウス入力
//
//=============================================================================

//インクルードファイル
#define NOMINMAX
#include "Windows_core.h"
#include "DX9_input.h"
#include"mouse.h"

void Mouse::Update(float deltaTime)
{
	//以前の状態の保管
	mPrevMouseState = mMouseState;

	BYTE *mouseState;
	mouseState = GetMouseStateDX9(&mMouseState.wheelMove);

	for (int i = 0; i < 3; i++)
	{
		if (mouseState[i] & 0x80)
		{
			mMouseState.mouseDownTime[i] += deltaTime;
		}
		else
		{
			mMouseState.mouseDownTime[i] = 0;
		}
	}

	//マウス位置取得
	POINT mousePt;
	GetCursorPos(&mousePt);

	//ウィンドウサイズ取得
	DWORD window_style = WS_OVERLAPPEDWINDOW & ~(WS_MAXIMIZEBOX | WS_THICKFRAME);
	RECT windowRect;	//非クライアント領域の大きさ計算用
	GetClientRect(GetHWND(), &windowRect);	//クライアント領域
	AdjustWindowRect(&windowRect, window_style, FALSE);	//非クライアント領域含めた大きさ
	RECT windowPosition;	//ウィンドウの位置
	GetWindowRect(GetHWND(), &windowPosition);
	
	int window_x = windowPosition.left - windowRect.left;	//ウィンドウの左上位置
	int window_y = windowPosition.top - windowRect.top;	//ウィンドウの左上位置
	int mouseX = mousePt.x - window_x;	//マウスの相対位置
	int mouseY = mousePt.y - window_y;	//マウスの相対位置

	mMouseState.pos.x = mouseX;
	mMouseState.pos.y = mouseY;
}

bool Mouse::MouseHit(int mousecode) const
{
	if (mMouseState.mouseDownTime[mousecode] > 0 && mPrevMouseState.mouseDownTime[mousecode] == 0)
	{
		return true;
	}
	return false;
}

float Mouse::MouseDown(int mousecode) const
{
	return mMouseState.mouseDownTime[mousecode];
}

bool Mouse::MouseUp(int mousecode) const
{
	if (mMouseState.mouseDownTime[mousecode] == 0 && mPrevMouseState.mouseDownTime[mousecode] > 0)
	{
		return true;
	}
	return false;
}
