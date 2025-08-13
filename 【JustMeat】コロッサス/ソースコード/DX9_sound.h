//=============================================================================
//
// [sound.h] ���y����
//
//=============================================================================
#ifndef _SOUND_H_
#define _SOUND_H_

//�C���N���[�h�t�@�C��
#include"Counter.h"
#include <string>

//�萔��`
static const int BGM_CH_NUM = 6;	//BGM�����Đ��̏��
static const int SE_CH_NUM = 50;	//���ʉ������Đ��̏��
static const int MAX_LINK = 1;	//BGM�ɘA��������BGM�̏��

//�N���X��`
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
	class IDirectSoundBuffer8 *mSound;	//����
	class IDirectSoundBuffer8 *mLinkSound[MAX_LINK];	//�A�����Ă��鉹��

	Counter mRestartDelay;	//���X�^�[�g�܂ł̒x��

	float mVolume;	//���݂̉���
	Counter mSetVolumeDelay;	//���ʐݒ�܂ł̒x��
	float mSetVolume;	//�ݒ肷�鉹�ʒl

	Counter mFadeinDelay;	//�t�F�[�h�C���܂ł̒x��
	float mFadeinVolume;	//1�b������̑�������
	bool mNowFadein;	//���݃t�F�[�h�C������

	Counter mFadeoutDelay;	//�t�F�[�h�A�E�g�܂ł̒x��
	float mFadeoutVolume;	//1�b������̌�������
	bool mNowFadeout;	//���݃t�F�[�h�A�E�g����

	float mPrimaryVolumePercentage;	//���C�����̉���(0-1.0)
	int mActiveLinkCh;	//�A�N�e�B�u�ɂȂ��Ă���LinkSound

	//(�A�����Ă���ȗp)
	Counter mLinkFadeinDelay[MAX_LINK];	//�t�F�[�h�C���܂ł̒x��
	float mLinkFadeinTime[MAX_LINK];	//1�b������̑�������
	bool mNowLinkFadein[MAX_LINK];	//���݃t�F�[�h�C������

	Counter mLinkFadeoutDelay[MAX_LINK];	//�t�F�[�h�A�E�g�܂ł̒x��
	float mLinkFadeoutTime[MAX_LINK];	//1�b������̌�������
	bool mNowLinkFadeout[MAX_LINK];	//���݃t�F�[�h�A�E�g����

public:
	void SetLoopPoint(int start, int end) { mLoopEnd = end; mLoopStart = start; }
	int GetLoopStart() { return mLoopStart; }
	int GetLoopEnd() { return mLoopEnd; }
	bool IsActive() { return mIsActive; }
	std::string mFilename;
private:
	int mLoopStart;	//���[�v�|�C���g�̊J�n�ʒu
	int mLoopEnd;
	bool mIsActive = false;
};

//�v���g�^�C�v�錾
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