#include"Scene.h"
#include"ComponentAnimationSprite.h"
#include"ComponentMoveClimb.h"
#include"TextureData.h"
#include"myMath.h"
#include"obj_player.h"
#include"CameraManager.h"
#include"keyboard.h"
#include"mouse.h"
#include"StageManager.h"
#include"obj_staminaGauge.h"
#include"ComponentCircleSprite.h"
#include"obj_wall.h"
#include"obj_golemEvent.h"
#include"CloudManager.h"
#include"ParticleSystem.h"
#include"MoveFloorObject.h"
#include"EventManager.h"
#include"obj_warpGate.h"
#include"ini.h"
#include"obj_ghostCat.h"
#include"AudioData.h"
#include"obj_AbstractGolemArm.h"
#include"obj_checkPoint.h"
#include"ComponentSystemMessage.h"

static const float GRAB_HITSTOP_TIME = 0.1f;

bool Player::mAlreadyLoadFile = false;
#define STAMINABLOCK 4	//スタミナ区切り
#define FREEFALL_BLOCK 15 //落下死距離
static int STOP_STAMINA = 0;	//壁で停止中のスタミナ自然回復量
static const int LAND_HEAL_STAMINA = 200;	//地上でのスタミナ自然回復量
static const float CANNOT_STAMINA_HEAL_TIME = 0.1f;	//着地からこの時間は地上でも自然回復しない

static float JUMPBLOCK = 1.0f;
static float JUMPINGTIME = 0.3f;	//長押しにより大ジャンプ可能

static float PLAYER_JUMP_SPEED;
static float PLAYER_HIGHJUMP_SPEED;
static float PLAYER_CLIMBJUMP_SPEED;

static float PLAYER_CLIMB_SPEED;
static float PLAYER_CLIMB_DASH_SPEED;
static float MOVE_STAMINA;
static float DASH_STAMINA;

static float MAX_XSPEED;
static float MIN_XSPEED;
static float PLAYER_ACCEL;




Player::StageChangeData Player::mStageChangeData;

Player::Player(Scene * scene, Vector2 position)
	: Object(scene, position)
{
	LoadSettingFile();
	mState = State::eAwaking;
	mNextMotionTime = 1.0f;
	SaveCheckPointData();

	SetObjectType(Type::ePlayer);
	GetScene()->GetCameraManager()->SetFollowObject(this) ;
	GetScene()->SetPlayer(this);
	//コンポーネントの作成
	mCASprite = new ComponentAnimationSprite(this, TextureTag::Climber, 0);
	mCASprite->SetAnimation(0, 1, 2, 2);
	mMove = new ComponentMove(this);
	mClimb = new ComponentMoveClimb(this);
	mMove->SetUseGravity(true);
	mMove->SetLanding(true);
	mClimb->SetUseGravity(false);
	SetDirection(Dir::eRight);
	mStaminaGauge = new StaminaGauge(GetScene(), GetPosition() - Vector2(BLOCKSIZE * 2, BLOCKSIZE));
	SetSize(Vector2(BLOCKSIZE * 0.9f, BLOCKSIZE * 1.8f));
	Translate(Vector2(0, -GetColSize().y / 2));
	Translate(Vector2(0, -0.2f * BLOCKSIZE));

	mSystemMessage = new ComponentSystemMessage(this, TextureTag::Invisible, 2);
	mSystemMessage->SetString("チェックポイントを通過しました", 0, 15);
	mSystemMessage->SetStringInfo(16, Vector2(GetWindowWidth() * 0.8f, GetWindowHeight() * 0.9f), Vector2(20, 20), false);
}

Player::Player(Scene * scene, int indexX, int indexY)
	: Player(scene, StageManager::ToPosition(indexX, indexY))
{
}

