//=============================================================================
//
// [sound.cpp] 音楽処理
//
//=============================================================================
//インクルードファイル
#include<math.h>
#include"Windows_core.h"
#include<stdio.h>

#include "DX9_sound.h"
#include"DX9_printf.h"
#include<windows.h>
#include <mmsystem.h>
#include <dsound.h>
#pragma comment(lib, "dsound.lib")
#pragma comment(lib, "dxguid.lib")
#pragma comment(lib, "winmm.lib")

static const char musicpath[] = "resources/audio/music";
static const char soundpath[] = "resources/audio/sound";

//グローバル変数
IDirectSound8* DS8;	//サウンドデバイス
int se_ch_idx;	//効果音を生成する配列位置
BGM *bgm;	//音配列のポインタ
static bool canControlLoop = false;
static bool uninitSound = false;

//プロトタイプ宣言
static void PlaySnd(const char* filename, bool repeat, int ch, bool isLink, int linkCh, bool isBGM);
static bool CreateSound(IDirectSoundBuffer8 **DSB, const char filename[]);
static float toDB(float nPer);


BGM::BGM()
	: mSound(nullptr)
	, mVolume(100)
	, mRestartDelay(0)
	, mSetVolumeDelay(0)
	, mSetVolume(0)
	, mFadeinDelay(0)
	, mFadeinVolume(0)
	, mNowFadein(false)
	, mFadeoutDelay(0)
	, mFadeoutVolume(0)
	, mNowFadeout(false)
	, mPrimaryVolumePercentage(1.0f)
	, mActiveLinkCh(0)
{
	for (int i = 0; i < MAX_LINK; i++)
	{
		mLinkSound[i] = nullptr;

		mLinkFadeinDelay[i] = 0;
		mLinkFadeinTime[i] = 0;
		mNowLinkFadein[i] = false;

		mLinkFadeoutDelay[i] = 0;
		mLinkFadeoutTime[i] = 0;
		mNowLinkFadeout[i] = false;
	}
}
BGM::~BGM()
{
	mIsActive = false;
	if (mSound)
	{
		auto temp = mSound;
		mSound = nullptr;
		temp->Release();
	}
	for (int i = 0; i < MAX_LINK; i++)
	{
		if (mLinkSound[i])
		{
			auto temp = mLinkSound[i];
			mLinkSound[i] = nullptr;
			temp->Release();
		}
	}
}
void BGM::SetPosition(unsigned pos)
{
	if (mSound)
	{
		mSound->SetCurrentPosition(pos);
	}
	for (int i = 0; i < MAX_LINK; i++)
	{
		if (mLinkSound[i])
		{
			mLinkSound[i]->SetCurrentPosition(pos);
		}
	}
}
unsigned BGM::GetPosition()
{
	if (mSound)
	{
		DWORD pos;
		DWORD noUse;
		mSound->GetCurrentPosition(&pos, &noUse);
		return pos;
	}
	return 0;
}
void BGM::Reset()
{
	mVolume = 100;
	mRestartDelay = 0;
	mSetVolumeDelay = 0;
	mSetVolume = 0;

	mFadeinDelay = 0;
	mFadeinVolume = 0;
	mNowFadein = false;

	mFadeoutDelay = 0;
	mFadeoutVolume = 0;
	mNowFadeout = false;

	mPrimaryVolumePercentage = 1.0f;
	mIsActive = false;
	if (mSound)
	{
		auto temp = mSound;
		mSound = nullptr;
		temp->Release();
	}

	for (int i = 0; i < MAX_LINK; i++)
	{
		mLinkFadeinDelay[i] = 0;
		mLinkFadeinTime[i] = 0;
		mNowLinkFadein[i] = false;

		mLinkFadeoutDelay[i] = 0;
		mLinkFadeoutTime[i] = 0;
		mNowLinkFadeout[i] = false;
		if (mLinkSound[i])
		{
			auto temp = mLinkSound[i];
			mLinkSound[i] = nullptr;
			temp->Release();
		}
	}
}
void BGM::SetBGM(const char* filename)
{
	CreateSound(&mSound, filename);
}
void BGM::SetLinkBGM(const char* filename, int linkch)
{
	CreateSound(&mLinkSound[linkch], filename);
}
void BGM::Play(bool repeat)
{
	if (mSound)
	{
		DS8->SetCooperativeLevel(GetHWND(), DSSCL_NORMAL);
		mSound->Play(0, 0, repeat);
	}
	for (int i = 0; i < MAX_LINK; i++)
	{
		if (mLinkSound[i])
		{
			DS8->SetCooperativeLevel(GetHWND(), DSSCL_NORMAL);
			mLinkSound[i]->Play(0, 0, repeat);
		}
	}
	SetVolume(mVolume, 0);
	mIsActive = true;
}
void BGM::Stop()
{
	mIsActive = false;
	if (mSound)
	{
		mSound->Stop();
	}
	for (int i = 0; i < MAX_LINK; i++)
	{
		if (mLinkSound[i])
		{
			mLinkSound[i]->Stop();
		}
	}
	//Reset();
}
void BGM::ChangeVolume(float volume)
{
	mVolume = volume;
	float primaryVolume = volume * mPrimaryVolumePercentage;
	float linkVolume = volume * (1 - mPrimaryVolumePercentage);
	//primaryVolume *= (1 - sinf(((100 - primaryVolume) / 100) * 90 * 3.14f / 180));
	//linkVolume *= (1 - sinf(((100 - linkVolume) / 100) * 90 * 3.14f / 180));
	if (mSound)
	{
		long vol = primaryVolume <= 0 ?
			DSBVOLUME_MIN : (long)(1000.0f * log10(primaryVolume / 100.0f));

		vol = primaryVolume <= 0 ? DSBVOLUME_MIN : (long)(2000 * log10(primaryVolume / 100));

		vol = toDB(primaryVolume);
		//DX9printf(5, 5, "%d", vol);

		if (vol < DSBVOLUME_MIN) vol = DSBVOLUME_MIN;
		if (vol > DSBVOLUME_MAX) vol = DSBVOLUME_MAX;
		mSound->SetVolume(vol);
	}

	for (int i = 0; i < MAX_LINK; i++)
	{
		if (i != mActiveLinkCh)
		{
			if (mLinkSound[i])
			{
				mLinkSound[i]->SetVolume(-10000);
			}
		}
	}
	if (mLinkSound[mActiveLinkCh])
	{
		long vol = linkVolume <= 0 ?
			DSBVOLUME_MIN : (long)(1000.0f * log10(linkVolume / 100.0f));

		if (vol < DSBVOLUME_MIN) vol = DSBVOLUME_MIN;
		if (vol > DSBVOLUME_MAX) vol = DSBVOLUME_MAX;
		mLinkSound[mActiveLinkCh]->SetVolume(vol);
	}
}

