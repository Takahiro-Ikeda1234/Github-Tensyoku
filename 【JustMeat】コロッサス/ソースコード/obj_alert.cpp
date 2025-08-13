#include "obj_alert.h"
#include "ComponentAnimationSprite.h"
#include "myMath.h"
#include "ComponentUISprite.h"
#include "CameraManager.h"
#include "obj_player.h"
#include "AudioData.h"
Alert::Alert(Scene * scene, Vector2 origin, Vector2 rect, Vector2 armStart, float lifetime, float blinktime, unsigned color)
	: Object(scene, Vector2(origin.x + rect.x / 2, origin.y + rect.y / 2))
	, mLifetime(lifetime)
	, mBlink(blinktime)
{
	mCASprite = new ComponentAnimationSprite(this, TextureTag::Plane);
	color = 0xFFFF2F2F;
	mCASprite->SetColor(color);
	SetSize(rect);
	mCASprite->SetColorRate(0.6f);
	mCASprite->SetAlphaRate(0.6f);
	mUI = new AlertUI(scene, armStart, GetPosition());
	mUIArrow = new AlertArrow(scene, armStart, GetPosition());
	PlaySE(SE_WARNING);
}

Alert::~Alert()
{
	mUI->SetState(State::eDead);
	mUIArrow->SetState(State::eDead);
}

void Alert::UpdateObject(float deltaTime)
{
	mLifetime -= deltaTime;
	mBlink -= deltaTime;
	if (mLifetime.IsFinish())
	{
		SetState(State::eDead);
	}
	mCASprite->SetAlphaRate(0.6f + 0.3f * degSin((1 - mBlink.Percentage()) * 360 / mBlink.GetMaxTime()));
}

void Alert::LoadCheckPointData()
{
	SetState(State::eDead);
}

AlertUI::AlertUI(Scene * scene, Vector2 armStart, Vector2 alertCenter)
	: UIObject(scene)
	, mArmStart(armStart)
	, mAlertCenter(alertCenter)
{
	mUISprite = new ComponentUISprite(this, TextureTag::Warning, 4);
	SetSize(Vector2(0.07f * GetWindowWidth(), 0.07f * GetWindowWidth()));
	UpdateUIObject(0);
}


void AlertUI::UpdateUIObject(float deltaTime)
{
	Vector2 scrnArm = GetScene()->GetCameraManager()->ToScreenPosition(mArmStart);
	Vector2 scrnAlert = GetScene()->GetCameraManager()->ToScreenPosition(mAlertCenter);
	Vector2 tempPos = scrnAlert + (scrnArm - scrnAlert) / 2;
	Vector2 pos = Vector2(tempPos.x / GetWindowWidth() + 0.5f, tempPos.y / GetWindowHeight() + 0.5f);
	if (pos.x < 0.11f)pos.x = 0.11f;
	if (pos.x > 0.89f)pos.x = 0.89f;

	if (pos.y < 0.17f)pos.y = 0.17f;
	if (pos.y > 0.75f)pos.y = 0.75f;
	SetPosition(Vector2(pos.x * GetWindowWidth(), pos.y * GetWindowHeight()));

}


AlertArrow::AlertArrow(Scene * scene, Vector2 armStart, Vector2 alertCenter)
	: UIObject(scene)
	, mArmStart(armStart)
	, mAlertCenter(alertCenter)
{
	mUISprite = new ComponentUISprite(this, TextureTag::Arrow, 1);
	mUISprite2 = new ComponentUISprite(this, TextureTag::Arrow, 2);
	mUISprite2->SetScale(1.1f, 1.1f);
	mUISprite2->SetColorRate(0.4f);
	SetSize(Vector2(0.05f * GetWindowWidth(), 0.05f * GetWindowWidth()));
	mBlink = 0.3f;
	UpdateUIObject(0);
}

void AlertArrow::UpdateUIObject(float deltaTime)
{
	mBlink -= deltaTime;
	mUISprite->SetGreen(0x66 + (0xFF - 0x66) * degSin(mBlink.Percentage() * 180));
	mUISprite->SetBlue(0x00 + (0xFF) * degSin(mBlink.Percentage() * 180));
	Vector2 scrnArm = GetScene()->GetCameraManager()->ToScreenPosition(mArmStart);
	Vector2 scrnAlert = GetScene()->GetCameraManager()->ToScreenPosition(mAlertCenter);
	Vector2 pos = Vector2(scrnArm.x / GetWindowWidth() + 0.5f, scrnArm.y / GetWindowHeight() + 0.5f);
	int xDiff = scrnArm.x - scrnAlert.x;
	int yDiff = scrnArm.y - scrnAlert.y;
	float xPos;
	float yPos;
	if (yDiff == 0)
	{
		if (xDiff < 0)
		{
			SetRotation(180);
		}
		else
		{
			SetRotation(0);
		}
	}
	else
	{
		if (yDiff > 0)
		{
			SetRotation(90);
		}
		else
		{
			SetRotation(270);
		}
	}
	if (pos.x < 0.05f)pos.x = 0.05f;
	if (pos.x > 0.95f)pos.x = 0.95f;
	if (pos.y < 0.07f)pos.y = 0.07f;
	if (pos.y > 0.85f)pos.y = 0.85f;
	SetPosition(Vector2(pos.x * GetWindowWidth(), pos.y * GetWindowHeight()));
}
