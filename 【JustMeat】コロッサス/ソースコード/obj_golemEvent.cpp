#include "obj_golemEvent.h"
#include"StageManager.h"
#include"CameraManager.h"
#include"Scene.h"
#include"obj_player.h"
#include"obj_ghostCat.h"
#include"ComponentAnimationSprite.h"
#include"ComponentMove.h"
#include"obj_alert.h"
#include"obj_AbstractForegroundWall.h"
#include"obj_golemArmVertical.h"
#include"obj_golemArmHorizontal.h"
#include"EventManager.h"
#include"file.h"

GolemEvent::GolemAttackData GolemEvent::mGolemAttackData[STAGE_MAX][MAX_OBJECT_CH];
bool GolemEvent::mAlreadyLoadFile;
GolemObject::GolemObject(AbstractGolemArm * owner, Scene * scene, int indexX, int indexY, int golemIndexX, int golemIndexY)
	: GolemObject(owner, scene, StageManager::ToPosition(indexX, indexY), golemIndexX, golemIndexY)
{	
}
GolemObject::GolemObject(AbstractGolemArm * owner, Scene * scene, Vector2 pos, int golemIndexX, int golemIndexY)
	: MoveFloorObject(scene, pos, 1, 1, 0, 0, 1, 0)
	, mGolemIndexX(golemIndexX)
	, mGolemIndexY(golemIndexY)
	, mOwner(owner)
{
	//SetInactive();
}

GolemObject::~GolemObject()
{
	//SetInactive();
}


GolemEvent::GolemEvent(Scene * scene, Vector2 pos)
	: Object(scene, pos)
{
	if (GetScene()->GetStageManager()->GetCurrentStageID() >= 6)
	{
		mLeftVerticalWallArm = new GolemArmVerticalArmored(scene, pos, true, false);
		mLeftVerticalBlockArm = new GolemArmVertical(scene, pos, false, false);
		mLeftHorizontalWallArm = new GolemArmHorizontalArmored(scene, pos, true, false);
		mLeftHorizontalBlockArm = new GolemArmHorizontal(scene, pos, false, false);
		mRightVerticalWallArm = new GolemArmVerticalArmored(scene, pos, true, true);
		mRightVerticalBlockArm = new GolemArmVertical(scene, pos, false, true);
		mRightHorizontalWallArm = new GolemArmHorizontalArmored(scene, pos, true, true);
		mRightHorizontalBlockArm = new GolemArmHorizontal(scene, pos, false, true);
		mLeftVerticalBlockArm->SetArmoredTexture();
		mLeftHorizontalBlockArm->SetArmoredTexture();
		mRightVerticalBlockArm->SetArmoredTexture();
		mRightHorizontalBlockArm->SetArmoredTexture();
	}
	else
	{
		mLeftVerticalWallArm = new GolemArmVertical(scene, pos, true, false);
		mLeftVerticalBlockArm = new GolemArmVertical(scene, pos, false, false);
		mLeftHorizontalWallArm = new GolemArmHorizontal(scene, pos, true, false);
		mLeftHorizontalBlockArm = new GolemArmHorizontal(scene, pos, false, false);
		mRightVerticalWallArm = new GolemArmVertical(scene, pos, true, true);
		mRightVerticalBlockArm = new GolemArmVertical(scene, pos, false, true);
		mRightHorizontalWallArm = new GolemArmHorizontal(scene, pos, true, true);
		mRightHorizontalBlockArm = new GolemArmHorizontal(scene, pos, false, true);
	}
	LoadSettingFile();
}
GolemEvent::GolemEvent(Scene * scene, int indexX, int indexY)
	: GolemEvent(scene, StageManager::ToPosition(indexX, indexY))
{
	
}


