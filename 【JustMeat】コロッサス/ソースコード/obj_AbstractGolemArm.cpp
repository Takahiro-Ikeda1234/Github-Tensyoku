#include"obj_AbstractGolemArm.h"
#include"StageManager.h"
#include"ComponentAnimationSprite.h"
#include"obj_alert.h"
#include"Scene.h"
#include"obj_player.h"
#include"CameraManager.h"
#include"ParticleSystem.h"
#include"AudioData.h"
#include "obj_golemArmHorizontal.h"
#include "obj_golemArmVertical.h"
bool AbstractGolemArm::mFirstAttack;
AbstractGolemArm::AbstractGolemArm(Scene * scene, Vector2 pos, bool isWall, bool isRight, bool isHorizontal, bool isSub)
	: MoveFloorObject(scene, pos, 0, 0, 0, 0, 0, 0)//Object(scene, pos)
	, mIsWall(isWall)
	, mIsRight(isRight)
	, mIsHorizontal(isHorizontal)
	, mIsSub(isSub)
{
	mFirstAttack = true;
	if (mIsWall) SetObjectType(Type::eWall);
	else SetObjectType(Type::eSolid);

	mIsGolemArm = true;
	
	if (!mIsSub)
	{
		if (mIsHorizontal)
		{
			if (mIsRight) mCrystal = new GolemCrystal(GetScene(), GetPosition() - Vector2(BLOCKSIZE * 15, 0), 180);
			else mCrystal = new GolemCrystal(GetScene(), GetPosition() + Vector2(BLOCKSIZE * 15, 0), 0);
		}
		else
		{
			if (mIsRight) mCrystal = new GolemCrystal(GetScene(), GetPosition() - Vector2(0, BLOCKSIZE * 15), 270);
			else mCrystal = new GolemCrystal(GetScene(), GetPosition() + Vector2(0, BLOCKSIZE * 15), 90);
		}
	}
}
AbstractGolemArm::AbstractGolemArm(Scene * scene, int indexX, int indexY, bool isWall, bool isRight, bool isHorizontal, bool isSub)
	: AbstractGolemArm(scene, StageManager::ToPosition(indexX, indexY), isWall, isRight, isHorizontal, isSub)
{
}
AbstractGolemArm::~AbstractGolemArm()
{
}
void AbstractGolemArm::UpdateStageObject(float deltaTime)
{
	mNextMotionTime -= deltaTime;
	switch (mState)
	{
	case State::eReady:
		StateReady(deltaTime);
		break;
	case State::ePunchStart:
		StatePunchStart(deltaTime);
		break;
	case State::ePunch:
		StatePunch(deltaTime);
		break;
	case State::ePunchStop:
		StatePunchStop(deltaTime);
		break;
	case State::ePunchEnd:
		StatePunchEnd(deltaTime);
		break;
	case State::eMove:
		StateMove(deltaTime);
		break;
	}
	mObject.clear();
}

bool AbstractGolemArm::IsHit(Vector2 pos, Vector2 col)
{
	return Vector2::RectCollision(pos, col, GetPosition(), GetColSize());
}

