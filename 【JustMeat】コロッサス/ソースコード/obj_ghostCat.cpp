#include "obj_ghostCat.h"
#include "mouse.h"
#include "ComponentAnimationSprite.h"
#include "obj_player.h"
#include "myMath.h"
#include "CameraManager.h"
#include "ComponentMessageBox.h"
#include "StageManager.h"
#include "EventManager.h"
#include "file.h"
#include "MoveFloorObject.h"

bool GhostCat::mAlreadyLoadFile = false;
GhostCat::MessageEventData GhostCat::mData[STAGE_MAX][MAX_OBJECT_CH];

GhostCat::GhostCat(Scene * scene, Vector2 position)
	: StageObject(scene, position)
{
	LoadMessageFile();
	mCASprite = new ComponentAnimationSprite(this, TextureTag::GhostCat, 1);
	mCASprite->SetAnimation(0, 1, 3, 1);
	mCASpriteDummy = new ComponentAnimationSprite(this, TextureTag::DummyPlayer, 1);
	mCASpriteDummy->SetAlphaRate(0);
	mCASpriteDummy->SetScale(0.9f, 1.8f);
	SetResistPause(true);
	SetSize(Vector2(1, 1) * BLOCKSIZE);
	mMessage = new ComponentMessageBox(this, TextureTag::UI9Sliced, 0);
	mMessage->SetStringInfo(16, GetPosition() - Vector2(GetColSize().x, GetColSize().y + BLOCKSIZE * 1.3f), Vector2(20, 20), false);
	SaveCheckPointData();

	mMessageIcon = new UIBox(GetScene(), mMessage->GetPosition().x, mMessage->GetPosition().y, 0.05f, 0.05f, TextureTag::CatIcon, -1);
}

void GhostCat::UpdateStageObject(float deltaTime)
{
	mMessage->SetStringInfo(16, GetPosition() - Vector2(GetColSize().x, GetColSize().y + BLOCKSIZE * 1.3f), Vector2(20, 20), false);
	mMessage->SetAlphaRate(1);
	mMessage->SetStringAlphaRate(1);
	mMessageIcon->SetPosition(mMessage->GetPosition() - mMessage->GetSize() / 2);

	if (mCatState == State::Wait)
	{
		if (degCos((GetScene()->GetPlayer()->GetPosition() - GetPosition()).deg()) > 0)
		{
			SetDirection(Dir::eRight);
		}
		else
		{
			SetDirection(Dir::eLeft);
		}
	}

	switch (mCatState)
	{
	case State::Hiding:
	case State::Invisible:
	case State::Jumping:
	case State::Thrust:
		mMessage->SetAlphaRate(0);
		mMessage->SetStringAlphaRate(0);
		break;
	case State::Wait:
		break;
	case State::FollowPlayer:
		StateFollowPlayer(deltaTime);
		break;
	}
	CatEventManager(deltaTime);
	UpdateMoveFloor();

	mDummyDelay -= deltaTime;
	if (mDummyDelay.IsFinish())
	{
		CreateDummy();
	}
}


void GhostCat::StateFollowPlayer(float deltaTime)
{
	Vector2 destination = GetScene()->GetPlayer()->GetPosition() + Vector2(BLOCKSIZE, BLOCKSIZE) * Vector2(1, -1);
	Vector2 diff = destination - GetPosition();
	float deg = degArctan(diff.x, diff.y);
	if (degCos(deg) > 0)
	{
		SetDirection(Dir::eRight);
	}
	else
	{
		SetDirection(Dir::eLeft);
	}
	float dist = diff.Length();
	if (dist > MAXSPEED) dist = MAXSPEED;
	if (dist > BLOCKSIZE * 0.1f)
	{
		Translate(Vector2(degCos(deg), degSin(deg)) * dist * deltaTime);
	}
}

void GhostCat::CatEventManager(float deltaTime)
{
	Vector2 playerPos = GetScene()->GetPlayer()->GetPosition();
	int playerIndexX = StageManager::ToIndexX(playerPos.x);
	int playerIndexY = StageManager::ToIndexY(playerPos.y);
	int eventID = GetScene()->GetStageManager()->GetMessageZone(playerIndexX, playerIndexY);
	if (eventID <= 0)return;
	if (!mCatEventID[eventID])
	{
		int stageID = GetScene()->GetStageManager()->GetCurrentStageID();
		mCatEventID[eventID] = true;
		if (mData[stageID][eventID].eventType == 0)
		{
			mMessage->SetString(mData[stageID][eventID].message, mData[stageID][eventID].coloredStart, mData[stageID][eventID].coloredCount);
			mMessage->Appear(mData[stageID][eventID].autoDisappearTime);
		}
		else if(mData[stageID][eventID].eventType == 1)
		{
			mMessage->Disappear();
		}
	}
}

