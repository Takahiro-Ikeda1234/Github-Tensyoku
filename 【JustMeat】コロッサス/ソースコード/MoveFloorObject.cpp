#include "MoveFloorObject.h"
#include "Scene.h"
#include "StageManager.h"	//StageManager::ToPosition(indexX, indexY)に必要
#include "ComponentAnimationSprite.h"	// new ComponentAnimationSpriteに必要
#include "file.h"

bool MoveFloorObject::mAlreadyLoadFile;	//初回実行時のみ読み込む
MoveFloorObject::MoveWallData MoveFloorObject::mData[4][STAGE_MAX][MAX_OBJECT_CH];

MoveFloorObject::MoveFloorObject(Scene * scene, Vector2 pos, float xScale, float yScale, float xMove, float yMove, float moveTime, float stopTime)
	: StageObject(scene, pos)
	, mBaseMoveTime(moveTime)
	, mBaseStopTime(stopTime)
	, mMoveTime(moveTime)
	, mStopTime(stopTime)
	, mMoveSpeed(xMove * BLOCKSIZE / moveTime, yMove * BLOCKSIZE / moveTime)
{
	GetScene()->AddMoveFloor(this);
	mPrevPos = GetPosition();
	SetSize(Vector2(xScale, yScale) * BLOCKSIZE);
}

MoveFloorObject::MoveFloorObject(Scene * scene, int indexX, int indexY, float xScale, float yScale, float xMove, float yMove, float moveTime, float stopTime)
	: MoveFloorObject(scene, StageManager::ToPosition(indexX, indexY), xScale, yScale, xMove, yMove, moveTime, stopTime)
{
}

MoveFloorObject::~MoveFloorObject()
{
	GetScene()->RemoveMoveFloor(this);
}


Vector2 MoveFloorObject::GetMove()
{
	return GetPosition() - mPrevPos;
}

void MoveFloorObject::Assign(Object * obj)
{
	mObject.push_back(obj);
}

void MoveFloorObject::SaveCheckPointData()
{
	StageObject::SaveCheckPointData();
	mCPMoveTime = mMoveTime;
	mCPStopTime = mStopTime;
	mCPMoveSpeed = mMoveSpeed;
}

void MoveFloorObject::LoadCheckPointData()
{
	StageObject::LoadCheckPointData();
	mMoveTime = mCPMoveTime;
	mStopTime = mCPStopTime;
	mMoveSpeed = mCPMoveSpeed;
}


void MoveFloorObject::LoadMoveWallFile()
{
	if (mAlreadyLoadFile) return;
	mAlreadyLoadFile = true;
	std::vector<std::string> fileData;
	FILE* fp = fopen("resources/moveFloor.txt", "r");
	loadFileAllData(fp, &fileData);


	int row = fileData.size();
	for (int ct = 0; ct < row; ct++)
	{
		if (IsSplit(fileData[ct][0])) continue;

		int index = 0;
		int stageID = 0;
		int wallID = 0;
		int typeID = 0;

		std::string temp = NextData(&fileData[ct], &index);
		stageID = atoi(temp.c_str());
		temp = NextData(&fileData[ct], &index);
		wallID = atoi(temp.c_str());
		temp = NextData(&fileData[ct], &index);
		typeID = atoi(temp.c_str());
		temp = NextData(&fileData[ct], &index);

		mData[typeID][stageID][wallID].xMove = atoi(temp.c_str());
		temp = NextData(&fileData[ct], &index);
		mData[typeID][stageID][wallID].yMove = atoi(temp.c_str());
		temp = NextData(&fileData[ct], &index);
		mData[typeID][stageID][wallID].moveTime = atoi(temp.c_str()) / 1000.0f;
		temp = NextData(&fileData[ct], &index);
		mData[typeID][stageID][wallID].stopTime = atoi(temp.c_str()) / 1000.0f;

		temp = NextData(&fileData[ct], &index);
		mData[typeID][stageID][wallID].xScale = atoi(temp.c_str());
		temp = NextData(&fileData[ct], &index);
		mData[typeID][stageID][wallID].yScale = atoi(temp.c_str());
		temp = NextData(&fileData[ct], &index);
		mData[typeID][stageID][wallID].texID = atoi(temp.c_str());
	}
	fclose(fp);
}

MoveFloorObject::MoveWallData * MoveFloorObject::GetMoveFloorData(int stageID, int id, int type)
{
	LoadMoveWallFile();
	return &mData[type][stageID][id];
}
