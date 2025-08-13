#include "ui_stageSelect.h"
#include "SceneStageSelect.h"
#include"keyboard.h"
#include"ComponentUISprite.h"
#include"EventManager.h"
#include"StageData.h"
#include"AudioData.h"
#include"UI9Slice.h"
const float UIStageSelect::LIMITTIME = 0.1f;
UIStageSelect::UIStageSelect(SceneStageSelect * scene)
	: mScene(scene)
{

	mUIStages[0] = new UIBox(mScene, 0.37f, 0.32f, 0.12f, 0.040f, TextureTag::StageSelectArm, 50);
	mUIStages[1] = new UIBox(mScene, 0.63f, 0.32f, 0.12f, 0.040f, TextureTag::StageSelectArm, 50);
	mUIStages[2] = new UIBox(mScene, 0.37f, 0.47f, 0.12f, 0.040f, TextureTag::StageSelectArm, 50);
	mUIStages[3] = new UIBox(mScene, 0.63f, 0.47f, 0.12f, 0.040f, TextureTag::StageSelectArm, 50);
	mUIStages[4] = new UIBox(mScene, 0.37f, 0.62f, 0.12f, 0.040f, TextureTag::StageSelectArm, 50);
	mUIStages[5] = new UIBox(mScene, 0.63f, 0.62f, 0.12f, 0.040f, TextureTag::StageSelectArm, 50);
	mUIStages[6] = new UIBox(mScene, 0.37f, 0.77f, 0.12f, 0.040f, TextureTag::StageSelectArm, 50);
	mUIStages[7] = new UIBox(mScene, 0.63f, 0.77f, 0.12f, 0.040f, TextureTag::StageSelectArm, 50);
	for (int i = 0; i < STAGE_MAX - 1; i++)
	{
		std::string title[2];
		mUIStages[i]->GetCSprite()->SetAnimation(i + 8, i + 8, 1, 0);
	}
	SetStageSelectCursor(0);
	new UIBox(scene, 0.5f, 0.96f, 0.5f, 0.04f, TextureTag::Control, 1);
	auto mWASD = new UIBox(scene, 0.25f, 0.96f, 0.025f, 0.0125f, TextureTag::KeyWASD, 0);
	mWASD->GetCSprite()->SetString("選択");
	mWASD->GetCSprite()->SetStringInfo(5, mWASD->GetPosition() + Vector2(GetWindowWidth() * 0.03f, 0), Vector2(20, 20), false);

	mWASD->GetCSprite()->SetScale(1, 1.5f);
	auto mEnter = new UIBox(scene, 0.5f, 0.96f, 0.0145f, 0.0185f, TextureTag::KeyEnter, 0);
	mEnter->GetCSprite()->SetString("決定");
	mEnter->GetCSprite()->SetStringInfo(5, mEnter->GetPosition() + Vector2(GetWindowWidth() * 0.02f, GetWindowHeight() * -0.01f), Vector2(20, 20), false);

	auto mESC = new UIBox(scene, 0.75f, 0.96f, 0.0125f, 0.0125f, TextureTag::KeyEsc, 0);
	mESC->GetCSprite()->SetString("キャンセル");
	mESC->GetCSprite()->SetStringInfo(5, mESC->GetPosition() + Vector2(GetWindowWidth() * 0.02f, GetWindowHeight() * -0.01f), Vector2(20, 20), false);

	//auto StageSelect = new UI9Slice(scene, 0.5f, 0.15f, 0.15f, 0.03f, TextureTag::UI9Sliced, 0);
	//StageSelect->GetCSprite()->SetString("ステージセレクト");
	//StageSelect->GetCSprite()->SetStringInfo(20, StageSelect->GetPosition(), Vector2(35, 35), true);
	auto StageSelect = new UIBox(scene, 0.5f, 0.15f, 0.23f, 0.03f, TextureTag::StageSelectText, 0);
}


void UIStageSelect::Update(float deltaTime)
{
	mKeyDownLimit -= deltaTime;

	switch (mState)
	{
	case StageSelectState::eStageSelect:
		if (mScene->GetSceneManager()->CanSceneChange())
		{
			StateStageSelect(deltaTime);
		}
		if (mScene->GetSceneManager()->GetNextScene() != SceneManager::SceneID::eTitle)
		{
			mUIStages[mStageSelectCursor]->GetCSprite()->SetColor(0xFFFFFFFF);
		}
		break;
	}
}

