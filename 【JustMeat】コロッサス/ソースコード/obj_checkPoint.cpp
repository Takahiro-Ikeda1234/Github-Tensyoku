#include "obj_checkPoint.h"
#include "StageManager.h"
#include "ComponentAnimationSprite.h"
#include "Scene.h"
#include "obj_player.h"
#include "ParticleSystem.h"

CheckPoint* CheckPoint::mCP[MAX_OBJECT_CH];

CheckPoint::CheckPoint(Scene * scene, int indexX, int indexY, int id)
	: CheckPoint(scene, StageManager::ToPosition(indexX, indexY), id)
{
}

CheckPoint::CheckPoint(Scene * scene, Vector2 pos, int id)
	: Object(scene, pos)
	, mID(id)
{
	mCP[id] = this;
}

CheckPoint::~CheckPoint()
{
	mCP[mID] = nullptr;
}

void CheckPoint::UpdateObject(float deltaTime)
{
	return;
}

void CheckPoint::ActiveCP(int id)
{

	if (!CheckPoint::mCP[id])return;
	ParticleInfo p;
	p.addBlend = true;
	p.degMin = 270;
	p.degMax = 270;
	p.speedMin = 0.5f * BLOCKSIZE;
	p.speedMax = 2.5f * BLOCKSIZE;
	p.tex = TextureTag::Spark;
	p.lifetimeMin = 0.45;
	p.lifetimeMax = 0.55f;
	p.redRateMin = 0;
	p.redRateMax = 0;
	p.greenRateMin = 0.5f;
	p.greenRateMax = 0.5f;
	p.spawnType = SpawnType::eLine;
	p.spawnLineLength = BLOCKSIZE;
	p.particlePerGenerateMin = 5;
	p.particlePerGenerateMax = 10;
	p.generateIntervalMin = 0.01f;
	p.generateIntervalMax = 0.02f;
	p.alphaRateMin = 0.25f;
	p.alphaRateMin = 0.5f;
	CheckPoint::mCP[id]->mParticle = new ParticleSystem(mCP[id]->GetScene(), mCP[id]->GetPosition() + Vector2(0, 0.4f * BLOCKSIZE), &p, 0);
}

void CheckPoint::InactiveCP(int id)
{
	if (!CheckPoint::mCP[id])return;
	if (!CheckPoint::mCP[id]->mParticle)return;
	CheckPoint::mCP[id]->mParticle->SetState(State::eDead);
	CheckPoint::mCP[id]->mParticle = nullptr;
}

void CheckPoint::LoadCheckPointData()
{
	Object::LoadCheckPointData();
	if (CheckPoint::mCP[mID]->mParticle)
	{
		ParticleInfo p;
		p.addBlend = true;
		p.degMin = 270;
		p.degMax = 270;
		p.speedMin = 0.5f * BLOCKSIZE;
		p.speedMax = 2.5f * BLOCKSIZE;
		p.tex = TextureTag::Spark;
		p.lifetimeMin = 0.45;
		p.lifetimeMax = 0.55f;
		p.redRateMin = 0;
		p.redRateMax = 0;
		p.greenRateMin = 0.5f;
		p.greenRateMax = 0.5f;
		p.spawnType = SpawnType::eLine;
		p.spawnLineLength = BLOCKSIZE;
		p.particlePerGenerateMin = 5;
		p.particlePerGenerateMax = 10;
		p.generateIntervalMin = 0.01f;
		p.generateIntervalMax = 0.02f;
		p.alphaRateMin = 0.25f;
		p.alphaRateMin = 0.5f;
		CheckPoint::mCP[mID]->mParticle = new ParticleSystem(mCP[mID]->GetScene(), mCP[mID]->GetPosition() + Vector2(0, 0.5f * BLOCKSIZE), &p, 0);

	}
}
