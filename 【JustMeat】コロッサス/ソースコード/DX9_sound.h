//=============================================================================
//
// [sound.h] 音楽処理
//
//=============================================================================
#ifndef _SOUND_H_
#define _SOUND_H_

//インクルードファイル
#include"Counter.h"
#include <string>

//定数定義
static const int BGM_CH_NUM = 6;	//BGM同時再生の上限
static const int SE_CH_NUM = 50;	//効果音同時再生の上限
static const int MAX_LINK = 1;	//BGMに連動させるBGMの上限

//クラス定義
class BGM
{
public:
	BGM();
	~BGM();
	void Play(bool repeat = true);
	void Stop();
	void Reset();
	void Update(float deltaTime);

	void SetBGM(const char* filename);
	void SetLinkBGM(const char* filename, int linkch);
	unsigned GetPosition();
	void SetPosition(unsigned pos);

	void SetRestart(float delay) { if (delay <= 0) { Play(); } else mRestartDelay = delay; }

	void SetVolume(float volume, float delay) { if (delay <= 0) { ChangeVolume(volume); }; mSetVolume = volume; mSetVolumeDelay = delay; }
	void SetFadein(float fadeinvolume, float delay) { mNowFadein = false; if (delay <= 0) { mNowFadein = true; ChangeVolume(0); } mFadeinVolume = fadeinvolume;  mFadeinDelay = delay; }
	void SetFadeout(float fadeoutvolume, float delay) { mNowFadeout = false;  if (delay <= 0) { mNowFadeout = true; }mFadeoutVolume = fadeoutvolume; mFadeoutDelay = delay; }

	void SetPrimaryVolumePercentage(float percentage) { mPrimaryVolumePercentage = percentage; ChangeVolume(mVolume); }
	void SetLinkFadein(float fadeinTime, float delay, int linkch) { mNowLinkFadein[linkch] = false;  if (delay <= 0) { mNowLinkFadein[linkch] = true; } mLinkFadeinTime[linkch] = fadeinTime; mLinkFadeinDelay[linkch] = delay; }
	void SetLinkFadeout(float fadeoutTime, float delay, int linkch) { mNowLinkFadeout[linkch] = false; if (delay <= 0) { mNowLinkFadeout[linkch] = true; } mLinkFadeoutTime[linkch] = fadeoutTime; mLinkFadeoutDelay[linkch] = delay; }

	void SetActiveLinkCh(int ch) { mActiveLinkCh = ch; ChangeVolume(mVolume); }
private:
	void ChangeVolume(float volume);
	class IDirectSoundBuffer8 *mSound;	//音源
	class IDirectSoundBuffer8 *mLinkSound[MAX_LINK];	//連動している音源

	Counter mRestartDelay;	//リスタートまでの遅延

	float mVolume;	//現在の音量
	Counter mSetVolumeDelay;	//音量設定までの遅延
	float mSetVolume;	//設定する音量値

	Counter mFadeinDelay;	//フェードインまでの遅延
	float mFadeinVolume;	//1秒あたりの増加音量
	bool mNowFadein;	//現在フェードイン中か

	Counter mFadeoutDelay;	//フェードアウトまでの遅延
	float mFadeoutVolume;	//1秒あたりの減少音量
	bool mNowFadeout;	//現在フェードアウト中か

	float mPrimaryVolumePercentage;	//メイン側の音量(0-1.0)
	int mActiveLinkCh;	//アクティブになっているLinkSound

	//(連動している曲用)
	Counter mLinkFadeinDelay[MAX_LINK];	//フェードインまでの遅延
	float mLinkFadeinTime[MAX_LINK];	//1秒あたりの増加音量
	bool mNowLinkFadein[MAX_LINK];	//現在フェードイン中か

	Counter mLinkFadeoutDelay[MAX_LINK];	//フェードアウトまでの遅延
	float mLinkFadeoutTime[MAX_LINK];	//1秒あたりの減少音量
	bool mNowLinkFadeout[MAX_LINK];	//現在フェードアウト中か

public:
	void SetLoopPoint(int start, int end) { mLoopEnd = end; mLoopStart = start; }
	int GetLoopStart() { return mLoopStart; }
	int GetLoopEnd() { return mLoopEnd; }
	bool IsActive() { return mIsActive; }
	std::string mFilename;
private:
	int mLoopStart;	//ループポイントの開始位置
	int mLoopEnd;
	bool mIsActive = false;
};

//プロトタイプ宣言
void InitSound();
void UninitSound();
void UpdateSound(float deltaTime);
void SetRestartBGM(int ch, float delayTime);

void PlayBGM(const char filename[], int ch, int volume, bool loop = true);
void SetLinkBGM(const char filename[], int ch, int volume, int linkCh, bool loop = true);
void PlaySE(const char filename[]);

void StopBGM(int ch);
void StopSE();
void SetVolumeBGM(int ch, float volume, float delayFrame = 0);
void SetFadein(int ch, float fadeinvolume, float delaytime = 0);
void SetFadeout(int ch, float fadeoutvolume, float delaytime);
unsigned GetSoundPosition(int ch);
void SetSoundPosition(int ch, unsigned pos);
void SetLinkFadeout(int ch, float fadeoutTime, float delayTime, int linkCh);
void SetLinkFadein(int ch, float fadeinTime, float delayTime, int linkCh);
void SetPrimaryVolumePercentage(int ch, float percentage);
void SetSEFadein(float fadeinvolume, float delayTime);
void SetSEFadeout(float fadeoutvolume, float delayTime);
void ResetSE();
void ResetBGM();
void ForceSetVolumeBGM();

void BGMLoop();
void MakeLoopPoint(const char* filename, int* start, int*end);


#endif