#include "StageObject.h"
#include"obj_golemEvent.h"
#include"obj_AbstractGolemArm.h"
StageObject::StageObject(Scene * scene, Vector2 pos)
	: Object(scene, pos)
{
}

StageObject::StageObject(Scene * scene, int indexX, int indexY)
	: StageObject(scene, StageManager::ToPosition(indexX, indexY))
{
}

void StageObject::SaveCheckPointData()
{
	mCPDone = mDone;

	mCPRotationDeg = GetRotation();	//回転角度
	mCPPosition = GetPosition();	//現在位置
	mCPState = GetState();			//現在の状態
	mCPSize = GetSize();		//外見サイズ
	mCPType = GetObjectType();
}

void StageObject::LoadCheckPointData()
{
	mDone = mCPDone;

	SetRotation(mCPRotationDeg);
	SetPosition(mCPPosition);
	SetState(mCPState);
	SetSize(mCPSize);
	SetObjectType(mCPType);
}

void StageObject::UpdateObject(float deltaTime)
{
	if (!mDone)
	{
		mEnterGolemArm = false;
		mExitGolemArm = false;
		mHitGolemArm = GetScene()->GetGolem()->HitGolemArm(GetPosition(), GetColSize());
		if (mHitGolemArm != nullptr)
		{
			if (!mOnGolemArm) mEnterGolemArm = true;
			mOnGolemArm = true;
		}
		else
		{
			if (mOnGolemArm)
			{
				mOnGolemArm = false;
				mExitGolemArm = true;
			}
		}

		if (mEnterGolemArm)EnterGolemArm();
		if (mOnGolemArm)OnGolemArm();
		if (mExitGolemArm)ExitGolemArm();
	}
	UpdateStageObject(deltaTime);
}