void BGM::Update(float deltaTime)
{
	if (!mSound)return;
	if (mSetVolumeDelay.IsFinish()) ChangeVolume(mSetVolume);
	if (mRestartDelay.IsFinish()) SetPosition(0);

	if (mFadeinDelay.IsFinish())
	{
		ChangeVolume(0);
		mNowFadein = true;
	}
	if (mFadeoutDelay.IsFinish()) mNowFadeout = true;
	if (mLinkFadeinDelay[mActiveLinkCh].IsFinish())mNowLinkFadein[mActiveLinkCh] = true;
	if (mLinkFadeoutDelay[mActiveLinkCh].IsFinish())mNowLinkFadeout[mActiveLinkCh] = true;

	if (mNowFadein)
	{
		ChangeVolume(mVolume + mFadeinVolume * deltaTime);
		if (mVolume >= 100)
		{
			mVolume = 100;
			mNowFadein = false;
		}
	}
	if (mNowFadeout)
	{
		ChangeVolume(mVolume - mFadeoutVolume * deltaTime);
		if (mVolume <= 0)
		{
			mVolume = 0;
			mNowFadeout = false;
			Stop();
		}
	}

	if (mNowLinkFadein[mActiveLinkCh])
	{
		mPrimaryVolumePercentage -= (1 / mLinkFadeinTime[mActiveLinkCh])*deltaTime;
		if (mPrimaryVolumePercentage <= 0)
		{
			mPrimaryVolumePercentage = 0;
			mNowLinkFadein[mActiveLinkCh] = false;
		}
		ChangeVolume(mVolume);
	}
	if (mNowLinkFadeout[mActiveLinkCh])
	{
		mPrimaryVolumePercentage += (1 / mLinkFadeoutTime[mActiveLinkCh])*deltaTime;
		if (mPrimaryVolumePercentage >= 1)
		{
			mPrimaryVolumePercentage = 1;
			mNowLinkFadeout[mActiveLinkCh] = false;
		}
		ChangeVolume(mVolume);
	}

	mRestartDelay -= deltaTime;
	mSetVolumeDelay -= deltaTime;
	mFadeinDelay -= deltaTime;
	mFadeoutDelay -= deltaTime;
	for (int i = 0; i < MAX_LINK; i++)
	{
		mLinkFadeinDelay[i] -= deltaTime;
		mLinkFadeoutDelay[i] -= deltaTime;
	}

	//int a = GetPosition();
	//if (a > 7337120 * 4)
	//{
	//	SetPosition(a - 7337120 * 4 + 1069309 * 4);
	//}
}