void AbstractGolemArm::Punch(int startX, int startY, int move, float moveTime, float stopTime, float backSpeed)
{
	if (GetScene()->GetStageManager()->GetCurrentStageID() == 0 && mFirstAttack)
	{
		SetFadeout(0, 100, 0);
	}
	GetScene()->GetGolem()->SetActiveArm(mIsRight, mIsHorizontal, this);
	mAttackStopTime = stopTime;
	mAttackMoveTime = moveTime;
	mStartX = startX;
	mStartY = startY;
	if (mIsHorizontal)
	{
		mMoveX = move;
		mMoveY = 0;
		if (mIsRight)
		{
			mBackSpeedX = -backSpeed;
		}
		else
		{
			mBackSpeedX = backSpeed;
		}
		
	}
	else
	{
		mMoveX = 0;
		mMoveY = move;
		if (mIsRight)
		{
			mBackSpeedY = -backSpeed;
		}
		else
		{
			mBackSpeedY = backSpeed;
		}
		
	}


	mState = State::ePunchStart;
	mNextMotionTime = 2.0f;
	mCASprite->SetAlphaRate(1);

	Vector2 pos, size, armStart;
	if (mIsHorizontal)
	{
		if (mIsRight)
		{
			mStartX = startX - (mSize / 2  - 1);
			SetPosition(StageManager::ToPosition(startX - (mSize / 2), startY) + Vector2(BLOCKSIZE * 0.5f, 0));
			pos = StageManager::ToPosition(startX, startY) + Vector2(BLOCKSIZE * 0.5f, BLOCKSIZE * -2.5f);
			size = Vector2(BLOCKSIZE * move, BLOCKSIZE * 5);
			armStart = GetPosition() + Vector2(GetColSize().x, 0);
		}
		else
		{
			mStartX = startX + (mSize / 2);
			SetPosition(StageManager::ToPosition(startX + (mSize / 2), startY) - Vector2(BLOCKSIZE * 0.5f, 0));
			pos = StageManager::ToPosition(startX + move, startY) + Vector2(BLOCKSIZE * -0.5f, BLOCKSIZE * -2.5f);
			size = Vector2(BLOCKSIZE * -move, BLOCKSIZE * 5);
			armStart = GetPosition() - Vector2(GetColSize().x, 0);
		}
	}
	else
	{
		if (mIsRight)
		{
			mStartY = startY - (mSize / 2 - 1);
			SetPosition(StageManager::ToPosition(startX, startY - (mSize / 2)) + Vector2(0, BLOCKSIZE * 0.5f));
			pos = StageManager::ToPosition(startX, startY) + Vector2(BLOCKSIZE * -2.5f, BLOCKSIZE * 0.5f);
			size = Vector2(BLOCKSIZE * 5, BLOCKSIZE * move);
			armStart = GetPosition() + Vector2(0, GetColSize().y);
		}
		else
		{
			mStartY = startY + (mSize / 2);
			SetPosition(StageManager::ToPosition(startX, startY + (mSize / 2)) - Vector2(0, BLOCKSIZE * 0.5f));
			pos = StageManager::ToPosition(startX, startY + move) + Vector2(BLOCKSIZE * -2.5f, BLOCKSIZE * -0.5f);
			size = Vector2(BLOCKSIZE * 5, BLOCKSIZE * -move);
			armStart = GetPosition() - Vector2(0, GetColSize().y);
		}
	}
	if(!mIsSub) new Alert(GetScene(), pos, size, armStart, 2.0f, 1, 0xFFFF0000);
}

void AbstractGolemArm::StateReady(float deltaTime)
{
}

void AbstractGolemArm::StatePunchStart(float deltaTime)
{
	if (mNextMotionTime.IsEnd())
	{
		if(!mIsSub) GenerateSpark();
		mState = State::ePunch;
		mNextMotionTime = mAttackMoveTime;
		PlaySE(SE_PUNCH);

		GetScene()->GetCameraManager()->SetCameraShake(BLOCKSIZE * 0.25f, BLOCKSIZE * 0.25f, mAttackMoveTime);
	}
}

