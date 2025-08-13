#include "StageManager.h"
#include"abstractObject.h"
#include"obj_player.h"
#include"obj_AbstractBlock.h"
#include"StageData.h"
#include"obj_prop.h"
#include"obj_wall.h"
#include"obj_AbstractForegroundWall.h"
#include"obj_caveEntrance.h"
#include"obj_wood.h"
#include"CloudManager.h"
#include"obj_moveBlock.h"
#include"obj_moveWall.h"
#include"obj_checkPoint.h"
#include"obj_warpGate.h"
#include"obj_moveWall.h"
#include"obj_moveBlock.h"
#include"obj_triggerButton.h"
#include"obj_triggeredMoveWall.h"
#include"obj_triggeredMoveBlock.h"
#include"obj_fragileBlock.h"
#include"obj_fragileFallRock.h"
#include"obj_pushBlock.h"
#include"obj_pushWall.h"
#include"obj_laser.h"
#include"obj_golemCore.h"
#include"obj_marker.h"
static void(*createObject[])(Scene*, int, int, int, int) = {
	[](Scene* scene, int x, int y, int drawPriority, int ch) {},
	[](Scene* scene, int x, int y, int drawPriority, int ch) { new AbstractBlock(scene, x, y, -1); },
	[](Scene* scene, int x, int y, int drawPriority, int ch) {},
	[](Scene* scene, int x, int y, int drawPriority, int ch) {},
	[](Scene* scene, int x, int y, int drawPriority, int ch) {},
	[](Scene* scene, int x, int y, int drawPriority, int ch) {},
	[](Scene* scene, int x, int y, int drawPriority, int ch) { new Player(scene, x, y); },
	[](Scene* scene, int x, int y, int drawPriority, int ch) { new Wall(scene, x, y); },
	[](Scene* scene, int x, int y, int drawPriority, int ch) { new TriggeredMoveWall(scene, x, y, ch); },
	[](Scene* scene, int x, int y, int drawPriority, int ch) { new TriggeredMoveBlock(scene, x, y, ch); },
	[](Scene* scene, int x, int y, int drawPriority, int ch) { new TriggerButton(scene, x, y, ch, 0);  },
	[](Scene* scene, int x, int y, int drawPriority, int ch) { new TriggerButton(scene, x, y, ch, 1);  },
	[](Scene* scene, int x, int y, int drawPriority, int ch) { new CloudGeneratorDummy(scene, x, y, true); },
	[](Scene* scene, int x, int y, int drawPriority, int ch) { new CloudGeneratorDummy(scene, x, y, false); },
	[](Scene* scene, int x, int y, int drawPriority, int ch) {},
	[](Scene* scene, int x, int y, int drawPriority, int ch) { new Prop(scene, x, y, drawPriority, ID_PROP_WALL); },
	[](Scene* scene, int x, int y, int drawPriority, int ch) { new FragileBlock(scene, x, y, 0); },
	[](Scene* scene, int x, int y, int drawPriority, int ch) { new PushBlock(scene, x, y); },
	[](Scene* scene, int x, int y, int drawPriority, int ch) { new PushWall(scene, x, y); },
	[](Scene* scene, int x, int y, int drawPriority, int ch) {},
	[](Scene* scene, int x, int y, int drawPriority, int ch) {},
	[](Scene* scene, int x, int y, int drawPriority, int ch) { new FragileFallRock(scene, x, y); },
	[](Scene* scene, int x, int y, int drawPriority, int ch) { new WarpGate(scene, x, y, ch); },
	[](Scene* scene, int x, int y, int drawPriority, int ch) { new MoveWall(scene, x, y, ch); },
	[](Scene* scene, int x, int y, int drawPriority, int ch) { new MoveBlock(scene, x, y, ch); },
	[](Scene* scene, int x, int y, int drawPriority, int ch) { new AbstractBlock(scene, x, y, 0); },
	[](Scene* scene, int x, int y, int drawPriority, int ch) { new AbstractBlock(scene, x, y, 1); },
	[](Scene* scene, int x, int y, int drawPriority, int ch) { scene->GetStageManager()->SetGolemAttackZone(ch, x, y); },
	[](Scene* scene, int x, int y, int drawPriority, int ch) { new LaserCannon(scene, x, y, 1); },
	[](Scene* scene, int x, int y, int drawPriority, int ch) { new LaserCannon(scene, x, y, 0); },
	[](Scene* scene, int x, int y, int drawPriority, int ch) { new LaserCannon(scene, x, y, 2); },
	[](Scene* scene, int x, int y, int drawPriority, int ch) { new LaserCannon(scene, x, y, 3); },
	[](Scene* scene, int x, int y, int drawPriority, int ch) { scene->GetStageManager()->SetMessageZone(ch, x, y); },
	[](Scene* scene, int x, int y, int drawPriority, int ch) { scene->GetStageManager()->SetCheckpointZone(ch, x, y); },
	[](Scene* scene, int x, int y, int drawPriority, int ch) { scene->GetStageManager()->SetCheckpointZone(ch, x, y);  scene->GetStageManager()->SetCheckpointRestartIndex(ch, x, y); new CheckPoint(scene, x, y, ch); } ,
	[](Scene* scene, int x, int y, int drawPriority, int ch) { new FragileBlock(scene, x, y, 1); },
	[](Scene* scene, int x, int y, int drawPriority, int ch) { new FragileBlock(scene, x, y, 2); },
	[](Scene* scene, int x, int y, int drawPriority, int ch) { new GolemCore(scene, x, y); },
	[](Scene* scene, int x, int y, int drawPriority, int ch) { new WarpGate(scene, x, y, ch, false, false); },
	[](Scene* scene, int x, int y, int drawPriority, int ch) { new WarpGate(scene, x, y, ch, false, true); },
	[](Scene* scene, int x, int y, int drawPriority, int ch) { new AbstractBlock(scene, x, y, 4); },
	[](Scene* scene, int x, int y, int drawPriority, int ch) { new TriggerButton(scene, x, y, ch, 2);  },
	[](Scene* scene, int x, int y, int drawPriority, int ch) { new TriggerButton(scene, x, y, ch, 3);  },
	[](Scene* scene, int x, int y, int drawPriority, int ch) { new Prop(scene, x, y, drawPriority, ID_GRASS);  },
	[](Scene* scene, int x, int y, int drawPriority, int ch) { new Prop(scene, x, y, drawPriority, ID_FLOWER);  },
	[](Scene* scene, int x, int y, int drawPriority, int ch) { new Prop(scene, x, y, drawPriority, ID_TUTORIAL_MOVE);  },
	[](Scene* scene, int x, int y, int drawPriority, int ch) { new Prop(scene, x, y, drawPriority, ID_TUTORIAL_CLIMB);  },
	[](Scene* scene, int x, int y, int drawPriority, int ch) { new Prop(scene, x, y, drawPriority, ID_TUTORIAL_JUMP);  },
	[](Scene* scene, int x, int y, int drawPriority, int ch) { new Prop(scene, x, y, drawPriority, ID_TUTORIAL_ENTER);  },
	[](Scene* scene, int x, int y, int drawPriority, int ch) { new Prop(scene, x, y, drawPriority, ID_TUTORIAL_DASH);  },
	[](Scene* scene, int x, int y, int drawPriority, int ch) { scene->GetStageManager()->SetCheckpointZone(ch, x, y);  scene->GetStageManager()->SetCheckpointRestartIndex(ch, x, y); new CheckPoint(scene,StageManager::ToPosition(x, y) + Vector2(BLOCKSIZE / 2, 0), ch); } ,
	[](Scene* scene, int x, int y, int drawPriority, int ch) { new Marker(scene, x, y);  },
};

