#include "obj_triggerButton.h"
#include "obj_golemEvent.h"
#include "ComponentTrigger.h"
#include "AudioData.h"
TriggerButton::TriggerButton(Scene * scene, Vector2 pos, int ch, int type)
	: StageObject(scene, pos)
{
	
	mCTrigger = new ComponentTrigger(this, ch);

	switch (type)
	{
	case 0:
		mCASprite = new ComponentAnimationSprite(this, TextureTag::TriggerButton);
		SetRotation(180);
		SetSize(BLOCKSIZE * Vector2(1, 5));
		break;
	case 1:
		mCASprite = new ComponentAnimationSprite(this, TextureTag::TriggerButton);
		SetRotation(0);
		SetSize(BLOCKSIZE * Vector2(1, 5));
		break;
	case 2:
		mCASprite = new ComponentAnimationSprite(this, TextureTag::TriggerButtonRot);
		SetRotation(180);
		SetSize(BLOCKSIZE * Vector2(5, 1));
		break;
	case 3:
		mCASprite = new ComponentAnimationSprite(this, TextureTag::TriggerButtonRot);
		SetRotation(0);
		SetSize(BLOCKSIZE * Vector2(5, 1));
		break;
	}
}

TriggerButton::TriggerButton(Scene * scene, int indexX, int indexY, int ch, int type)
	: TriggerButton(scene, StageManager::ToPosition(indexX, indexY), ch, type)
{
}

void TriggerButton::UpdateStageObject(float deltaTime)
{
}

void TriggerButton::EnterGolemArm()
{
	mCASprite->SetAnimation(1, 1, 4, 16, false);
	mCTrigger->SetActive(true);
}

void TriggerButton::ExitGolemArm()
{
	mCASprite->SetAnimation(2, 2, 4, 16, false);
	mCTrigger->SetActive(false);
}

