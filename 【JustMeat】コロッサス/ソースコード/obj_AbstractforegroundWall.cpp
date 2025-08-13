#include "obj_AbstractForegroundWall.h"
#include "StageManager.h"
#include "ComponentAnimationSprite.h"
AbstractForegroundWall::AbstractForegroundWall(Scene * scene, Vector2 pos)
	: Object(scene, pos)
{
	mIsForeground = true;
	mCASprite = new ComponentAnimationSprite(this, TextureTag::Block, 10);
	SetObjectType(Type::eForeground);
}
AbstractForegroundWall::AbstractForegroundWall(Scene * scene, int indexX, int indexY)
	: AbstractForegroundWall(scene, StageManager::ToPosition(indexX, indexY))
{
}


void AbstractForegroundWall::UpdateObject(float deltaTime)
{
	//float destinationAlpha;
	//switch (GetScene()->InCave())
	//{
	//case -1:
	//	destinationAlpha = 1;
	//	break;
	//case 0:
	//	destinationAlpha = 0.25f;
	//	break;
	//case 1:
	//	destinationAlpha = 0;
	//	break;
	//}
	//if (mFloatColor > destinationAlpha)
	//{
	//	mFloatColor -= (1 / FOREGROUND_FADESPEED) * deltaTime;
	//	if (mFloatColor < 0) mFloatColor = 0;
	//	mCASprite->SetAlphaRate(mFloatColor);
	//}

	//if (mFloatColor < destinationAlpha)
	//{
	//	mFloatColor += (1 / FOREGROUND_FADESPEED) * deltaTime;
	//	if (mFloatColor > 0) mFloatColor = 0;
	//	mCASprite->SetAlphaRate(mFloatColor);
	//}
}