void GolemEvent::UpdateObject(float deltaTime)
{
	switch (GetScene()->GetStageManager()->GetCurrentStageID())
	{
	case 0:
		Stage0GolemArmEvent(deltaTime);
		break;
	case 1:
		Stage1GolemArmEvent(deltaTime);
		break;
	case 2:
		Stage2GolemArmEvent(deltaTime);
		break;
	case 3:
		Stage3GolemArmEvent(deltaTime);
		break;
	case 4:
		Stage4GolemArmEvent(deltaTime);
		break;
	}
	GolemAttackZone(deltaTime);
	ColorUpdate(deltaTime);
}

void GolemEvent::ColorUpdate(float deltaTime)
{

}

bool GolemEvent::AllArmReady()
{
	if (mLeftVerticalWallArm->IsReady() &&
		mLeftVerticalBlockArm->IsReady() &&
		mLeftHorizontalWallArm->IsReady() &&
		mLeftHorizontalBlockArm->IsReady() &&
		mRightVerticalWallArm->IsReady() &&
		mRightVerticalBlockArm->IsReady() &&
		mRightHorizontalWallArm->IsReady() &&
		mRightHorizontalBlockArm->IsReady())
	{
		return true;
	}
	return false;
}

void GolemEvent::LoadSettingFile()
{
	if (GolemEvent::mAlreadyLoadFile) return;
	GolemEvent::mAlreadyLoadFile = true;
	std::vector<std::string> fileData;
	FILE* fp = fopen("resources/golem.txt", "r");
	loadFileAllData(fp, &fileData);


	int row = fileData.size();
	for (int ct = 0; ct < row; ct++)
	{
		if (IsSplit(fileData[ct][0])) continue;

		int index = 0;
		int stageID = 0;
		int ch = 0;

		std::string temp = NextData(&fileData[ct], &index);
		stageID = atoi(temp.c_str());
		temp = NextData(&fileData[ct], &index);
		ch = atoi(temp.c_str());
		temp = NextData(&fileData[ct], &index);

		mGolemAttackData[stageID][ch].armType = atoi(temp.c_str());
		temp = NextData(&fileData[ct], &index);
		mGolemAttackData[stageID][ch].moveIndex = atoi(temp.c_str());
		temp = NextData(&fileData[ct], &index);
		mGolemAttackData[stageID][ch].stopIndex = atoi(temp.c_str());
		temp = NextData(&fileData[ct], &index);
		mGolemAttackData[stageID][ch].moveTime = atoi(temp.c_str()) / 1000.0f;
		temp = NextData(&fileData[ct], &index);
		mGolemAttackData[stageID][ch].stopTime = atoi(temp.c_str()) / 1000.0f;
		temp = NextData(&fileData[ct], &index);
		mGolemAttackData[stageID][ch].targetType = atoi(temp.c_str());
		temp = NextData(&fileData[ct], &index);
		mGolemAttackData[stageID][ch].targetIndexX = atoi(temp.c_str());
		temp = NextData(&fileData[ct], &index);
		mGolemAttackData[stageID][ch].targetIndexY = atoi(temp.c_str());
		temp = NextData(&fileData[ct], &index);
		int tempBackSpeed = atoi(temp.c_str());
		if (tempBackSpeed >= 0)
		{
			mGolemAttackData[stageID][ch].backSpeed = tempBackSpeed;
		}
		else
		{
			mGolemAttackData[stageID][ch].backSpeed = mGolemAttackData[stageID][ch].moveIndex / mGolemAttackData[stageID][ch].moveTime;
		}
	}
	fclose(fp);
}