StageManager::StageManager(Scene* scene)
	: Manager(scene)
	, mStageData()
	, mCurrentStageID(0)
	, mWidth(999)
	, mHeight(999)
{
}

StageManager::~StageManager()
{
	for (int i = 0; i < eStageObjectLayerLayerMax; i++)
	{
		if (mStageData[i])
		{
			delete[] mStageData[i];
			mStageData[i] = nullptr;
		}
	}
	for (int i = 0; i < eStageObjectLayerLayerMax; i++)
	{
		if (mGolemStageData[i])
		{
			delete[] mGolemStageData[i];
			mGolemStageData[i] = nullptr;
		}
	}
	delete[] mGolemBuildableStageData;
	mGolemBuildableStageData = nullptr;
	delete[] mGolemAttackZone;
	mGolemAttackZone = nullptr;
	delete[] mMessageZone;
	mMessageZone = nullptr;
	delete[] mCheckpointZone;
	mCheckpointZone = nullptr;
}

void StageManager::Update(float deltaTime)
{
	
}

int StageManager::GetStageData(ObjectLayer layer, int indexX, int indexY)
{
	if (indexX < 0) return 0;
	if (indexY < 0) return 0;
	if (indexX >= mWidth) return 0;
	if (indexY >= mHeight) return 0;

	if (mGolemStageData[layer][indexY * mWidth + indexX] != 0)
	{
		return mGolemStageData[layer][indexY * mWidth + indexX];
	}
	return mStageData[layer][indexY * mWidth + indexX];
}

