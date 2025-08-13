#pragma once
#include"Object.h"

#include "StageManager.h"
#include "ComponentAnimationSprite.h"

class StageObject : public Object
{
public:
	StageObject(Scene* scene, Vector2 pos);
	StageObject(Scene* scene, int indexX, int indexY);
	~StageObject() {}
	virtual void SaveCheckPointData() override;
	virtual void LoadCheckPointData() override;
	virtual void UpdateObject(float deltaTime) final;
	virtual void UpdateStageObject(float deltaTime) = 0;
	void Completed() { mDone = true; }	//ギミックが終了したらこれを実行する(崩れた状態などで固定される)
	virtual void EnterGolemArm() {}
	virtual void OnGolemArm() {}
	virtual void ExitGolemArm() {}
	bool IsEnterGolemArm() { if (mDone) return false; return mEnterGolemArm; }
	bool IsOnGolemArm() { if (mDone) return false; return mOnGolemArm; }
	bool IsExitGolemArm() { if (mDone) return false; return mExitGolemArm; }

	class AbstractGolemArm* GetHitGolemArm() { return mHitGolemArm; }
private:
	bool mEnterGolemArm = false;
	bool mOnGolemArm = false;
	bool mExitGolemArm = false;
	class AbstractGolemArm* mHitGolemArm;

	bool mDone = false;	//trueならチェックポイントによるロードを行わない

	//チェックポイント時のデータ
	bool mCPDone = false;
	float mCPRotationDeg;	//回転角度
	Vector2 mCPPosition;	//現在位置
	State mCPState;			//現在の状態
	Vector2 mCPSize;		//サイズ
	Type mCPType;
};