void AbstractGolemArm::StatePunch(float deltaTime)
{
	if (mIsHorizontal)
	{
		SetPosition(Vector2(mStartX * BLOCKSIZE + mMoveX * BLOCKSIZE * (1 - mNextMotionTime.Percentage()), GetPosition().y));
		if(mSpark) mSpark->SetPosition(FrontPosition());
	}
	else
	{
		SetPosition(Vector2(GetPosition().x, mStartY * BLOCKSIZE + mMoveY * BLOCKSIZE * (1 - mNextMotionTime.Percentage())));
		if (mSpark) mSpark->SetPosition(FrontPosition());
	}
	Vector2 playerPos = GetScene()->GetPlayer()->GetPosition();
	Vector2 playerColSize = GetScene()->GetPlayer()->GetColSize();
	if (Vector2::RectCollision(GetPosition(), GetColSize(), playerPos, playerColSize) && !GetScene()->GetPlayer()->IsDying())
	{
		GetScene()->GetPlayer()->HitGolemArm(0);
		if (mIsSub && mIsHorizontal) ((GolemArmHorizontalArmoredSub*)this)->GetParent()->SetObjectType(Type::eNone);
		if (mIsSub && !mIsHorizontal) ((GolemArmVerticalArmoredSub*)this)->GetParent()->SetObjectType(Type::eNone);
		SetObjectType(Type::eNone);
	}

	mCASprite->SetColor(0xFFFF5F3C);
	if (mNextMotionTime.IsEnd())
	{
		if (mSpark) mSpark->Erase();
		mSpark = nullptr;
		GetScene()->GetCameraManager()->SetCameraShake(BLOCKSIZE, BLOCKSIZE, 0.5f);

		if (GetScene()->GetStageManager()->GetCurrentStageID() == 0 && mFirstAttack)
		{
			mFirstAttack = false;
			PlayBGM("stage4", 0, 100);
		}

		mCASprite->SetColor(0xFFFFFFFF);
		mState = State::ePunchStop;
		mNextMotionTime = mAttackStopTime;
		if(!GetScene()->GetPlayer()->IsDying()) StartBlockMode();

		if (!mIsSub)
		{
			auto objects = GetScene()->OnCollision(FrontPosition(), Vector2(BLOCKSIZE, BLOCKSIZE), this, Object::Type::eSolid);
			for (auto it : objects)
			{
				if (it->IsBlock())
				{
					GenerateFragments();
					GenerateSmoke();
					PlaySE(SE_BREAK);
					break;
				}
			}
		}
		else
		{
			SetObjectType(Type::eEnemy);	//eNone以外なら何でも
		}
	}

	if (!mIsSub)
	{
		auto objects = GetScene()->GetMoveFloors();
		for (auto it : objects)
		{
			if (it->IsPushBlock())
			{
				Vector2 totaloffset(0, 0);
				Vector2 offset(mMoveX == 0 ? 0 : mMoveX / fabsf(mMoveX), mMoveY == 0 ? 0 : mMoveY / fabsf(mMoveY));
				while (Vector2::RectCollision(it->GetPosition() + totaloffset, it->GetColSize(), GetPosition(), GetColSize()))
				{
					totaloffset = totaloffset + offset;
				}
				it->SetSpeed(totaloffset);
			}
		}
	}
}

void AbstractGolemArm::StatePunchStop(float deltaTime)
{
	if (mNextMotionTime.IsEnd())
	{
		mState = State::ePunchEnd;
		EndBlockMode();
	}
}

void AbstractGolemArm::StatePunchEnd(float deltaTime)
{
	//Translate(Vector2(-mMoveX * BLOCKSIZE, -mMoveY * BLOCKSIZE) * deltaTime);
	Translate(Vector2(mBackSpeedX * BLOCKSIZE, mBackSpeedY * BLOCKSIZE) * deltaTime);

	bool endFlag = false;
	if (GetScene()->GetCameraManager()->InCamera(&GetPosition(), &GetSize()) != 0)
		endFlag = true;

	//Vector2 playerPos = GetScene()->GetPlayer()->GetPosition();
	//Vector2 playerColSize = GetScene()->GetPlayer()->GetColSize();
	//if (Vector2::RectCollision(playerPos, playerColSize, GetPosition(), GetColSize()) || mObject.size() > 0)
	//{
	//	if (GetObjectType() != Type::eNone && GetScene()->GetPlayer()->GetGolemCount() == 0)
	//	{
	//		GetScene()->GetPlayer()->HitGolemArm(0);
	//		SetObjectType(Type::eNone);
	//	}
	//}

	//if (mMoveX > 0)
	//{
	//	if (GetPosition().x + GetColSize().x < 0)
	//		endFlag = true;
	//}
	//else if (mMoveX < 0)
	//{
	//	if (GetPosition().x - GetColSize().x > GetScene()->GetStageManager()->GetWidth() * BLOCKSIZE)
	//		endFlag = true;
	//}
	//else if (mMoveY > 0)
	//{
	//	if (GetPosition().y + GetColSize().y < 0)
	//		endFlag = true;
	//}
	//else if (mMoveY < 0)
	//{
	//	if (GetPosition().y - GetColSize().y > GetScene()->GetStageManager()->GetHeight() * BLOCKSIZE)
	//		endFlag = true;
	//}


	if (endFlag)
	{
		mObject.clear();	//連動オブジェクトを移動させない
		Translate(-1 * GetPosition());
		mState = State::eReady;
		if (mIsWall) SetObjectType(Type::eWall);
		else SetObjectType(Type::eSolid);
	}
}