void Player::UpdateObject(float deltaTime)
{
	UpdateBGMVolume(deltaTime);
	if (GetScene()->GetCameraManager()->InCamera(&GetPosition(), &GetColSize()))
	{
		mIsDying = true;
		mIsDead = true;
		mState = State::eFreefallDying;
	}
	int currentCheckpoint = GetScene()->GetStageManager()->GetCheckpointZone(StageManager::ToIndexX(GetPosition().x), StageManager::ToIndexY(GetPosition().y));
	if (currentCheckpoint > mSavedCheckPoint && !mIsDying)
	{
		CheckPoint::InactiveCP(mSavedCheckPoint);
		CheckPoint::ActiveCP(currentCheckpoint);
		mSystemMessage->Appear(2);

		mSavedCheckPoint = currentCheckpoint;
		GetScene()->AllObjectSaveCheckPointData();
	}
	//タイマーの更新
	mNextMotionTime -= deltaTime;
	mCannotClimbTime -= deltaTime;
	mJumpingTime -= deltaTime;
	mCannotStaminaHealTime -= deltaTime;
	mDummyCoolTime -= deltaTime;
	mFootstepTime -= deltaTime;

	if (mFootstepTime.IsEnd() && mMove->IsLanding() && mMove->GetSpeed().x != 0 && mState == State::eLandDefault)
	{
		mFootstepTime = 0.3f;
		PlaySE(SE_FOOTSTEP);
	}
	if (mFootstepTime.IsEnd() && (mClimb->GetSpeed().x != 0 || mClimb->GetSpeed().y != 0) && mState == State::eClimbDefault)
	{
		mFootstepTime = 0.4f;
		PlaySE(SE_CLIMB);
	}

	switch (mState)
	{
	case State::eLandDefault:
		CollisionUpdateLandDefault(deltaTime);
		StateLandDefault(deltaTime);
		break; 
	case State::eClimbDefault:
		CollisionUpdateClimbDefault(deltaTime);
		StateClimbDefault(deltaTime);
		break;
	case State::eClimbJump:
		CollisionUpdateClimbJump(deltaTime);
		StateClimbJump(deltaTime);
		break;
	case State::eEntering:
		StateEntering(deltaTime);
		break;
	case State::eFallDefault:
		CollisionUpdateFall(deltaTime);
		StateFallDefault(deltaTime);
		break;
	case State::eAwaking:
		CollisionUpdateLandDefault(deltaTime);
		StateAwaking(deltaTime);
		break;
	case State::eAwakingClimb:
		CollisionUpdateClimbDefault(deltaTime);
		StateAwakingClimb(deltaTime);
		break;
	case State::eLandDying:
		CollisionUpdateLandDefault(deltaTime);
		StateLandDying(deltaTime);
		break;
	case State::eFallDying:
		CollisionUpdateFall(deltaTime);
		StateFallDying(deltaTime);
		break;
	case State::eFreefallDying:
		CollisionUpdateFall(deltaTime);
		StateFreefallDying(deltaTime);
		break;
	case State::eGrab:
		CollisionUpdateClimbDefault(deltaTime);
		StateGrab(deltaTime);
		break;
	case State::eLanding:
		StateLanding(deltaTime);
		break;
	case State::eLandDown:
		CollisionUpdateLandDefault(deltaTime);
		StateLandDown(deltaTime);
		break;

	case State::eStage0Event1_Move:
		CollisionUpdateClimbDefault(deltaTime);
		StateStage0Event1Move(deltaTime);
		break;
	case State::eStage0Event1_Jump:
		CollisionUpdateFall(deltaTime);
		StateStage0Event1Jump(deltaTime);
		break;
	case State::eStage0Event1_Stay:
		CollisionUpdateLandDefault(deltaTime);
		StateStage0Event1Stay(deltaTime);
		break;
	case State::eStage0Event1_CameraMove:
		CollisionUpdateLandDefault(deltaTime);
		StateStage0Event1CameraMove(deltaTime);
		break;
	case State::eStage0Event2_Thrust:
		CollisionUpdateFall(deltaTime);
		StateStage0Event2Thrust(deltaTime);
		break;
	case State::eStage0Event2_Pause:
		CollisionUpdateFall(deltaTime);
		StateStage0Event2Pause(deltaTime);
		break;
	case State::eStage0Event2_Grab:
		CollisionUpdateClimbDefault(deltaTime);
		StateStage0Event2Grab(deltaTime);
		break;
	case State::eStage0Event2_Fall:
		CollisionUpdateFall(deltaTime);
		StateStage0Event2Fall(deltaTime);
		break;
	}
	mStaminaGauge->SetFill(mStamina / mMaxStamina);
	UpdateAnimation();
	UpdateMoveFloor();
	if (GetScene()->GetSceneManager()->GetDebugMode())
	{
		STOP_STAMINA = 100;
	}
	else
	{
		STOP_STAMINA = 0;
	}

	if (GetScene()->GetGolem()->HitGolemArm(GetPosition(), GetColSize()))
	{
		mGolemCount++;
	}
	else
	{
		mGolemCount = 0;
	}
}
void Player::StateAwaking(float deltaTime)
{
	if (mNextMotionTime.IsEnd())
	{
		mState = State::eLandDefault;
		//SaveCheckPointData();
	}
}
void Player::StateAwakingClimb(float deltaTime)
{
	if (mNextMotionTime.IsEnd())
	{
		mState = State::eClimbDefault;
	}
}
void Player::StateLandDying(float deltaTime)
{
	if (mNextMotionTime.IsEnd())
	{
		mIsDead = true;
	}
}
void Player::StateFallDying(float deltaTime)
{
	if (mNextMotionTime.IsEnd())
	{
		mIsDead = true;
	}
}
void Player::StateFreefallDying(float deltaTime)
{
	if (mNextMotionTime.IsEnd())
	{
		mIsDead = true;
	}
}
void Player::StateLandDefault(float deltaTime)
{
	mIsStaminaZero = false;
	if (mCannotStaminaHealTime.IsEnd())
	{
		HealStamina(LAND_HEAL_STAMINA * deltaTime);
	}
	
	UpdateMoveLandAndFall(deltaTime);
	if (mMove->GetSpeed().y != 0)
	{
		mState = State::eFallDefault;
	}
	CheckWarpGate();
}