void InitSound()
{
	// COMの初期化
	CoInitialize(NULL);

	HRESULT hr = DirectSoundCreate8(NULL, &DS8, NULL);
	if (FAILED(hr)) return;

	se_ch_idx = 0;
	bgm = new BGM[BGM_CH_NUM + SE_CH_NUM];
	canControlLoop = true;
}



void UninitSound()
{
	canControlLoop = false;
	uninitSound = true;
	delete[] bgm;


	if (DS8)
	{
		DS8->Release();
		DS8 = NULL;
	}

	// COMの終了
	CoUninitialize();
}


void UpdateSound(float deltaTime)
{

	for (int i = 0; i < BGM_CH_NUM + SE_CH_NUM; i++)
	{
		bgm[i].Update(deltaTime);
	}
}

bool CreateSound(IDirectSoundBuffer8 **DSB, const char filename[])
{

	if (*DSB) { (*DSB)->Release(); }

	HRESULT hr;
	WAVEFORMATEX wfex;


	DSBUFFERDESC DSBufferDesc = {};
	IDirectSoundBuffer* ptmpBuf = 0;

	HMMIO hMmio = NULL;
	MMIOINFO mmioInfo;

	// Waveファイルオープン
	memset(&mmioInfo, 0, sizeof(MMIOINFO));
	hMmio = mmioOpen((LPSTR)filename, &mmioInfo, MMIO_READ);
	if (!hMmio) return false; // ファイルオープン失敗

	// RIFFチャンク検索
	MMRESULT mmRes;
	MMCKINFO riffChunk;
	riffChunk.fccType = mmioFOURCC('W', 'A', 'V', 'E');
	mmRes = mmioDescend(hMmio, &riffChunk, NULL, MMIO_FINDRIFF);
	if (mmRes != MMSYSERR_NOERROR) {
		mmioClose(hMmio, 0);
		return false;
	}

	// フォーマットチャンク検索
	MMCKINFO formatChunk;
	formatChunk.ckid = mmioFOURCC('f', 'm', 't', ' ');
	mmRes = mmioDescend(hMmio, &formatChunk, &riffChunk, MMIO_FINDCHUNK);
	if (mmRes != MMSYSERR_NOERROR) {
		mmioClose(hMmio, 0);
		return false;
	}

	DWORD fmsize = formatChunk.cksize;
	DWORD size = mmioRead(hMmio, (HPSTR)&wfex, fmsize);
	if (size != fmsize) {
		mmioClose(hMmio, 0);
		return false;
	}

	mmioAscend(hMmio, &formatChunk, 0);

	// データチャンク検索
	MMCKINFO dataChunk;
	dataChunk.ckid = mmioFOURCC('d', 'a', 't', 'a');
	mmRes = mmioDescend(hMmio, &dataChunk, &riffChunk, MMIO_FINDCHUNK);
	if (mmRes != MMSYSERR_NOERROR) {
		mmioClose(hMmio, 0);
		return false;
	}

	char* pData = new char[dataChunk.cksize];
	size = mmioRead(hMmio, (HPSTR)pData, dataChunk.cksize);
	if (size != dataChunk.cksize) {
		delete[] pData;
		return false;
	}

	mmioClose(hMmio, 0);

	DSBufferDesc.dwSize = sizeof(DSBufferDesc);
	DSBufferDesc.dwBufferBytes = dataChunk.cksize;
	DSBufferDesc.lpwfxFormat = &wfex;
	DSBufferDesc.guid3DAlgorithm = GUID_NULL;
	DSBufferDesc.dwFlags = DSBCAPS_CTRLVOLUME | DSBCAPS_GLOBALFOCUS;
	DS8->CreateSoundBuffer(&DSBufferDesc, &ptmpBuf, NULL);
	ptmpBuf->QueryInterface(IID_IDirectSoundBuffer8, (void**)DSB);
	ptmpBuf->Release();

	// セカンダリバッファにWaveデータ書き込み
	LPVOID lpvWrite = 0;
	DWORD dwLength = 0;
	if (DS_OK == (*DSB)->Lock(0, 0, &lpvWrite, &dwLength, NULL, NULL, DSBLOCK_ENTIREBUFFER)) {
		memcpy(lpvWrite, pData, dwLength);
		(*DSB)->Unlock(lpvWrite, dwLength, NULL, 0);
		delete[] pData;
	}
	else
	{
		return false;
	}

	return true;
}

