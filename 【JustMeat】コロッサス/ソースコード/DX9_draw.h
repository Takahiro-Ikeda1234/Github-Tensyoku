#pragma once
#include"Draw.h"
#include<d3dx9.h>
typedef struct
{
	D3DXVECTOR4 position;
	D3DCOLOR colorRGBA;
	D3DXVECTOR2 texcoord;
}VERTEX_3D_DX9;
void BlendDefaultDX9();
void BlendAddBlendDX9();
void BlendMultipleBlendDX9();

void DrawStartDX9(int r, int g, int b);
void SwapBufferDX9();
void EnableStencilDX9();
void DisableStencilDX9();
void EnableStencilTest();
void DisableStencilTestDX9();
void ClearStencilDX9();
void DrawVertexDX9(VERTEX_3D vertex[], void* texture);
void DrawVertexLineDX9(VERTEX_3D vertex[]);
void DrawVertexTriangleDX9(VERTEX_3D vertex[]);
VERTEX_3D_DX9 VERTEX3DtoDX9(VERTEX_3D& vertex);