void Player::StateFallDefault(float deltaTime)
{
	UpdateMoveLandAndFall(deltaTime);

	if (mMove->GetSpeed().y > GRAVITY * sqrtf(FREEFALL_BLOCK * 2.0f / (GRAVITY / BLOCKSIZE)) && mState != State::eStage0Event2_Fall)
	{
		DyingEffect();
		mIsDying = true;
		mState = State::eFreefallDying;
		GetScene()->GetEventManager()->Fadeout(0.5f, 0.1f);
		mNextMotionTime = 0.5f;
	}
}

void Player::StateClimbDefault(float deltaTime)
{
	UpdateDummy();
	mClimb->SetMoveNoWall(false);
	mClimb->SetUseGravity(false);
	int x = 0;
	int y = 0;
	mMovingDirection = 270;
	if (Keyboard::Inst()->KeyDown(DIK_B) && mCanControl && mHasBlockCount > 0)
	{
		int idxx = StageManager::ToIndexX(GetPosition().x);
		int idxy = StageManager::ToIndexY(GetPosition().y) - 1;
		if (GetScene()->GetStageManager()->GetBuildableStageData(idxx, idxy) == 1)
		{
			GetScene()->GetGolem()->CreateBlock(idxx, idxy);
			mHasBlockCount--;
		}
		else if (GetScene()->GetStageManager()->GetStageData(StageManager::ObjectLayer::eStageObjectLayerWall, idxx, idxy) == 0)
		{
			new Wall(GetScene(), StageManager::ToPosition(idxx, idxy));
			mHasBlockCount--;
		}
	}
	if (Keyboard::Inst()->KeyDown(DIK_A) && mCanControl)
	{
		x += -1;
		SetDirection(Dir::eLeft);
	}
	else if (Keyboard::Inst()->KeyDown(DIK_D) && mCanControl)
	{
		x += 1;
		SetDirection(Dir::eRight);
	}

	if (Keyboard::Inst()->KeyDown(DIK_W) && mCanControl)
	{
		y += -1;
	}
	else if (Keyboard::Inst()->KeyDown(DIK_S) && mCanControl)
	{
		y += 1;
	}

	if (x != 0 || y != 0)
	{

		if (GetScene()->OnCollision(GetPosition(), GetColSize() / 10, this, Type::eWall).size() == 0)
		{
			ClimbToFall();
			mState = State::eFallDefault;
		}
		else if ((Keyboard::Inst()->KeyDown(DIK_LSHIFT) || Keyboard::Inst()->KeyDown(DIK_RSHIFT)) && UseStamina(DASH_STAMINA * deltaTime))
		{
			mMovingDirection = degArctan(x, y);
			mClimb->SetSpeed(Vector2(PLAYER_CLIMB_DASH_SPEED * degCos(mMovingDirection), PLAYER_CLIMB_DASH_SPEED * degSin(mMovingDirection)));
		}
		else if (UseStamina(MOVE_STAMINA * deltaTime))
		{
			mMovingDirection = degArctan(x, y);
			mClimb->SetSpeed(Vector2(PLAYER_CLIMB_SPEED * degCos(mMovingDirection), PLAYER_CLIMB_SPEED * degSin(mMovingDirection)));
		}
		else
		{
			ClimbToFall();
			mState = State::eFallDefault;
			mIsStaminaZero = true;
		}
	}
	else
	{
		HealStamina(STOP_STAMINA * deltaTime);
		mClimb->SetSpeed(Vector2(0, 0));
	}
	if (Keyboard::Inst()->KeyHit(DIK_SPACE) && mCanControl)
	{
		mMovingDirection = 270;
		if (Keyboard::Inst()->KeyDown(DIK_D) > 0)
		{
			mMovingDirection = 300;
		}
		else if (Keyboard::Inst()->KeyDown(DIK_A) > 0)
		{
			mMovingDirection = 240;
		}
		if (Keyboard::Inst()->KeyDown(DIK_S))
		{
			ClimbToFall();
			mState = State::eFallDefault;
		}
		else if(JumpUseStamina())
		{
			mClimb->SetSpeed(Vector2(PLAYER_CLIMBJUMP_SPEED * degCos(mMovingDirection), PLAYER_CLIMBJUMP_SPEED * degSin(mMovingDirection)));
			mClimb->SetMoveNoWall(true);
			mClimb->SetUseGravity(true);
			mMove->SetCanJump(false);
			mState = State::eClimbJump;
			mNextMotionTime = 0.2f;
			PlaySE(SE_JUMP);
		}
	}

	if (y > 0 && GetScene()->OnCollision(GetPosition() + Vector2(0, 0.1f * BLOCKSIZE), GetColSize(), this, Type::eSolid).size() > 0)
	{
		mState = State::eLandDefault;
		ClimbToFall();
	}
}

void Player::StateClimbJump(float deltaTime)
{
	mClimb->SetMoveNoWall(true);
	mClimb->SetUseGravity(true);
	if (mNextMotionTime.IsEnd())
	{

		if (GetScene()->OnCollision(GetPosition(), GetColSize() / 10, this, Object::Type::eWall).size() == 0)
		{
			mMove->SetSpeed(mClimb->GetSpeed());
			mState = State::eFallDefault;
		}
		else
		{
			mState = State::eClimbDefault;
		}
	}
}

