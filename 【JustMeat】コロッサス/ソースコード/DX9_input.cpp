#include"DX9_input.h"

LPDIRECTINPUT8 DInput;
LPDIRECTINPUTDEVICE8 DInputDeviceKeyboard;
LPDIRECTINPUTDEVICE8 DInputDeviceMouse;

static bool InitDInputKeyboard();
bool InitDInputMouse();

bool InitInputDX9(HINSTANCE hInstance)
{
	if (DInput == NULL)
	{
		if (FAILED(DirectInput8Create(hInstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&DInput, NULL)))
		{
			MessageErrorOK("DirectInput�I�u�W�F�N�g�̍쐬�Ɏ��s");
			return true;
		}
	}
	if (InitDInputKeyboard())
	{
		return true;
	}
	if (InitDInputMouse())
	{
		return true;
	}

	return false;
}
void UninitInputDX9()
{
	if (DInputDeviceMouse != NULL)
	{
		DInputDeviceMouse->Release();
		DInputDeviceMouse = NULL;
	}
	if (DInputDeviceKeyboard != NULL)
	{
		DInputDeviceKeyboard->Release();
		DInputDeviceKeyboard = NULL;
	}
	if (DInput != NULL)
	{
		DInput->Release();
		DInput = NULL;
	}
}

BYTE* GetKeyboardStateDX9()
{
	static BYTE keyState[NUM_KEY_MAX];
	ZeroMemory(keyState, sizeof(keyState));
	if (FAILED(DInputDeviceKeyboard->GetDeviceState(sizeof(keyState), keyState)))
	{
		DInputDeviceKeyboard->Acquire();
		DInputDeviceKeyboard->GetDeviceState(sizeof(keyState), keyState);
	}
	return keyState;
}

BYTE* GetMouseStateDX9(int *wheelMove)
{
	static DIMOUSESTATE mouseState;
	ZeroMemory(&mouseState, sizeof(mouseState));

	if (FAILED(DInputDeviceMouse->GetDeviceState(sizeof(mouseState), &mouseState)))
	{
		DInputDeviceMouse->Acquire();
		DInputDeviceMouse->GetDeviceState(sizeof(mouseState), &mouseState);
	}

	*wheelMove = mouseState.lZ;
	return mouseState.rgbButtons;
}

bool InitDInputKeyboard()
{
	if (FAILED(DInput->CreateDevice(GUID_SysKeyboard, &DInputDeviceKeyboard, NULL)))
	{
		MessageErrorOK("�L�[�{�[�h��������܂���ł���");
		return true;
	}

	if (FAILED(DInputDeviceKeyboard->SetDataFormat(&c_dfDIKeyboard)))
	{
		MessageErrorOK("�L�[�{�[�h�̃f�[�^�t�H�[�}�b�g�ݒ�Ɏ��s");
		return true;
	}

	if (FAILED(DInputDeviceKeyboard->SetCooperativeLevel(GetHWND(), (DISCL_FOREGROUND | DISCL_NONEXCLUSIVE))))
	{
		MessageErrorOK("�L�[�{�[�h�̋������[�h�ݒ�Ɏ��s");
		return true;
	}

	DInputDeviceKeyboard->Acquire();
	return false;
}

bool InitDInputMouse()
{
	if (FAILED(DInput->CreateDevice(GUID_SysMouse, &DInputDeviceMouse, NULL)))
	{
		MessageErrorOK("�}�E�X��������܂���ł���");
		return true;
	}

	if (FAILED(DInputDeviceMouse->SetDataFormat(&c_dfDIMouse)))
	{
		MessageErrorOK("�}�E�X�̃f�[�^�t�H�[�}�b�g�ݒ�Ɏ��s");
		return true;
	}

	if (FAILED(DInputDeviceMouse->SetCooperativeLevel(GetHWND(), (DISCL_FOREGROUND | DISCL_NONEXCLUSIVE))))
	{
		MessageErrorOK("�}�E�X�̋������[�h�ݒ�Ɏ��s");
		return true;
	}

	DIPROPDWORD diprop;
	diprop.diph.dwSize = sizeof(diprop);
	diprop.diph.dwHeaderSize = sizeof(diprop.diph);
	diprop.diph.dwObj = 0;
	diprop.diph.dwHow = DIPH_DEVICE;
	diprop.dwData = DIPROPAXISMODE_REL; // ���Βl���[�h�Őݒ�i��Βl��DIPROPAXISMODE_ABS�j   
	if (FAILED(DInputDeviceMouse->SetProperty(DIPROP_AXISMODE, &diprop.diph)))
	{
		MessageErrorOK("�}�E�X�̐ݒ�Ɏ��s");
		return true;
	}

	DInputDeviceMouse->Acquire();
	return false;
}
