#include "ComponentUI9Slice.h"
#include "draw.h"

static const float MAX_CORNER_SIZE = 32;

void ComponentUI9Slice::Draw()
{
	if (!mIsActive)return;
	if (mOwner->GetState() == AbstractObject::State::eInactive)return;

	Vector2 pos = mOwner->GetPosition();
	Vector2 size = mOwner->GetSize();
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
		DrawUIString(mText, mStringInfoCharactersPerLine, mStringInfoOrigin, mStringInfoFontSize, TextColor());
	}
}
