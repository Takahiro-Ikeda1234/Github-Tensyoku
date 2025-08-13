#pragma once
#include "StageObject.h"
#include "Counter.h"
#include <vector>
#include"StageManager.h"
class MoveFloorObject : public StageObject
{
public:
	MoveFloorObject(Scene* scene, Vector2 pos, float xScale, float yScale, float xMove, float yMove, float moveTime, float stopTime);
	MoveFloorObject(Scene* scene, int indexX, int indexY, float xScale, float yScale, float xMove, float yMove, float moveTime, float stopTime);
	virtual ~MoveFloorObject();
	virtual void UpdateStageObject(float deltaTime) override = 0;
	Vector2 GetMove();
	void Assign(Object* obj);
	virtual void SaveCheckPointData() override;
	virtual void LoadCheckPointData() override;
	bool IsPushBlock() { return mIsPushBlock; }
	void SetSpeed(Vector2 spd) { mMoveSpeed = spd; }
	bool IsGolemArm()const { return mIsGolemArm; }
protected:
	bool mIsGolemArm = false;
	bool mIsPushBlock = false;
	const float mBaseMoveTime;
	const float mBaseStopTime;

	Vector2 mMoveSpeed;
	Vector2 mPrevPos;
	Counter mMoveTime;
	Counter mStopTime;
	std::vector<Object*> mObject;

	Vector2 mCPMoveSpeed;
	Counter mCPMoveTime;
	Counter mCPStopTime;

	typedef struct
	{
		float xScale;
		float yScale;
		float xMove;
		float yMove;
		float moveTime;
		float stopTime;
		int texID;
	}MoveWallData;
	static void LoadMoveWallFile();
	MoveWallData* GetMoveFloorData(int stageID, int id, int type);
	static bool mAlreadyLoadFile;	//èââÒé¿çséûÇÃÇ›ì«Ç›çûÇﬁ
	static MoveWallData mData[4][STAGE_MAX][MAX_OBJECT_CH];
};