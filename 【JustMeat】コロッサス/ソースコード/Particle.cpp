#include "Particle.h"
#include "Scene.h"
#include"textureData.h"
#include"ComponentAnimationSprite.h"
#include"myMath.h"
#include"ParticleSystem.h"
Particle::Particle(Scene * scene, Vector2 pos, ParticleInfo* info, float deg)
	: AbstractObject(scene)
	, mInfo(info)
	, mLifetime(randomRange(info->lifetimeMin, info->lifetimeMax))
	, baseScale(randomRange(info->scaleMin, info->scaleMax))
	, baseDeg(deg)
	, sin(degSin(baseDeg))
	, cos(degCos(baseDeg))
	, baseSpeed(randomRange(info->speedMin, info->speedMax))
	, baseAlphaRate(randomRange(info->alphaRateMin, info->alphaRateMin))
	//, baseColorRate(randomRange(info->colorRateMin, info->colorRateMax))
	, baseRedRate(randomRange(info->redRateMin, info->redRateMax))
	, baseGreenRate(randomRange(info->greenRateMin, info->greenRateMax))
	, baseBlueRate(randomRange(info->blueRateMin, info->blueRateMax))
	, baseRotationSpeed(randomRange(info->rotationSpeedMin, info->rotationSpeedMax))
{
	mCASprite = new ComponentAnimationSprite(this, info->tex, info->drawPriority);
	mCASprite->SetAlphaRate(baseAlphaRate);
	//mCASprite->SetColorRate(baseColorRate);
	mCASprite->SetRedRate(baseRedRate);
	mCASprite->SetGreenRate(baseGreenRate);
	mCASprite->SetBlueRate(baseBlueRate);
	if (mInfo->addBlend)
	{
		mCASprite->SetDrawMode(ComponentSprite::DrawMode::eAdd);
	}
	if (mInfo->multipleBlend)
	{
		mCASprite->SetDrawMode(ComponentSprite::DrawMode::eMultiple);
	}
	SetRotation(randomRange(info->rotationDefaultMin, info->rotationDefaultMax));

	SetPosition(pos);
	SetScale(baseScale);
	SetSize(mInfo->size);
	UpdateParameter();
}

void Particle::UpdateObject(float deltaTime)
{
	UpdateParameter();
	if (mInfo->useGravity)
	{
		gravity = gravity + mInfo->gravity * deltaTime;
	}
	Vector2 vec;
	if (!mInfo->useMoveCurve)
	{
		vec = baseSpeed * deltaTime * Vector2(cos, sin) * (mInfo->speedLifetimeChangeBaseValue + LifetimeCurve[(int)mInfo->speedLifetimeChangeType](GetLifetimePercentage())) + gravity * deltaTime;
	}
	else
	{
		float deg = mInfo->curveDegRange * degSin(360 * mInfo->lifetimeCurveCount * GetLifetimePercentage());
		vec = baseSpeed * deltaTime * Vector2(degCos(baseDeg + deg), degSin(baseDeg + deg)) * (mInfo->speedLifetimeChangeBaseValue + LifetimeCurve[(int)mInfo->speedLifetimeChangeType](GetLifetimePercentage())) + gravity * deltaTime;
	}
	Translate(vec);
	if (mInfo->moveForward)
	{
		SetRotation(Vector2::degArctan(vec) - 90);
	}
	else if (mInfo->moveBackward)
	{
		SetRotation(Vector2::degArctan(vec) + 90);
	}
	else
	{
		Rotation(baseRotationSpeed * deltaTime * LifetimeCurve[(int)mInfo->rotationSpeedLifetimeChangeType](GetLifetimePercentage()));
	}
	mLifetime -= deltaTime;
}

void Particle::UpdateParameter()
{
	if (mInfo->alphaLifetimeChangeType != LifetimeChangeType::eNone)
	{
		mCASprite->SetAlphaRate(GetAlphaRate() * LifetimeCurve[(int)mInfo->alphaLifetimeChangeType](GetLifetimePercentage()));
		//mCASprite->SetColorRate(GetColorRate() * LifetimeCurve[(int)mInfo->colorLifetimeChangeType](GetLifetimePercentage()));
		mCASprite->SetRedRate(GetRedRate() * LifetimeCurve[(int)mInfo->redLifetimeChangeType](GetLifetimePercentage()));
		mCASprite->SetGreenRate(GetGreenRate() * LifetimeCurve[(int)mInfo->greenLifetimeChangeType](GetLifetimePercentage()));
		mCASprite->SetBlueRate(GetBlueRate() * LifetimeCurve[(int)mInfo->blueLifetimeChangeType](GetLifetimePercentage()));
	}
	if (mInfo->scaleLifetimeChangeType != LifetimeChangeType::eNone)
	{
		SetScale(baseScale * LifetimeCurve[(int)mInfo->scaleLifetimeChangeType](GetLifetimePercentage()));
	}


	if (mInfo->moveForward)
	{
		Vector2 vec = baseSpeed * Vector2(degCos(baseDeg), degSin(baseDeg)) * (mInfo->speedLifetimeChangeBaseValue + LifetimeCurve[(int)mInfo->speedLifetimeChangeType](GetLifetimePercentage()));
		SetRotation(Vector2::degArctan(vec) - 90);
	}
	else if (mInfo->moveBackward)
	{
		Vector2 vec = baseSpeed * Vector2(degCos(baseDeg), degSin(baseDeg)) * (mInfo->speedLifetimeChangeBaseValue + LifetimeCurve[(int)mInfo->speedLifetimeChangeType](GetLifetimePercentage()));
		SetRotation(Vector2::degArctan(vec) + 90);
	}
}
