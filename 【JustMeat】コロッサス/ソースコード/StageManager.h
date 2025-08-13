#pragma once
#include"AbstractObject.h"
#include"Manager.h"
#include"Vector2.h"
static const int STAGE_MAX = 9;
class StageManager : public Manager
{
public:
	enum ObjectLayer
	{
		eStageObjectLayerBlock = 0,
		eStageObjectLayerWall,
		eStageObjectLayerLayerMax,
	};
	StageManager(Scene* scene);
	virtual ~StageManager();
	virtual void Update(float deltaTime) override;
	void SetStageData(ObjectLayer layer, int id, int indexX, int indexY) { if (indexX >= mWidth || indexY >= mHeight) { return; }mStageData[layer][indexY * mWidth + indexX] = id; }
	int GetStageData(ObjectLayer layer, int indexX, int indexY);
	void SetGolemStageData(ObjectLayer layer, int id, int indexX, int indexY) { mGolemStageData[layer][indexY * mWidth + indexX] = id; }
	void SetBuildableStageData(int id, int indexX, int indexY) { mGolemBuildableStageData[indexY * mWidth + indexX] = id; }
	int GetBuildableStageData(int indexX, int indexY) { return mGolemBuildableStageData[indexY * mWidth + indexX]; }
	void SetGolemAttackZone(int id, int indexX, int indexY) { mGolemAttackZone[indexY * mWidth + indexX] = id; }
	int GetGolemAttackZone(int indexX, int indexY) { LimitIndex(&indexX, &indexY); return mGolemAttackZone[indexY * mWidth + indexX]; }
	void SetMessageZone(int id, int indexX, int indexY) { mMessageZone[indexY * mWidth + indexX] = id; }
	int GetMessageZone(int indexX, int indexY) { LimitIndex(&indexX, &indexY); return mMessageZone[indexY * mWidth + indexX]; }
	void SetCheckpointZone(int id, int indexX, int indexY) { mCheckpointZone[indexY * mWidth + indexX] = id; }
	int GetCheckpointZone(int indexX, int indexY) { LimitIndex(&indexX, &indexY); return mCheckpointZone[indexY * mWidth + indexX]; }
	void SetCheckpointRestartIndex(int id, int indexX, int indexY) { mCheckpointRestartPoint[id][0] = indexX; mCheckpointRestartPoint[id][1] = indexY; }
	void GetCheckpointRestartIndex(int id, int* indexX, int* indexY) { *indexX = mCheckpointRestartPoint[id][0]; *indexY = mCheckpointRestartPoint[id][1]; }
	
	void LimitIndex(int* indexX, int* indexY) { if (*indexX < 0) *indexX = 0; if (*indexX > mWidth - 1) *indexX = mWidth - 1; if (*indexY < 0) *indexY = 0; if (*indexY > mHeight - 1) *indexY = mHeight - 1; }
	static Vector2 ToPosition(int indexX, int indexY) { return Vector2(ToPosX(indexX), ToPosY(indexY)); }
	static Vector2 AdjustPos(Vector2 pos);
	static int ToIndexX(float xPos) { return (xPos) / BLOCKSIZE; }
	static int ToIndexY(float yPos) { return (yPos) / BLOCKSIZE; }
	static float ToPosX(int indexX) { return indexX * BLOCKSIZE + BLOCKSIZE / 2; }
	static float ToPosY(int indexY) { return indexY * BLOCKSIZE + BLOCKSIZE / 2; }
	float LimitX();
	float LimitY();
	void SetWidth(int width) { mWidth = width; }
	void SetHeight(int height) { mHeight = height; }
	int GetWidth() { return mWidth; }
	int GetHeight() { return mHeight; }
	void LoadStage(int stageID);
	void SetStageSize(int width, int height);
	void CreateObject(int id, int indexX, int indexY, int propDrawLayer = 0, int ch = -1);
	int GetCurrentStageID() { return mCurrentStageID; }
private:
	void ZeroStageData();
	int mCurrentStageID;
	int mWidth;	//ステージ幅
	int mHeight;	//ステージ高さ
	int* mStageData[eStageObjectLayerLayerMax];
	int* mGolemStageData[eStageObjectLayerLayerMax];
	int* mGolemBuildableStageData;
	int* mGolemAttackZone;
	int* mMessageZone;
	int* mCheckpointZone;
	int mCheckpointRestartPoint[MAX_OBJECT_CH][2];	//indexX,Y
};