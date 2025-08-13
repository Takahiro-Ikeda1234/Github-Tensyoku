//=============================================================================
//
// [mouse.cpp] �}�E�X����
//
//=============================================================================

//�C���N���[�h�t�@�C��
#define NOMINMAX
#include "Windows_core.h"
#include "DX9_input.h"
#include"mouse.h"

void Mouse::Update(float deltaTime)
{
	//�ȑO�̏�Ԃ̕ۊ�
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

	//�}�E�X�ʒu�擾
	POINT mousePt;
	GetCursorPos(&mousePt);

	//�E�B���h�E�T�C�Y�擾
	DWORD window_style = WS_OVERLAPPEDWINDOW & ~(WS_MAXIMIZEBOX | WS_THICKFRAME);
	RECT windowRect;	//��N���C�A���g�̈�̑傫���v�Z�p
	GetClientRect(GetHWND(), &windowRect);	//�N���C�A���g�̈�
	AdjustWindowRect(&windowRect, window_style, FALSE);	//��N���C�A���g�̈�܂߂��傫��
	RECT windowPosition;	//�E�B���h�E�̈ʒu
	GetWindowRect(GetHWND(), &windowPosition);
	
	int window_x = windowPosition.left - windowRect.left;	//�E�B���h�E�̍���ʒu
	int window_y = windowPosition.top - windowRect.top;	//�E�B���h�E�̍���ʒu
	int mouseX = mousePt.x - window_x;	//�}�E�X�̑��Έʒu
	int mouseY = mousePt.y - window_y;	//�}�E�X�̑��Έʒu

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
