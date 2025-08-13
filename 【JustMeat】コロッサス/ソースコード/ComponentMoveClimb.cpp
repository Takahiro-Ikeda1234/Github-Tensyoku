#include"ComponentMoveClimb.h"
#include"obj_wall.h"
//マクロ定義
#define FRAME_XSPEED_MAX BLOCKSIZE	//フレームごとの速度上限
#define FRAME_YSPEED_MAX BLOCKSIZE	//フレームごとの速度上限

//衝突判定用
enum Hit
{
	eNoHit = 0,	//当たっていない
	eHitRight,	//右のブロックに当たった
	eHitLeft,	//左のブロックに当たった
	eHitTop,	//上のブロックに当たった
	eHitBottom,	//下のブロックに当たった
};

ComponentMoveClimb::ComponentMoveClimb(Object * owner, bool useGravity, int gravitySpeed, int updatePriority)
	: ComponentMove(owner, useGravity, gravitySpeed, updatePriority)
{

}

void ComponentMoveClimb::Update(float deltaTime)
{
	mPrevPrevPos = mPrevPos;
	mPrevPos = mPos = mOwner->GetPosition();
	if (mUseGravity)
	{
		mSpeed.y += mGravitySpeed * deltaTime;
	}
	Vector2 speed(mSpeed.x * deltaTime, mSpeed.y * deltaTime);

	//ブロックを通り抜けないための速度上限
	if (speed.x > FRAME_XSPEED_MAX) speed.x = FRAME_XSPEED_MAX;
	if (speed.x < -FRAME_XSPEED_MAX) speed.x = -FRAME_XSPEED_MAX;
	if (speed.y > FRAME_YSPEED_MAX) speed.y = FRAME_YSPEED_MAX;
	if (speed.y < -FRAME_YSPEED_MAX) speed.y = -FRAME_YSPEED_MAX;

	mSpeedDeltaTime = Vector2(speed.x, speed.y);
	mPos.x += speed.x;
	mPos.y += speed.y;
}
bool ComponentMoveClimb::Collision()
{

	ComponentMove::Collision();
	std::vector<Object*> objects = mOwner->GetScene()->OnCollision(mPos, mOwner->GetColSize() / 10, (Object*)mOwner, Object::Type::eWall);
	int findCount = objects.size();
	if (findCount == 0)
	{
		if (mCanMoveNoWall)
		{
			return true;
		}

		std::vector<Object*> hObjects = mOwner->GetScene()->OnCollision(mPrevPos + Vector2(mSpeedDeltaTime.x, 0), mOwner->GetColSize() / 10, (Object*)mOwner, Object::Type::eWall);
		std::vector<Object*> vObjects = mOwner->GetScene()->OnCollision(mPrevPos + Vector2(0, mSpeedDeltaTime.y), mOwner->GetColSize() / 10, (Object*)mOwner, Object::Type::eWall);
		if (hObjects.size() > 0)
		{
			mPos = mPrevPos + Vector2(mSpeedDeltaTime.x, 0);
			std::vector<Object*> blocks = mOwner->GetScene()->OnCollision(mPos, mOwner->GetColSize(), (Object*)mOwner, Object::Type::eSolid);
			if (blocks.size() > 0)
			{
 				mSpeed.x = 0;
				mPos = mPrevPos;
			}
			mSpeed.y = 0;
			ComponentMove::Collision();
 			return true;
		}
		if (vObjects.size() > 0)
		{
			mPos = mPrevPos + Vector2(0, mSpeedDeltaTime.y);
			std::vector<Object*> blocks = mOwner->GetScene()->OnCollision(mPos, mOwner->GetColSize(), (Object*)mOwner, Object::Type::eSolid);
			if (blocks.size() > 0)
			{
				mSpeed.y = 0;
				mPos = mPrevPos;
			}
			mSpeed.x = 0;
			return true;
		}
		if(mPrevPos == mPos)
			return false;
		mSpeed.x = 0;
		mSpeed.y = 0;
		mPos = mPrevPos;
		return true;
	}
	else
	{
		SetUseGravity(false);
		return true;
	}
}

/*bool ComponentMoveClimb::Collision()
{
	//bool touchBlock = false;
	//int patternCount[5] = {};
	std::vector<Object*> objects = mOwner->GetScene()->OnCollision(mPos, mOwner->GetColSize() / 10, (Object*)mOwner, Object::Type::eWall);
	int findCount = objects.size();
	if (findCount == 0)
	{
		std::vector<Object*> objects2 = mOwner->GetScene()->OnCollision(mPrevPos, mOwner->GetColSize() / 10, (Object*)mOwner, Object::Type::eWall);
		if(objects2.size() == 0)
			SetUseGravity(true);
		return false;
	}
	else
	{
		SetUseGravity(false);
		return true;
	}
	//if (findCount > 0)
	//{
	//	Vector2 tempSpeed = mSpeed;
	//	for (int i = 0; i < findCount; i++)	//重なっている全てのブロックとの判定
	//	{
	//		Object* obj = objects.at(i);
	//		Vector2 ownCol = mOwner->GetColSize();
	//		Vector2 objCol = obj->GetColSize();
	//		Vector2 objPos = obj->GetPosition();

	//		int pattern = ColPattern(obj, ownCol, objCol, objPos, ((Wall*)objects[i])->GetIndexX(), ((Wall*)objects[i])->GetIndexY());

	//		patternCount[pattern]++;
	//		if (pattern != 0)touchBlock = true;
	//		switch (pattern)
	//		{
	//		case eHitRight:
	//			tempSpeed.x = 0;
	//			mPos.x = objPos.x - (ownCol.x + objCol.x) - 0.1f;
	//			if (!mIsLanding)
	//			{
	//				mIsTouchWall = TouchWall::eRight;
	//				mOwner->SetDirection(Object::Dir::eLeft);
	//				mCanJump = true;
	//			}
	//			break;
	//		case eHitLeft:
	//			tempSpeed.x = 0;
	//			mPos.x = objPos.x + (ownCol.x + objCol.x) + 0.1f;
	//			if (!mIsLanding)
	//			{
	//				mIsTouchWall = TouchWall::eLeft;
	//				mOwner->SetDirection(Object::Dir::eRight);
	//				mCanJump = true;
	//			}
	//			break;
	//		case eHitTop:
	//			tempSpeed.y = 0;
	//			mPos.y = objPos.y + (ownCol.y + objCol.y) + 0.1f;
	//			break;
	//		case eHitBottom:
	//			tempSpeed.y = 0;
	//			mPos.y = objPos.y - (ownCol.y + objCol.y) - 0.1f;
	//			mIsLanding = true;
	//			mCanJump = true;
	//			break;
	//		}
	//	}
	//	mSpeed = tempSpeed;
	//}
	//return touchBlock;
}*/