void PlaySnd(const char* filename, bool repeat, int ch, bool isLink, int linkCh, bool isBGM)
{
	char filepath[MAX_PATH];
	if (isBGM)
	{
		sprintf(filepath, "%s/%s.wav", musicpath, filename);
	}
	else
	{
		sprintf(filepath, "%s/%s.wav", soundpath, filename);
	}


	if (isBGM)
	{
		if (!isLink)
		{
			int s, e;
			MakeLoopPoint(filepath, &s, &e);
			bgm[ch].SetLoopPoint(s, e);
			bgm[ch].SetBGM(filepath);
			bgm[ch].Play(repeat);
		}
		else
		{
			bgm[ch].SetLinkBGM(filepath, linkCh);
			bgm[ch].Play(repeat);
		}
	}
	else
	{
		bgm[BGM_CH_NUM + se_ch_idx].SetBGM(filepath);
		bgm[BGM_CH_NUM + se_ch_idx].Play(repeat);
		se_ch_idx = (se_ch_idx + 1) % SE_CH_NUM;
	}
}
void PlayBGM(const char filename[], int ch, int volume, bool loop)
{
	PlaySnd(filename, loop, ch, false, 0, true);
	SetVolumeBGM(ch, volume, 0);
}

void SetLinkBGM(const char filename[], int ch, int volume, int linkCh, bool loop)
{
	PlaySnd(filename, loop, ch, true, linkCh, true);
	SetVolumeBGM(ch, volume, 0);
	SetSoundPosition(0, 0);
}

void PlaySE(const char filename[])
{
	PlaySnd(filename, false, -1, false, -1, false);
}


void StopBGM(int ch)
{
	bgm[ch].Stop();
}

void StopSE()
{
	for (int i = 0; i < SE_CH_NUM; i++)
	{
		bgm[BGM_CH_NUM + i].Stop();
	}
}

void SetVolumeBGM(int ch, float volume, float delayFrame)
{
	bgm[ch].SetVolume(volume, delayFrame);
}


void SetFadein(int ch, float fadeinvolume, float delaytime)
{
	bgm[ch].SetVolume(0, 0);
	bgm[ch].SetFadein(fadeinvolume, delaytime);
}

void SetFadeout(int ch, float fadeoutvolume, float delaytime)
{
	bgm[ch].SetFadeout(fadeoutvolume, delaytime);
}

unsigned GetSoundPosition(int ch)
{
	return bgm[ch].GetPosition();
}