void Player::StateEntering(float deltaTime)
{
	if (mNextMotionTime.IsEnd())
	{
		mInCave = !mInCave;
		auto objects = GetScene()->FindObjects(this, Type::eWarpGate);
		for (auto it : objects)
		{
			if (Vector2::RectCollision(it->GetPosition(), it->GetColSize(), GetPosition(), GetColSize() * Vector2(0.5f, 1)))
			{
				mState = State::eLandDefault;
				GetScene()->GetEventManager()->Fadein(1.0f, 0);
				SetPosition(((WarpGate*)it)->GetConnect()->GetPosition());
				
				GetScene()->GetCat()->WarpGate();
				CancelDummy(false);
				break;
			}
		}
	}
}

void Player::StateGrab(float deltaTime)
{
	if (mNextMotionTime.IsEnd())
	{
		mState = State::eClimbDefault;
	}
}

void Player::StateLanding(float deltaTime)
{
	if (mNextMotionTime.IsEnd())
	{
		mState = State::eLandDefault;
	}
}

void Player::StateLandDown(float deltaTime)
{
	if (mNextMotionTime.IsEnd())
	{
		mState = State::eLandDying;
		mNextMotionTime = 2.0f - mNextMotionTime.GetMaxTime();
	}
}


void Player::CollisionUpdateLandDefault(float deltaTime)
{
	mMove->Collision();
	SetPosition(mMove->GetPosition());

}
void Player::CollisionUpdateClimbDefault(float deltaTime)
{
	if (!mClimb->Collision())
	{
		ClimbToFall();
		if (!mIsDying)
		{
			mState = State::eFallDefault;
		}
		else
		{
			mState = State::eFallDying;
			DyingEffect();
		}
	}
	SetPosition(mClimb->GetPosition());

}
void Player::CollisionUpdateClimbJump(float deltaTime)
{
	if (!mClimb->Collision())
	{
		if (GetScene()->OnCollision(GetPosition(), GetColSize() / 10, this, Object::Type::eWall).size() == 0)
		{
			mMove->SetSpeed(mClimb->GetSpeed());
			if (!mIsDying)
			{
				mState = State::eFallDefault;
			}
			else
			{
				mState = State::eFallDying;
				DyingEffect();
			}
		}
		else
		{
			mState = State::eClimbDefault;
		}

	}

	SetPosition(mClimb->GetPosition());
}

void Player::CollisionUpdateFall(float deltaTime)
{
	float ysp = mMove->GetSpeed().y;
	mMove->Collision();
	SetPosition(mMove->GetPosition());
	if (mMove->IsLanding())
	{
		mCannotClimbTime = 0;
		mMove->SetCanJump(true);
		if (!mIsDying)
		{
			mFootstepTime = 0.3f;
			PlaySE(SE_FOOTSTEP);
			mState = State::eLandDefault;
			if (ysp / BLOCKSIZE > 30)
			{
				mState = State::eLanding;
				mNextMotionTime = 0.1f;
			}
			mCannotStaminaHealTime = CANNOT_STAMINA_HEAL_TIME;
		}
		else
		{
			mMove->SetSpeed(Vector2(0, 0));
			mNextMotionTime = mNextMotionTime.GetLeftTime();
			mState = State::eLandDying;
		}
	}
}

bool Player::CanUpdateCheckPoint()
{
	switch (mState)
	{
	case State::eLandDefault:
	case State::eClimbDefault:
	case State::eClimbJump:
	case State::eFallDefault:
	case State::eAwaking:
	case State::eAwakingClimb:
	case State::eEntering:
	case State::eGrab:
	case State::eLanding:
		return true;
	case State::eLandDying:
	case State::eFallDying:
	case State::eFreefallDying:
	case State::eStage0Event1_Move:
	case State::eStage0Event1_Jump:
	case State::eStage0Event1_Stay:
	case State::eStage0Event1_CameraMove:
	case State::eStage0Event2_Thrust:
	case State::eStage0Event2_Pause:
	case State::eStage0Event2_Grab:
	case State::eStage0Event2_Fall:
		return false;


	}
	return false;
}
void Player::HitGolemArm(int hitType)
{
	if (mIsDying)return;
	if (hitType == 0)
	{
		PlaySE(SE_HITARM);
	}
	else if(hitType == 1)
	{
		PlaySE(SE_HITLASER);
	}
	mIsDying = true;
	GetScene()->GetEventManager()->SetHitstop(0.2f);
	switch (mState)
	{
	case State::eAwakingClimb:
	case State::eClimbJump:
	case State::eGrab:
	case State::eFallDying:
	case State::eFreefallDying:
	case State::eClimbDefault:
	case State::eFallDefault:
		ClimbToFall();
		mState = State::eFallDying;
		DyingEffect();
		mNextMotionTime = 2.0f;
		break;

	case State::eAwaking:
	case State::eEntering:
	case State::eLanding:
	case State::eLandDying:
	case State::eLandDefault:
		mMove->SetSpeed(Vector2(0, 0));
		mState = State::eLandDown;
		DyingEffect();
		mNextMotionTime = 0.1f;
		break;
	}
}

