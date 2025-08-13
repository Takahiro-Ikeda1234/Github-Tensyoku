
//=============================================================================
//
// [mytime.cpp] 時間処理
//
//=============================================================================

//インクルードファイル
#define NOMINMAX
#include <Windows.h>
#include <time.h>
#include "mytime.h"
#include "ini.h"
static const float DELTATIME_LIMIT = 1000 / 30.0f;	//ミリ秒

MyTime::MyTime()
	: mExecLastTime(0)
	, mFPSLastTime(0)
	, mCurrentTime(0)
	, mFrameCount(0)
	, mDeltaTime(0)
	, mFPS(0)
	, mFPSCount(0)
	, Singleton()
{

	timeBeginPeriod(1);					// 分解能を設定
	mCurrentTime = mExecLastTime = mFPSLastTime = timeGetTime();	// システム時刻をミリ秒単位で取得
	mFrameCount = mDeltaTime = 0;
	mFPS = GetConfigInt("System", "FPS");
	if (mFPS < 30) mFPS = 30;
	if (mFPS > 60) mFPS = 60;
}

void MyTime::Update()
{
	while (1)
	{
		//FPSの計測
		if (mCurrentTime - mFPSLastTime >= 500) // 500m/s毎にチェックしてる
		{
			mFPSCount = mFrameCount * 1000.0f / (mCurrentTime - mFPSLastTime);
			mFPSLastTime = mCurrentTime;
			mFrameCount = 0;
		}

		if ((mCurrentTime - mExecLastTime) >= (1000 / mFPS))	// 1/60秒ごとに実行
		{
			mDeltaTime = mCurrentTime - mExecLastTime;
			mExecLastTime = mCurrentTime;	// 処理した時刻を保存
			mFrameCount++;		// 処理回数のカウントを加算

			break;
		}
		mCurrentTime = timeGetTime();					// システム時刻を取得

	}
}

float MyTime::GetDeltaTime()
{
	if (mDeltaTime > DELTATIME_LIMIT)
	{
		mDeltaTime = DELTATIME_LIMIT;
	}
	return mDeltaTime / 1000.0f;
}

float MyTime::GetFPSCount()
{
	return mFPSCount;
}
