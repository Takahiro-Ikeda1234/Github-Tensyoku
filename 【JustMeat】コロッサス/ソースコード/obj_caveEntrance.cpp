#include "obj_caveEntrance.h"
#include "StageManager.h"
#include "ComponentAnimationSprite.h"
#include "Scene.h"
#include"obj_player.h"
CaveEntrance::CaveEntrance(Scene * scene, Vector2 pos, bool isRight)
	: AbstractForegroundWall(scene, pos)
	, mIsRight(isRight)
{
	SetSize(Vector2(BLOCKSIZE, BLOCKSIZE * 3));
	mIsForeground = true;
	if (isRight)
	{
		delete mCASprite;
		mCASprite = new ComponentAnimationSprite(this, TextureTag::Entrance, 10);
	}
	else
	{
		delete mCASprite;
		mCASprite = new ComponentAnimationSprite(this, TextureTag::Entrance, 10);
	}
}
CaveEntrance::CaveEntrance(Scene * scene, int indexX, int indexY, bool isRight)
	: CaveEntrance(scene, StageManager::ToPosition(indexX, indexY), isRight)
{

}


void CaveEntrance::UpdateObject(float deltaTime)
{
	AbstractForegroundWall::UpdateObject(deltaTime);

	bool isPlayerEnteringCurrent;
	Vector2 plPos;
	Vector2 plCol;
	Vector2 pos;
	Vector2 col;
	if (mIsRight)
	{
		plPos = GetScene()->GetPlayer()->GetPosition();
		plCol = GetScene()->GetPlayer()->GetColSize();
		pos = GetPosition() - Vector2(BLOCKSIZE * 2, 0);
		col = Vector2(BLOCKSIZE * 0.5f, BLOCKSIZE * 0.5f);
		isPlayerEnteringCurrent = Vector2::RectCollision(pos, col, plPos, plCol);
		if (isPlayerEnteringCurrent)
			GetScene()->SetInCave(0);
		else if (mIsPlayerEntering && !isPlayerEnteringCurrent)
		{
			if (plPos.x - plCol.x > pos.x + col.x)
			{
				GetScene()->SetInCave(1);
			}
			else
			{
				GetScene()->SetInCave(-1);
			}
		}
	}
	else
	{
		plPos = GetScene()->GetPlayer()->GetPosition();
		plCol = GetScene()->GetPlayer()->GetColSize();
		pos = GetPosition() + Vector2(BLOCKSIZE * 2, 0);
		col = Vector2(BLOCKSIZE * 0.5f, BLOCKSIZE * 0.5f);
		isPlayerEnteringCurrent = Vector2::RectCollision(pos, col, plPos, plCol);
		if (isPlayerEnteringCurrent)
			GetScene()->SetInCave(0);
		else if (mIsPlayerEntering && !isPlayerEnteringCurrent)
		{
			if (plPos.x + plCol.x < pos.x - col.x)
			{
				GetScene()->SetInCave(1);
			}
			else
			{
				GetScene()->SetInCave(-1);
			}
		}
	}
	mIsPlayerEntering = isPlayerEnteringCurrent;
}