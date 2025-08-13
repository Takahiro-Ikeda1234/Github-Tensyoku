#include "ComponentUISprite.h"
#include"object.h"
#include"CameraManager.h"
#include"draw.h"
void ComponentUISprite::Draw()
{
	if (!mIsActive)return;
	if (mOwner->GetState() == AbstractObject::State::eInactive)return;
	Vector2 screenPos = mOwner->GetPosition();
	switch (mDrawMode)
	{
	case DrawMode::eAdd:
		BlendAddBlend();
		DrawUITexture(mTexture->GetTexture(), screenPos, mOwner->GetSize() * mTextureScale, mOwner->GetRotation(), mUseID, mTexX, mTexY, Color());
		BlendDefault();
		break;
	case DrawMode::eMultiple:
		BlendMultipleBlend();
		DrawUITexture(mTexture->GetTexture(), screenPos, mOwner->GetSize() * mTextureScale, mOwner->GetRotation(), mUseID, mTexX, mTexY, Color());
		BlendDefault();
		break;
	case DrawMode::eDefault:
		DrawUITexture(mTexture->GetTexture(), screenPos, mOwner->GetSize() * mTextureScale, mOwner->GetRotation(), mUseID, mTexX, mTexY, Color());
		break;
	}
	if (mDrawText)
	{
		DrawUIString(mText, mStringInfoCharactersPerLine, mStringInfoOrigin, mStringInfoFontSize, TextColor());
	}
}