void GolemEvent::CreateBlock(int x, int y)
{
	GetScene()->GetStageManager()->SetBuildableStageData(0 , x, y);
	x = mActiveArm->IndexToGolemIndexX(x);
	y = mActiveArm->IndexToGolemIndexY(y);
	if (mIsActiveArmRight)
	{
		if (mIsActiveArmHorizontal)
		{
			mRightHorizontalBlockArm->CreateBlock(x, y);
			mRightHorizontalWallArm->CreateBlock(x, y);

			mRightVerticalBlockArm->CreateBlock(6 - y, x);
			mRightVerticalWallArm->CreateBlock(6 - y, x);
		}
		else
		{
			mRightVerticalBlockArm->CreateBlock(x, y);
			mRightVerticalWallArm->CreateBlock(x, y);

			mRightHorizontalBlockArm->CreateBlock(y, 6 - x);
			mRightHorizontalWallArm->CreateBlock(y, 6 - x);

		}
	}
	else
	{

		if (mIsActiveArmHorizontal)
		{
			mLeftHorizontalBlockArm->CreateBlock(x, y);
			mLeftHorizontalWallArm->CreateBlock(x, y);

			mLeftVerticalBlockArm->CreateBlock(y, 29 - x);
			mLeftVerticalWallArm->CreateBlock(y, 29 - x);
		}
		else
		{
			mLeftVerticalBlockArm->CreateBlock(x, y);
			mLeftVerticalWallArm->CreateBlock(x, y);

			mLeftHorizontalBlockArm->CreateBlock(29 - y, x);
			mLeftHorizontalWallArm->CreateBlock(29 - y, x);
		}
	}
}

AbstractGolemArm* GolemEvent::HitGolemArm(const Vector2& pos, const Vector2& col)
{
	if (AllArmReady()) return nullptr;
	if (!mLeftVerticalWallArm->IsReady() && mLeftVerticalWallArm->IsHit(pos, col))
		return mLeftVerticalWallArm;
	if (!mRightVerticalWallArm->IsReady() && mRightVerticalWallArm->IsHit(pos, col))
		return mRightVerticalWallArm;
	if (!mLeftHorizontalWallArm->IsReady() && mLeftHorizontalWallArm->IsHit(pos, col))
		return mLeftHorizontalWallArm;
	if (!mRightHorizontalWallArm->IsReady() && mRightHorizontalWallArm->IsHit(pos, col))
		return mRightHorizontalWallArm;
	if (!mLeftVerticalBlockArm->IsReady() && mLeftVerticalBlockArm->IsHit(pos, col))
		return mLeftVerticalBlockArm;
	if (!mRightVerticalBlockArm->IsReady() && mRightVerticalBlockArm->IsHit(pos, col))
		return mRightVerticalBlockArm;
	if (!mLeftHorizontalBlockArm->IsReady() && mLeftHorizontalBlockArm->IsHit(pos, col))
		return mLeftHorizontalBlockArm;
	if (!mRightHorizontalBlockArm->IsReady() && mRightHorizontalBlockArm->IsHit(pos, col))
		return mRightHorizontalBlockArm;
	return nullptr;
}

