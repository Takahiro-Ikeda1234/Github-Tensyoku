//=============================================================================
//
// [UIObject.cpp] オブジェクト共通処理
//
//=============================================================================

//インクルードファイル
#include"Scene.h"
#include<vector>
#include"UIObject.h"
#include"ComponentClickEvent.h"
#include"Windows_core.h"
#include"ComponentUISprite.h"
#include"keyboard.h"
#include"myMath.h"
// ---------------------------------------------------------------------------
// 関数名: UIObject::UIObject(Scene* scene)
// 引数:   Scene* scene シーンのポインタ
// 説明:   UIObjectのコンストラクタ
// ---------------------------------------------------------------------------
UIObject::UIObject(Scene* scene)
	: AbstractObject(scene)
	, mClick(nullptr)
	, mUISprite(nullptr)
{
	GetScene()->AddUIObject(this);
}
// ---------------------------------------------------------------------------
// 関数名: UIObject::~UIObject()
// 説明:   UIObjectのデストラクタ
// ---------------------------------------------------------------------------
UIObject::~UIObject()
{
	//シーンから自身をRemove
	GetScene()->RemoveUIObject(this);
}

bool UIObject::Clicked()
{
	if (GetState() == State::eInactive) return false;
	return mClick->OnClicked();
}

bool UIObject::OuterClicked()
{
	if (GetState() == State::eInactive) return false;
	return mClick->OuterClicked();
}

void UIObject::SetVisible()
{
	mUISprite->SetActive(true);
	SetState(State::eActive);
}

void UIObject::SetInisible()
{
	mUISprite->SetActive(false);
	SetState(State::eInactive);
}

void UIObject::UpdateObject(float deltaTime)
{
	if (mBlink.GetMaxTime() != 0)
	{
		mUISprite->SetStringColorRate(0.7f + 0.3f * degSin(360 * (1 - mBlink.Percentage())));
		mUISprite->SetColorRate(0.7f + 0.3f * degSin(360 * (1 - mBlink.Percentage())));
		//mUISprite->SetStringAlpha(0x90 + 0x6F * degSin(360 * (1 - mBlink.Percentage())));
		//mUISprite->SetAlpha(0x90 + 0x6F * degSin(360 * (1 - mBlink.Percentage())));
	}
	if (mBlinkTime.IsFinish())
	{
		SetBlink(0, 0);
	}
	mBlink -= deltaTime;
	mBlinkTime -= deltaTime;
	UpdateUIObject(deltaTime);
}

float UIObject::ToHeightSize(float ySize)
{
	return ySize * GetWindowHeight() / GetWindowWidth();
}
float UIObject::ToWidthSize(float ySize)
{
	return ySize * GetWindowWidth() / GetWindowHeight();
}

void UIObject::SetBlink(float blink, float blinkTime)
{
	mBlinkTime = blinkTime;
	mBlink = blink;
	if (blink == 0)
	{
		mUISprite->SetStringColorRate(1);
		mUISprite->SetColorRate(1);
	}
	else
	{
		mUISprite->SetStringColorRate(0.7f);
		mUISprite->SetColorRate(0.7f);
	}
}

UICircleButton::UICircleButton(Scene* scene, float xPos, float yPos, float radius, TextureTag tag, int drawPriority)
	: UIObject(scene)
{
	SetPosition(Vector2(xPos*GetWindowWidth(), yPos*GetWindowHeight()));
	SetSize(Vector2(2 * radius*GetWindowWidth(), 2 * radius*GetWindowWidth()));
	mClick = new ComponentClickEventCircleUI(this, radius*GetWindowWidth());
	mUISprite = new ComponentUISprite(this, tag, drawPriority);
}

void UICircleButton::UpdateUIObject(float deltaTime)
{
	if (mClick->OnMouse())
	{
		mUISprite->SetColor(mUISprite->GetAlpha() << 24 | 0xFFFFFF);
		//mUISprite->SetAlpha(0xFF);
		//mUISprite->SetColorRate(1);
	}
	else
	{
		mUISprite->SetColor(mUISprite->GetAlpha() << 24 | 0x7F7F7F);
		//mUISprite->SetColor(0xFF7F7F7F);
		//mUISprite->SetAlpha(0x4F);
		//mUISprite->SetColorRate(0.5f);
	}
}

UIRectButton::UIRectButton(Scene * scene, float xPos, float yPos, float xSize, float ySize, TextureTag tag, int drawPriority)
	: UIObject(scene)
{
	SetPosition(Vector2(xPos*GetWindowWidth(), yPos*GetWindowHeight()));
	SetSize(Vector2(2 * xSize*GetWindowWidth(), 2 * ySize*GetWindowWidth()));
	mClick = new ComponentClickEventRectUI(this);
	mUISprite = new ComponentUISprite(this, tag, drawPriority);
}