Vector2 StageManager::AdjustPos(Vector2 pos)
{
	return Vector2(ToPosX(ToIndexX(pos.x)), ToPosY(ToIndexY(pos.y)));
}

float StageManager::LimitX()
{
	if (mWidth == 0) return 999 * BLOCKSIZE;
	return mWidth * BLOCKSIZE;
}

float StageManager::LimitY()
{
	if (mHeight == 0) return 999 * BLOCKSIZE;
	return mHeight * BLOCKSIZE;
}

void StageManager::LoadStage(int stageID)
{
	mCurrentStageID = stageID;
	for (int i = 0; i < eStageObjectLayerLayerMax; i++)
	{
		if (mStageData[i])
		{
			delete[] mStageData[i];
			mStageData[i] = nullptr;
		}
	}
	for (int i = 0; i < eStageObjectLayerLayerMax; i++)
	{
		if (mGolemStageData[i])
		{
			delete[] mGolemStageData[i];
			mGolemStageData[i] = nullptr;
		}
	}
	delete[] mGolemBuildableStageData;
	mGolemBuildableStageData = nullptr;
	delete[] mGolemAttackZone;
	mGolemAttackZone = nullptr;
	delete[] mMessageZone;
	mMessageZone = nullptr;
	delete[] mCheckpointZone;
	mCheckpointZone = nullptr;
	StageData::Inst()->CreateStage(this, stageID);
}

void StageManager::SetStageSize(int width, int height)
{
	SetWidth(width);
	SetHeight(height);
	for (int i = 0; i < eStageObjectLayerLayerMax; i++)
	{
		if (mStageData[i])
		{
			delete[] mStageData[i];
			mStageData[i] = nullptr;
		}
		mStageData[i] = new int[mWidth * mHeight];
	}
	for (int i = 0; i < eStageObjectLayerLayerMax; i++)
	{
		if (mGolemStageData[i])
		{
			delete[] mGolemStageData[i];
			mGolemStageData[i] = nullptr;
		}
		mGolemStageData[i] = new int[mWidth * mHeight];
	}
	mGolemBuildableStageData = new int[mWidth * mHeight];
	mGolemAttackZone = new int[mWidth * mHeight];
	mMessageZone = new int[mWidth * mHeight];
	mCheckpointZone = new int[mWidth * mHeight];
	ZeroStageData();
}

void StageManager::CreateObject(int id, int indexX, int indexY, int propDrawLayer, int ch)
{
	createObject[id](mScene, indexX, indexY, propDrawLayer, ch);
}


void StageManager::ZeroStageData()
{
	for (int i = 0; i < eStageObjectLayerLayerMax; i++)
	{
		std::fill_n(mStageData[i], mHeight * mWidth, 0);
		std::fill_n(mGolemStageData[i], mHeight * mWidth, 0);
	}
	std::fill_n(mGolemBuildableStageData, mHeight * mWidth, 0);
	std::fill_n(mGolemAttackZone, mHeight * mWidth, 0);
	std::fill_n(mMessageZone, mHeight * mWidth, 0);
	std::fill_n(mCheckpointZone, mHeight * mWidth, 0);
}