void AbstractGolemArm::StateMove(float deltaTime)
{
	mMoveDelay -= deltaTime;
	if (mMoveDelay.IsEnd()) mMoveTime -= deltaTime;

	if (mMoveDelay.IsEnd())
	{
		float percentage = mMoveTime.Percentage();
		float delta = mPrevPercentage - percentage;
		Translate(Vector2(mMove_MoveX * delta, mMove_MoveY* delta));
		mPrevPercentage = percentage;
	}
	if (mMoveTime.IsEnd())
	{
		mState = State::eReady;
	}
}


void AbstractGolemArm::Translate(Vector2 v)
{
	Object::Translate(v);
	if(!mIsSub)mCrystal->Translate(v);
	for (auto it : mBlocks)
	{
		it->Translate(v);
	}

	//接触オブジェクト移動処理
	for (auto it : mObject)
	{
		if (GetObjectType() == Type::eSolid)
		{
			if (it == GetScene()->GetPlayer())
			{
				if (((Player*)it)->IsLanding())
				{
					it->Translate(v);
				}
			}
			else
			{
				it->Translate(v);
			}
		}
		else
		{
			it->Translate(v);
		}
		auto objects = GetScene()->OnCollision(it->GetPosition(), it->GetColSize(), it, Object::Type::eSolid);
		int findCount = objects.size();
		if (findCount > 0)
		{
			for (auto block : objects)
			{
				if (this == block)continue;
				Vector2 vec(-v.x, -v.y);
				float len = vec.Length();
				if (len == 0)break;
				vec = vec / len;
				while (Vector2::RectCollision(block->GetPosition(), block->GetColSize(), it->GetPosition(), it->GetColSize()))
				{
					it->Translate(vec);
				}
			}
		}
		auto objects2 = GetScene()->OnCollision(it->GetPosition(), it->GetColSize(), it, Object::Type::eSolid);
		int findCount2 = objects2.size();
		if (findCount2 > 0 && it->GetObjectType() == Type::ePlayer)
		{
			if (!((Player*)it)->IsDying()) ((Player*)it)->HitGolemArm(0);
			if (mIsSub && mIsHorizontal) ((GolemArmHorizontalArmoredSub*)this)->GetParent()->SetObjectType(Type::eNone);
			if (mIsSub && !mIsHorizontal) ((GolemArmVerticalArmoredSub*)this)->GetParent()->SetObjectType(Type::eNone);
			SetObjectType(Type::eNone);
			continue;
		}
		//auto objects2 = GetScene()->OnCollision(it->GetPosition(), it->GetColSize(), it, Object::Type::eSolid);
		//int findCount2 = objects2.size();
		//if (findCount2 > 0)
		//{
		//	for (auto block : objects2)
		//	{
		//		Vector2 vec(-mMoveSpeed.x, -mMoveSpeed.y);
		//		float len = vec.Length();
		//		if (len == 0)break;
		//		vec = vec / len;
		//		while (Vector2::RectCollision(block->GetPosition(), block->GetColSize(), it->GetPosition(), it->GetColSize()))
		//		{
		//			it->Translate(vec);
		//		}
		//	}
		//}
	}
}

