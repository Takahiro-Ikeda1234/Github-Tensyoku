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

	//D3D�C���^�[�t�F�[�X�̎擾
	gD3D = Direct3DCreate9(D3D_SDK_VERSION);
	if (gD3D == NULL)
	{
		MessageErrorOK("Direct3D�C���^�[�t�F�[�X�̎擾�Ɏ��s���܂���");
		return true;
	}

	D3DPRESENT_PARAMETERS d3dpp = {};
	d3dpp.BackBufferWidth = GetWindowWidth();		//�o�b�N�o�b�t�@�̉���
	d3dpp.BackBufferHeight = GetWindowHeight();		//�o�b�N�o�b�t�@�̍���
	d3dpp.BackBufferFormat = D3DFMT_UNKNOWN;	//�o�b�N�o�b�t�@�̃t�H�[�}�b�g
	d3dpp.BackBufferCount = 1;					//�o�b�N�o�b�t�@�̐�
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;	//�X���b�v���@
	d3dpp.Windowed = TRUE;						//�E�B���h�E���[�h�ݒ�
	d3dpp.EnableAutoDepthStencil = TRUE;		//�[�x�o�b�t�@�E�X�e���V���o�b�t�@�̎g�p
	d3dpp.AutoDepthStencilFormat = D3DFMT_D24S8;	//�[�x�o�b�t�@�E�X�e���V���o�b�t�@�̃t�H�[�}�b�g
	d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;//�t���X�N���[�����̃��t���b�V�����[�g�ݒ�
	d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_DEFAULT;//���t���b�V�����[�g��Present�̊֌W
	d3dpp.Flags = D3DPRESENTFLAG_LOCKABLE_BACKBUFFER;

	// Direct3D�f�o�C�X�̎擾
	if (FAILED(gD3D->CreateDevice( D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd, D3DCREATE_HARDWARE_VERTEXPROCESSING, &d3dpp, &gD3DDevice)))
	{
		// �f�o�C�X�̍쐬�Ɏ��s
		MessageErrorOK("Direct3D�f�o�C�X�̍쐬�Ɏ��s���܂���");
		return true;
	}

	// �A�h���b�V���O���[�h�̐ݒ�
	// �{�[�_�[�i�w��F�œh��Ԃ��j
	gD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_BORDER);
	gD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_BORDER);
	gD3DDevice->SetSamplerState(0, D3DSAMP_BORDERCOLOR, D3DCOLOR_RGBA(255, 0, 255, 0));

	// �t�B���^�����O�̐ݒ�
	// �|�C���g�t�B���^
	// �k�������̃t�B���^���@���w��
	gD3DDevice->SetSamplerState(0,D3DSAMP_MINFILTER, D3DTEXF_POINT);
	// �g������̃t�B���^���@���w��
	gD3DDevice->SetSamplerState(0,D3DSAMP_MAGFILTER, D3DTEXF_POINT);
	gD3DDevice->SetSamplerState(0,D3DSAMP_MIPFILTER, D3DTEXF_NONE);

		// �A���t�@�u�����h�ݒ�
	gD3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE); // �A���t�@�u�����h��L���ɂ���
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
	//�f�o�C�X�̉������
	if (gD3DDevice != NULL)
	{
		gD3DDevice->Release();
		gD3DDevice = NULL;
	}

	//D3D�̉������
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