void GhostCat::SaveCheckPointData()
{
	StageObject::SaveCheckPointData();
	memcpy(mCPEventID, mCatEventID, sizeof(mCatEventID));
	mCPCatState = mCatState;
	mCPEventCounter = mEventCounter;
}

void GhostCat::LoadCheckPointData()
{
	StageObject::LoadCheckPointData();
	GetScene()->GetPlayer()->LoadCheckPointData();
	SetPosition(GetScene()->GetPlayer()->GetPosition() + Vector2(BLOCKSIZE, BLOCKSIZE) * Vector2(1, -1));
	memcpy(mCatEventID, mCPEventID, sizeof(mCatEventID));
	mCatState = mCPCatState;
	mEventCounter = mCPEventCounter;
	mDummyDelay = 0;
	if (mCatState == State::Dummy)
	{
		EndDummy(false);
		SetPosition(GetScene()->GetPlayer()->GetPosition() + Vector2(BLOCKSIZE, BLOCKSIZE) * Vector2(1, -1));
	}
	mMessage->Disappear();
}

void GhostCat::StartDummy(bool isClimb, Dir dir)
{
	mCatState = State::Dummy;
	GenerateWarp(GetPosition());
	mDummyPos = GetScene()->GetPlayer()->GetPosition();
	mDummyDir = dir;
	mIsDummyClimb = isClimb;
	mDummyDelay = 0.1f;
}

void GhostCat::CreateDummy()
{
	SetDirection(mDummyDir);
	if (!mIsDummyClimb)
	{
		mCASpriteDummy->SetAnimation(0, 1, 1, 1, false);
	}
	else
	{
		mCASpriteDummy->SetAnimation(2, 2, 1, 1, false);
	}
	mCASpriteDummy->SetAlphaRate(1);
	mCASprite->SetAlphaRate(0);
	mCASpriteDummy->SetAnimationSpeed(0);
	mIsClimb = mIsDummyClimb;
	GenerateSmoke(mDummyPos);
	GenerateStar(mDummyPos);
	SetPosition(mDummyPos);
}

void GhostCat::EndDummy(bool makeEffect)
{
	mCASpriteDummy->SetAlphaRate(0);
	mCASprite->SetAlphaRate(1);
	mCatState = State::FollowPlayer;
	mIsClimb = false;
	if (makeEffect)
	{
		GenerateSmoke(GetPosition());
		GenerateStar(GetPosition());
	}
}

void GhostCat::WarpGate()
{
	if (mCatState == State::FollowPlayer || mCatState == State::Dummy)
	{
		EndDummy(false);
		SetPosition(GetScene()->GetPlayer()->GetPosition() + Vector2(BLOCKSIZE, BLOCKSIZE) * Vector2(1, -1));
	}
}

void GhostCat::EnterGolemArm()
{
	if (mCatState == State::Dummy)
	{
		GetScene()->GetPlayer()->CancelDummy(true);
	}
}


void GhostCat::UpdateMoveFloor()
{
	if (mCatState != State::Dummy)return;
	auto movefloor = GetScene()->GetMoveFloors();
	for (auto it : movefloor)
	{
		if (it->GetObjectType() == Type::eWall && mIsClimb)
		{
			if (Vector2::RectCollision(it->GetPosition(), it->GetColSize(), GetPosition(), GetColSize() / 10))
			{
				it->Assign(this);
			}
		}

		if (it->GetObjectType() == Type::eSolid && !mIsClimb)
		{
			if (Vector2::RectCollision(it->GetPosition(), it->GetColSize(), GetPosition(), Vector2(5, GetColSize().y) * 1.1f))
			{

				it->Assign(this);
			}
		}
	}
}


#include"ParticleSystem.h"
void GhostCat::GenerateSmoke(Vector2 position)
{
	ParticleInfo info;
	info.tex = TextureTag::Smoke;
	info.redRateMax = 0.7f;
	info.redRateMin = 0.7f;
	info.greenRateMax = 0.7f;
	info.greenRateMin = 0.7f;
	info.particlePerGenerateMax = 40;
	info.particlePerGenerateMin = 40;
	info.speedMin = 3 * BLOCKSIZE;
	info.speedMax = 4.5f * BLOCKSIZE;
	info.greenLifetimeChangeType = LifetimeChangeType::eCos90Half;
	info.redLifetimeChangeType = LifetimeChangeType::eCos90Half;
	info.blueLifetimeChangeType = LifetimeChangeType::eCos90Half;
	info.speedLifetimeChangeType = LifetimeChangeType::eCos90Half;
	info.scaleLifetimeChangeType = LifetimeChangeType::eCos90Half;
	info.scaleMin = 2.5f;
	info.scaleMax = 5;
	info.generateIntervalMin = 0.05f;
	info.generateIntervalMax = 0.15f;
	info.lifetimeMin = 1;
	info.lifetimeMax = 1.5f;
	info.addBlend = true;
	info.alphaRateMin = 0.15f;
	info.alphaRateMax = 0.15f;
	info.alphaLifetimeChangeType = LifetimeChangeType::eCos90Half;
	info.spawnType = SpawnType::eCircle;
	info.spawnCircleRadiusMin = 0;
	info.spawnCircleRadiusMax = BLOCKSIZE / 2;
	info.useGravity = true;
	info.gravity = Vector2(0, -GRAVITY / 10);
	new ParticleSystem(GetScene(), position, &info, 0.2f, false);
}

