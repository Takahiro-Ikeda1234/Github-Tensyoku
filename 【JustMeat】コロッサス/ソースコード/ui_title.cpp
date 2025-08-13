#include "ui_title.h"
#include "ComponentUISprite.h"
#include "CameraManager.h"
#include "SceneTitle.h"
#include "mouse.h"
#include "keyboard.h"
#include "SceneManager.h"
#include "EventManager.h"
#include "UI9Slice.h"
#include "AudioData.h"
#include "ini.h"

const float UITitle::LIMITTIME = 0.1f;

UITitle::UITitle(SceneTitle * scene)
	: mScene(scene)
{
	float xPos = GetConfigFloat("UI", "TitleX");
	float yPos = GetConfigFloat("UI", "TitleY");
	float xOffset = GetConfigFloat("UI", "FrameOffsetX");
	float yOffset = GetConfigFloat("UI", "FrameOffsetY");
	bool anyKeyCenter = GetConfigInt("UI", "PressAnyKeyCenter");
	float titleUIScale = 0.8f;
	Vector2 framePos = Vector2(xPos + xOffset, yPos + yOffset);
	if (anyKeyCenter)
	{
		mUIPressAnyKey = new UIRectButton(scene, 0.5f, 0.65f, 0.125f, UIObject::ToHeightSize(0.02f), TextureTag::Invisible, 50);
	}
	else
	{
		mUIPressAnyKey = new UIRectButton(scene, xPos, 0.65f, 0.125f, UIObject::ToHeightSize(0.02f), TextureTag::Invisible, 50);
	}
	mUIPressAnyKey->GetCSprite()->SetString(mTextPressAnyKey);
	mUIPressAnyKey->GetCSprite()->SetStringColor(0xFFFFFFFF);
	mUIPressAnyKey->GetCSprite()->SetStringInfo(10, mUIPressAnyKey->GetPosition(), Vector2(32, 32), true);
	HidePressAnyKey();

	//mUIMenuSelect[0] = new UI9Slice(scene, 0.5f, 0.64f, 0.125f, UIObject::ToHeightSize(0.03f), TextureTag::UI9Sliced, 50);
	//mUIMenuSelect[1] = new UI9Slice(scene, 0.5f, 0.72f, 0.125f, UIObject::ToHeightSize(0.03f), TextureTag::UI9Sliced, 50);
	//mUIMenuSelect[2] = new UI9Slice(scene, 0.5f, 0.80f, 0.125f, UIObject::ToHeightSize(0.03f), TextureTag::UI9Sliced, 50);
	mUIFrame = new UIBox(scene, framePos.x, framePos.y, 512 * 0.0005f * titleUIScale, 272 * 0.0005f * titleUIScale, TextureTag::TitleFrame, 51);
	mUIMenuSelect[0] = new UIBox(scene, framePos.x, framePos.y - 0.105f, 0.15f * titleUIScale , UIObject::ToHeightSize(0.04f) * titleUIScale, TextureTag::TitleButton, 50);
	mUIMenuSelect[1] = new UIBox(scene, framePos.x, framePos.y - 0.025f, 0.15f * titleUIScale , UIObject::ToHeightSize(0.04f) * titleUIScale, TextureTag::TitleButton, 50);
	mUIMenuSelect[2] = new UIBox(scene, framePos.x, framePos.y + 0.055f, 0.15f * titleUIScale , UIObject::ToHeightSize(0.04f) * titleUIScale, TextureTag::TitleButton, 50);

	for (int i = 0; i < MENU_CURSOR_MAX; i++)
	{
		mUIMenuSelect[i]->GetCSprite()->SetString(mTextMenuSelect[i]);
		mUIMenuSelect[i]->GetCSprite()->SetStringInfo(10, mUIMenuSelect[i]->GetPosition(), Vector2(32, 32) * titleUIScale, true);
		mUIMenuSelect[i]->GetCSprite()->SetAlphaRate(0.8f);
		mUIMenuSelect[i]->GetCSprite()->SetStringColor(0xFFFFFFFF);
	}
	mBox = new UI9Slice(scene, 0.5f, 0.72f, 0.15f, 0.08f, TextureTag::UI9Sliced, 101);

	HideMenuSelect();

	SetState(TitleState::ePressAnyKey);
	mControl = new UIBox(scene, 0.5f, 0.96f, 0.5f, 0.04f, TextureTag::Control, 1);
	mWASD = new UIBox(scene, 0.25f, 0.96f, 0.025f, 0.0125f, TextureTag::KeyWASD, 0);
	mWASD->GetCSprite()->SetString("選択");
	mWASD->GetCSprite()->SetStringInfo(5, mWASD->GetPosition() + Vector2(GetWindowWidth() * 0.03f, 0), Vector2(20, 20), false);
	
	mWASD->GetCSprite()->SetScale(1, 1.5f);
	mEnter = new UIBox(scene, 0.5f, 0.96f, 0.0145f, 0.0185f, TextureTag::KeyEnter, 0);
	mEnter->GetCSprite()->SetString("決定");
	mEnter->GetCSprite()->SetStringInfo(5, mEnter->GetPosition() + Vector2(GetWindowWidth() * 0.02f, GetWindowHeight() * -0.01f), Vector2(20, 20), false);

	mESC = new UIBox(scene, 0.75f, 0.96f, 0.0125f, 0.0125f, TextureTag::KeyEsc, 0);
	mESC->GetCSprite()->SetString("キャンセル");
	mESC->GetCSprite()->SetStringInfo(5, mESC->GetPosition() + Vector2(GetWindowWidth() * 0.02f, GetWindowHeight() * -0.01f), Vector2(20, 20), false);

	auto title = new UIBox(scene, xPos, yPos, 0.25f, 0.125f, TextureTag::Colossus, 200);
}