void Player::HitLaser(int hitType)
{
	HitGolemArm(hitType);
}

bool Player::JumpUseStamina()
{
	float blockStamina = mMaxStamina / STAMINABLOCK;
	int curBlock = ceil(mStamina / blockStamina);
	if (curBlock > 0)
	{
		mStamina = (curBlock - 1)*blockStamina;
		return true;
	}
	else
	{
		return false;
	}
}

void Player::StageChangeSave()
{
	mStageChangeData.animframe = mCASprite->GetNowFrame();
	mStageChangeData.offsetCloudGenerator = GetPosition() - CloudManager::Inst()->GetTopCloudGenPos();
}

void Player::StageChangeLoad()
{
	mCASprite->SetNowFrame(mStageChangeData.animframe);
	mNextMotionTime = 5.5f;
	mState = State::eAwakingClimb;
	SetPosition(CloudManager::Inst()->GetBottomCloudGenPos() + mStageChangeData.offsetCloudGenerator);
	LandToClimb();
	
}

void Player::WoodJump()
{
	mMove->SetSpeedY(BLOCKSIZE * -25);
	mState = State::eFallDefault;
}


bool Player::IsLanding()
{
	return mMove->IsLanding();
}

void Player::CancelDummy(bool makeEffect)
{
	GetScene()->GetCat()->EndDummy(makeEffect);
	mIsUsingDummy = false;
	mDummyCoolTime = 2.0f;
}

void Player::DyingEffect()
{
	if (!GetScene()->GetSceneManager()->CanSceneChange())return;
	GetScene()->GetEventManager()->SetSlowSpeed(0.1f);
	GetScene()->GetEventManager()->FadeSlow(0.2f, 0.3f, 0.2f);
}

void Player::UpdateBGMVolume(float deltaTime)
{
	if (!mCanControl)
	{
		SetVolumeBGM(5, 0);
		SetVolumeBGM(6, 0);
		return;
	}

	auto objects = GetScene()->FindObjects(this, Object::Type::eLaser);
	float nearestDist = 65535;
	for (auto it : objects)
	{
		float dist = Vector2::Distance(GetPosition(), it->GetPosition());
		if (dist < nearestDist)
		{
			nearestDist = dist;
		}
	}
	int volume = 0;
	if (nearestDist < BLOCKSIZE * 10)  volume = 100;
	else if (nearestDist > BLOCKSIZE * 30)volume = 0;
	else volume = 100 * (degCos((nearestDist - BLOCKSIZE * 10) / (BLOCKSIZE * 20) * 90));
	SetVolumeBGM(5, volume);



	nearestDist = 65535;
	float dist = Vector2::Distance(GetPosition(), CloudManager::Inst()->GetBottomCloudGenPos());
	if (dist < nearestDist) nearestDist = dist;
	dist = Vector2::Distance(GetPosition(), CloudManager::Inst()->GetTopCloudGenPos());
	if (dist < nearestDist) nearestDist = dist;

	volume = 0;
	if (nearestDist < BLOCKSIZE * 5)  volume = 100;
	else if (nearestDist > BLOCKSIZE * 20)volume = 0;
	else volume = 100 * (degCos((nearestDist - BLOCKSIZE * 5) / (BLOCKSIZE * 15) * 90));
	SetVolumeBGM(6, volume);
}

void Player::UpdateDummy()
{
	if (Keyboard::Inst()->KeyHit(DIK_R) && mCanControl)
	{
		if (GetScene()->GetStageManager()->GetCurrentStageID() == 0) return;
		if (mIsUsingDummy)
		{
			CancelDummy(true);
		}
		else if (mDummyCoolTime.IsEnd())
		{
			if (mState == State::eClimbDefault || mState == State::eLandDefault)
			{
				mIsUsingDummy = true;
				GetScene()->GetCat()->StartDummy(mState == State::eClimbDefault, GetDirection());
				PlaySE(SE_DUMMY);
			}
		}
	}
}

void Player::UpdateAnimation()
{
	switch (mState)
	{
	case State::eLandDown:
		mCASprite->SetAnimation(17, 18, 2, 16, false);
		break;
	case State::eLandDying:
		mCASprite->SetAnimation(14, 14, 3, 8, false);
		break;
	case State::eStage0Event1_CameraMove:
	case State::eStage0Event1_Stay:
	case State::eEntering:
	case State::eLandDefault:
		if (mMove->GetSpeed().x == 0)
		{
			mCASprite->SetAnimation(0, 1, 2, 2);
		}
		else
		{
			mCASprite->SetAnimation(2, 3, 6, 6);
		}
		break;
	case State::eAwaking:
		mCASprite->SetAnimation(15, 16, 6, 6, false);
		break;
	case State::eStage0Event2_Thrust:
	case State::eStage0Event1_Jump:
	case State::eFallDefault:
		if (mMove->GetSpeed().y < 0)
		{
			mCASprite->SetAnimation(4, 5, 1, 0);
		}
		else
		{
			mCASprite->SetAnimation(6, 7, 1, 0);
		}
		break;
	case State::eStage0Event2_Grab:
	case State::eGrab:
		mCASprite->SetAnimation(8, 9, 3, 10, false);
		break;
	case State::eLanding:
		mCASprite->SetAnimation(12, 13, 1, 0);
		break;
	case State::eStage0Event2_Pause:
	case State::eStage0Event2_Fall:
	case State::eFallDying:
	case State::eFreefallDying:
		mCASprite->SetAnimation(10, 10, 3, 8, false);
		break;
	case State::eStage0Event1_Move:
	case State::eAwakingClimb:
	case State::eClimbDefault:
		if (mClimb->GetSpeed().x == 0 && mClimb->GetSpeed().y == 0)
		{
			mCASprite->SetAnimation(11, 11, 1, 1);
		}
		else
		{
			mCASprite->SetAnimation(11, 11, 4, 4);
		}
		break;
	}

}

