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


// ���C���֐�
int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	// �g�p���Ȃ��ꎞ�ϐ��𖾎�
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);
	// �Q�[���̏�����(Direct3D�̏�����)
	if (InitDX9(hInstance, nCmdShow)) {
		// �Q�[���̏������Ɏ��s����
		return -1;
	}
	// Windows�Q�[���p���C�����[�v
	
	GameProcess* gameProc = new GameProcess;


	while (GameLoopDX9())
	{

		if (gameProc->Update())
		{
			break;
		}
	}

	// �Q�[���̏I������(Direct3D�̏I������)
	delete gameProc;
	UninitDX9();

	return 0;
}