void AbstractGolemArm::SetPosition(Vector2 v)
{
	Translate(v - GetPosition());
}

void AbstractGolemArm::LoadCheckPointData()
{
	mObject.clear();	//連動オブジェクトを移動させない
	Translate(-1 * GetPosition());
	mState = State::eReady;
	if (mIsWall) SetObjectType(Type::eWall);
	else SetObjectType(Type::eSolid);

	if (mSpark)
	{
		delete mSpark;
		mSpark = nullptr;
	}
}

void AbstractGolemArm::SetArmoredTexture()
{
	if (mIsHorizontal)
	{
		mCASprite->SetTexture(TextureTag::HorizontalArmArmor);
	}
	else
	{
		mCASprite->SetTexture(TextureTag::VerticalArmArmor);
	}


	if (mIsWall)
	{
		mCASprite->SetAnimation(0, 1, 1, 1, false);
	}
	else
	{
		int stage = GetScene()->GetStageManager()->GetCurrentStageID();
		if (stage < 3) mCASprite->SetAnimation(2, 3, 1, 1, false);
		else if (stage < 6) mCASprite->SetAnimation(4, 5, 1, 1, false);
		else mCASprite->SetAnimation(6, 7, 1, 1, false);
	}
	
}

void AbstractGolemArm::GenerateSpark()
{
	ParticleInfo info;
	if (mIsHorizontal)
	{
		if (mIsRight)
		{
			info.degMin = 175;
			info.degMax = 185;
			info.spawnLineDeg = 270;
		}
		else
		{
			info.degMin = -5;
			info.degMax = 5;
			info.spawnLineDeg = 90;
		}
	}
	else
	{
		if (mIsRight)
		{
			info.degMin = 265;
			info.degMax = 275;
			info.spawnLineDeg = 0;
		}
		else
		{
			info.degMin = 85;
			info.degMax = 95;
			info.spawnLineDeg = 180;
		}
	}
	info.useGravity = true;
	info.generateIntervalMin = 0;
	info.generateIntervalMax = 0;
	info.tex = TextureTag::Spark;
	info.spawnType = SpawnType::eLine;
	info.spawnLineLength = BLOCKSIZE * 5;
	info.moveBackward = true;
	info.spawnLineFan = true;
	info.particlePerGenerateMin = 5;
	info.particlePerGenerateMax = 8;
	info.speedMin = BLOCKSIZE * 15;
	info.speedMax = BLOCKSIZE * 30;
	info.scaleMin = 0.20f;
	info.scaleMax = 0.40f;
	info.size = Vector2(BLOCKSIZE, BLOCKSIZE * 5);
	info.lifetimeMin = 0.3f;
	info.lifetimeMax = 0.6f;
	info.alphaLifetimeChangeType = LifetimeChangeType::eCos90;
	info.scaleLifetimeChangeType = LifetimeChangeType::eSin90Reverce;
	info.addBlend = true;
	mSpark = new ParticleSystem(GetScene(), Vector2(FrontPosition()), &info, 0);
}

