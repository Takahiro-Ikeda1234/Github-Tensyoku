#include "DX9_glitch.h"
#include "DX9_core.h"
#include "Windows_core.h"
#include "Counter.h"
#include "CameraManager.h"

static const int NOISE_COUNT = 10;
#define NOISE_X (rand() % (Width - 20) + 10)	//ランダムなノイズの発生位置
#define NOISE_Y (rand() % (Height - 10) + 5)	//ランダムなノイズの発生位置
static int g_noiseRandx[NOISE_COUNT];
static int g_noiseRandy[NOISE_COUNT];

//グローバル変数
unsigned screenTex;

static void randomNoise()
{
	int Width = GetWindowWidth();
	int Height = GetWindowHeight();
	for (int i = 0; i < NOISE_COUNT; i++)
	{
		g_noiseRandx[i] = NOISE_X;
		g_noiseRandy[i] = NOISE_Y;
	}
}

void DrawGlitch(CameraManager* camera, float deltaTime)
{
	static Counter randomNoiseGenerateTime = 1.0f;
	randomNoiseGenerateTime -= deltaTime;
	if (randomNoiseGenerateTime.IsEnd())
	{
		randomNoise();
		randomNoiseGenerateTime = 1.0f;
	}


	LPDIRECT3DSURFACE9 backBuffer;
	GetD3DDevice()->GetBackBuffer(0, 0, D3DBACKBUFFER_TYPE_MONO, &backBuffer);
	D3DLOCKED_RECT lockedRect;
	HRESULT hr = backBuffer->LockRect(&lockedRect, 0, NULL);


	int Width = GetWindowWidth();
	int Height = GetWindowHeight();
	unsigned* pixel = ((unsigned*)lockedRect.pBits);
	if (camera->GetShakeX() > BLOCKSIZE / 2 && camera->GetShakeX() >= camera->GetShakeY() * 0.95f)
	{
		//モザイク化
		if (0.10f <= camera->GetShakeTime() && camera->GetShakeTime() <= 0.25f)
		{
			const int mos = 16;
			for (int y = Height - mos; y > -1; y -= mos)
			{
				for (int x = 0; x < Width; x += mos)
				{
					for (int _y = 0; _y < mos; _y++)
					{
						for (int _x = 0; _x < mos; _x++)
						{
							pixel[(y + _y) * Width + (x + _x)] = pixel[y * Width + x];
						}
					}
				}
			}
		}
		for (int i = 0; i < NOISE_COUNT; i++)
		{
			if (rand() % 2)
			{
				for (int j = 0; j < 2; j++)
				{
					if (g_noiseRandy[i] + j > Height - 1)continue;
					int y = (g_noiseRandy[i] + j);
					int x = g_noiseRandx[i] + j;
					unsigned* temp = new unsigned[Width];
					unsigned* p = pixel;

					memcpy(temp, &p[y * Width + x], (Width - (y)) * 3);
					memcpy(&p[y * Width + x], &p[y * Width], (x) * 3);
					memcpy(&p[y * Width], temp, (Width - (x)) * 3);
				}
			}
			else
			{
				for (int j = 0; j < 2; j++)
				{
					if (g_noiseRandy[i] + j > Height - 1)continue;
					int y = (g_noiseRandy[i] + j);
					int x = g_noiseRandx[i] + j;
					unsigned* temp = new unsigned[Width];
					unsigned* p = pixel;

					memcpy(temp, &p[y * Width], (x) * 3);
					memcpy(&p[y * Width], &p[y * Width + x], (Width - x) * 3);
					memcpy(&p[y * Width + Width - x], temp, (x) * 3);
				}
			}
		}

	}

	backBuffer->UnlockRect();
}
