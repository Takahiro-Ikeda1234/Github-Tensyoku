#pragma once
#include"Windows_core.h"
// dinput.h���C���N���[�h����O�ɂ�������Ȃ��ƌx�����o��̂Œ���
#define DIRECTINPUT_VERSION (0x0800)
#include <dinput.h>

#define NUM_KEY_MAX 256
void UninitInputDX9();
BYTE * GetKeyboardStateDX9();
BYTE * GetMouseStateDX9(int * wheelMove);
bool InitInputDX9(HINSTANCE hInstance);