void UIRectButton::UpdateUIObject(float deltaTime)
{
	if (mClick->OnMouse())
	{
		mUISprite->SetColor(mUISprite->GetAlpha() << 24 | 0xFFFFFF);
		//mUISprite->SetColor(0xFFFFFFFF);
		//mUISprite->SetAlpha(0xFF);
		//mUISprite->SetColorRate(1);
	}
	else
	{
		mUISprite->SetColor(mUISprite->GetAlpha() << 24 | 0x7F7F7F);
		//mUISprite->SetColor(0xFF7F7F7F);
		//mUISprite->SetAlpha(0x4F);
		//mUISprite->SetColorRate(0.5f);
	}
}

UIBox::UIBox(Scene * scene, float xPos, float yPos, float xSize, float ySize, TextureTag tag, int drawPriority)
	: UIObject(scene)
{
	SetPosition(Vector2(xPos*GetWindowWidth(), yPos*GetWindowHeight()));
	SetSize(Vector2(2 * xSize*GetWindowWidth(), 2 * ySize*GetWindowWidth()));
	mClick = new ComponentClickEventRectUI(this);
	mUISprite = new ComponentUISprite(this, tag, drawPriority);
}

void UIRectSwitch::UpdateUIObject(float deltaTime)
{
	UIRectButton::UpdateUIObject(deltaTime);
	mUISprite->SetAlpha(0x4F);
	if (Clicked())
	{
		SetSwitchActive(true);
	}

	if (mIsSwitchActive)
	{
		Active(deltaTime);
	}
}

void UIRectSwitch::Active(float deltaTime)
{
	mUISprite->SetAlpha(0xFF);
}

UIRectNumBox::UIRectNumBox(Scene * scene, int defaultNum, int min, int max, float xPos, float yPos, float xSize, float ySize, TextureTag tag, int drawPriority)
	: UIRectButton(scene, xPos, yPos, xSize, ySize, tag, drawPriority)
	, mCurrentNum(defaultNum)
	, mMin(min)
	, mMax(max)
	, mIsBoxActive(false)
	, mDigit(0)
{
}

void UIRectNumBox::UpdateUIObject(float deltaTime)
{
	if (mClick->OnClicked()) mIsBoxActive = true;
	if (mIsBoxActive)
	{
		mUISprite->SetAlpha(0xFF);
		for (int i = 0; i < 9; i++)
		{
			if (Keyboard::Inst()->KeyHit(DIK_1) + i)
			{
				if (mDigit < mMaxDigit)
				{
					mCurrentNumString[mDigit] = i + 1 + '0';
					mDigit++;
				}
			}
		}
		if (Keyboard::Inst()->KeyHit(DIK_0))
		{
			if (mDigit < mMaxDigit)
			{
				mCurrentNumString[mDigit] = '0';
				mDigit++;
			}
		}
		if (Keyboard::Inst()->KeyHit(DIK_BACK))
		{
			if (mDigit > 0)
			{
				mCurrentNumString[mDigit] = '\0';
				mDigit--;
			}
		}
		if (mClick->OuterClicked() || Keyboard::Inst()->KeyHit(DIK_RETURN))
		{
			mCurrentNum = atoi(mCurrentNumString);
			if (mMin > mCurrentNum) mCurrentNum = mMin;
			if (mMax < mCurrentNum) mCurrentNum = mMax;
			mIsBoxActive = false;
			mDigit = sprintf(mCurrentNumString, "%d", mCurrentNum);
		}
	}
}

void UIBoxFade::UpdateUIObject(float deltaTime)
{
	if (!mFadeinDelay.IsEnd())
	{
		GetCSprite()->SetAlphaRate(0);
		GetCSprite()->SetStringAlphaRate(0);
		mFadeinDelay -= deltaTime;
		return;
	}
	if (!mFadeinTime.IsEnd())
	{
		GetCSprite()->SetAlphaRate(1 - mFadeinTime.Percentage());
		GetCSprite()->SetStringAlphaRate(1 - mFadeinTime.Percentage());
		mFadeinTime -= deltaTime;
		return;
	}
	if (!mShowTime.IsEnd())
	{
		GetCSprite()->SetAlphaRate(1);
		GetCSprite()->SetStringAlphaRate(1);
		mShowTime -= deltaTime;
		return;
	}
	if (!mFadeoutTime.IsEnd())
	{
		GetCSprite()->SetAlphaRate(mFadeoutTime.Percentage());
		GetCSprite()->SetStringAlphaRate(mFadeoutTime.Percentage());
		mFadeoutTime -= deltaTime;
		return;
	}
	GetCSprite()->SetAlphaRate(0);
	GetCSprite()->SetStringAlphaRate(0);
}

void UIBoxFade::SetShowTime(float fadeinTime, float showTime, float fadeoutTime, float fadeinDelay)
{
	mFadeinTime = fadeinTime;
	mFadeinDelay = fadeinDelay;
	mShowTime = showTime;
	mFadeoutTime = fadeoutTime;
}