void UIStageSelect::StateStageSelect(float deltaTime)
{
	if (Keyboard::Inst()->KeyHit(DIK_S))
	{
		PlaySE(SE_MENU_SELECT);
		SetStageSelectCursor((mStageSelectCursor + 2) % SELECTABLE_STAGE_NUM);
		mKeyDownLimit = LIMITTIME * 5;
	}
	else if (Keyboard::Inst()->KeyDown(DIK_S) && mKeyDownLimit.IsEnd())
	{
		PlaySE(SE_MENU_SELECT);
		SetStageSelectCursor((mStageSelectCursor + 2) % SELECTABLE_STAGE_NUM);
		mKeyDownLimit = LIMITTIME;
	}

	if (Keyboard::Inst()->KeyHit(DIK_W))
	{
		PlaySE(SE_MENU_SELECT);
		SetStageSelectCursor((mStageSelectCursor + SELECTABLE_STAGE_NUM - 2) % SELECTABLE_STAGE_NUM);
		mKeyDownLimit = LIMITTIME * 5;
	}
	else if (Keyboard::Inst()->KeyDown(DIK_W) && mKeyDownLimit.IsEnd())
	{
		PlaySE(SE_MENU_SELECT);
		SetStageSelectCursor((mStageSelectCursor + SELECTABLE_STAGE_NUM - 2) % SELECTABLE_STAGE_NUM);
		mKeyDownLimit = LIMITTIME;
	}

	if (Keyboard::Inst()->KeyHit(DIK_D) || Keyboard::Inst()->KeyHit(DIK_A))
	{
		PlaySE(SE_MENU_SELECT);
		mKeyDownLimit = LIMITTIME * 5;
		if (mStageSelectCursor % 2)
		{
			SetStageSelectCursor(mStageSelectCursor - 1);
		}
		else
		{
			SetStageSelectCursor(mStageSelectCursor + 1);
		}	
	}
	else if ((Keyboard::Inst()->KeyDown(DIK_D) || Keyboard::Inst()->KeyDown(DIK_A)) && mKeyDownLimit.IsEnd())
	{
		PlaySE(SE_MENU_SELECT);
		mKeyDownLimit = LIMITTIME;
		if (mStageSelectCursor % 2)
		{
			SetStageSelectCursor(mStageSelectCursor - 1);
		}
		else
		{
			SetStageSelectCursor(mStageSelectCursor + 1);
		}
	}

	if (Keyboard::Inst()->KeyHit(DIK_RETURN) && mScene->GetSceneManager()->CanSceneChange())
	{
		PlaySE(SE_MENU_DECIDE);
		mUIStages[mStageSelectCursor]->SetBlink(0.2f, 0.5f);

		Parameter param;
		param.Set("ステージ", mStageSelectCursor + 1);
		mScene->GetSceneManager()->SceneChange(SceneManager::eGame, &param, 0.5f, 0.75f, 0.5f);
	}
	if (Keyboard::Inst()->KeyHit(DIK_ESCAPE) && mScene->GetSceneManager()->CanSceneChange())
	{
		PlaySE(SE_MENU_CANCEL);
		mUIStages[mStageSelectCursor]->SetBlink(0);

		Parameter param;
		param.Set("メニュー状態", 1);
		param.Set("カーソル位置", 1);
		mScene->GetSceneManager()->SceneChange(SceneManager::eTitle, &param, 0.25f, 0.25f, 0.25f);
	}
}

void UIStageSelect::SetStageSelectCursor(int cursor)
{
	if (mState == StageSelectState::eStageSelect)
	{
		for (int i = 0; i < SELECTABLE_STAGE_NUM; i++)
		{
			mUIStages[i]->GetCSprite()->SetAlpha(0xFF);
			mUIStages[i]->GetCSprite()->SetStringAlpha(0xFF);
			mUIStages[i]->GetCSprite()->SetColor(0xFF7F7F7F);
			mUIStages[i]->SetBlink(0);
			mUIStages[i]->GetCSprite()->SetAnimation(i + 8, i + 8, 1, 0);
		}
		mUIStages[cursor]->SetBlink(2);
		mUIStages[cursor]->GetCSprite()->SetAnimation(cursor, cursor, 1, 0);
	}
	mStageSelectCursor = cursor;
}