void GhostCat::GenerateStar(Vector2 position)
{
	ParticleInfo info;
	info.generateIntervalMin = 0;
	info.generateIntervalMax = 0;
	info.redRateMax = 0.7f;
	info.redRateMin = 0.7f;
	info.greenRateMax = 0.3f;
	info.greenRateMin = 0.3f;
	info.tex = TextureTag::Star;
	info.moveBackward = true;
	info.particlePerGenerateMin = 10;
	info.particlePerGenerateMax = 10;
	info.speedMin = BLOCKSIZE * 3;
	info.speedMax = BLOCKSIZE * 7;
	info.scaleMin = 0.6f;
	info.scaleMax = 0.9f;
	info.degMin = 0;
	info.degMax = 360;
	info.size = Vector2(BLOCKSIZE, BLOCKSIZE);
	info.lifetimeMin = 0.5f;
	info.lifetimeMax = 0.7f;
	info.alphaLifetimeChangeType = LifetimeChangeType::eCos90;
	info.scaleLifetimeChangeType = LifetimeChangeType::eFadeinout;
	info.speedLifetimeChangeType = LifetimeChangeType::eCos90Half;
	info.addBlend = true;
	new ParticleSystem(GetScene(), position, &info, 0, true);
}

void GhostCat::GenerateWarp(Vector2 position)
{
	ParticleInfo info;
	info.useGravity = false;
	info.generateIntervalMin = 0.015f;
	info.generateIntervalMax = 0.03f;
	info.redRateMax = 0.6f;
	info.redRateMin = 0.5f;
	info.greenRateMax = 0.5f;
	info.greenRateMin = 0.3f;
	info.blueRateMax = 0.9f;
	info.blueRateMin = 0.6f;
	info.alphaRateMax = 0.9f;
	info.alphaRateMin = 0.5f;
	info.tex = TextureTag::Plane;
	info.moveBackward = true;
	info.particlePerGenerateMin = 40;
	info.particlePerGenerateMax = 60;
	info.speedMin = BLOCKSIZE * 4;
	info.speedMax = BLOCKSIZE * 6;
	info.scaleMin = 0.05f;
	info.scaleMax = 0.2f;
	info.degMin = 270;
	info.degMax = 270;
	info.size = Vector2(BLOCKSIZE * 0.1f, BLOCKSIZE * 4);
	info.spawnType = SpawnType::eLine;
	info.spawnLineLength = BLOCKSIZE;
	info.spawnLineWidth = BLOCKSIZE;
	info.lifetimeMin = 0.1f;
	info.lifetimeMax = 0.2f;
	info.alphaLifetimeChangeType = LifetimeChangeType::eFadeinout;
	info.scaleLifetimeChangeType = LifetimeChangeType::eFadeinout;
	info.speedLifetimeChangeType = LifetimeChangeType::eCos90;
	info.addBlend = true;
	new ParticleSystem(GetScene(), position + Vector2(0, BLOCKSIZE), &info, 0.1f);
}

void GhostCat::LoadMessageFile()
{
	if (mAlreadyLoadFile) return;
	mAlreadyLoadFile = true;
	std::vector<std::string> fileData;
	FILE* fp = fopen("resources/message.txt", "r");
	loadFileAllData(fp, &fileData);


	int row = fileData.size();
	for (int ct = 0; ct < row; ct++)
	{
		if (IsSplit(fileData[ct][0])) continue;

		int index = 0;
		int stageID = 0;
		int areaID = 0;

		std::string temp = NextData(&fileData[ct], &index);
		stageID = atoi(temp.c_str());
		temp = NextData(&fileData[ct], &index);
		areaID = atoi(temp.c_str());
		temp = NextData(&fileData[ct], &index);
		mData[stageID][areaID].eventType = atoi(temp.c_str());
		temp = NextData(&fileData[ct], &index);
		mData[stageID][areaID].message = temp.c_str();
		temp = NextData(&fileData[ct], &index);
		mData[stageID][areaID].autoDisappearTime = atoi(temp.c_str()) / 1000.0f;
		temp = NextData(&fileData[ct], &index);
		mData[stageID][areaID].coloredStart = atoi(temp.c_str());
		temp = NextData(&fileData[ct], &index);
		mData[stageID][areaID].coloredCount = atoi(temp.c_str());
	}
	fclose(fp);
}