void UITitle::Update(float deltaTime)
{
	mKeyDownLimit -= deltaTime;
	switch (mState)
	{
	case TitleState::eMenuSelect:
		if(mScene->GetSceneManager()->CanSceneChange())
		{
			mUIMenuSelect[mMenuSelectCursorY]->GetCSprite()->SetColor(0xFFFFFFFF);
			StateMenuSelect(deltaTime);
		}
		mControl->GetCSprite()->SetAlphaRate(1);
		mControl->GetCSprite()->SetStringAlphaRate(1);
		mWASD->GetCSprite()->SetAlphaRate(1);
		mWASD->GetCSprite()->SetStringAlphaRate(1);
		mEnter->GetCSprite()->SetAlphaRate(1);
		mEnter->GetCSprite()->SetStringAlphaRate(1);
		mESC->GetCSprite()->SetAlphaRate(1);
		mESC->GetCSprite()->SetStringAlphaRate(1);
		break;
	case TitleState::ePressAnyKey:
		StatePressAnyKey(deltaTime);
		mControl->GetCSprite()->SetAlphaRate(0);
		mControl->GetCSprite()->SetStringAlphaRate(0);
		mWASD->GetCSprite()->SetAlphaRate(0);
		mWASD->GetCSprite()->SetStringAlphaRate(0);
		mEnter->GetCSprite()->SetAlphaRate(0);
		mEnter->GetCSprite()->SetStringAlphaRate(0);
		mESC->GetCSprite()->SetAlphaRate(0);
		mESC->GetCSprite()->SetStringAlphaRate(0);
		break;
	}
}

void UITitle::SetState(TitleState state)
{ 
	switch (mState)
	{
	case TitleState::eMenuSelect:
		HideMenuSelect();
		break;
	case TitleState::ePressAnyKey:
		HidePressAnyKey();
		break;
	}

	mState = state;

	switch (mState)
	{
	case TitleState::eMenuSelect:
		ShowMenuSelect();
		break;
	case TitleState::ePressAnyKey:
		ShowPressAnyKey();
		break;
	}
}

void UITitle::SetMenuSelectCursor(int cursor)
{
	if (mState == TitleState::eMenuSelect)
	{
		for (int i = 0; i < MENU_CURSOR_MAX; i++)
		{
			mUIMenuSelect[i]->GetCSprite()->SetAlpha(0xFF);
			mUIMenuSelect[i]->GetCSprite()->SetStringAlpha(0xFF);
			mUIMenuSelect[i]->GetCSprite()->SetColor(0xFF7F7F7F);
			mUIMenuSelect[i]->GetCSprite()->SetStringColor(0xFFFFFFFF);
			mUIMenuSelect[i]->SetBlink(0);
			mUIMenuSelect[i]->GetCSprite()->SetAnimation(1, 1, 1, 0);
		}
		mUIMenuSelect[cursor]->GetCSprite()->SetStringColor(0xFFEE7800);
		mUIMenuSelect[cursor]->SetBlink(2);
		mUIMenuSelect[cursor]->GetCSprite()->SetAnimation(0, 0, 1, 0);
	}
	mMenuSelectCursorY = cursor;
}

