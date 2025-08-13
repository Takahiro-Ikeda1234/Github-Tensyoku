#include "obj_marker.h"
#include "StageManager.h"
#include "ComponentAnimationSprite.h"

Marker::Marker(Scene * scene, int indexX, int indexY)
	: StageObject(scene, StageManager::ToPosition(indexX, indexY))
{
	mCASprite = new ComponentAnimationSprite(this, TextureTag::Marker, 2);
	mCASprite->SetScale(1.0f / 3, 1.0f / 3);
	mCASprite->SetAlphaRate(0);
	mCASprite->SetAnimation(0, 0, 8, 8);
	SetSize(Vector2(3, 3)* BLOCKSIZE);
}

void Marker::UpdateStageObject(float deltaTime)
{
	if (!mFade.IsEnd())
	{
		if (mIsFadein)
		{
			mCASprite->SetAlphaRate(1 - mFade.Percentage());
		}
		else
		{
			mCASprite->SetAlphaRate(mFade.Percentage());
		}
	}
	else
	{
		if (mIsFadein)
		{
			mCASprite->SetAlphaRate(1);
		}
		else
		{
			mCASprite->SetAlphaRate(0);
		}
	}
	mFade -= deltaTime;
}

void Marker::EnterGolemArm()
{
	mIsFadein = true;

	if (mFade.IsEnd())
	{
		mFade = 0.1f;
	}
	else
	{
		mFade.Reverse();
	}
}

void Marker::ExitGolemArm()
{
	mIsFadein = false;

	if (mFade.IsEnd())
	{
		mFade = 0.1f;
	}
	else
	{
		mFade.Reverse();
	}
}
