#include "ui_game.h"
#include "ComponentUISprite.h"
#include "obj_player.h"
#include "SceneGame.h"
#include "SceneManager.h"
#include "ui_title.h"
#include"CameraManager.h"
#include"ComponentCircleUISprite.h"
#include"obj_filter.h"
#include "keyboard.h"
#include "EventManager.h"
#include "UI9Slice.h"
#include "AudioData.h"
#include "StageData.h"
#include "ini.h"
const float UIGame::LIMITTIME = 0.1f;
static int stageBGMArray[] = { 0,1,1,2,2,2,3,3,4 };

UIGame::UIGame(SceneGame * scene)
	: mScene(scene)
{
	new DummyUI(scene);
	auto r = new UIBox(scene, 0.5f, 0.86f, 0.01f, 0.01f, TextureTag::KeyR, 0);
	float pauseUIAlpha = GetConfigFloat("UI", "PauseUIAlpha");

	if (scene->GetStageManager()->GetCurrentStageID() == 0)
	{
		r->GetCSprite()->SetTexture(TextureTag::Invisible);
	}
	//r->GetCSprite()->SetStringInfo(5, r->GetPosition() + Vector2(GetWindowWidth() * 0.03f, 0), Vector2(24,24), true);
	//r->GetCSprite()->SetString("囮");

	mFilter = new Filter(scene, TextureTag::Plane, -100);
	mFilter->GetCSprite()->SetColor(0xFF000000);

	mBox = new UI9Slice(scene, 0.5f, (0.525f + 0.60f) / 2, 0.1f, 0.11f, TextureTag::UI9Sliced, -101);
	mPause = new UIBox(scene, 0.5f, 0.15f, 0.22f, 0.045f, TextureTag::PauseText, -102);

	mUIMenuSelect[0] = new UI9Slice(scene, 0.5f, 0.45f, 0.09f, 0.015f, TextureTag::UI9Sliced, -103);
	mUIMenuSelect[1] = new UI9Slice(scene, 0.5f, 0.525f, 0.09f, 0.015f, TextureTag::UI9Sliced, -103);
	mUIMenuSelect[2] = new UI9Slice(scene, 0.5f, 0.60f, 0.09f, 0.015f, TextureTag::UI9Sliced, -103);
	mUIMenuSelect[3] = new UI9Slice(scene, 0.5f, 0.675f, 0.09f, 0.015f, TextureTag::UI9Sliced, -103);
	mUIMenuCursor[0] = new UIBox(scene, 0.5f, 0.45f,  0.10f, 0.025f, TextureTag::PauseSelect, -102);
	mUIMenuCursor[1] = new UIBox(scene, 0.5f, 0.525f, 0.10f, 0.025f, TextureTag::PauseSelect, -102);
	mUIMenuCursor[2] = new UIBox(scene, 0.5f, 0.60f,  0.10f, 0.025f, TextureTag::PauseSelect, -102);
	mUIMenuCursor[3] = new UIBox(scene, 0.5f, 0.675f, 0.10f, 0.025f, TextureTag::PauseSelect, -102);

	mUIMenuSelect[0]->SetResistPause(true);
	mUIMenuSelect[1]->SetResistPause(true);
	mUIMenuSelect[2]->SetResistPause(true);
	mUIMenuSelect[3]->SetResistPause(true);
	mUIMenuCursor[0]->SetResistPause(true);
	mUIMenuCursor[1]->SetResistPause(true);
	mUIMenuCursor[2]->SetResistPause(true);
	mUIMenuCursor[3]->SetResistPause(true);

	mBox->GetCSprite()->SetColor(0xFFFFFF | (((int)(0xFF * (pauseUIAlpha))) << 24));

	mUIMenuSelect[0]->GetCSprite()->SetColor(0x00CFCFCF);
	mUIMenuSelect[0]->GetCSprite()->SetString("再開");
	mUIMenuSelect[0]->GetCSprite()->SetStringInfo(20, mUIMenuSelect[0]->GetPosition(), Vector2(21, 21), true);
	mUIMenuSelect[0]->GetCSprite()->SetStringColor(0xFFFFFFFF);

	mUIMenuSelect[1]->GetCSprite()->SetColor(0x00CFCFCF);
	mUIMenuSelect[1]->GetCSprite()->SetString("チェックポイントから再開");
	mUIMenuSelect[1]->GetCSprite()->SetStringInfo(20, mUIMenuSelect[1]->GetPosition(), Vector2(21, 21), true);
	mUIMenuSelect[1]->GetCSprite()->SetStringColor(0xFFFFFFFF);

	mUIMenuSelect[2]->GetCSprite()->SetColor(0x00CFCFCF);
	mUIMenuSelect[2]->GetCSprite()->SetString("ステージセレクトに戻る");
	mUIMenuSelect[2]->GetCSprite()->SetStringInfo(20, mUIMenuSelect[2]->GetPosition(), Vector2(21, 21), true);
	mUIMenuSelect[2]->GetCSprite()->SetStringColor(0xFFFFFFFF);

	mUIMenuSelect[3]->GetCSprite()->SetColor(0x00CFCFCF);
	mUIMenuSelect[3]->GetCSprite()->SetString("タイトルに戻る");
	mUIMenuSelect[3]->GetCSprite()->SetStringInfo(20, mUIMenuSelect[3]->GetPosition(), Vector2(21, 21), true);
	mUIMenuSelect[3]->GetCSprite()->SetStringColor(0xFFFFFFFF);


	mControl = new UIBox(scene, 0.5f, 0.96f, 0.5f, 0.04f, TextureTag::Control, -101);
	mWASD = new UIBox(scene, 0.25f, 0.96f, 0.025f, 0.0125f, TextureTag::KeyWASD, -102);
	mWASD->GetCSprite()->SetString("選択");
	mWASD->GetCSprite()->SetStringInfo(5, mWASD->GetPosition() + Vector2(GetWindowWidth() * 0.03f, 0), Vector2(20, 20), false);

	mWASD->GetCSprite()->SetScale(1, 1.5f);
	mEnter = new UIBox(scene, 0.5f, 0.96f, 0.0145f, 0.0185f, TextureTag::KeyEnter, -102);
	mEnter->GetCSprite()->SetString("決定");
	mEnter->GetCSprite()->SetStringInfo(5, mEnter->GetPosition() + Vector2(GetWindowWidth() * 0.02f, GetWindowHeight() * -0.01f), Vector2(20, 20), false);

	mESC = new UIBox(scene, 0.75f, 0.96f, 0.0125f, 0.0125f, TextureTag::KeyEsc, -102);
	mESC->GetCSprite()->SetString("キャンセル");
	mESC->GetCSprite()->SetStringInfo(5, mESC->GetPosition() + Vector2(GetWindowWidth() * 0.02f, GetWindowHeight() * -0.01f), Vector2(20, 20), false);


	std::string title[2];
	StageData::Inst()->GetStageTitle(mScene->GetStageManager()->GetCurrentStageID(), &title[0], &title[1]);
	mDepth = new UIBoxFade(scene, 0.06f, 0.07f, 0.05f, 0.025f, TextureTag::Paper, 0);
	mDepth->GetCSprite()->SetAlphaRate(0);
	if (title[0] != "")
	{
		mDepth->SetShowTime(1, 5, 1, 0.5f);
		mDepth->GetCSprite()->SetString(title[0]);
		mDepth->GetCSprite()->SetStringInfo(40, mDepth->GetPosition(), Vector2(30, 30), true);
		mDepth->GetCSprite()->SetStringColor(0xFF000000);

		mDepthBG = new UIBoxFade(scene, 0.06f, 0.07f, 0.052f, 0.027f, TextureTag::Plane, 2);
		mDepthBG->GetCSprite()->SetColor(0xFF000000);
		mDepthBG->GetCSprite()->SetAlphaRate(0);
		mDepthBG->SetShowTime(1, 5, 1, 0.5f);
	}


	mStageTitle = new UIBoxFade(scene, 0.15f, 0.15f, 0.12f, 0.04f, TextureTag::Paper, 1);
	mStageTitle->GetCSprite()->SetAlphaRate(0);
	if (title[1] != "")
	{
		mStageTitle->SetShowTime(1, 5, 1, 0.5f);
		mStageTitle->GetCSprite()->SetString(title[1]);
		mStageTitle->GetCSprite()->SetStringInfo(40, mStageTitle->GetPosition(), Vector2(45, 45), true);
		mStageTitle->GetCSprite()->SetStringColor(0xFF000000);

		mStageTitleBG = new UIBoxFade(scene, 0.15f, 0.15f, 0.122f, 0.042f, TextureTag::Plane, 2);
		mStageTitleBG->GetCSprite()->SetColor(0xFF000000);
		mStageTitleBG->GetCSprite()->SetAlphaRate(0);
		mStageTitleBG->SetShowTime(1, 5, 1, 0.5f);
	}

	SetAlphaPauseMenu(mScene->IsPause());

}

