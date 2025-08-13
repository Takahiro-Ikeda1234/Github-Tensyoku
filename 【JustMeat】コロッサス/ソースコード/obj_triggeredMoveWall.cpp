#include "obj_triggeredMoveWall.h"
#include "StageManager.h"
#include "ComponentAnimationSprite.h"
#include "file.h"
#include"obj_player.h"
#include "ComponentTrigger.h"

TriggeredMoveWall::TriggeredMoveWall(Scene * scene, Vector2 pos, int id)
	: MoveFloorObject(scene, pos
		, GetMoveFloorData(scene->GetStageManager()->GetCurrentStageID(), id, 3)->xScale
		, GetMoveFloorData(scene->GetStageManager()->GetCurrentStageID(), id, 3)->yScale
		, GetMoveFloorData(scene->GetStageManager()->GetCurrentStageID(), id, 3)->xMove
		, GetMoveFloorData(scene->GetStageManager()->GetCurrentStageID(), id, 3)->yMove
		, GetMoveFloorData(scene->GetStageManager()->GetCurrentStageID(), id, 3)->moveTime
		, 0)
{
	static const TextureTag tex[] = {
		TextureTag::MoveWall,
		TextureTag::MoveWall,
		TextureTag::MoveWall,
		TextureTag::MoveWallStage2,
		TextureTag::MoveWallStage2,
		TextureTag::MoveWallStage2,
		TextureTag::MoveWallStage3,
		TextureTag::MoveWallStage3,
		TextureTag::MoveWallStage3,
	};
	int idOffset = GetMoveFloorData(scene->GetStageManager()->GetCurrentStageID(), id, 3)->texID;
	mCASprite = new ComponentAnimationSprite(this, tex[scene->GetStageManager()->GetCurrentStageID() + idOffset], 3);
	mTriggered = new ComponentTriggered(this, id);
	SetObjectType(Type::eWall);
	mMoveTime = 0;
}

TriggeredMoveWall::TriggeredMoveWall(Scene * scene, int indexX, int indexY, int id)
	: TriggeredMoveWall(scene, StageManager::ToPosition(indexX, indexY), id)
{
}

void TriggeredMoveWall::UpdateStageObject(float deltaTime)
{
	if (!mIsActive && mTriggered->IsActive())
	{
		mIsActive = true;
		if (mMoveTime.IsEnd())	//非動作・初期位置
		{
			mMoveTime = mBaseMoveTime;
		}
		else //非動作・初期位置への戻り中
		{
			mMoveTime.Reverse();
		}
	}
	else if(mIsActive && !mTriggered->IsActive())
	{
		mIsActive = false;
		if (mMoveTime.IsEnd())	//動作・終点位置
		{
			mMoveTime = mBaseMoveTime;
		}
		else //動作・終点位置への移動中
		{
			mMoveTime.Reverse();
		}
	}

	int sign = (mIsActive ? 1 : -1);
	if (!mMoveTime.IsEnd())
	{
		Translate(Vector2(mMoveSpeed) * deltaTime * sign);
	}
	for (auto it : mObject)
	{
		if (!mMoveTime.IsEnd())
		{
			it->Translate(mMoveSpeed * deltaTime * sign);

			auto objects = GetScene()->OnCollision(it->GetPosition(), it->GetColSize(), it, Object::Type::eSolid);
			int findCount = objects.size();
			if (findCount > 0)
			{
				for (auto block : objects)
				{
					Vector2 vec(-mMoveSpeed.x * sign, -mMoveSpeed.y * sign);
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
	mMoveTime -= deltaTime;
	mObject.clear();

	mPrevPos = GetPosition();
}

void TriggeredMoveWall::SaveCheckPointData()
{
	MoveFloorObject::SaveCheckPointData();
	mCPIsActive = mIsActive;
}

void TriggeredMoveWall::LoadCheckPointData()
{
	MoveFloorObject::LoadCheckPointData();
	mIsActive = mCPIsActive;
}

