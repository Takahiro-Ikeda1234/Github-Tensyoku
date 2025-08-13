//=============================================================================
//
// [mytime.h] 時間処理
//
//=============================================================================
#ifndef _MYTIME_H_
#define _MYTIME_H_

//インクルードファイル
#include"Singleton.h"
#include<Windows.h>
class MyTime : public Singleton<MyTime>
{
private:
	friend Singleton<MyTime>;
	MyTime();
public:
	void Update();
	float GetDeltaTime();
	float GetFPSCount();
private:
	DWORD mExecLastTime;
	DWORD mFPSLastTime;
	DWORD mCurrentTime;
	DWORD mFrameCount;
	DWORD mDeltaTime;
	float mFPS;
	float mFPSCount;
};

#endif