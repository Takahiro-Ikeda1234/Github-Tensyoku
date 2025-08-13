#pragma once
#include"Manager.h"
#include"Counter.h"
#include<vector>
#include<Windows.h>
#include"StageManager.h"
#include"CameraManager.h"
static const int EV_MAX = 10;
typedef struct
{
	CameraManager::Rect area;
	int x;
	int y;
}EventData;
class EventManager : public Manager
{
public:
	EventManager(class Scene* scene);
	~EventManager() {}
	virtual void Update(float deltaTime) override;
	void Fadein(float time, float delay) { mFadeinTime = time; mFadeinDelay = delay; mFadeMode = 1; }
	void Fadeout(float time, float delay) { mFadeoutTime = time; mFadeoutDelay = delay; mFadeMode = -1; }
	bool IsFadeinEnd();
	bool IsFadeoutEnd();
	void SetSlowSpeed(float slowSpeed) { mCurrentSlowSpeed = slowSpeed; mSlowTime = 0; ; mIncreaseSlowSpeedPerSecond = 0; }
	void FadeSlow(float slowTime, float increaseSlowSpeedPerSecond, float resetTime = 0) { mSlowTime = slowTime; mIncreaseSlowSpeedPerSecond = increaseSlowSpeedPerSecond; mSlowResetTime = resetTime; }
	float GetSlow() { return mCurrentSlowSpeed; }
	bool IsHitstop() { if (!mHitstopTime.IsEnd() && mHitstopDelay.IsEnd())return true; return false; }
	void SetHitstop(float hitstopTime, float delay = 0) { mHitstopTime = hitstopTime; mHitstopDelay = delay; }
	void SetWhiteFade();
private:
	//�X���[���[�V����
	void UpdateSlow(float deltaTime);
	float mCurrentSlowSpeed = 1;
	float mIncreaseSlowSpeedPerSecond = 0;
	Counter mSlowTime = 0;
	Counter mSlowResetTime = 0;

	void UpdateFade(float deltaTime);
	//�t�F�[�h�C���A�E�g
	class Filter* mFade;
	class Filter* mFog;
	Counter mFadeinTime;
	Counter mFadeoutTime;
	Counter mFadeinDelay;
	Counter mFadeoutDelay;
	Counter mHitstopTime;	//�q�b�g�X�g�b�v����
	Counter mHitstopDelay;
	int mFadeMode = 0;	//-1 �t�F�[�h�A�E�g,0 �ʏ�,1 �t�F�[�h�C��

	//�C�x���g�����͈�
public:
	static EventData GetCatEventArea(int stageID, int eventID) { return mCatEvent[stageID][eventID]; }
	static EventData GetGolemEventArea(int stageID, int eventID) { return mGolemEvent[stageID][eventID]; }
private:
	static void LoadEventFile();
	static EventData mCatEvent[STAGE_MAX][EV_MAX];	//�X�e�[�W��,�C�x���g��
	static EventData mGolemEvent[STAGE_MAX][EV_MAX];	//�X�e�[�W��,�C�x���g��
	static bool mAlreadyLoadEventFile;	//������s���̂ݓǂݍ���
};

bool InRect(CameraManager::Rect& rect, int x, int y);