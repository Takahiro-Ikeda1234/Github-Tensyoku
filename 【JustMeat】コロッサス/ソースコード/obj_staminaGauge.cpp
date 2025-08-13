#include "obj_staminaGauge.h"
#include"ComponentCircleUISprite.h"
#include"ComponentUISprite.h"
#include"CameraManager.h"
#include"obj_player.h"
#include"ComponentAnimationSprite.h"
//オブジェクトに変更
StaminaGauge::StaminaGauge(Scene * scene, Vector2 pos)
	: UIObject(scene)
{
	SetPosition(Vector2(0.5f*GetWindowWidth(), 0.92f*GetWindowHeight()));
	mCCSprite = new ComponentCircleSprite(this, TextureTag::Stamina, -5);
	mCASprite = new ComponentAnimationSprite(this, TextureTag::Stamina, -4);
	mCASprite->SetAnimation(3, 3, 1, 1, false);
}

void StaminaGauge::UpdateUIObject(float deltaTime)
{
	if (mCCSprite->GetFill() > 0.5f) mCCSprite->SetAnimation(0, 0, 1, 1, false);
	else if (mCCSprite->GetFill() > 0.25f)mCCSprite->SetAnimation(1, 1, 1, 1, false);
	else mCCSprite->SetAnimation(2, 2, 1, 1, false);
	//Vector2 pos = GetScene()->GetCameraManager()->ToScreenPosition(GetScene()->GetPlayer()->GetPosition());
	//pos = pos + Vector2(GetWindowWidth() / 2, GetWindowHeight() / 2);
	Vector2 pos = GetScene()->GetPlayer()->GetPosition();
	SetPosition(pos + Vector2(0, 2.0f) * BLOCKSIZE);
	SetSize(Vector2(1.5f, 1.5f) * BLOCKSIZE);
	//SetSize(Vector2(0.01875f * GetWindowWidth(), 0.01875f * GetWindowWidth()));
	mAlphaTime -= deltaTime;
	if (mCCSprite->GetFill() >= 0.9f && mAppear)
	{
		mAppear = false;
		if (mAlphaTime.IsEnd())
		{
			mAlphaTime = 0.5f;
		}
		else
		{
			mAlphaTime.Reverse();
			mAlphaTime *= 0.5f;
		}
	}
	if (mCCSprite->GetFill() < 0.9f && !mAppear)
	{
		mAppear = true;
		if (mAlphaTime.IsEnd())
		{
			mAlphaTime = 1;
		}
		else
		{
			mAlphaTime.Reverse();
			mAlphaTime *= 2;
		}
	}
	if (mAppear)
	{
		SetAlphaRate(1 - mAlphaTime.Percentage());
	}
	else
	{
		SetAlphaRate(mAlphaTime.Percentage());
	}
}

void StaminaGauge::SetFill(float percentage)
{
	mCCSprite->SetFill(percentage);
}

void StaminaGauge::SetAlphaRate(float rate)
{
	mCCSprite->SetAlphaRate(rate);
	mCASprite->SetAlphaRate(rate);
	//mUISprite->SetAlphaRate(rate);
}
