//=============================================================================
//
// [mytime.h] ���ԏ���
//
//=============================================================================
#ifndef _MYTIME_H_
#define _MYTIME_H_

//�C���N���[�h�t�@�C��
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