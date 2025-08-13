//=============================================================================
//
// [keyboard.h] キーボード制御
//
//=============================================================================
#ifndef _KEYBOARD_H_
#define _KEYBOARD_H_
#include"Singleton.h"
#include"DX9_input.h"
class Keyboard : public Singleton<Keyboard>
{
private:
	friend Singleton<Keyboard>;
	Keyboard() : mKeyDownTime(), mPrevKeyDownTime(), Singleton() {}
public:
	void Update(float deltaTime);
	bool KeyHit(int keycode) const;
	float KeyDown(int keycode) const;
	bool KeyUp(int keycode) const;
private:
	float mKeyDownTime[256];	//キーボード情報
	float mPrevKeyDownTime[256];	//前フレームのキーボード情報
};

//マクロ定義
#define VK_0 0x30
#define VK_1 0x31
#define VK_2 0x32
#define VK_3 0x33
#define VK_4 0x34
#define VK_5 0x35
#define VK_6 0x36
#define VK_7 0x37
#define VK_8 0x38
#define VK_9 0x39
#define VK_A 0x41
#define VK_B 0x42
#define VK_C 0x43
#define VK_D 0x44
#define VK_E 0x45
#define VK_F 0x46
#define VK_G 0x47
#define VK_H 0x48
#define VK_I 0x49
#define VK_J 0x4A
#define VK_K 0x4B
#define VK_L 0x4C
#define VK_M 0x4D
#define VK_N 0x4E
#define VK_O 0x4F
#define VK_P 0x50
#define VK_Q 0x51
#define VK_R 0x52
#define VK_S 0x53
#define VK_T 0x54
#define VK_U 0x55
#define VK_V 0x56
#define VK_W 0x57
#define VK_X 0x58
#define VK_Y 0x59
#define VK_Z 0x5Ad
#define KEY_LEFT VK_A
#define KEY_RIGHT VK_D
#define KEY_JUMP VK_SPACE
#define KEY_ESC VK_ESCAPE
#define KEY_SHOW_UI VK_TAB

//プロトタイプ宣言
void UpdateKeyboard();
bool KeyHit(int keycode);
int KeyDown(int keycode);
bool KeyUp(int keycode);
#endif