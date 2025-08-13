//=============================================================================
//
// [Keyboard.cpp] キーボード制御
//
//=============================================================================

//インクルードファイル
#include"keyboard.h"
#include"DX9_input.h"


void Keyboard::Update(float deltaTime)
{
	memcpy(mPrevKeyDownTime, mKeyDownTime, sizeof(mKeyDownTime));

	BYTE* KB = GetKeyboardStateDX9();

	for (int i = 0; i < 256; i++)
	{
		if (KB[i] & 0x80)
		{
			mKeyDownTime[i] += deltaTime;
		}
		else
		{
			mKeyDownTime[i] = 0;
		}
	}
}

bool Keyboard::KeyHit(int keycode) const
{
	if (mKeyDownTime[keycode] > 0 && mPrevKeyDownTime[keycode] == 0)
	{
		return true;
	}
	return false;
}

float Keyboard::KeyDown(int keycode) const
{
	return mKeyDownTime[keycode];
}

bool Keyboard::KeyUp(int keycode) const
{
	if (mKeyDownTime[keycode] == 0 && mPrevKeyDownTime[keycode] > 0)
	{
		return true;
	}
	return false;
}
