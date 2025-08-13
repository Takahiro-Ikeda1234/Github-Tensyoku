#include "ComponentSystemMessage.h"
#include"AbstractObject.h"
#include"CameraManager.h"
#include"Scene.h"
#include"draw.h"
void ComponentSystemMessage::Draw()
{
	if (!mIsActive)return;
	if (mOwner->GetState() == AbstractObject::State::eInactive)return;
	if (mText.length() == 0)return;



	//Vector2 offset = Vector2(GetWindowWidth()* 0.3f, 0);
	//if (!mAppearTime.IsEnd())
	//{
	//	offset = offset * mAppearTime.Percentage();
	//}
	//else if (!mDisappearTime.IsEnd())
	//{
	//	offset = offset * (1 - mDisappearTime.Percentage());
	//}
	//else if (mShow)
	//{
	//	offset = Vector2(0, 0);
	//}
	if (!mAppearTime.IsEnd())
	{
		SetStringAlphaRate(1 - mAppearTime.Percentage());
	}
	else if (!mDisappearTime.IsEnd())
	{
		SetStringAlphaRate(mDisappearTime.Percentage());
	}
	else if (mShow)
	{
		SetStringAlphaRate(1);
	}
	else
	{
		SetStringAlphaRate(0);
	}
	UpdateMessageColor(GetStringAlphaRate());
	Vector2 offset = Vector2(0, 0);
	
	Vector2 pos = Vector2(GetWindowWidth() * 0.85f, GetWindowHeight() * 0.95f) + offset;
	Vector2 size(GetWindowWidth() * 0.3f, GetWindowHeight() * 0.16f);

	float centerSizeY = size.y / 2;
	float centerPosY = pos.y;
	float centerSizeX = size.x / 2;
	float centerPosX = pos.x;

	centerSizeY *= 2;
	centerSizeX *= 2;

	Vector2 screenPos = { centerPosX, centerPosY };
	Vector2 screenSize = { centerSizeX, centerSizeY};


	switch (mDrawMode)
	{
	case DrawMode::eAdd:
		BlendAddBlend();
		DrawUITexture(mTexture->GetTexture(), screenPos, screenSize * mTextureScale, 0, 0, mTexX, mTexY, Color());
		BlendDefault();
		break;
	case DrawMode::eMultiple:
		BlendMultipleBlend();
		DrawUITexture(mTexture->GetTexture(), screenPos, screenSize * mTextureScale, 0, 0, mTexX, mTexY, Color());
		BlendDefault();
		break;
	case DrawMode::eDefault:
		DrawUITexture(mTexture->GetTexture(), screenPos, screenSize * mTextureScale, 0, 0, mTexX, mTexY, Color());
		break;
	}
	if (mDrawText)
	{
		DrawUIString(mText, mStringInfoCharactersPerLine, pos - size / 2 + size * 0.1f, mStringInfoFontSize, mColorArray);
	}
}


void ComponentSystemMessage::Update(float deltaTime)
{
	ComponentSprite::Update(deltaTime);
	mAppearTime -= deltaTime;
	mDisappearTime -= deltaTime;
	mMessageTime -= deltaTime;
	if (mMessageTime.IsFinish())
	{
		Disappear();
	}
}


#include"DX9_font.h"
void ComponentSystemMessage::SetString(std::string str, int coloredStart, int coloredCount)
{
	if (str == mText)return;
	mText = str;
	mColoredStart = coloredStart;
	mColoredCount = coloredCount;
	mColorArray = new unsigned[str.size()];
	UpdateMessageColor(GetStringAlphaRate());
	if (mDrawText)
	{
		FontSystem::Inst()->Release(mText);
	}
	mDrawText = true;
	FontSystem::Inst()->Acquire(mText);

	int width = FontSystem::Inst()->GetWidth(mText, mStringInfoFontSize, mStringInfoCharactersPerLine);
	int height = FontSystem::Inst()->GetHeight(mText, mStringInfoFontSize, mStringInfoCharactersPerLine);
	mStringOffset = -1 * (Vector2(width / 2, height / 2));
}

void ComponentSystemMessage::UpdateMessageColor(float alphaRate)
{
	unsigned char alpha = 0xFF * alphaRate;
	int i = 0;
	for (; i < mColoredStart; i++) mColorArray[i] = 0x00FFFFFF | alpha << 24;
	for (; i < mColoredStart + mColoredCount; i++) mColorArray[i] = 0x00EE7800 | alpha << 24;
	for (; i < mText.size(); i++) mColorArray[i] = 0x00FFFFFF | alpha < 24;
}