void UIGame::Update(float deltaTime)
{
	mKeyDownLimit -= deltaTime;
	if (mScene->GetPlayer()->IsDying())return;
	if (Keyboard::Inst()->KeyHit(DIK_ESCAPE) && mScene->GetSceneManager()->CanSceneChange())
	{
		if (mScene->IsPause()) PlaySE(SE_PAUSE_CLOSE);
		else PlaySE(SE_PAUSE_OPEN);
		mScene->SetPause(!mScene->IsPause());
		SetAlphaPauseMenu(mScene->IsPause());
		SetMenuSelectCursor(0);
	}

	if (mScene->IsPause())
	{
		for (int i = 0; i < 4; i++)
		{
			mUIMenuCursor[i]->GetCSprite()->SetAlpha(0);
		}
		mUIMenuSelect[mMenuSelectCursorY]->GetCSprite()->SetColor(0x00FFFFFF);
		mUIMenuCursor[mMenuSelectCursorY]->GetCSprite()->SetColor(0xFFFFFFFF);
		UpdatePauseMenu(deltaTime);
		if (Keyboard::Inst()->KeyHit(DIK_RETURN) && mScene->GetSceneManager()->CanSceneChange() && mRetryTime.IsEnd())
		{
			PlaySE(SE_MENU_DECIDE);
			Parameter param;
			switch (mMenuSelectCursorY)
			{
			case 0:
				mScene->SetPause(false);
				SetAlphaPauseMenu(mScene->IsPause());
				SetMenuSelectCursor(0);
				break;
			case 1:
				mRetryTime = 1.0f;
				mUIMenuSelect[mMenuSelectCursorY]->SetBlink(0.2f, 0.5f);
				mUIMenuCursor[mMenuSelectCursorY]->SetBlink(0.2f, 0.5f);
				mScene->GetEventManager()->Fadeout(0.5f, 0.5f);
				break;
			case 2:
			{
				mUIMenuSelect[mMenuSelectCursorY]->SetBlink(0.2f, 0.5f);
				mUIMenuCursor[mMenuSelectCursorY]->SetBlink(0.2f, 0.5f);
				SetFadeout(stageBGMArray[mScene->GetStageManager()->GetCurrentStageID()], 200, 0.5f);

				int stage = mScene->GetStageManager()->GetCurrentStageID() - 1;
				if (stage == -1) stage = 0;
				param.Set("ステージカーソル", stage);
				mScene->GetSceneManager()->SceneChange(SceneManager::eStageSelect, &param, 0.5f, 0.75f, 0.5f);
				break;
			}
			case 3:
			{
				mUIMenuSelect[mMenuSelectCursorY]->SetBlink(0.2f, 0.5f);
				mUIMenuCursor[mMenuSelectCursorY]->SetBlink(0.2f, 0.5f);
				SetFadeout(stageBGMArray[mScene->GetStageManager()->GetCurrentStageID()], 200, 0.5f);

				param.Set("メニュー状態", (int)UITitle::TitleState::eMenuSelect);
				mScene->GetSceneManager()->SceneChange(SceneManager::eTitle, &param, 0.5f, 0.75f, 0.5f);
				break;
			}
			}
		}
		if (mRetryTime.IsFinish())
		{
			mScene->AllObjectLoadCheckPointData();
			mScene->SetPause(false);
			SetAlphaPauseMenu(mScene->IsPause());
			SetMenuSelectCursor(0);
		}
		mRetryTime -= deltaTime;
	}

}

