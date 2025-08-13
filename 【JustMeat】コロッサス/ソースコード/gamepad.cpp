#include<Windows.h>
#include<XInput.h>
#pragma comment(lib, "xinput.lib")
#include"gamepad.h"

XINPUT_STATE state;

bool UpdateXInput(float deltaTime)
{
	if (ERROR_SUCCESS == XInputGetState(0, &state)) return false;
	return true;
}

void UninitXInput()
{
	XInputEnable(false);
}

void Gamepad::Update(float deltaTime)
{
	for (int i = 0; i < XINPUT_CONTROLLER_MAX; i++)
	{
		mPrevGamepadState[i] = mGamepadState[i];
		XINPUT_STATE state;
		XInputGetState(i, &state);
		DWORD buttonMask = 0b0001;
		for (int j = 0; j < XINPUT_BUTTON_MAX; j++)
		{
			if (j == GAMEPAD_LT)
			{
				mGamepadState[i].LTrigger = state.Gamepad.bLeftTrigger;
				if (mGamepadState[i].LTrigger > 0)
				{
					mGamepadState[i].buttonDownTime[GAMEPAD_LT] += deltaTime;
				}
				else
				{
					mGamepadState[i].buttonDownTime[GAMEPAD_LT] = 0;
				}
			}
			else if (j == GAMEPAD_RT)
			{
				mGamepadState[i].RTrigger = state.Gamepad.bRightTrigger;
				if (mGamepadState[i].RTrigger > 0)
				{
					mGamepadState[i].buttonDownTime[GAMEPAD_RT] += deltaTime;
				}
				else
				{
					mGamepadState[i].buttonDownTime[GAMEPAD_RT] = 0;
				}
			}
			else
			{
				if (state.Gamepad.wButtons & buttonMask)
				{
					mGamepadState[i].buttonDownTime[j] += deltaTime;
				}
				else
				{
					mGamepadState[i].buttonDownTime[j] = 0;
				}
			}
			buttonMask <<= 1;
		}

		mGamepadState[i].leftX = state.Gamepad.sThumbLX;
		mGamepadState[i].leftY = state.Gamepad.sThumbLY;
		mGamepadState[i].rightX = state.Gamepad.sThumbRX;
		mGamepadState[i].rightY = state.Gamepad.sThumbRY;
	}
}

bool Gamepad::ButtonHit(int buttoncode, int ch) const
{
	if (mGamepadState[ch].buttonDownTime[buttoncode] > 0 && mPrevGamepadState[ch].buttonDownTime[buttoncode] == 0)
	{
		return true;
	}
	return false;
}

float Gamepad::ButtonDown(int buttoncode, int ch) const
{
	return mGamepadState[ch].buttonDownTime[buttoncode];
}

bool Gamepad::ButtonUp(int buttoncode, int ch) const
{
	if (mGamepadState[ch].buttonDownTime[buttoncode] == 0 && mPrevGamepadState[ch].buttonDownTime[buttoncode] > 0)
	{
		return true;
	}
	return false;
}

void Gamepad::Release() const
{
	XInputEnable(false);
}