void SetSoundPosition(int ch, unsigned pos)
{
	bgm[ch].SetPosition(pos);
}

void SetLinkFadeout(int ch, float fadeoutTime, float delayTime, int linkCh)
{
	bgm[ch].SetLinkFadeout(fadeoutTime, delayTime, linkCh);
	bgm[ch].SetActiveLinkCh(linkCh);
}

void SetLinkFadein(int ch, float fadeinTime, float delayTime, int linkCh)
{
	bgm[ch].SetLinkFadein(fadeinTime, delayTime, linkCh);
	bgm[ch].SetActiveLinkCh(linkCh);
}

void SetPrimaryVolumePercentage(int ch, float percentage)
{
	bgm[ch].SetPrimaryVolumePercentage(percentage);
}

void SetRestartBGM(int ch, float delayTime)
{
	bgm[ch].SetRestart(delayTime);
}

void SetSEFadein(float fadeinvolume, float delayTime)
{
	for (int i = 0; i < SE_CH_NUM; i++)
	{
		bgm[BGM_CH_NUM + i].SetFadein(fadeinvolume, delayTime);
	}
}

void SetSEFadeout(float fadeoutvolume, float delayTime)
{
	for (int i = 0; i < SE_CH_NUM; i++)
	{
		bgm[BGM_CH_NUM + i].SetFadeout(fadeoutvolume, delayTime);
	}
}

void ResetSE()
{
	for (int i = 0; i < SE_CH_NUM; i++)
	{
		bgm[BGM_CH_NUM + i].Reset();
	}
}

void ForceSetVolumeBGM()
{
	for (int i = 0; i < BGM_CH_NUM; i++)
	{
		bgm[i].SetVolume(0, 0);
	}
}

void ResetBGM()
{
	for (int i = 0; i < BGM_CH_NUM; i++)
	{
		bgm[i].Reset();
	}
}

float toDB(float nPer)
{
	float nDB_;
	if (nPer >= 50)
	{// 0 ～ -600
		nDB_ = -600 + 600 * (nPer - 50) / 50;
	}
	else if (nPer >= 25)
	{// -600 ～ -1200
		nDB_ = -1200 + 600 * (nPer - 25) / 25;
	}
	else if (nPer >= 12)
	{// -1200 ～ -1800
		nDB_ = -1800 + 600 * (nPer - 12) / 12;
	}
	else if (nPer >= 6)
	{// -1800 ～ -2400
		nDB_ = -2400 + 600 * (nPer - 6) / 6;
	}
	else if (nPer >= 3)
	{// -2400 ～ -3000
		nDB_ = -3000 + 600 * (nPer - 3) / 3;
	}
	else if (nPer >= 1)
	{// -3000～-3600
		nDB_ = -3600 + 600 * (nPer - 1);
	}
	else
	{
		nDB_ = DSBVOLUME_MIN;
	}
	return nDB_;
}


void BGMLoop()
{
	while (1)
	{
		if (uninitSound)return;
		if (canControlLoop)
		{
			canControlLoop = false;
			for (int i = 0; i < BGM_CH_NUM; i++)
			{
				if (!bgm[i].IsActive())continue;
				if (bgm[i].GetLoopEnd() == 0)continue;
				if (bgm[i].GetPosition() > bgm[i].GetLoopEnd() * 4)
				{
					bgm[i].SetPosition(bgm[i].GetPosition() - bgm[i].GetLoopEnd() * 4 + bgm[i].GetLoopStart() * 4);
				}
			}
			canControlLoop = true;
		}
	}
}

void MakeLoopPoint(const char* filename, int* start, int*end)
{
	FILE*fp = fopen(filename, "rb");
	fseek(fp, -40, SEEK_END);
	char buff[5] = "";
	fread(buff, sizeof(char), 4, fp);
	if (!strcmp(buff, "SAUR"))
	{
		fseek(fp, -20, SEEK_CUR);
		fread(start, sizeof(int), 1, fp);
		fread(end, sizeof(int), 1, fp);
	}
	else
	{
		*start = 0;
		*end = 0;
	}
}