void Player::UpdateMoveFloor()
{
	auto movefloor = GetScene()->GetMoveFloors();
	for (auto it : movefloor)
	{

		if (it->GetObjectType() == Type::eSolid && (mState == State::eLandDefault || mState == State::eLandDying))
		{
			if (Vector2::RectCollision(it->GetPosition(), it->GetColSize(), GetPosition(), Vector2(5, GetColSize().y) * 1.1f))
			{
				it->Assign(this);
			}
		}

		if (it->GetObjectType() == Type::eWall && (mState == State::eClimbDefault || mState == State::eStage0Event2_Grab || mState == State::eClimbJump))
		{
			Vector2 colSize = GetColSize() / 10;
			if (it->IsGolemArm())colSize = GetColSize();
			if (Vector2::RectCollision(it->GetPosition(), it->GetColSize(), GetPosition(), colSize))
			{
				it->Assign(this);
			}
		}

		if (it->IsGolemArm())
		{
			if (it->GetObjectType() == Type::eSolid)
			{
				if ((mState != State::eClimbDefault && mState != State::eClimbJump))continue;
				if (Vector2::RectCollision(it->GetPosition(), it->GetColSize(), GetPosition(), Vector2(GetColSize().x, GetColSize().y * 1.1f)))
				{
					if (((AbstractGolemArm*)it)->IsHorizontal())
					{
						if (!((AbstractGolemArm*)it)->IsRight())
						{
							if (Vector2::RectCollision(it->GetPosition() + Vector2(it->GetColSize().x, 0), it->GetColSize(), GetPosition(), GetColSize()))
							{
								it->Assign(this);
								continue;
							}
						}
						else
						{
							if (Vector2::RectCollision(it->GetPosition() - Vector2(it->GetColSize().x * 3, 0), it->GetColSize(), GetPosition(), GetColSize()))
							{
								it->Assign(this);
								continue;
							}
						}
					}
					else
					{
						if (!((AbstractGolemArm*)it)->IsRight())
						{
							if (Vector2::RectCollision(it->GetPosition() + Vector2(0, it->GetColSize().y), it->GetColSize(), GetPosition(), GetColSize()))
							{
								it->Assign(this);
								continue;
							}
						}
						else
						{
							if (Vector2::RectCollision(it->GetPosition() - Vector2(0, it->GetColSize().y * 3), it->GetColSize(), GetPosition(), GetColSize()))
							{
								it->Assign(this);
								continue;
							}
						}
					}
				}
			}
		}
	}
}

void Player::UpdateMoveLandAndFall(float deltaTime)
{
	UpdateDummy();
	int x = 0;
	if (Keyboard::Inst()->KeyDown(DIK_A) && mCanControl)
	{
		x += -1;
		SetDirection(Dir::eLeft);
	}
	else if (Keyboard::Inst()->KeyDown(DIK_D) && mCanControl)
	{
		x += 1;
		SetDirection(Dir::eRight);
	}



	if (mMove->GetSpeed().y >= 0)
	{
		mJumpingTime = 0;
	}
	if (Keyboard::Inst()->KeyDown(DIK_SPACE) > 0 && mCanControl && !mMove->CanJump() && !mJumpingTime.IsEnd())
	{
		//float dt = (1 - degCos((1 - mJumpingTime.Percentage()) * 90)) * JUMPINGTIME;
		float dt = (1 - mJumpingTime.Percentage()) * JUMPINGTIME;
		float x = PLAYER_HIGHJUMP_SPEED + GRAVITY;
		mMove->SetSpeedY(PLAYER_JUMP_SPEED + x * dt);
	}
	else
	{
		mJumpingTime = 0;
	}
	if (Keyboard::Inst()->KeyHit(DIK_SPACE) && mCanControl)
	{
		if (mMove->CanJump() && JumpUseStamina())
		{
			mMove->SetCanJump(false);
			mMove->SetLanding(false);
			mMove->SetSpeedY(PLAYER_JUMP_SPEED);
			mJumpingTime = JUMPINGTIME;
			PlaySE(SE_JUMP);
		}
	}
	if (Keyboard::Inst()->KeyDown(DIK_W) && mCanControl && mCannotClimbTime.IsEnd() && mStamina > 10)
	{
		if (GetScene()->OnCollision(GetPosition(), GetColSize() / 10, this, Type::eWall).size() > 0)
		{
			mState = State::eClimbDefault;
			LandToClimb();
			mClimb->SetUseGravity(false);
		}
	}

	if (x != 0)
	{
		if (mMove->IsLanding())
		{
			if (x == 1 && mMove->GetSpeed().x < MIN_XSPEED)
			{
				mMove->SetSpeedX(MIN_XSPEED);
			}
			else if (x == -1 && mMove->GetSpeed().x > -MIN_XSPEED)
			{
				mMove->SetSpeedX(-MIN_XSPEED);
			}
			
		}
		mMove->AddSpeed(x * PLAYER_ACCEL * deltaTime, 0);
	}
	else if(mMove->IsLanding())
	{
		mMove->SetSpeedX(0);
	}

	//速度制限
	if (mMove->GetSpeed().x > MAX_XSPEED)
	{
		mMove->SetSpeedX(MAX_XSPEED);
	}
	else if (mMove->GetSpeed().x < -MAX_XSPEED)
	{
		mMove->SetSpeedX(-MAX_XSPEED);
	}
}

