#include "ComponentMessageBox.h"
#include"AbstractObject.h"
#include"CameraManager.h"
#include"Scene.h"
#include"draw.h"
static const float MAX_CORNER_SIZE = 32;

void ComponentMessageBox::Draw()
{
	if (!mIsActive)return;
	if (mOwner->GetState() == AbstractObject::State::eInactive)return;
	if (mText.length() == 0)return;


	Vector2 offset = Vector2(GetWindowWidth()* 0.5f, 0);
	if (!mAppearTime.IsEnd())
	{
		offset = offset * mAppearTime.Percentage();
	}
	else if (!mDisappearTime.IsEnd())
	{
		offset = offset * (1 - mDisappearTime.Percentage());
	}
	else if (mShow)
	{
		offset = Vector2(0, 0);
	}
	Vector2 pos = Vector2(GetWindowWidth() * 0.85f, GetWindowHeight() * 0.75f) + offset;
	Vector2 size(GetWindowWidth() * 0.3f, GetWindowHeight() * 0.16f);
	float cornerSize = MIN(MIN(size.x / 2, size.y / 2), MAX_CORNER_SIZE);

	float topSizeY = cornerSize;
	float centerSizeY = size.y / 2 - cornerSize;
	float bottomSizeY = cornerSize;
	float topPosY = pos.y - centerSizeY - cornerSize / 2;
	float centerPosY = pos.y;
	float bottomPosY = pos.y + centerSizeY + cornerSize / 2;

	float leftSizeX = cornerSize;
	float centerSizeX = size.x / 2 - cornerSize;
	float rightSizeX = cornerSize;
	float leftPosX = pos.x - centerSizeX - cornerSize / 2;
	float centerPosX = pos.x;
	float rightPosX = pos.x + centerSizeX + cornerSize / 2;

	centerSizeY *= 2;
	centerSizeX *= 2;

	Vector2 screenPos[9] = { {leftPosX, topPosY},{centerPosX, topPosY},{rightPosX, topPosY}, {leftPosX, centerPosY},{centerPosX, centerPosY},{rightPosX, centerPosY} , {leftPosX, bottomPosY},{centerPosX, bottomPosY},{rightPosX, bottomPosY} };
	Vector2 screenSize[9] = { {leftSizeX, topSizeY},{centerSizeX, topSizeY},{rightSizeX, topSizeY}, {leftSizeX, centerSizeY},{centerSizeX, centerSizeY},{rightSizeX, centerSizeY} , {leftSizeX, bottomSizeY},{centerSizeX, bottomSizeY},{rightSizeX, bottomSizeY} };


	switch (mDrawMode)
	{
	case DrawMode::eAdd:
		BlendAddBlend();
		for (int i = 0; i < 9; i++)
		{
			DrawUITexture(mTexture->GetTexture(), screenPos[i], screenSize[i] * mTextureScale, 0, i, mTexX, mTexY, Color());
		}
		BlendDefault();
		break;
	case DrawMode::eMultiple:
		BlendMultipleBlend();

		for (int i = 0; i < 9; i++)
		{
			DrawUITexture(mTexture->GetTexture(), screenPos[i], screenSize[i] * mTextureScale, 0, i, mTexX, mTexY, Color());
		}
		BlendDefault();
		break;
	case DrawMode::eDefault:

		for (int i = 0; i < 9; i++)
		{
			DrawUITexture(mTexture->GetTexture(), screenPos[i], screenSize[i] * mTextureScale, 0, i, mTexX, mTexY, Color());
		}
		break;
	}
	if (mDrawText)
	{
		//DrawUIString(mText, mStringInfoCharactersPerLine, mStringInfoOrigin, mStringInfoFontSize, TextColor());
		DrawUIString(mText, mStringInfoCharactersPerLine, pos - size / 2 + size * 0.1f/*mStringOffset*/, mStringInfoFontSize, mColorArray);
	}
}


void ComponentMessageBox::Update(float deltaTime)
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
void ComponentMessageBox::SetString(std::string str, int coloredStart, int coloredCount)
{
	if (str == mText)return;
	mColorArray = new unsigned[str.size()];
	int i = 0;
	for (; i < coloredStart; i++) mColorArray[i] = 0xFFFFFFFF;
	for (; i < coloredStart + coloredCount; i++) mColorArray[i] = 0xFFEE7800;
	for (; i < str.size(); i++) mColorArray[i] = 0xFFFFFFFF;
	if (mDrawText)
	{
		FontSystem::Inst()->Release(mText);
	}
	mText = str;
	mDrawText = true;
	FontSystem::Inst()->Acquire(mText);

	int width = FontSystem::Inst()->GetWidth(mText, mStringInfoFontSize, mStringInfoCharactersPerLine);
	int height = FontSystem::Inst()->GetHeight(mText, mStringInfoFontSize, mStringInfoCharactersPerLine);
	mStringOffset = -1 * (Vector2(width / 2, height / 2));
}

Vector2 ComponentMessageBox::GetPosition()
{
	Vector2 offset = Vector2(GetWindowWidth()* 0.5f, 0);
	if (!mAppearTime.IsEnd())
	{
		offset = offset * mAppearTime.Percentage();
	}
	else if (!mDisappearTime.IsEnd())
	{
		offset = offset * (1 - mDisappearTime.Percentage());
	}
	else if (mShow)
	{
		offset = Vector2(0, 0);
	}
	Vector2 pos = Vector2(GetWindowWidth() * 0.85f, GetWindowHeight() * 0.75f) + offset;
	return pos;
}

Vector2 ComponentMessageBox::GetSize()
{
	Vector2 size(GetWindowWidth() * 0.3f, GetWindowHeight() * 0.16f);
	return size;
}
