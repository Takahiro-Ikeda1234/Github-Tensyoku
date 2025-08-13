#include"DX9_texture.h"
#include"DX9_core.h"
#include"DX9_draw.h"
#include"TextureData.h"
void *LoadTextureDX9(std::string filepath, int width, int height)
{
	LPDIRECT3DTEXTURE9 texture;
	D3DXCreateTextureFromFileEx(GetD3DDevice(), filepath.c_str(), width, height, 1, 0,
		D3DFMT_A8R8G8B8, D3DPOOL_MANAGED, D3DX_FILTER_NONE, D3DX_FILTER_NONE, 0x00000000, NULL, NULL, &texture
	);


	return (void*)texture;
}
void UnloadTextureDX9(void* texture)
{
	((LPDIRECT3DTEXTURE9)texture)->Release();
}


static LPDIRECT3DTEXTURE9 StageData[4];
static int Width;	//ステージ幅
static int Height;	//ステージ高さ
void StartExportStageData(int w, int h)
{
	Width = w;
	Height = h;
	bool oddh = Height % 2;
	bool oddw = Width % 2;
	HRESULT result1 = GetD3DDevice()->CreateTexture((w/ 2) *32, (h / 2) * 32, 1, 0, D3DFMT_A8R8G8B8, D3DPOOL_MANAGED, &StageData[0], NULL);
	HRESULT result2 = GetD3DDevice()->CreateTexture((w / 2+oddw) * 32, (h / 2) * 32, 1, 0, D3DFMT_A8R8G8B8, D3DPOOL_MANAGED, &StageData[1], NULL);
	HRESULT result3 = GetD3DDevice()->CreateTexture((w / 2) * 32, (h / 2 + oddh) * 32, 1, 0, D3DFMT_A8R8G8B8, D3DPOOL_MANAGED, &StageData[2], NULL);
	HRESULT result4 = GetD3DDevice()->CreateTexture((w/2+oddw) * 32, (h / 2 + oddh) *32, 1, 0, D3DFMT_A8R8G8B8, D3DPOOL_MANAGED, &StageData[3], NULL);
}
void ExportObject(void* texture, int indexX, int indexY, int useID, int divX, int divY)
{
	((LPDIRECT3DTEXTURE9)texture);

	int idxy = 0;
	if (indexY >= Height / 2)
	{
		idxy = 1;
	}
	int idxx = 0;
	if (indexX >= Width / 2)
	{
		idxx = 1;
	}

	// テクスチャにフォントビットマップ情報を書き込み
	D3DLOCKED_RECT sourceRect;
	((LPDIRECT3DTEXTURE9)texture)->LockRect(0, &sourceRect, NULL, 0);
	DWORD *pSourceBuf = (DWORD*)sourceRect.pBits;

	D3DLOCKED_RECT lockedRect;
	StageData[idxy * 2 + idxx]->LockRect(0, &lockedRect, NULL, 0);  // ロック
	DWORD *pBuf = (DWORD*)lockedRect.pBits;   // テクスチャメモリへのポインタ
	int texStartX = (useID % divX) * 32;
	int texStartY = (useID / divX) * 32;
	int startX = (indexX - idxx * (Width / 2)) * 32;
	int startY = (indexY - idxy * (Height / 2)) * 32;
	for (int y = 0; y < 32; y++) {
		for (int x = 0; x < 32; x++) {
			DWORD argb = pSourceBuf[(texStartY + y)*(32 * divX) + (texStartX + x)];
			if (argb >> 24 == 0x00)continue;
			pBuf[(startY + y)*(32 * Width / 2) + (startX + x)] = argb;
		}
	}

	((LPDIRECT3DTEXTURE9)texture)->UnlockRect(0);
	StageData[idxy * 2 + idxx]->UnlockRect(0);  // アンロック

}
void EndExportStageData(const char saveFileName[])
{
	char buff[100];
	sprintf(buff, "%s_0.png", saveFileName);
	D3DXSaveTextureToFile(buff, D3DXIFF_PNG, StageData[0], NULL);
	StageData[0]->Release();
	sprintf(buff, "%s_1.png", saveFileName);
	D3DXSaveTextureToFile(buff, D3DXIFF_PNG, StageData[1], NULL);
	StageData[1]->Release();
	sprintf(buff, "%s_2.png", saveFileName);
	D3DXSaveTextureToFile(buff, D3DXIFF_PNG, StageData[2], NULL);
	StageData[2]->Release();
	sprintf(buff, "%s_3.png", saveFileName);
	D3DXSaveTextureToFile(buff, D3DXIFF_PNG, StageData[3], NULL);
	StageData[3]->Release();
}
