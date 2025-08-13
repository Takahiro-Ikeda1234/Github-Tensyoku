#include "obj_triggeredMoveBlock.h"
#include "StageManager.h"
#include "ComponentAnimationSprite.h"
#include "obj_player.h"
#include "ComponentTrigger.h"
TriggeredMoveBlock::TriggeredMoveBlock(Scene * scene, Vector2 pos, int id)
	: MoveFloorObject(scene, pos
		, GetMoveFloorData(scene->GetStageManager()->GetCurrentStageID(), id, 2)->xScale
		, GetMoveFloorData(scene->GetStageManager()->GetCurrentStageID(), id, 2)->yScale
		, GetMoveFloorData(scene->GetStageManager()->GetCurrentStageID(), id, 2)->xMove
		, GetMoveFloorData(scene->GetStageManager()->GetCurrentStageID(), id, 2)->yMove
		, GetMoveFloorData(scene->GetStageManager()->GetCurrentStageID(), id, 2)->moveTime
		, 0)
{
	static const TextureTag tex[] = {
	TextureTag::MoveGroundBlockSize3,
	TextureTag::MoveGroundBlockSize3,
	TextureTag::MoveGroundBlockSize3,
	TextureTag::MoveGroundBlockSize3Stage2,
	TextureTag::MoveGroundBlockSize3Stage2,
	TextureTag::MoveGroundBlockSize3Stage2,
	TextureTag::MoveGroundBlockSize3Stage3,
	TextureTag::MoveGroundBlockSize3Stage3,
	TextureTag::MoveGroundBlockSize3Stage3,
	};

	static const TextureTag texH[] = {
	TextureTag::MoveGroundBlockSize3Horizontal,
	TextureTag::MoveGroundBlockSize3Horizontal,
	TextureTag::MoveGroundBlockSize3Horizontal,
	TextureTag::MoveGroundBlockSize3HorizontalStage2,
	TextureTag::MoveGroundBlockSize3HorizontalStage2,
	TextureTag::MoveGroundBlockSize3HorizontalStage2,
	TextureTag::MoveGroundBlockSize3HorizontalStage3,
	TextureTag::MoveGroundBlockSize3HorizontalStage3,
	TextureTag::MoveGroundBlockSize3HorizontalStage3,
	};

	int texID = GetMoveFloorData(scene->GetStageManager()->GetCurrentStageID(), id, 2)->texID;
	if (texID == 1)
	{
		mCASprite = new ComponentAnimationSprite(this, TextureTag::MoveGroundBlockSize9, LAYER_OBJECT + 2);
	}
	else
	{
		if (GetMoveFloorData(scene->GetStageManager()->GetCurrentStageID(), id, 2)->xScale < GetMoveFloorData(scene->GetStageManager()->GetCurrentStageID(), id, 2)->yScale)
		{
			mCASprite = new ComponentAnimationSprite(this, tex[scene->GetStageManager()->GetCurrentStageID()], LAYER_OBJECT + 2);
		}
		else
		{
			mCASprite = new ComponentAnimationSprite(this, texH[scene->GetStageManager()->GetCurrentStageID()], LAYER_OBJECT + 2);
		}
	}
	
	mTriggered = new ComponentTriggered(this, id);
	SetObjectType(Type::eSolid);
	mMoveTime = 0;
}

TriggeredMoveBlock::TriggeredMoveBlock(Scene * scene, int indexX, int indexY, int id)
	: TriggeredMoveBlock(scene, StageManager::ToPosition(indexX, indexY), id)
{
}

void TriggeredMoveBlock::UpdateStageObject(float deltaTime)
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
	else if (mIsActive && !mTriggered->IsActive())
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
	if (!Vector2::RectCollision(GetPosition() + Vector2(mMoveSpeed) * deltaTime * sign, GetColSize(), GetScene()->GetPlayer()->GetPosition(), GetScene()->GetPlayer()->GetColSize()))
	{
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
	}
	mObject.clear();

	mPrevPos = GetPosition();
}

void TriggeredMoveBlock::SaveCheckPointData()
{
	MoveFloorObject::SaveCheckPointData();
	mCPIsActive = mIsActive;
}

void TriggeredMoveBlock::LoadCheckPointData()
{
	MoveFloorObject::LoadCheckPointData();
	mIsActive = mCPIsActive;
}