void AbstractGolemArm::GenerateFragments()
{
	ParticleInfo info;
	if (mIsHorizontal)
	{
		if (mIsRight)
		{
			info.degMin = 90;
			info.degMax = 270;
			info.spawnLineDeg = 270;
		}
		else
		{
			info.degMin = -90;
			info.degMax = 90;
			info.spawnLineDeg = 90;
		}
	}
	else
	{
		if (mIsRight)
		{
			info.degMin = 180;
			info.degMax = 360;
			info.spawnLineDeg = 0;
		}
		else
		{
			info.degMin = 0;
			info.degMax = 180;
			info.spawnLineDeg = 180;
		}
	}
	info.tex = TextureTag::Fragment4;
	info.spawnLineFan = true;
	info.spawnType = SpawnType::eLine;
	info.spawnLineLength = BLOCKSIZE  * 5;
	info.spawnLineWidth = BLOCKSIZE;
	info.generateIntervalMax = 0;
	info.generateIntervalMin = 0;
	info.scaleMin = 0.5f;
	info.scaleMax = 1;
	info.speedMin = BLOCKSIZE * 3;
	info.speedMax = BLOCKSIZE * 5;
	info.useGravity = true;
	info.gravity = Vector2(0, GRAVITY / 20);
	info.lifetimeMin = 0.25f;
	info.lifetimeMax = 1.5f;
	info.alphaLifetimeChangeType = LifetimeChangeType::eCos90;
	info.particlePerGenerateMin = 20;
	info.particlePerGenerateMax = 20;
	info.moveBackward = true;
	new ParticleSystem(GetScene(), FrontPosition(), &info, 0, true);
}

void AbstractGolemArm::GenerateSmoke()
{
	GenerateSmoke1();
	GenerateSmoke2();
}

void AbstractGolemArm::GenerateSmoke1()
{
	ParticleInfo info;
	info.tex = TextureTag::Smoke;
	info.greenRateMax = 0.7f;
	info.greenRateMin = 0.7f;
	info.blueRateMax = 0.3f;
	info.blueRateMin = 0.3f;
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
	new ParticleSystem(GetScene(), FrontPosition(), &info, 0.2f, false);
}

void AbstractGolemArm::GenerateSmoke2()
{
	ParticleInfo info;
	if (mIsHorizontal)
	{
		if (mIsRight)
		{
			info.degMin = 255;
			info.degMax = 270;
			info.spawnLineDeg = 270;
		}
		else
		{
			info.degMin = 270;
			info.degMax = 285;
			info.spawnLineDeg = 90;
		}
	}
	else
	{
		if (mIsRight)
		{
			info.degMin = -15;
			info.degMax = 0;
			info.spawnLineDeg = 0;
		}
		else
		{
			info.degMin = 0;
			info.degMax = 15;
			info.spawnLineDeg = 180;
		}
	}

	info.tex = TextureTag::Smoke;
	info.greenRateMax = 0.8f;
	info.greenRateMin = 0.8f;
	info.blueRateMax = 0.4f;
	info.blueRateMin = 0.4f;
	info.particlePerGenerateMax = 40;
	info.particlePerGenerateMin = 40;
	info.speedMin = 6.f * BLOCKSIZE;
	info.speedMax = 9.f * BLOCKSIZE;
	info.greenLifetimeChangeType = LifetimeChangeType::eCos90;
	info.redLifetimeChangeType = LifetimeChangeType::eCos90;
	info.blueLifetimeChangeType = LifetimeChangeType::eCos90;
	info.speedLifetimeChangeType = LifetimeChangeType::eCos90Half;
	info.scaleLifetimeChangeType = LifetimeChangeType::eSin90Half;
	info.scaleMin = 3.5f;
	info.scaleMax = 4;
	info.generateIntervalMin = 0.05f;
	info.generateIntervalMax = 0.15f;
	info.lifetimeMin = 0.5f;
	info.lifetimeMax = 1.0f;
	info.addBlend = true;
	info.alphaRateMin = 0.15f;
	info.alphaRateMax = 0.15f;
	info.alphaLifetimeChangeType = LifetimeChangeType::eCos90;

	info.spawnType = SpawnType::eLine;
	info.spawnLineLength = BLOCKSIZE * 5;
	new ParticleSystem(GetScene(), FrontPosition(), &info, 0.2f, false);

	if (mIsHorizontal)
	{
		if (mIsRight)
		{
			info.degMin = 90;
			info.degMax = 105;
			info.spawnLineDeg = 270;
		}
		else
		{
			info.degMin = 75;
			info.degMax = 90;
			info.spawnLineDeg = 90;
		}
	}
	else
	{
		if (mIsRight)
		{
			info.degMin = 180;
			info.degMax = 195;
			info.spawnLineDeg = 0;
		}
		else
		{
			info.degMin = 165;
			info.degMax = 180;
			info.spawnLineDeg = 180;
		}
	}
	new ParticleSystem(GetScene(), FrontPosition(), &info, 0.2f, false);
}

