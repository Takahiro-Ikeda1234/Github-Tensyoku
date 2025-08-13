#pragma once
bool UpdateXInput(float deltaTime);
void UninitXInput();

static const int XINPUT_CONTROLLER_MAX = 1;
static const int XINPUT_BUTTON_MAX = 16;

static const int GAMEPAD_UP = 0;
static const int GAMEPAD_DOWN = 1;
static const int GAMEPAD_LEFT = 2;
static const int GAMEPAD_RIGHT = 3;
static const int GAMEPAD_START = 4;
static const int GAMEPAD_BACK = 5;
static const int GAMEPAD_LSTICK = 6;	//LEFT_THUMB スティック押し込み
static const int GAMEPAD_RSTICK = 7;	//RIGHT_THUMB スティック押し込み
static const int GAMEPAD_LB = 8;	//LEFT_SHOULDER
static const int GAMEPAD_RB = 9;	//RIGHT_SHOULDER
static const int GAMEPAD_LT = 10;
static const int GAMEPAD_RT = 11;
static const int GAMEPAD_A = 12;
static const int GAMEPAD_B = 13;
static const int GAMEPAD_X = 14;
static const int GAMEPAD_Y = 15;

#include"Singleton.h"
class Gamepad : public Singleton<Gamepad>
{
private:
	friend Singleton<Gamepad>;
	Gamepad() : Singleton() {}

public:
	void Update(float deltaTime);
	bool ButtonHit(int buttoncode, int ch = 0) const;
	float ButtonDown(int buttoncode, int ch = 0) const;
	bool ButtonUp(int buttoncode, int ch = 0) const;
	int LeftX(int ch = 0) const { return mGamepadState[ch].leftX; }
	int LeftY(int ch = 0) const { return mGamepadState[ch].leftY; }
	int RightX(int ch = 0) const { return mGamepadState[ch].rightX; }
	int RightY(int ch = 0) const { return mGamepadState[ch].rightY; }
	int LTrigger(int ch = 0) const { return mGamepadState[ch].LTrigger; }
	int RTrigger(int ch = 0) const { return mGamepadState[ch].RTrigger; }
	void Release() const;
private:
	typedef struct
	{
		float buttonDownTime[XINPUT_BUTTON_MAX];
		int leftX;
		int leftY;
		int rightX;
		int rightY;
		int LTrigger;
		int RTrigger;
	}GamepadState;

	GamepadState mGamepadState[XINPUT_CONTROLLER_MAX];
	GamepadState mPrevGamepadState[XINPUT_CONTROLLER_MAX];
};