#include "obj_fragileBlock.h"
#include "ParticleSystem.h"
#include "AudioData.h"

FragileBlock::FragileBlock(Scene * scene, int indexX, int indexY, int type)
	: StageObject(scene, indexX, indexY)
{
	switch (type)
	{
	case 0:
		SetSize(BLOCKSIZE * Vector2(5, 5));
		mCASprite = new ComponentAnimationSprite(this, TextureTag::FragileRock1);
		break;
	case 1:
		SetSize(BLOCKSIZE * Vector2(5, 6));
		mCASprite = new ComponentAnimationSprite(this, TextureTag::FragileRock2);
		Translate(Vector2(0, BLOCKSIZE / 2));
		break;
	case 2:
		SetSize(BLOCKSIZE * Vector2(7, 5));
		mCASprite = new ComponentAnimationSprite(this, TextureTag::FragileRock3);
		break;
	}
	SetObjectType(Type::eSolid);
}

void FragileBlock::EnterGolemArm()
{
	SetState(State::eInactive);
	SetObjectType(Type::eNone);
	GenerateFragments();
	GenerateSmoke();
	GenerateSpark();
	PlaySE(SE_BREAK);
}

void FragileBlock::GenerateFragments()
{
	ParticleInfo info;
	info.tex = TextureTag::Fragment1;
	info.spawnLineFan = true;
	info.spawnType = SpawnType::eLine;
	info.spawnLineLength = GetSize().x;
	info.spawnLineWidth = GetSize().y;
	info.generateIntervalMax = 0;
	info.generateIntervalMin = 0;
	info.scaleMin = 0.5f;
	info.scaleMax = 1;
	info.speedMin = BLOCKSIZE * 1;
	info.speedMax = BLOCKSIZE * 1;
	info.useGravity = true;
	info.lifetimeMin = 0.25f;
	info.lifetimeMax = 1.5f;
	info.alphaLifetimeChangeType = LifetimeChangeType::eCos90;
	info.particlePerGenerateMin = 20;
	info.particlePerGenerateMax = 20;
	info.moveForward = true;
	new ParticleSystem(GetScene(), GetPosition(), &info, 0, true);
	info.tex = TextureTag::Fragment2;
	info.degMin = 225;
	info.degMax = 315;
	info.speedMin = BLOCKSIZE * 3;
	info.speedMax = BLOCKSIZE * 5;
	new ParticleSystem(GetScene(), GetPosition(), &info, 0, true);
	info.tex = TextureTag::Fragment3;
	info.degMin = 165;
	info.degMax = 270;
	info.particlePerGenerateMin = 10;
	info.particlePerGenerateMax = 10;
	info.speedMin = BLOCKSIZE * 5;
	info.speedMax = BLOCKSIZE * 7;
	new ParticleSystem(GetScene(), GetPosition(), &info, 0, true);
	info.degMin = -90;
	info.degMax = 15;
	info.particlePerGenerateMin = 10;
	info.particlePerGenerateMax = 10;
	info.speedMin = BLOCKSIZE * 5;
	info.speedMax = BLOCKSIZE * 7;
	new ParticleSystem(GetScene(), GetPosition(), &info, 0, true);
	info.tex = TextureTag::Fragment4;
	info.degMin = 0;
	info.degMax = 180;
	new ParticleSystem(GetScene(), GetPosition(), &info, 0, true);
}

void FragileBlock::GenerateSmoke()
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
	info.greenLifetimeChangeType = LifetimeChangeType::eCos90;
	info.redLifetimeChangeType = LifetimeChangeType::eCos90;
	info.blueLifetimeChangeType = LifetimeChangeType::eCos90;
	info.speedLifetimeChangeType = LifetimeChangeType::eCos90Half;
	info.scaleLifetimeChangeType = LifetimeChangeType::eCos90;
	info.scaleMin = 5.5f;
	info.scaleMax = 8;
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
	new ParticleSystem(GetScene(), GetPosition(), &info, 0.2f, false);
}

void FragileBlock::GenerateSpark()
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
	info.size = Vector2(BLOCKSIZE, BLOCKSIZE * 5);
	info.lifetimeMin = 0.2f;
	info.lifetimeMax = 0.3f;
	info.alphaLifetimeChangeType = LifetimeChangeType::eCos90;
	info.scaleLifetimeChangeType = LifetimeChangeType::eCos90;
	info.speedLifetimeChangeType = LifetimeChangeType::eCos90;
	info.addBlend = true;
	new ParticleSystem(GetScene(), GetPosition(), &info, 0, true);
}