void AbstractGolemArm::StartBlockMode()
{
	for (auto it : mBlocks)
	{
		it->SetActive();
	}
}

void AbstractGolemArm::EndBlockMode()
{
	for (auto it : mBlocks)
	{
		it->SetInactive();
	}
}

Vector2 AbstractGolemArm::FrontPosition()
{
	Vector2 result = GetPosition();
	if (mIsHorizontal)
	{
		if (mIsRight)
		{
			result.x += GetColSize().x * 0.95f;
		}
		else
		{
			result.x -= GetColSize().x * 0.95f;
		}
	}
	else
	{
		if (mIsRight)
		{
			result.y += GetColSize().y * 0.95f;
		}
		else
		{
			result.y -= GetColSize().y * 0.95f;
		}
	}
	return result;
}




GolemBlock::GolemBlock(AbstractGolemArm * owner, Scene * scene, int indexX, int indexY, int golemIndexX, int golemIndexY)
	: GolemBlock(owner, scene, StageManager::ToPosition(indexX, indexY), golemIndexX, golemIndexY)
{
}

GolemBlock::GolemBlock(AbstractGolemArm * owner, Scene * scene, Vector2 pos, int golemIndexX, int golemIndexY)
	: GolemObject(owner, scene, pos, golemIndexX, golemIndexY)
{
	mCASprite = new ComponentAnimationSprite(this, TextureTag::Plane, 49);
	owner->SetBlockData(1, GetGolemIndexX(), GetGolemIndexY());
	SetInactive();
	SetSize(Vector2(10, 5) * BLOCKSIZE);
}

void GolemBlock::Translate(Vector2 v)
{
	AbstractObject::Translate(v);
	if (GetObjectType() == Type::eNone)
	{
		mObject.clear();
		return;
	}
	for (auto it : mObject)
	{
		it->Translate(v);

		auto objects = GetScene()->OnCollision(it->GetPosition(), it->GetColSize(), it, Object::Type::eSolid);
		int findCount = objects.size();
		if (findCount > 0)
		{
			SetObjectType(Type::eNone);
			for (auto block : objects)
			{
				Vector2 vec(-v.x, -v.y);
				float len = vec.Length();
				if (len == 0)break;
				vec = vec / len;
				while (Vector2::RectCollision(block->GetPosition(), block->GetColSize(), it->GetPosition(), it->GetColSize()))
				{
					it->Translate(vec);
				}
			}
		}


		Vector2 offset(v.x == 0 ? 0 : v.x / fabsf(v.x), v.y == 0 ? 0 : v.y / fabsf(v.y));
		Vector2 colSize = it->GetColSize();
		Vector2 itPos = it->GetPosition();
		if (GetScene()->OnCollision(itPos + offset * BLOCKSIZE, colSize, it, Type::eSolid).size() > 0)
		{
			if (((Player*)it)->IsDying())continue;
			SetObjectType(Type::eNone);
			((Player*)it)->HitGolemArm(0);
			continue;
		}

		auto objects2 = GetScene()->OnCollision(it->GetPosition(), it->GetColSize(), it, Object::Type::eSolid);
		int findCount2 = objects2.size();
		if (findCount2 > 0)
		{
			for (auto block : objects2)
			{
				Vector2 vec(-mMoveSpeed.x, -mMoveSpeed.y);
				float len = vec.Length();
				if (len == 0)break;
				vec = vec / len;
				while (Vector2::RectCollision(block->GetPosition(), block->GetColSize(), it->GetPosition(), it->GetColSize()))
				{
					it->Translate(vec);
				}
			}
		}
	}
	mObject.clear();
}