void Player::CheckWarpGate()
{
	auto objects = GetScene()->FindObjects(this, Type::eWarpGate);
	if (!Keyboard::Inst()->KeyDown(DIK_W))return;
	for (auto it : objects)
	{
		if (!((WarpGate*)it)->CanEnter())continue;
		if (Vector2::RectCollision(it->GetPosition(), it->GetColSize(), GetPosition(), GetColSize() * Vector2(0.5f, 1)))
		{
			mMove->SetSpeed(Vector2(0, 0));
			mState = State::eEntering;
			mNextMotionTime = 1.0f;
			GetScene()->GetEventManager()->Fadeout(1.0f, 0);
			break;
		}
	}
}

void Player::ClimbToFall()
{
	mMove->SetSpeed(Vector2(0, 0));
	mClimb->SetSpeed(Vector2(0, 0));
	mMove->SetLanding(false);
}

void Player::LandToClimb()
{
	mMove->SetSpeed(Vector2(0, 0));
	mClimb->SetSpeed(Vector2(0, 0));
	mMove->SetLanding(false);
}

void Player::SaveCheckPointData()
{
	mCPInCave = mInCave;
	mCPDir = GetDirection();

	if (mSavedCheckPoint == 0)
	{
		mCheckPointData["xPos"] = GetPosition().x;
		mCheckPointData["yPos"] = GetPosition().y;
	}
	else
	{
		int x;
		int y;
		GetScene()->GetStageManager()->GetCheckpointRestartIndex(mSavedCheckPoint, &x, &y);
		mCheckPointData["xPos"] = StageManager::ToPosition(x, y).x;
		mCheckPointData["yPos"] = StageManager::ToPosition(x, y).y - GetColSize().y / 2;
	}
}

void Player::LoadCheckPointData()
{
	SetPosition(Vector2(mCheckPointData["xPos"], mCheckPointData["yPos"] ));
	GetScene()->GetEventManager()->Fadein(0.5f, 0);
	mMove->SetSpeed(Vector2(0, 0));
	ResetMoved();
	mMove->SetPrevPosition(GetPosition());
	mMove->SetLanding(true);
	mState = State::eAwaking;
	mNextMotionTime = 1.0f;
	mIsDead = false;
	mIsDying = false;
	mIsStaminaZero = false;
	mStamina = mMaxStamina;


	CancelDummy(false);
	mDummyCoolTime = 0;

	mInCave = mCPInCave;

	SetDirection(mCPDir);
}



void Player::Stage0Event1Start(Vector2 destination)
{
	mState = State::eStage0Event1_Move;
	mStage0Event1Move_Destination = destination;
	mStamina = mMaxStamina;
	mStaminaGauge->SetAlphaRate(0);
}

void Player::StateStage0Event1Move(float deltaTime)
{
	if (Vector2::Distance(mStage0Event1Move_Destination, GetPosition()) < BLOCKSIZE	/*適当な距離の値*/)
	{
		ClimbToFall();
		EventData ev01 = EventManager::GetGolemEventArea(0, 1);
		//目的地までにかかる時間から必要なx方向の速度を算出
		float y = (StageManager::ToPosY(ev01.y) - GetPosition().y);
		float yForce = -(BLOCKSIZE * sqrtf(4 * (2.2f * GRAVITY / BLOCKSIZE)));
		float sqrt_bb4ac = sqrtf(pow(yForce, 2) - 2 * GRAVITY * -y);
		float delta = (-yForce + sqrt_bb4ac) / GRAVITY;
		mStage0Event1MoveX = (StageManager::ToPosX(ev01.x) - GetPosition().x) / delta;
		mMove->SetSpeed(Vector2(mStage0Event1MoveX, yForce));
		mState = State::eStage0Event1_Jump;
	}
	else
	{
		float deg = Vector2::degArctan(mStage0Event1Move_Destination - GetPosition());
		mClimb->SetSpeed(Vector2::degVector(deg) * PLAYER_CLIMB_SPEED);
	}
}

