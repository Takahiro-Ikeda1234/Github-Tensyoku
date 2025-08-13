#include "ParticleSystem.h"
#include"Particle.h"
#include"Scene.h"
#include"StageManager.h"
#include"CameraManager.h"

ParticleSystem::ParticleSystem(Scene * scene, Vector2 position, ParticleInfo * info, float lifetime, bool once)
	: Object(scene, position)
	, mLifetime(lifetime)
	, mParticleInfo(*info)
	, mGenerateInterval(randomRange(info->generateIntervalMin, info->generateIntervalMax))
	, mOnce(once)
{
	if (lifetime == 0)
		mPermanent = true;
	if (once && GetScene()->GetCameraManager()->InCamera(&GetPosition(), &Vector2(1, 1)))
	{
		SetState(State::eDead);
	}
}

ParticleSystem::~ParticleSystem()
{
	while (!mParticles.empty())
	{
		delete *mParticles.begin();
		auto itr = mParticles.erase(mParticles.begin());
	}
}

void ParticleSystem::UpdateObject(float deltaTime)
{
	std::list<Particle*> mDestroy;

	for (auto itr = mParticles.begin(); itr != mParticles.end(); )
	{

		(*itr)->Update(deltaTime);
		if ((*itr)->GetLifetimePercentage() == 0)
		{
			delete *itr;
			itr = mParticles.erase(itr);
		}
		else if(mParticleInfo.fragile)
		{
			int indexX = StageManager::ToIndexX((*itr)->GetPosition().x);
			int indexY = StageManager::ToIndexY((*itr)->GetPosition().y);
			if (GetScene()->GetStageManager()->GetStageData(StageManager::ObjectLayer::eStageObjectLayerBlock, indexX, indexY) > 0)
			{
				delete *itr;
				itr = mParticles.erase(itr);
			}
			else
			{
				itr++;
			}
		}
		else
		{
			itr++;
		}
	}
	if (mGenerateInterval.IsEnd())
	{
		mGenerateInterval = randomRange(mParticleInfo.generateIntervalMin, mParticleInfo.generateIntervalMax);
		if (GetScene()->GetCameraManager()->InCamera(&GetPosition(), &Vector2(1, 1)))
		{
			mGenerateInterval -= deltaTime;
			mLifetime -= deltaTime;

			if (!mPermanent && mLifetime.IsEnd() && mParticles.size() == 0)
			{
				SetState(State::eDead);
				return;
			}
			return;
		}
		int generateCount = mParticleInfo.particlePerGenerateMax;
		if (mParticleInfo.particlePerGenerateMax != mParticleInfo.particlePerGenerateMin)
		{
			generateCount = rand() % (mParticleInfo.particlePerGenerateMax - mParticleInfo.particlePerGenerateMin) + mParticleInfo.particlePerGenerateMin;
		}
		for (int i = 0; i < generateCount; i++)
		{
			if (mLifetime.IsEnd() && !mPermanent)break;

			Vector2 generatePos = GetPosition();
			float deg;
			switch (mParticleInfo.spawnType)
			{
			case SpawnType::eCircle:
			{
				float randomDeg = randomRange(0, 360);
				generatePos = generatePos + randomRange(mParticleInfo.spawnCircleRadiusMin, mParticleInfo.spawnCircleRadiusMax)*Vector2(degCos(randomDeg), degSin(randomDeg));
				deg = randomRange(mParticleInfo.degMin, mParticleInfo.degMax);
				break;
			}
			case SpawnType::eLine:
			{
				float randomX = randomRange(-mParticleInfo.spawnLineLength / 2, mParticleInfo.spawnLineLength / 2);
				float randomY = randomRange(-mParticleInfo.spawnLineWidth / 2, mParticleInfo.spawnLineWidth / 2);
				Vector2 vec(randomX, randomY);
				generatePos = generatePos + vec.Rotation(mParticleInfo.spawnLineDeg);
				deg = mParticleInfo.degMin + (mParticleInfo.degMax - mParticleInfo.degMin) * (randomX + mParticleInfo.spawnLineLength / 2) / mParticleInfo.spawnLineLength;
				break;
			}
			default:
				deg = randomRange(mParticleInfo.degMin, mParticleInfo.degMax);
				break;
			}
			if (!mOnceGenerated)
			{
				mParticles.push_back(new Particle(GetScene(), generatePos, &mParticleInfo, deg));
			}
		}
		if (mOnce)
		{
			mOnceGenerated = true;
		}
	}
	mGenerateInterval -= deltaTime;
	mLifetime -= deltaTime;

	if (!mPermanent && mLifetime.IsEnd() && mParticles.size() == 0)
	{
		SetState(State::eDead);
		return;
	}
}

void ParticleSystem::Erase()
{
	mPermanent = false;
	mLifetime = 0;
}
