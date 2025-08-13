#include "ComponentCircleUISprite.h"
#include"object.h"
#include"CameraManager.h"
#include"draw.h"
#include "DX9_draw.h"
void ComponentCircleUISprite::Draw()
{
	if (!mIsActive)return;
	if (mOwner->GetState() == AbstractObject::State::eInactive)return;
	Vector2 size = mOwner->GetSize() / 2 * mTextureScale;	//2x2に4分割した時の1つの大きさ
	Vector2 scrnpos = mOwner->GetPosition();
	Vector2 screenPos[4] = {
		scrnpos - size / 2,
		Vector2(scrnpos.x - size.x / 2, scrnpos.y + size.y / 2),
		Vector2(scrnpos.x + size.x / 2, scrnpos.y - size.y / 2),
		scrnpos + size / 2,
	};//2x2に4分割した時のそれぞれの中心位置

	EnableStencilDX9();
	Mask();
	DisableStencilDX9();
	//EnableStencilTest();
	switch (mDrawMode)
	{
	case DrawMode::eAdd:
		BlendAddBlend();
		if (mFill > 0.75f)
			DrawUITexture(mTexture->GetTexture(), screenPos[2], size, mOwner->GetRotation(), mUseID * 4 + 1, 2, mTexture->mDivY * 2, Color());
		if (mFill > 0.50f)
			DrawUITexture(mTexture->GetTexture(), screenPos[3], size, mOwner->GetRotation(), mUseID * 4 + 3, 2, mTexture->mDivY * 2, Color());
		if (mFill > 0.25f)
			DrawUITexture(mTexture->GetTexture(), screenPos[1], size, mOwner->GetRotation(), mUseID * 4 + 2, 2, mTexture->mDivY * 2, Color());
		if (mFill > 0.0f)
			DrawUITexture(mTexture->GetTexture(), screenPos[0], size, mOwner->GetRotation(), mUseID * 4 + 0, 2, mTexture->mDivY * 2, Color());
		BlendDefault();
		break;
	case DrawMode::eMultiple:
		BlendMultipleBlend();
		if (mFill > 0.75f)
			DrawUITexture(mTexture->GetTexture(), screenPos[2], size, mOwner->GetRotation(), mUseID * 4 + 1, 2, mTexture->mDivY * 2, Color());
		if (mFill > 0.50f)
			DrawUITexture(mTexture->GetTexture(), screenPos[3], size, mOwner->GetRotation(), mUseID * 4 + 3, 2, mTexture->mDivY * 2, Color());
		if (mFill > 0.25f)
			DrawUITexture(mTexture->GetTexture(), screenPos[1], size, mOwner->GetRotation(), mUseID * 4 + 2, 2, mTexture->mDivY * 2, Color());
		if (mFill > 0.0f)
			DrawUITexture(mTexture->GetTexture(), screenPos[0], size, mOwner->GetRotation(), mUseID * 4 + 0, 2, mTexture->mDivY * 2, Color());
		BlendDefault();
		break;
	case DrawMode::eDefault:
		if (mFill > 0.75f)
			DrawUITexture(mTexture->GetTexture(), screenPos[2], size, mOwner->GetRotation(), mUseID * 4 + 1, 2, mTexture->mDivY * 2, Color());
		if (mFill > 0.50f)
			DrawUITexture(mTexture->GetTexture(), screenPos[3], size, mOwner->GetRotation(), mUseID * 4 + 3, 2, mTexture->mDivY * 2, Color());
		if (mFill > 0.25f)
			DrawUITexture(mTexture->GetTexture(), screenPos[1], size, mOwner->GetRotation(), mUseID * 4 + 2, 2, mTexture->mDivY * 2, Color());
		if (mFill > 0.0f)
			DrawUITexture(mTexture->GetTexture(), screenPos[0], size, mOwner->GetRotation(), mUseID * 4 + 0, 2, mTexture->mDivY * 2, Color());
		break;
	}
	DisableStencilTestDX9();
	ClearStencilDX9();
}

