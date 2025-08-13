#include "obj_golemCore.h"
#include "Scene.h"
#include "SceneManager.h"
#include "EventManager.h"
#include "obj_player.h"
#include "ParticleSystem.h"
#include "AudioData.h"
#include "obj_player.h"
GolemCore::GolemCore(Scene * scene, int indexX, int indexY)
	: StageObject(scene, StageManager::ToPosition(indexX, indexY))
{
	mCASprite = new ComponentAnimationSprite(this, TextureTag::Core, 101);
	SetSize(BLOCKSIZE * Vector2(4, 5));
	SetObjectType(Type::eSolid);
}

void GolemCore::UpdateStageObject(float deltaTime)
{
	mExplosionTime -= deltaTime;
	if (mExplosionTime.IsFinish())
	{
		switch (mExplosionCount)
		{
		case 0:
			Explosion1(GetPosition() + Vector2(2, +2.5f) * BLOCKSIZE, 1);
			break;
		case 1:
			Explosion2(GetPosition() + Vector2(2, -2.5f) * BLOCKSIZE, 1);
			break;
		case 2:
			Explosion2(GetPosition() + Vector2(-2, -2.5f) * BLOCKSIZE, 0.5f);
			break;
		case 3:
			Explosion1(GetPosition() + Vector2(-2, +2.5f) * BLOCKSIZE, 0.5f);
			break;
		case 4:
			Explosion3(GetPosition() + Vector2(-1, -6) * BLOCKSIZE, 2);
			break;
		case 5:
			Explosion3(GetPosition() + Vector2(3, -6) * BLOCKSIZE, 2);
			break;
		default:
			break;
		}
		mExplosionCount++;
		if (mExplosionCount == 6)
		{
			Explosion3(GetPosition() + Vector2(rand() % 8 - 4, rand() % 6 - 3) * BLOCKSIZE, 2);
		}
		mExplosionCount = mExplosionCount % 6;
		mExplosionTime = 0.1f;
	}
}

void GolemCore::EnterGolemArm()
{
	Parameter param;
	GetScene()->GetEventManager()->SetSlowSpeed(0.1f);
	GetScene()->GetEventManager()->FadeSlow(0.5f, 2, 0.5f);
	GetScene()->GetSceneManager()->SceneChange(SceneManager::SceneID::eEnding, &param, 5.0f, 5, 3, true);
	GetScene()->GetCameraManager()->SetCameraShake(0.9f * BLOCKSIZE, 1 * BLOCKSIZE, 10);
	Explosion1(GetPosition(), 1);
	PlaySE(SE_EXPLOSION);
	mExplosionTime = 0.1f;
	SetFadeout(4, 100, 0.5f);
	GetScene()->GetPlayer()->SetCanControl(false);
}