void GolemBlock::SetActive()
{
	SetObjectType(Type::eSolid);
	mIndexX = StageManager::ToIndexX(GetPosition().x);
	mIndexY = StageManager::ToIndexY(GetPosition().y);
	//if (mIndexX >= 0 && mIndexY >= 0)
		//GetScene()->GetStageManager()->SetGolemStageData(StageManager::ObjectLayer::eStageObjectLayerBlock, 1, mIndexX, mIndexY);
}

void GolemBlock::SetInactive()
{
	SetObjectType(Type::eNone);
	//GetScene()->GetStageManager()->SetGolemStageData(StageManager::ObjectLayer::eStageObjectLayerBlock, 0, mIndexX, mIndexY);
}

GolemWall::GolemWall(AbstractGolemArm * owner, Scene * scene, int indexX, int indexY, int golemIndexX, int golemIndexY)
	: GolemWall(owner, scene, StageManager::ToPosition(indexX, indexY), golemIndexX, golemIndexY)
{
}

GolemWall::GolemWall(AbstractGolemArm * owner, Scene * scene, Vector2 pos, int golemIndexX, int golemIndexY)
	: GolemObject(owner, scene, pos, golemIndexX, golemIndexY)
{
	mCASprite = new ComponentAnimationSprite(this, TextureTag::Invisible, 49);
	owner->SetBlockData(1, GetGolemIndexX(), GetGolemIndexY());
}

void GolemWall::SetActive()
{
	SetObjectType(Type::eWall);
	mIndexX = StageManager::ToIndexX(GetPosition().x);
	mIndexY = StageManager::ToIndexY(GetPosition().y);
	if(mIndexX >= 0 && mIndexY >= 0)
		GetScene()->GetStageManager()->SetGolemStageData(StageManager::ObjectLayer::eStageObjectLayerWall, 1, mIndexX, mIndexY);
	SetInactive();
}

void GolemWall::SetInactive()
{
	SetObjectType(Type::eNone);
	GetScene()->GetStageManager()->SetGolemStageData(StageManager::ObjectLayer::eStageObjectLayerWall, 0, mIndexX, mIndexY);
}

GolemBuildable::GolemBuildable(AbstractGolemArm * owner, Scene * scene, int indexX, int indexY, int golemIndexX, int golemIndexY)
	: GolemBuildable(owner, scene, StageManager::ToPosition(indexX, indexY), golemIndexX, golemIndexY)
{
}

GolemBuildable::GolemBuildable(AbstractGolemArm * owner, Scene * scene, Vector2 pos, int golemIndexX, int golemIndexY)
	: GolemObject(owner, scene, pos, golemIndexX, golemIndexY)
{
	mCASprite = new ComponentAnimationSprite(this, TextureTag::Invisible, 50);
	mCASprite->SetBlue(0x00);
	mCASprite->SetRed(0x00);
	mCASprite->SetAlphaRate(0);
}

void GolemBuildable::SetActive()
{
	mIndexX = StageManager::ToIndexX(GetPosition().x);
	mIndexY = StageManager::ToIndexY(GetPosition().y);
	if (mOwner->GetBlockData(GetGolemIndexX(), GetGolemIndexY()) == 0)
	{
		GetScene()->GetStageManager()->SetBuildableStageData(1, mIndexX, mIndexY);
		mCASprite->SetAlphaRate(0.6f);
	}
}

void GolemBuildable::SetInactive()
{
	GetScene()->GetStageManager()->SetBuildableStageData(0, mIndexX, mIndexY);
	mCASprite->SetAlphaRate(0);
}

GolemCrystal::GolemCrystal(Scene * scene, Vector2 pos, float rotation)
	: Object(scene, pos)
{
	mCASprite = new ComponentAnimationSprite(this, TextureTag::Crystal, 6);
	SetSize(BLOCKSIZE * Vector2(7, 7));
	SetRotation(rotation);
}
