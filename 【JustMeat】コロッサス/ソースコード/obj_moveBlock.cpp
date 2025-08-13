#include "obj_moveBlock.h"
#include "StageManager.h"
#include "ComponentAnimationSprite.h"
#include "obj_player.h"
MoveBlock::MoveBlock(Scene * scene, Vector2 pos, int id)
	: MoveFloorObject(scene, pos
		, GetMoveFloorData(scene->GetStageManager()->GetCurrentStageID(), id, 0)->xScale
		, GetMoveFloorData(scene->GetStageManager()->GetCurrentStageID(), id, 0)->yScale
		, GetMoveFloorData(scene->GetStageManager()->GetCurrentStageID(), id, 0)->xMove
		, GetMoveFloorData(scene->GetStageManager()->GetCurrentStageID(), id, 0)->yMove
		, GetMoveFloorData(scene->GetStageManager()->GetCurrentStageID(), id, 0)->moveTime
		, GetMoveFloorData(scene->GetStageManager()->GetCurrentStageID(), id, 0)->stopTime)
{
	static const TextureTag tex[] = {
	TextureTag::MoveGroundBlockSize3Horizontal,
	TextureTag::MoveGroundBlockSize3HorizontalStage2,
	TextureTag::MoveGroundBlockSize3HorizontalStage3,
	};
	mCASprite = new ComponentAnimationSprite(this, tex[GetMoveFloorData(scene->GetStageManager()->GetCurrentStageID(), id, 0)->texID], LAYER_OBJECT + 2);
	SetObjectType(Type::eSolid);
}

MoveBlock::MoveBlock(Scene * scene, int indexX, int indexY, int id)
	: MoveBlock(scene, StageManager::ToPosition(indexX, indexY), id)
{
}

void MoveBlock::UpdateStageObject(float deltaTime)
{
	if (!mMoveTime.IsEnd())
	{
		Translate(Vector2(mMoveSpeed) * deltaTime);

		auto itr = mObject.begin();
		for (; itr != mObject.end(); itr++)
		{
			if (*itr == GetScene()->GetPlayer())break;
		}
		if (itr==mObject.end())
		{
			Vector2 playerPos = GetScene()->GetPlayer()->GetPosition();
			Vector2 playerSize = GetScene()->GetPlayer()->GetColSize();
			if (Vector2::RectCollision(playerPos, playerSize, GetPosition(), GetColSize()))
			{
				GetScene()->GetPlayer()->Translate(Vector2(mMoveSpeed) * deltaTime);
			}
		}
	}

	for (auto it : mObject)
	{
		if (!mMoveTime.IsEnd())
		{
			it->Translate(mMoveSpeed * deltaTime);

			auto objects = GetScene()->OnCollision(it->GetPosition(), it->GetColSize(), it, Object::Type::eSolid);
			int findCount = objects.size();
			if (findCount > 0)
			{
				for (auto block : objects)
				{
					Vector2 vec(-mMoveSpeed.x, -mMoveSpeed.y);
					float len = vec.Length();
					if (len == 0)break;
					vec = vec / len;
					while (Vector2::RectCollision(block->GetPosition(), block->GetColSize(), it->GetPosition(), it->GetColSize()))
					{
						it->Translate(vec);
					}
				}
			}
		}
	}
	mObject.clear();

	mPrevPos = GetPosition();
	mMoveTime -= deltaTime;
	mStopTime -= deltaTime;
	if (mMoveTime.IsFinish())
	{
		mStopTime = mBaseStopTime;
	}
	if (mMoveTime.IsEnd() && mStopTime.IsEnd())
	{
		mMoveTime = mBaseMoveTime;
		mMoveSpeed = -1 * mMoveSpeed;
	}
}