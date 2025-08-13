#include "obj_fragileFallRock.h"
#include"ComponentMove.h"
#include"obj_player.h"
#include"obj_golemEvent.h"
#include"ParticleSystem.h"
#include"AudioData.h"
FragileFallRock::FragileFallRock(Scene * scene, int indexX, int indexY)
	: StageObject(scene, indexX, indexY)
{
	SetObjectType(Type::eSolid);
	mCASprite = new ComponentAnimationSprite(this, TextureTag::FallRock, 3);
	mMove = new ComponentMove(this);
	mMove->SetUseGravity(false);
	mMove->SetNoCollision(true);
	mSpawnPos = GetPosition();
	SetSize(Vector2(2, 3) * BLOCKSIZE);
}

void FragileFallRock::UpdateStageObject(float deltaTime)
{
	mMove->Collision();
	SetPosition(mMove->GetPosition());
	if (Vector2::RectCollision(GetScene()->GetPlayer()->GetPosition(), GetScene()->GetPlayer()->GetColSize(), GetPosition(), GetColSize() * 0.95f))
	{
		if (GetScene()->GetPlayer()->IsDying() == false)
		{
			GetScene()->GetPlayer()->HitGolemArm(0);
		}
	}
	//if (mMove->IsLanding())
	if (mMove->GetUseGravity())
	{
		if (GetScene()->OnCollision(GetPosition() + Vector2(0, 0.1f) * BLOCKSIZE, GetColSize(), this, Type::eSolid).size() > 0 && !GetScene()->GetGolem()->HitGolemArm(GetPosition(), GetColSize()))
		{
			SetObjectType(Type::eNone);
			SetState(State::eInactive);
			GenerateFragments();
			GenerateSmoke();
			GenerateSpark();
			PlaySE(SE_BREAK);
		}
	}
	mFallDelay -= deltaTime;
	if (mFallDelay.IsFinish())
	{
		mMove->SetUseGravity(true);
	}
}

void FragileFallRock::EnterGolemArm()
{
	mFallDelay = 0.2f;
}

void FragileFallRock::LoadCheckPointData()
{
	StageObject::LoadCheckPointData();
	SetPosition(mSpawnPos);
	mMove->SetUseGravity(false);
	mMove->SetSpeed(Vector2(0, 0));
	mFallDelay = 0;
}

void FragileFallRock::GenerateFragments()
{
	ParticleInfo info;
	info.tex = TextureTag::Fragment1;
	info.spawnLineFan = true;
	info.spawnType = SpawnType::eLine;
	info.spawnLineLength = GetSize().x;
	info.spawnLineWidth = GetSize().y;
	info.generateIntervalMax = 0;
	info.generateIntervalMin = 0;
	info.scaleMin = 0.25f;
	info.scaleMax = 0.5f;
	info.speedMin = BLOCKSIZE * 1;
	info.speedMax = BLOCKSIZE * 1;
	info.useGravity = true;
	info.lifetimeMin = 1;
	info.lifetimeMax = 1.5f;
	info.alphaLifetimeChangeType = LifetimeChangeType::eCos90;
	info.particlePerGenerateMin = 20;
	info.particlePerGenerateMax = 20;
	info.moveForward = true;
	info.fragile = true;
	new ParticleSystem(GetScene(), GetPosition(), &info, 0, true);
	info.tex = TextureTag::Fragment2;
	info.degMin = 225;
	info.degMax = 315;
	info.speedMin = BLOCKSIZE * 3;
	info.speedMax = BLOCKSIZE * 5;
	new ParticleSystem(GetScene(), GetPosition(), &info, 0, true);
	info.tex = TextureTag::Fragment4;
	info.degMin = 0;
	info.degMax = 180;
	new ParticleSystem(GetScene(), GetPosition(), &info, 0, true);
}

void FragileFallRock::GenerateSmoke()
{
	ParticleInfo info;
	info.tex = TextureTag::Smoke;
	info.greenRateMax = 0.7f;
	info.greenRateMin = 0.7f;
	info.blueRateMax = 0.3f;
	info.blueRateMin = 0.3f;
	info.particlePerGenerateMax = 20;
	info.particlePerGenerateMin = 20;
	info.speedMin = 3 * BLOCKSIZE;
	info.speedMax = 4.5f * BLOCKSIZE;
	info.greenLifetimeChangeType = LifetimeChangeType::eCos90;
	info.redLifetimeChangeType = LifetimeChangeType::eCos90;
	info.blueLifetimeChangeType = LifetimeChangeType::eCos90;
	info.speedLifetimeChangeType = LifetimeChangeType::eCos90Half;
	info.scaleLifetimeChangeType = LifetimeChangeType::eCos90;
	info.scaleMin = 2.5f;
	info.scaleMax = 5;
	info.generateIntervalMin = 0.05f;
	info.generateIntervalMax = 0.15f;
	info.lifetimeMin = 1;
	info.lifetimeMax = 1.5f;
	info.addBlend = true;
	info.alphaRateMin = 0.15f;
	info.alphaRateMax = 0.15f;
	info.alphaLifetimeChangeType = LifetimeChangeType::eCos90;
	info.spawnType = SpawnType::eCircle;
	info.spawnCircleRadiusMin = 0;
	info.spawnCircleRadiusMax = BLOCKSIZE / 2;
	info.useGravity = true;
	info.gravity = Vector2(BLOCKSIZE * 2, -GRAVITY / 10);
	new ParticleSystem(GetScene(), GetPosition() + Vector2(0, GetColSize().y), &info, 0.2f, false);
}

void FragileFallRock::GenerateSpark()
{
	ParticleInfo info;
	info.useGravity = true;
	info.generateIntervalMin = 0;
	info.generateIntervalMax = 0;
	info.tex = TextureTag::Spark;
	info.moveBackward = true;
	info.particlePerGenerateMin = 40;
	info.particlePerGenerateMax = 40;
	info.speedMin = BLOCKSIZE * 15;
	info.speedMax = BLOCKSIZE * 30;
	info.scaleMin = 0.2f;
	info.scaleMax = 0.4f;
	info.degMin = 190;
	info.degMax = 350;
	info.size = Vector2(BLOCKSIZE, BLOCKSIZE * 5);
	info.lifetimeMin = 0.2f;
	info.lifetimeMax = 0.3f;
	info.alphaLifetimeChangeType = LifetimeChangeType::eCos90;
	info.scaleLifetimeChangeType = LifetimeChangeType::eCos90;
	info.speedLifetimeChangeType = LifetimeChangeType::eCos90;
	info.addBlend = true;
	new ParticleSystem(GetScene(), GetPosition() + Vector2(0, GetColSize().y), &info, 0, true);
}