void GolemCore::Explosion1(Vector2 pos, float scale)
{
	ParticleInfo info;
	info.tex = TextureTag::Smoke;
	info.greenRateMax = 0.3f;
	info.greenRateMin = 0.3f;
	info.blueRateMax = 0.1f;
	info.blueRateMin = 0.1f;
	info.particlePerGenerateMax = 40;
	info.particlePerGenerateMin = 40;
	info.speedMin = 1 * BLOCKSIZE * scale;
	info.speedMax = 1.5f * BLOCKSIZE * scale;
	info.greenLifetimeChangeType = LifetimeChangeType::eCos90;
	info.redLifetimeChangeType = LifetimeChangeType::eCos90;
	info.blueLifetimeChangeType = LifetimeChangeType::eCos90;
	info.speedLifetimeChangeType = LifetimeChangeType::eCos90Half;
	info.scaleLifetimeChangeType = LifetimeChangeType::eCos90;
	info.scaleMin = 1 * scale;
	info.scaleMax = 3 * scale;
	info.generateIntervalMin = 0.05f;
	info.generateIntervalMax = 0.15f;
	info.lifetimeMin = 1;
	info.lifetimeMax = 1.5f;
	info.addBlend = true;
	info.alphaRateMin = 0.25f;
	info.alphaRateMax = 0.25f;
	info.alphaLifetimeChangeType = LifetimeChangeType::eCos90;
	info.spawnType = SpawnType::eCircle;
	info.spawnCircleRadiusMin = 0;
	info.spawnCircleRadiusMax = BLOCKSIZE / 2;
	info.useGravity = true;
	info.gravity = Vector2(BLOCKSIZE * 2, -GRAVITY / 10) * scale;
	new ParticleSystem(GetScene(), pos, &info, 0.2f, false);
}
void GolemCore::Explosion2(Vector2 pos, float scale)
{
	ParticleInfo info;
	info.tex = TextureTag::Smoke;
	info.greenRateMax = 0.3f;
	info.greenRateMin = 0.3f;
	info.blueRateMax = 0.1f;
	info.blueRateMin = 0.1f;
	info.particlePerGenerateMax = 40;
	info.particlePerGenerateMin = 40;
	info.speedMin = 1 * BLOCKSIZE * scale;
	info.speedMax = 1.5f * BLOCKSIZE * scale;
	info.greenLifetimeChangeType = LifetimeChangeType::eCos90;
	info.redLifetimeChangeType = LifetimeChangeType::eCos90;
	info.blueLifetimeChangeType = LifetimeChangeType::eCos90;
	info.speedLifetimeChangeType = LifetimeChangeType::eCos90Half;
	info.scaleLifetimeChangeType = LifetimeChangeType::eCos90;
	info.scaleMin = 1 * scale;
	info.scaleMax = 3 * scale;
	info.generateIntervalMin = 0.05f;
	info.generateIntervalMax = 0.15f;
	info.lifetimeMin = 1;
	info.lifetimeMax = 1.5f;
	info.addBlend = true;
	info.alphaRateMin = 0.25f;
	info.alphaRateMax = 0.25f;
	info.alphaLifetimeChangeType = LifetimeChangeType::eCos90;
	info.spawnType = SpawnType::eCircle;
	info.spawnCircleRadiusMin = 0;
	info.spawnCircleRadiusMax = BLOCKSIZE / 2;
	info.useGravity = true;
	info.gravity = Vector2(BLOCKSIZE * -2, -GRAVITY / 10) * scale;
	new ParticleSystem(GetScene(), pos, &info, 0.2f, false);
}

void GolemCore::Explosion3(Vector2 pos, float scale)
{
	ParticleInfo info;
	info.tex = TextureTag::Smoke;
	info.greenRateMax = 0.3f;
	info.greenRateMin = 0.3f;
	info.blueRateMax = 0.1f;
	info.blueRateMin = 0.1f;
	info.particlePerGenerateMax = 40;
	info.particlePerGenerateMin = 40;
	info.speedMin = 1 * BLOCKSIZE * scale;
	info.speedMax = 1.5f * BLOCKSIZE * scale;
	info.greenLifetimeChangeType = LifetimeChangeType::eCos90;
	info.redLifetimeChangeType = LifetimeChangeType::eCos90;
	info.blueLifetimeChangeType = LifetimeChangeType::eCos90;
	info.speedLifetimeChangeType = LifetimeChangeType::eCos90Half;
	info.scaleLifetimeChangeType = LifetimeChangeType::eCos90;
	info.scaleMin = 1 * scale;
	info.scaleMax = 3 * scale;
	info.generateIntervalMin = 0.05f;
	info.generateIntervalMax = 0.15f;
	info.lifetimeMin = 1;
	info.lifetimeMax = 1.5f;
	info.addBlend = true;
	info.alphaRateMin = 0.25f;
	info.alphaRateMax = 0.25f;
	info.alphaLifetimeChangeType = LifetimeChangeType::eCos90;
	info.spawnType = SpawnType::eCircle;
	info.spawnCircleRadiusMin = 0;
	info.spawnCircleRadiusMax = BLOCKSIZE / 2;
	new ParticleSystem(GetScene(), pos, &info, 0.2f, false);
}