void Player::StateStage0Event1Jump(float deltaTime)
{
	mMove->SetSpeedX(mStage0Event1MoveX);
	if (mMove->IsLanding())
	{
		mMove->SetSpeedX(0);
		mNextMotionTime = 2;
		mState = State::eStage0Event1_Stay;
	}
}

void Player::StateStage0Event1Stay(float deltaTime)
{
	if (mNextMotionTime.IsEnd())
	{
		mState = State::eStage0Event1_CameraMove;
		mNextMotionTime = 6.0f;
		GetScene()->GetCameraManager()->SetOffset(GetScene()->GetCameraManager()->GetZoom(), Vector2(0, -8) * BLOCKSIZE, 2, 4, 0.5f);
	}
}

void Player::StateStage0Event1CameraMove(float deltaTime)
{
	if (mNextMotionTime.IsEnd())
	{
		EventData ev02 = EventManager::GetGolemEventArea(0, 2);
		mState = State::eStage0Event2_Thrust;
		float y = (StageManager::ToPosY(ev02.y) - GetPosition().y);
		float ysp = (BLOCKSIZE * sqrtf(1 * (2.2f * GRAVITY / BLOCKSIZE)));
		float sqrt_bb4ac = sqrtf(pow(-ysp, 2) - 2 * GRAVITY * -y);
		float delta = (ysp + sqrt_bb4ac) / GRAVITY;
		mStage0Event1MoveX = (StageManager::ToPosX(ev02.x) - GetPosition().x) / delta;

		mMove->SetSpeed(Vector2(mStage0Event1MoveX, -ysp));
		GetScene()->GetEventManager()->FadeSlow(1.0f, -0.80f);
	}
}

void Player::StateStage0Event2Thrust(float deltaTime)
{
	if (GetScene()->GetEventManager()->GetSlow() < 0.25f)
	{
		GetScene()->GetEventManager()->SetSlowSpeed(0);
		mState = State::eStage0Event2_Pause;
	}
}

void Player::StateStage0Event2Pause(float deltaTime)
{
	if (Keyboard::Inst()->KeyHit(DIK_W))
	{
		GetScene()->GetEventManager()->SetSlowSpeed(1);
		mState = State::eStage0Event2_Grab;
		mNextMotionTime = 2.5f;
		LandToClimb();
	}
}

void Player::StateStage0Event2Grab(float deltaTime)
{
	if (mNextMotionTime.IsEnd())
	{
		ClimbToFall();
		mState = State::eStage0Event2_Fall;
		mNextMotionTime = 1.0f;
		GetScene()->GetEventManager()->Fadeout(0.4f, 0.5f);
	}
}
void Player::StateStage0Event2Fall(float deltaTime)
{
	if (mNextMotionTime.IsEnd())
	{
		Parameter param;
		param.Set("ステージ", 1);
		GetScene()->GetSceneManager()->SceneChange(SceneManager::SceneID::eGame, &param, 0, 0, 0);
	}
}




void Player::LoadSettingFile()
{
	if (mAlreadyLoadFile)return;
	float JumpMin = GetConfigFloat("Player", "JumpMin");
	float JumpMax = GetConfigFloat("Player", "JumpMax");
	float WallJump = GetConfigFloat("Player", "WallJump");
	float JumpTime = GetConfigFloat("Player", "JumpTime");
	float LandSpeedMax = GetConfigFloat("Player", "LandSpeedMax");
	float WallSpeed = GetConfigFloat("Player", "WallSpeed");
	float WallRunSpeed = GetConfigFloat("Player", "WallRunSpeed");
	float WallStamina = GetConfigFloat("Player", "WallStamina");
	float WallRunStamina = GetConfigFloat("Player", "WallRunStamina");

	//計算ミスなのでJumpMax = 3, JumpTime = 0.3fにする
	JUMPINGTIME = JumpTime;
	PLAYER_JUMP_SPEED = -(BLOCKSIZE * sqrtf(JumpMin * (2.2f * GRAVITY / BLOCKSIZE)));
	PLAYER_HIGHJUMP_SPEED = -2 * (((JumpMax + (GRAVITY / BLOCKSIZE) * JumpTime * JumpTime / 2) * BLOCKSIZE) + JumpTime * PLAYER_JUMP_SPEED) / (JumpTime * JumpTime);
	PLAYER_CLIMBJUMP_SPEED = (BLOCKSIZE * sqrtf(WallJump * (2.2f * GRAVITY / BLOCKSIZE)));

	PLAYER_CLIMB_SPEED = WallSpeed * BLOCKSIZE;
	PLAYER_CLIMB_DASH_SPEED = WallRunSpeed * BLOCKSIZE;
	MOVE_STAMINA = WallStamina;
	DASH_STAMINA = WallRunStamina;

	MAX_XSPEED = LandSpeedMax * BLOCKSIZE;
	MIN_XSPEED = MAX_XSPEED / 2;
	PLAYER_ACCEL = ((MAX_XSPEED - MIN_XSPEED) * 8);


	mAlreadyLoadFile = true;
}
