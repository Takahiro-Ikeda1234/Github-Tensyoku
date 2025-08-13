#include"DX9_core.h"
#include"DX9_printf.h"
#include"DX9_input.h"
#include"Windows_core.h"
static LPDIRECT3D9 gD3D;
static LPDIRECT3DDEVICE9 gD3DDevice;
static bool InitD3DDevice9();

LPDIRECT3DDEVICE9 GetD3DDevice()
{
	return gD3DDevice;
}

bool InitDX9(HINSTANCE hInst, int nCmdShow)
{
	if (InitWINAPI(hInst, nCmdShow) == NULL) return true;
	if (InitD3DDevice9()) return true;
	if (InitInputDX9(hInst)) return true;
	InitDX9printf();

	return false;
}

bool InitD3DDevice9()
{
	HWND hWnd = GetHWND();

	//D3Dインターフェースの取得
	gD3D = Direct3DCreate9(D3D_SDK_VERSION);
	if (gD3D == NULL)
	{
		MessageErrorOK("Direct3Dインターフェースの取得に失敗しました");
		return true;
	}

	D3DPRESENT_PARAMETERS d3dpp = {};
	d3dpp.BackBufferWidth = GetWindowWidth();		//バックバッファの横幅
	d3dpp.BackBufferHeight = GetWindowHeight();		//バックバッファの高さ
	d3dpp.BackBufferFormat = D3DFMT_UNKNOWN;	//バックバッファのフォーマット
	d3dpp.BackBufferCount = 1;					//バックバッファの数
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;	//スワップ方法
	d3dpp.Windowed = TRUE;						//ウィンドウモード設定
	d3dpp.EnableAutoDepthStencil = TRUE;		//深度バッファ・ステンシルバッファの使用
	d3dpp.AutoDepthStencilFormat = D3DFMT_D24S8;	//深度バッファ・ステンシルバッファのフォーマット
	d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;//フルスクリーン時のリフレッシュレート設定
	d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_DEFAULT;//リフレッシュレートとPresentの関係
	d3dpp.Flags = D3DPRESENTFLAG_LOCKABLE_BACKBUFFER;

	// Direct3Dデバイスの取得
	if (FAILED(gD3D->CreateDevice( D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd, D3DCREATE_HARDWARE_VERTEXPROCESSING, &d3dpp, &gD3DDevice)))
	{
		// デバイスの作成に失敗
		MessageErrorOK("Direct3Dデバイスの作成に失敗しました");
		return true;
	}

	// アドレッシングモードの設定
	// ボーダー（指定色で塗りつぶす）
	gD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_BORDER);
	gD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_BORDER);
	gD3DDevice->SetSamplerState(0, D3DSAMP_BORDERCOLOR, D3DCOLOR_RGBA(255, 0, 255, 0));

	// フィルタリングの設定
	// ポイントフィルタ
	// 縮小方向のフィルタ方法を指定
	gD3DDevice->SetSamplerState(0,D3DSAMP_MINFILTER, D3DTEXF_POINT);
	// 拡大方向のフィルタ方法を指定
	gD3DDevice->SetSamplerState(0,D3DSAMP_MAGFILTER, D3DTEXF_POINT);
	gD3DDevice->SetSamplerState(0,D3DSAMP_MIPFILTER, D3DTEXF_NONE);

		// アルファブレンド設定
	gD3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE); // アルファブレンドを有効にする
	gD3DDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	gD3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	gD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
	gD3DDevice->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
	gD3DDevice->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);

	return false;
}

void UninitDX9()
{
	UninitDX9printf();
	UninitInputDX9();
	//デバイスの解放処理
	if (gD3DDevice != NULL)
	{
		gD3DDevice->Release();
		gD3DDevice = NULL;
	}

	//D3Dの解放処理
	if (gD3D != NULL)
	{
		gD3D->Release();
		gD3D = NULL;
	}
	UninitWINAPI();
}

bool GameLoopDX9()
{
	return GameLoopWINAPI();
}