void GolemEvent::GolemAttackZone(float deltaTime)
{
	Vector2 targetPos;
	targetPos = GetScene()->GetPlayer()->GetPosition();
	if (GetScene()->GetPlayer()->IsUsingDummy())
	{
		targetPos = GetScene()->GetCat()->GetPosition();
	}
	int areaID = GetScene()->GetStageManager()->GetGolemAttackZone(StageManager::ToIndexX(targetPos.x), StageManager::ToIndexY(targetPos.y));
	if (areaID == 0)
	{
		targetPos = GetScene()->GetPlayer()->GetPosition();
		areaID = GetScene()->GetStageManager()->GetGolemAttackZone(StageManager::ToIndexX(targetPos.x), StageManager::ToIndexY(targetPos.y));
		if (areaID == 0)return;
	}
	
	if (AllArmReady() && GetScene()->GetSceneManager()->CanSceneChange())
	{
		GolemAttackData data = mGolemAttackData[GetScene()->GetStageManager()->GetCurrentStageID()][areaID];
		AbstractGolemArm* arm;
		int targetIndexX = StageManager::ToIndexX(targetPos.x);
		int targetIndexY = StageManager::ToIndexY(targetPos.y);

		if (data.targetType == 2)
		{
			targetIndexX = data.targetIndexX;
			targetIndexY = data.targetIndexY;
		}
		switch (data.armType)
		{
		case 0:
			arm = mRightHorizontalWallArm;
			targetIndexX = data.stopIndex;
			arm->Punch(targetIndexX - data.moveIndex, targetIndexY, data.moveIndex, data.moveTime, data.stopTime, data.backSpeed);
			break;
		case 1:
			arm = mLeftHorizontalWallArm;
			targetIndexX = data.stopIndex;
			arm->Punch(targetIndexX + data.moveIndex, targetIndexY, -data.moveIndex, data.moveTime, data.stopTime, data.backSpeed);
			break;
		case 2:
			arm = mRightVerticalWallArm;
			targetIndexY = data.stopIndex;
			arm->Punch(targetIndexX, targetIndexY - data.moveIndex, data.moveIndex, data.moveTime, data.stopTime, data.backSpeed);
			break;
		case 3:
			arm = mLeftVerticalWallArm;
			targetIndexY = data.stopIndex;
 			arm->Punch(targetIndexX, targetIndexY + data.moveIndex, -data.moveIndex, data.moveTime, data.stopTime, data.backSpeed);
			break;
		case 4:
			arm = mRightHorizontalBlockArm;
			targetIndexX = data.stopIndex;
			arm->Punch(targetIndexX - data.moveIndex, targetIndexY, data.moveIndex, data.moveTime, data.stopTime, data.backSpeed);
			break;
		case 5:
			arm = mLeftHorizontalBlockArm;
			targetIndexX = data.stopIndex;
			arm->Punch(targetIndexX + data.moveIndex, targetIndexY, -data.moveIndex, data.moveTime, data.stopTime, data.backSpeed);
			break;
		case 6:
			arm = mRightVerticalBlockArm;
			targetIndexY = data.stopIndex;
			arm->Punch(targetIndexX, targetIndexY - data.moveIndex, data.moveIndex, data.moveTime, data.stopTime, data.backSpeed);
			break;
		case 7:
			arm = mLeftVerticalBlockArm;
			targetIndexY = data.stopIndex;
			arm->Punch(targetIndexX, targetIndexY + data.moveIndex, -data.moveIndex, data.moveTime, data.stopTime, data.backSpeed);
			break;
		}
	}
}

void GolemEvent::Stage0GolemArmEvent(float deltaTime)
{
	Vector2 playerPos = GetScene()->GetPlayer()->GetPosition();
	int posToIndexX = StageManager::ToIndexX(playerPos.x);
	int posToIndexY = StageManager::ToIndexY(playerPos.y);


	EventData ev00 = EventManager::GetGolemEventArea(0, 0);
	if (InRect(ev00.area, posToIndexX, posToIndexY) && !(mEventFlag & 0b00000001))
	{
		if (GetScene()->GetPlayer()->GetPlayerState() == Player::State::eClimbDefault)
		{
			mEventFlag |= 0b00000001;
			GetScene()->GetPlayer()->Stage0Event1Start(StageManager::ToPosition(ev00.x, ev00.y));
			GetScene()->GetCameraManager()->SetFollowCenter(true);
			GetScene()->GetCameraManager()->SetLimitRight(StageManager::ToPosX(149));
		}
	}

	EventData ev03 = EventManager::GetGolemEventArea(0, 3);
	if (!(mEventFlag & 0b00000010))
	{
		if (GetScene()->GetPlayer()->GetPlayerState() == Player::State::eStage0Event2_Thrust)
		{
			mEventFlag |= 0b00000010;
			mLeftHorizontalWallArm->SetMove(50 * BLOCKSIZE, 0, 10, 0);
			mRightVerticalWallArm->SetMove(0, 20 * BLOCKSIZE, 0.5f, 0);
		}
	}
}