void ComponentCircleUISprite::Mask()
{
	//TODO: tanのエラー対策をする
	Vector2 o = mOwner->GetPosition();
	Vector2 col = mOwner->GetColSize()  * mTextureScale;
	if (mFill <= 0)
	{
		mFill = 0;
		return;
	}
	if (mFill >= 0.99f)
	{
		mFill = 1;
		return;
	}
	float deg = (1 - mFill) * 360;


	if (deg < 45)
	{
		Vector2 stencilTriangle1[3];
		stencilTriangle1[0] = Vector2(o.x, o.y - col.y);
		stencilTriangle1[1] = Vector2(o.x + col.x * degTan(deg), o.y - col.y);
		stencilTriangle1[2] = mOwner->GetPosition();
		DrawUITriangle(stencilTriangle1);
	}
	else if (deg < 90)
	{
		Vector2 stencilTriangle1[3];
		stencilTriangle1[0] = Vector2(o.x, o.y - col.y);
		stencilTriangle1[1] = Vector2(o.x + col.x, o.y - col.y);
		stencilTriangle1[2] = mOwner->GetPosition();
		DrawUITriangle(stencilTriangle1);

		Vector2 stencilTriangle2[3];
		stencilTriangle2[0] = mOwner->GetPosition();
		stencilTriangle2[1] = Vector2(o.x + col.x, o.y - col.y);
		stencilTriangle2[2] = Vector2(o.x + col.x, o.y - col.x * (1 / degTan(deg)));
		DrawUITriangle(stencilTriangle2);
	}
	else if (deg < 135)
	{
		Vector2 stencilTriangle1[3];
		stencilTriangle1[0] = mOwner->GetPosition();
		stencilTriangle1[1] = Vector2(o.x + col.x, o.y);
		stencilTriangle1[2] = Vector2(o.x + col.x, o.y - col.x * (1 / degTan(deg)));
		DrawUITriangle(stencilTriangle1);
	}
	else if (deg < 180)
	{
		Vector2 stencilTriangle1[3];
		stencilTriangle1[0] = mOwner->GetPosition();
		stencilTriangle1[1] = Vector2(o.x + col.x, o.y);
		stencilTriangle1[2] = Vector2(o.x + col.x, o.y + col.x);
		DrawUITriangle(stencilTriangle1);


		Vector2 stencilTriangle2[3];
		stencilTriangle2[0] = mOwner->GetPosition();
		stencilTriangle2[1] = Vector2(o.x + col.x, o.y + col.y);
		stencilTriangle2[2] = Vector2(o.x - col.y * degTan(deg), o.y + col.y);
		DrawUITriangle(stencilTriangle2);
	}
	else if (deg < 225)
	{
		Vector2 stencilTriangle1[3];
		stencilTriangle1[0] = Vector2(o.x - col.y * degTan(deg), o.y + col.y);
		stencilTriangle1[1] = mOwner->GetPosition();
		stencilTriangle1[2] = Vector2(o.x, o.y + col.y);
		DrawUITriangle(stencilTriangle1);
	}
	else if (deg < 270)
	{
		Vector2 stencilTriangle1[3];
		stencilTriangle1[0] = Vector2(o.x - col.x, o.y + col.y);
		stencilTriangle1[1] = mOwner->GetPosition();
		stencilTriangle1[2] = Vector2(o.x, o.y + col.y);
		DrawUITriangle(stencilTriangle1);

		Vector2 stencilTriangle2[3];
		stencilTriangle2[0] = Vector2(o.x - col.x, o.y + col.y * (1 / degTan(deg)));
		stencilTriangle2[1] = mOwner->GetPosition();
		stencilTriangle2[2] = Vector2(o.x - col.x, o.y + col.y);
		DrawUITriangle(stencilTriangle2);
	}
	else if (deg < 315)
	{
		Vector2 stencilTriangle1[3];
		stencilTriangle1[0] = Vector2(o.x - col.x, o.y);
		stencilTriangle1[1] = Vector2(o.x - col.x, o.y + col.y * (1 / degTan(deg)));
		stencilTriangle1[2] = mOwner->GetPosition();
		DrawUITriangle(stencilTriangle1);
	}
	else
	{
		Vector2 stencilTriangle1[3];
		stencilTriangle1[0] = Vector2(o.x - col.x, o.y);
		stencilTriangle1[1] = Vector2(o.x - col.x, o.y - col.y);
		stencilTriangle1[2] = mOwner->GetPosition();
		DrawUITriangle(stencilTriangle1);

		Vector2 stencilTriangle2[3];
		stencilTriangle2[0] = Vector2(o.x - col.y, o.y - col.y);
		stencilTriangle2[1] = Vector2(o.x + col.y * degTan(deg), o.y - col.y);
		stencilTriangle2[2] = mOwner->GetPosition();
		DrawUITriangle(stencilTriangle2);
	}
}