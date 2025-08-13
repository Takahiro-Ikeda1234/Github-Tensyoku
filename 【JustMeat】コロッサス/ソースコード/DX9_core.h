#pragma once
#include<d3d9.h>
#include<d3dx9.h>
#include"Windows_core.h"

LPDIRECT3DDEVICE9 GetD3DDevice();

bool InitDX9(HINSTANCE hInst, int nCmdShow);

bool GameLoopDX9();

void UninitDX9();
