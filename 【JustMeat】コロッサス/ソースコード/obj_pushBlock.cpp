#include "obj_pushBlock.h"

PushBlock::PushBlock(Scene * scene, int indexX, int indexY)
	: MoveFloorObject(scene, StageManager::ToPosition(indexX, indexY), 3
		, 3, 0, 0, 1, 0)
{
	mCASprite = new ComponentAnimationSprite(this, TextureTag::PushBlock);
	mIsPushBlock = true;
	SetObjectType(Type::eSolid);
	mMoveSpeed = Vector2(0, 0);
}

void PushBlock::UpdateStageObject(float deltaTime)
{
	Translate(Vector2(mMoveSpeed));
	for (auto it : mObject)
	{
		it->Translate(mMoveSpeed);

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
	mObject.clear();

	mPrevPos = GetPosition();
	mMoveSpeed = Vector2(0, 0);
}
