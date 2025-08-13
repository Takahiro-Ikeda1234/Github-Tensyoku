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
	//スローモーション
	void UpdateSlow(float deltaTime);
	float mCurrentSlowSpeed = 1;
	float mIncreaseSlowSpeedPerSecond = 0;
	Counter mSlowTime = 0;
	Counter mSlowResetTime = 0;

	void UpdateFade(float deltaTime);
	//フェードインアウト
	class Filter* mFade;
	class Filter* mFog;
	Counter mFadeinTime;
	Counter mFadeoutTime;
	Counter mFadeinDelay;
	Counter mFadeoutDelay;
	Counter mHitstopTime;	//ヒットストップ時間
	Counter mHitstopDelay;
	int mFadeMode = 0;	//-1 フェードアウト,0 通常,1 フェードイン

	//イベント発生範囲
public:
	static EventData GetCatEventArea(int stageID, int eventID) { return mCatEvent[stageID][eventID]; }
	static EventData GetGolemEventArea(int stageID, int eventID) { return mGolemEvent[stageID][eventID]; }
private:
	static void LoadEventFile();
	static EventData mCatEvent[STAGE_MAX][EV_MAX];	//ステージ数,イベント数
	static EventData mGolemEvent[STAGE_MAX][EV_MAX];	//ステージ数,イベント数
	static bool mAlreadyLoadEventFile;	//初回実行時のみ読み込む
};

bool InRect(CameraManager::Rect& rect, int x, int y);