#include"Draw.h"
#include"DX9_core.h"
#include"DX9_draw.h"
#define FVF_VERTEX2D (D3DFVF_XYZRHW|D3DFVF_DIFFUSE|D3DFVF_TEX1)
DWORD g_ZEnableState;
DWORD g_ZFuncState;
void BlendDefaultDX9()
{
	GetD3DDevice()->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	GetD3DDevice()->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
}
void BlendAddBlendDX9()
{
	GetD3DDevice()->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	GetD3DDevice()->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
}
void BlendMultipleBlendDX9()
{
	GetD3DDevice()->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_ZERO);
	GetD3DDevice()->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_SRCCOLOR);
}
void DrawStartDX9(int r, int g, int b)
{
	GetD3DDevice()->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_STENCIL | D3DCLEAR_ZBUFFER, D3DCOLOR_RGBA(r, g, b, 255), 1.0f, 0);
	GetD3DDevice()->BeginScene();
	GetD3DDevice()->SetFVF(FVF_VERTEX2D);
}

void SwapBufferDX9()
{
	GetD3DDevice()->EndScene();
	GetD3DDevice()->Present(NULL, NULL, NULL, NULL);
}

void EnableStencilDX9()
{
	GetD3DDevice()->GetRenderState(D3DRS_ZENABLE, &g_ZEnableState);
	GetD3DDevice()->GetRenderState(D3DRS_ZFUNC, &g_ZFuncState);
	GetD3DDevice()->SetRenderState(D3DRS_ZENABLE, TRUE);
	GetD3DDevice()->SetRenderState(D3DRS_ZFUNC, D3DCMP_NEVER);
	GetD3DDevice()->SetRenderState(D3DRS_STENCILENABLE, TRUE);	//ステンシルテストを有効にする
	GetD3DDevice()->SetRenderState(D3DRS_STENCILFUNC, D3DCMP_ALWAYS); // ステンシルテストは常に行う
	GetD3DDevice()->SetRenderState(D3DRS_STENCILPASS, D3DSTENCILOP_REPLACE);
	GetD3DDevice()->SetRenderState(D3DRS_STENCILZFAIL, D3DSTENCILOP_REPLACE);
	GetD3DDevice()->SetRenderState(D3DRS_STENCILREF, 0x01);
	GetD3DDevice()->SetRenderState(D3DRS_STENCILMASK, 0xff);
}
void DisableStencilDX9()
{
	GetD3DDevice()->SetRenderState(D3DRS_STENCILFUNC, D3DCMP_GREATER);
	GetD3DDevice()->SetRenderState(D3DRS_STENCILFAIL, D3DSTENCILOP_KEEP);	//何もしない
	GetD3DDevice()->SetRenderState(D3DRS_STENCILPASS, D3DSTENCILOP_KEEP);	//何もしない
	GetD3DDevice()->SetRenderState(D3DRS_STENCILZFAIL, D3DSTENCILOP_KEEP);	//何もしない
	//GetD3DDevice()->SetRenderState(D3DRS_STENCILENABLE, FALSE);	//ステンシルテストを無効にする
	GetD3DDevice()->SetRenderState(D3DRS_ZENABLE, g_ZEnableState);
	GetD3DDevice()->SetRenderState(D3DRS_ZFUNC, g_ZFuncState);
}
void EnableStencilTest()
{
	GetD3DDevice()->SetRenderState(D3DRS_STENCILENABLE, TRUE);	//ステンシルテストを有効にする
}
void DisableStencilTestDX9()
{
	GetD3DDevice()->SetRenderState(D3DRS_STENCILENABLE, FALSE);	//ステンシルテストを無効にする
}
void ClearStencilDX9()
{
	GetD3DDevice()->Clear(0, NULL, D3DCLEAR_STENCIL, D3DCOLOR_RGBA(0, 0, 0, 255), 1.0f, 0);
}

void DrawVertexDX9(VERTEX_3D vertex[], void* texture)
{
	VERTEX_3D_DX9 vertexDX9[4];
	for (int i = 0; i < 4; i++)
	{
		vertexDX9[i] = VERTEX3DtoDX9(vertex[i]);
	}
	GetD3DDevice()->SetTexture(0, (LPDIRECT3DTEXTURE9)texture);
	GetD3DDevice()->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, vertexDX9, sizeof(VERTEX_3D_DX9));
}

void DrawVertexLineDX9(VERTEX_3D vertex[])
{
	VERTEX_3D_DX9 vertexDX9[5];
	for (int i = 0; i < 5; i++)
	{
		vertexDX9[i] = VERTEX3DtoDX9(vertex[i]);
	}
	GetD3DDevice()->SetTexture(0, NULL);
	GetD3DDevice()->DrawPrimitiveUP(D3DPT_LINESTRIP, 4, vertexDX9, sizeof(VERTEX_3D_DX9));
}
void DrawVertexTriangleDX9(VERTEX_3D vertex[])
{
	VERTEX_3D_DX9 vertexDX9[3];
	for (int i = 0; i < 3; i++)
	{
		vertexDX9[i] = VERTEX3DtoDX9(vertex[i]);
	}
	GetD3DDevice()->SetTexture(0, NULL);

	GetD3DDevice()->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 1, vertexDX9, sizeof(VERTEX_3D_DX9));
}

VERTEX_3D_DX9 VERTEX3DtoDX9(VERTEX_3D& vertex)
{
	VERTEX_3D_DX9 temp;
	temp.colorRGBA = vertex.Color;
	temp.position.x = vertex.Position.x;
	temp.position.y = vertex.Position.y;
	temp.position.z = vertex.Position.z;
	temp.position.w = vertex.Position.w;
	temp.texcoord.x = vertex.TexCoord.x;
	temp.texcoord.y = vertex.TexCoord.y;

	return temp;
}