void UIGame::SetAlphaPauseMenu(bool flag)
{
	float alpha = flag;
	mFilter->GetCSprite()->SetAlphaRate(alpha * 0.5f);

	mBox->GetCSprite()->SetAlphaRate(alpha);
	mPause->GetCSprite()->SetAlphaRate(alpha);
	mControl->GetCSprite()->SetAlphaRate(alpha);
	mWASD->GetCSprite()->SetAlphaRate(alpha);
	mESC->GetCSprite()->SetAlphaRate(alpha);
	mEnter->GetCSprite()->SetAlphaRate(alpha);

	mBox->GetCSprite()->SetStringAlphaRate(alpha);
	mPause->GetCSprite()->SetStringAlphaRate(alpha);
	mControl->GetCSprite()->SetStringAlphaRate(alpha);
	mWASD->GetCSprite()->SetStringAlphaRate(alpha);
	mESC->GetCSprite()->SetStringAlphaRate(alpha);
	mEnter->GetCSprite()->SetStringAlphaRate(alpha);

	for (int i = 0; i < MENU_CURSOR_MAX; i++)
	{
		mUIMenuSelect[i]->GetCSprite()->SetAlphaRate(alpha);
		mUIMenuSelect[i]->GetCSprite()->SetStringAlphaRate(alpha);
		mUIMenuCursor[i]->GetCSprite()->SetAlphaRate(alpha);
		mUIMenuCursor[i]->GetCSprite()->SetStringAlphaRate(alpha);
	}
}

