
//=============================================================================
//
// [mytime.cpp] ���ԏ���
//
//=============================================================================

//�C���N���[�h�t�@�C��
#define NOMINMAX
#include <Windows.h>
#include <time.h>
#include "mytime.h"
#include "ini.h"
static const float DELTATIME_LIMIT = 1000 / 30.0f;	//�~���b

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

	timeBeginPeriod(1);					// ����\��ݒ�
	mCurrentTime = mExecLastTime = mFPSLastTime = timeGetTime();	// �V�X�e���������~���b�P�ʂŎ擾
	mFrameCount = mDeltaTime = 0;
	mFPS = GetConfigInt("System", "FPS");
	if (mFPS < 30) mFPS = 30;
	if (mFPS > 60) mFPS = 60;
}

void MyTime::Update()
{
	while (1)
	{
		//FPS�̌v��
		if (mCurrentTime - mFPSLastTime >= 500) // 500m/s���Ƀ`�F�b�N���Ă�
		{
			mFPSCount = mFrameCount * 1000.0f / (mCurrentTime - mFPSLastTime);
			mFPSLastTime = mCurrentTime;
			mFrameCount = 0;
		}

		if ((mCurrentTime - mExecLastTime) >= (1000 / mFPS))	// 1/60�b���ƂɎ��s
		{
			mDeltaTime = mCurrentTime - mExecLastTime;
			mExecLastTime = mCurrentTime;	// ��������������ۑ�
			mFrameCount++;		// �����񐔂̃J�E���g�����Z

			break;
		}
		mCurrentTime = timeGetTime();					// �V�X�e���������擾

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
