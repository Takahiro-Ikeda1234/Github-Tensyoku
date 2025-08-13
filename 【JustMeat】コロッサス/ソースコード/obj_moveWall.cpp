#include "obj_moveWall.h"
#include "StageManager.h"
#include "ComponentAnimationSprite.h"
#include "file.h"


MoveWall::MoveWall(Scene * scene, Vector2 pos, int id)
	: MoveFloorObject(scene, pos
		, GetMoveFloorData(scene->GetStageManager()->GetCurrentStageID(), id, 1)->xScale
		, GetMoveFloorData(scene->GetStageManager()->GetCurrentStageID(), id, 1)->yScale
		, GetMoveFloorData(scene->GetStageManager()->GetCurrentStageID(), id, 1)->xMove
		, GetMoveFloorData(scene->GetStageManager()->GetCurrentStageID(), id, 1)->yMove
		, GetMoveFloorData(scene->GetStageManager()->GetCurrentStageID(), id, 1)->moveTime
		, GetMoveFloorData(scene->GetStageManager()->GetCurrentStageID(), id, 1)->stopTime)
{
	static const TextureTag tex[] = {
		TextureTag::MoveWall0,
		TextureTag::MoveWall1,
		TextureTag::MoveWall2,
	};
	mCASprite = new ComponentAnimationSprite(this, tex[GetMoveFloorData(scene->GetStageManager()->GetCurrentStageID(), id, 1)->texID], 101);
	SetObjectType(Type::eWall);
}

MoveWall::MoveWall(Scene * scene, int indexX, int indexY, int id)
	: MoveWall(scene, StageManager::ToPosition(indexX, indexY), id)
{
}

void MoveWall::UpdateStageObject(float deltaTime)
{
	if (!mMoveTime.IsEnd())
	{
		Translate(Vector2(mMoveSpeed) * deltaTime);
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