void UITitle::StatePressAnyKey(float deltaTime)
{
	if (Keyboard::Inst()->KeyHit(DIK_RETURN))
	{
		SetState(TitleState::eMenuSelect);
		PlaySE(SE_MENU_DECIDE);
	}
	//if (Keyboard::Inst()->KeyHit(DIK_F2))
	//{
	//	Parameter param;
	//	mScene->GetSceneManager()->SceneChange(SceneManager::SceneID::eCredit, &param, 0, 0, 0, false);
	//}
}

void UITitle::HidePressAnyKey()
{
	mUIPressAnyKey->GetCSprite()->SetAlpha(0x00);
	mUIPressAnyKey->GetCSprite()->SetStringAlpha(0x00);
}

void UITitle::ShowPressAnyKey()
{
	mUIPressAnyKey->GetCSprite()->SetAlpha(0xFF);
	mUIPressAnyKey->GetCSprite()->SetStringAlpha(0xFF);
	mUIPressAnyKey->SetBlink(2);
}

void UITitle::StateMenuSelect(float deltaTime)
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


	if (Keyboard::Inst()->KeyHit(DIK_ESCAPE))
	{
		SetState(TitleState::ePressAnyKey);
		PlaySE(SE_MENU_CANCEL);
	}
	else if (Keyboard::Inst()->KeyHit(DIK_RETURN))
	{
		if (mScene->GetSceneManager()->CanSceneChange())
		{
			PlaySE(SE_MENU_DECIDE);
			Parameter param;
			switch (mMenuSelectCursorY)
			{
			case 0:
				SetFadeout(0, 100, 0.75f);
				mUIMenuSelect[mMenuSelectCursorY]->SetBlink(0.2f, 0.5f);
				mScene->GetSceneManager()->SceneChange(SceneManager::eOpening, &param, 0.5f, 0.75f, 0.5f);
				break;
			case 1:
				param.Set("ステージ", 0);
				mUIMenuSelect[mMenuSelectCursorY]->SetBlink(0.2f, 0.5f);
				mScene->GetSceneManager()->SceneChange(SceneManager::eStageSelect, &param, 0.5f, 0.75f, 0.5f);
				break;
			case 2:
				mScene->GetEventManager()->Fadeout(0.4f, 0.1f);
				mUIMenuSelect[mMenuSelectCursorY]->SetBlink(0.1f, 0.4f);
				mScene->GetSceneManager()->SceneChange(SceneManager::eExit, &param, 0.4f, 0.4f, 0);
				break;
			}
		}
	}
	if (!mScene->GetSceneManager()->GetDebugMode())return;
	int editorID = -1;
	if (Keyboard::Inst()->KeyHit(DIK_0)) editorID = 0;
	if (Keyboard::Inst()->KeyHit(DIK_1)) editorID = 1;
	if (Keyboard::Inst()->KeyHit(DIK_2)) editorID = 2;
	if (Keyboard::Inst()->KeyHit(DIK_3)) editorID = 3;
	if (Keyboard::Inst()->KeyHit(DIK_4)) editorID = 4;
	if (Keyboard::Inst()->KeyHit(DIK_5)) editorID = 5;
	if (Keyboard::Inst()->KeyHit(DIK_6)) editorID = 6;
	if (Keyboard::Inst()->KeyHit(DIK_7)) editorID = 7;
	if (Keyboard::Inst()->KeyHit(DIK_8)) editorID = 8;

	if (editorID != -1)
	{
		Parameter param;
		param.Set("ステージ", editorID);
		mScene->GetSceneManager()->SceneChange(SceneManager::eEditor, &param, 0, 0, 0);
	}
}

void UITitle:: HideMenuSelect()
{
	for (int i = 0; i < MENU_CURSOR_MAX; i++)
	{
		mUIMenuSelect[i]->SetBlink(0, 0);
		mUIMenuSelect[i]->GetCSprite()->SetAlpha(0x00);
		mUIMenuSelect[i]->GetCSprite()->SetStringAlpha(0x00);
	}
	mUIFrame->GetCSprite()->SetAlpha(0);
	mBox->GetCSprite()->SetAlpha(0);
}

void UITitle::ShowMenuSelect()
{
	SetMenuSelectCursor(mMenuSelectCursorY);
	mBox->GetCSprite()->SetAlpha(0);
	mUIFrame->GetCSprite()->SetAlpha(0xFF);
}