void UIGame::UpdatePauseMenu(float deltaTime)
{
	if (Keyboard::Inst()->KeyHit(DIK_S))
	{
		PlaySE(SE_MENU_SELECT);
		SetMenuSelectCursor((mMenuSelectCursorY + 1) % MENU_CURSOR_MAX);
		mKeyDownLimit = LIMITTIME * 5;
	}
	else if (Keyboard::Inst()->KeyDown(DIK_S) > 0 && mKeyDownLimit.IsEnd())
	{
		PlaySE(SE_MENU_SELECT);
		SetMenuSelectCursor((mMenuSelectCursorY + 1) % MENU_CURSOR_MAX);
		mKeyDownLimit = LIMITTIME;
	}

	if (Keyboard::Inst()->KeyHit(DIK_W))
	{
		PlaySE(SE_MENU_SELECT);
		SetMenuSelectCursor((mMenuSelectCursorY + MENU_CURSOR_MAX - 1) % MENU_CURSOR_MAX);
		mKeyDownLimit = LIMITTIME * 5;
	}
	else if (Keyboard::Inst()->KeyDown(DIK_W) > 0 && mKeyDownLimit.IsEnd())
	{
		PlaySE(SE_MENU_SELECT);
		SetMenuSelectCursor((mMenuSelectCursorY + MENU_CURSOR_MAX - 1) % MENU_CURSOR_MAX);
		mKeyDownLimit = LIMITTIME;
	}
}


void UIGame::SetMenuSelectCursor(int cursor)
{
	for (int i = 0; i < MENU_CURSOR_MAX; i++)
	{
		mUIMenuSelect[i]->GetCSprite()->SetColor(0x00CFCFCF);
		mUIMenuSelect[i]->GetCSprite()->SetStringColor(0xFFFFFFFF);
		mUIMenuSelect[i]->SetBlink(0);
	}
	mUIMenuSelect[cursor]->GetCSprite()->SetStringColor(0xFFEE7800);
	mUIMenuSelect[cursor]->SetBlink(2);
	mMenuSelectCursorY = cursor;
}


DummyUI::DummyUI(Scene * scene)
	: UIObject(scene)
{
	SetPosition(Vector2(0.5f*GetWindowWidth(), 0.92f*GetWindowHeight()));
	SetSize(Vector2(0.05f*GetWindowWidth(), 0.05f* GetWindowWidth()));
	mUISprite = new ComponentUISprite(this, TextureTag::CatSkill, 0);
	mCCUSprite = new ComponentCircleUISprite(this, TextureTag::CatSkill, -1);
	if (scene->GetStageManager()->GetCurrentStageID() == 0)
	{
		mUISprite->SetTexture(TextureTag::Invisible);
		mCCUSprite->SetTexture(TextureTag::Invisible);
	}
	mUISprite->SetAnimation(1, 1, 1, 0, false);
	mCCUSprite->SetAnimation(0, 0, 1, 0, false);
	mUISprite->SetColor(0xFF7F7F7F);
}

void DummyUI::UpdateUIObject(float deltaTime)
{
	//mUISprite->SetGreen(0);
	//mUISprite->SetBlue(0);
	if (GetScene()->GetPlayer()->IsUsingDummy())
	{
		if (!mIsBlink)
		{
			mIsBlink = true;
			SetBlink(1, 0);
		}
	}
	else
	{
		mIsBlink = false;
		SetBlink(0, 0);
	}
	float p = 1 - GetScene()->GetPlayer()->DummyCTPercentage();
	mCCUSprite->SetFill(p);
	if (p != 1) mCCUSprite->SetColorRate(0.3f);
	else mCCUSprite->SetColorRate(1.0f);
}

void DummyUI::SetAlphaRate(float rate)
{
	mUISprite->SetAlphaRate(rate);
	mCCUSprite->SetAlphaRate(rate);
}
