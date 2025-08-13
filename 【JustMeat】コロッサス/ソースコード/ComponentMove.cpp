//=============================================================================
//
// [ComponentMove.cpp] 移動用コンポーネント
//
//=============================================================================

//インクルードファイル
#include"ComponentMove.h"
#include"Object.h"
#include"Scene.h"
#include"Manager.h"
#include<vector>
#include"config.h"
#include"StageManager.h"
#include"obj_abstractBlock.h"

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

// ---------------------------------------------------------------------------
// 関数名: ComponentMove::ComponentMove(Object* owner, bool useGravity, int gravitySpeed, int updatePriority)
// 引数:   Object* owner 所持者のポインタ
// 引数:   bool useGravity trueなら重力を利用する
// 引数:   int gravitySpeed 重力の強さ
// 引数:   int updatePriority 更新優先度。低いほど他のコンポーネントよりも先に処理される
// 説明:   ComponentMoveのコンストラクタ
// ---------------------------------------------------------------------------
ComponentMove::ComponentMove(Object* owner, bool useGravity, int gravitySpeed, int updatePriority)
	: Component(owner, updatePriority)
	, mUseGravity(useGravity)
	, mGravitySpeed(gravitySpeed)
	, mPos(0, 0)
	, mPrevPos(0, 0)
	, mPrevPrevPos(0, 0)
	, mSpeed(0, 0)
	, mSpeedDeltaTime(0, 0)
	, mCanJump(true)
	, mIsLanding(false)
	, mIsTouchWall(TouchWall::eFalse)
{

}

// ---------------------------------------------------------------------------
// 関数名: void ComponentMove::Update(float deltaTime)
// 引数:   float deltaTime 
// 説明:   移動更新処理
// ---------------------------------------------------------------------------
void ComponentMove::Update(float deltaTime)
{
	mPrevPrevPos = mPrevPos;
	mPrevPos = mPos = mOwner->GetPosition();
	if (mUseGravity && !mIsLanding)
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

	mCollisionMove = Vector2(0, 0);
}


// ---------------------------------------------------------------------------
// 関数名: bool ComponentMove::Collision()
// 戻値:   false = 当たってない
// 戻値:   true  = 当たった
// 説明:   ブロックとの衝突処理。ブロックと重ならない位置に移動する
// ---------------------------------------------------------------------------
bool ComponentMove::Collision()
{
	if (mNoCollision) return true;
	Vector2 expectedPos = mPos;
	bool touchBlock = false;
	int patternCount[5] = {};
	std::vector<Object*> objects = mOwner->GetScene()->OnCollision(mPos, mOwner->GetColSize(), (Object*)mOwner, Object::Type::eSolid);
	int findCount = objects.size();
	if (findCount > 0)
	{
		Vector2 tempSpeed = mSpeed;
		for (int i = 0; i < findCount; i++)	//重なっている全てのブロックとの判定
		{
			Object* obj = objects.at(i);
			Vector2 ownCol = mOwner->GetColSize();
			Vector2 objCol = obj->GetColSize();
			Vector2 objPos = obj->GetPosition();

			//int pattern = ColPattern(obj, ownCol, objCol, objPos, ((AbstractBlock*)objects[i])->GetIndexX(), ((AbstractBlock*)objects[i])->GetIndexY());
			int indexX = -100;
			int indexY = -100;
			if (obj->IsBlock())
			{
				indexX = StageManager::ToIndexX(objects[i]->GetPosition().x);
				indexY = StageManager::ToIndexY(objects[i]->GetPosition().y);
			}
			int pattern = ColPattern(obj, ownCol, objCol, objPos, indexX, indexY);

			patternCount[pattern]++;
			if (pattern != 0)touchBlock = true;
			switch (pattern)
			{
			case eHitRight:
				tempSpeed.x = 0;
				mPos.x = objPos.x - (ownCol.x + objCol.x) - 0.1f;
				if (!mIsLanding)
				{
					mIsTouchWall = TouchWall::eRight;
					//mOwner->SetDirection(Object::Dir::eLeft);
				}
				break;
			case eHitLeft:
				tempSpeed.x = 0;
				mPos.x = objPos.x + (ownCol.x + objCol.x) + 0.1f;
				if (!mIsLanding)
				{
					mIsTouchWall = TouchWall::eLeft;
					//mOwner->SetDirection(Object::Dir::eRight);
				}
				break;
			case eHitTop:
				tempSpeed.y = 0;
				mPos.y = objPos.y + (ownCol.y + objCol.y) + 0.1f;
				break;
			case eHitBottom:
				tempSpeed.y = 0;
				mPos.y = objPos.y - (ownCol.y + objCol.y) - 0.1f;
				mIsLanding = true;
				mCanJump = true;
				break;
			}
		}
		mSpeed = tempSpeed;
	}

	PushObject();

	//今フレームで壁に接触していないのに接壁判定だった場合、その方向にブロックがあるか確認する
	if (patternCount[1] == 0 && mIsTouchWall == TouchWall::eLeft)
	{
		std::vector<Object*> leftBlocks = mOwner->GetScene()->OnCollision(Vector2(mPos.x - 1, mPos.y), mOwner->GetColSize(), (Object*)mOwner, Object::Type::eSolid);
		if (leftBlocks.size() == 0)
		{
			mIsTouchWall = TouchWall::eFalse;
		}
	}
	if (patternCount[2] == 0 && mIsTouchWall == TouchWall::eRight)
	{
		std::vector<Object*> rightBlocks = mOwner->GetScene()->OnCollision(Vector2(mPos.x + 1, mPos.y), mOwner->GetColSize(), (Object*)mOwner, Object::Type::eSolid);
		if (rightBlocks.size() == 0)
		{
			mIsTouchWall = TouchWall::eFalse;
		}
	}
	//今フレームで接地していないのに接地判定だった場合、下にブロックがあるか確認する
	if (patternCount[4] == 0 && mIsLanding == true)
	{
		std::vector<Object*> groundBlocks = mOwner->GetScene()->OnCollision(Vector2(mPos.x, mPos.y + 1), mOwner->GetColSize(), (Object*)mOwner, Object::Type::eSolid);
		if (groundBlocks.size() == 0)
		{
			mIsLanding = false;
		}
	}
	if (mIsLanding)
	{
		mIsTouchWall = TouchWall::eFalse;
	}
	mCollisionMove = expectedPos - mPos;
	return touchBlock;
}
// ---------------------------------------------------------------------------
// 関数名: unsigned ComponentMove::CollisionCustom()
// 戻値:   0b0000の形式返す。左から順に、下で当たっていれば1、上で当たっていれば1、左で当たっていれば1、右で当たっていれば1
// 説明:   ブロックとの衝突判定だけ行う関数
// ---------------------------------------------------------------------------
unsigned ComponentMove::CollisionCustom()
{
	unsigned hitPattern = 0b0000;
	bool touchBlock = false;
	int patternCount[5] = {};
	std::vector<Object*> objects = mOwner->GetScene()->OnCollision(mPos, mOwner->GetColSize(), (Object*)mOwner, Object::Type::eSolid);
	int findCount = objects.size();
	if (findCount > 0)
	{
		Vector2 tempSpeed = mSpeed;
		for (int i = 0; i < findCount; ++i)
		{
			Object* obj = objects.at(i);
			Vector2 ownCol = mOwner->GetColSize();
			Vector2 objCol = obj->GetColSize();
			Vector2 objPos = obj->GetPosition();

			//int pattern = ColPattern(obj, ownCol, objCol, objPos, ((AbstractBlock*)objects[i])->GetIndexX(), ((AbstractBlock*)objects[i])->GetIndexY());
			int indexX = -100;
			int indexY = -100;

			if (obj->IsBlock())
			{
				indexX = StageManager::ToIndexX(objects[i]->GetPosition().x);
				indexY = StageManager::ToIndexY(objects[i]->GetPosition().y);
			}
			int pattern = ColPattern(obj, ownCol, objCol, objPos, indexX, indexY);

			patternCount[pattern]++;
			if (pattern != 0)touchBlock = true;
			switch (pattern)
			{
			case eHitRight:
				hitPattern |= 0b0001;
				mPos.x = objPos.x - (ownCol.x + objCol.x) - 0.1f;
				if (!mIsLanding)
				{
					mIsTouchWall = TouchWall::eRight;
					//mOwner->SetDirection(Object::Dir::eLeft);
				}
				break;
			case eHitLeft:
				hitPattern |= 0b0010;
				mPos.x = objPos.x + (ownCol.x + objCol.x) + 0.1f;
				if (!mIsLanding)
				{
					mIsTouchWall = TouchWall::eLeft;
					//mOwner->SetDirection(Object::Dir::eRight);
				}
				break;
			case eHitTop:
				hitPattern |= 0b0100;
				mPos.y = objPos.y + (ownCol.y + objCol.y) + 0.1f;
				break;
			case eHitBottom:
				hitPattern |= 0b1000;
				mPos.y = objPos.y - (ownCol.y + objCol.y) - 0.1f;
				mIsLanding = true;
				mCanJump = true;
				break;
			}
		}
	}
	//今フレームで接地していないのに接地判定だった場合、下にブロックがあるか確認する
	if (patternCount[4] == 0 && mIsLanding == true)
	{
		std::vector<Object*> groundBlocks = mOwner->GetScene()->OnCollision(Vector2(mPos.x, mPos.y + 1), mOwner->GetColSize(), (Object*)mOwner, Object::Type::eSolid);
		if (groundBlocks.size() == 0)
		{
			mIsLanding = false;
		}
	}
	return hitPattern;
}


void ComponentMove::PushObject()
{	//押し出し処理
	auto objects = mOwner->GetScene()->OnCollision(mPos, mOwner->GetColSize(), (Object*)mOwner, Object::Type::eSolid);
	int findCount = objects.size();
	if (findCount > 0)
	{
		for (auto it : objects)
		{
			Vector2 vec = it->GetVelocity();
			float len = vec.Length();
			if (len == 0)
			{
				vec = mPrevPrevPos - mPos;
				len = vec.Length();
			}
			if (len == 0)break;
			vec = vec / len;
			while (Vector2::RectCollision(it->GetPosition(), it->GetColSize(), mPos, mOwner->GetColSize()))
			{
				mPos = mPos + vec;
				mSpeed = Vector2(0, 0);
			}
		}
	}
}

// ---------------------------------------------------------------------------
// 関数名: int ComponentMove::ColPattern(Object* obj, Vector2 ownCol, Vector2 objCol, Vector2 objPos, int indexX, int indexY, int stageDataID)
// 引数:   Object* obj コンポーネント所持者のポインタ
// 引数:   Vector2 ownCol コンポーネント所持者のサイズ
// 引数:   Vector2 objCol ブロックのサイズ
// 引数:   Vector2 objPos ブロックの位置
// 引数:   int indexX ブロックの配列位置
// 引数:   int indexY ブロックの配列位置
// 引数:   int stageDataID ブロックが属する配列の番号
// 戻値:   0:重なっていない、1:左から右に衝突している、2:右から左に衝突している、3:下から上に衝突している、4:上から下に衝突している
// 説明:   重なり判定をおこなう関数
// ---------------------------------------------------------------------------
int ComponentMove::ColPattern(Object* obj, Vector2 ownCol, Vector2 objCol, Vector2 objPos, int indexX, int indexY)
{
	int hitPattern = 0;
	Vector2 relativeVelocity = mSpeed - obj->GetVelocity();
	if (relativeVelocity.x > 0 && relativeVelocity.y > 0)
	{
		//TLtoBR
		
		hitPattern = CollisionTLtoBR(mOwner->GetPosition(), ownCol, objPos, objCol, indexX, indexY);
	}
	else if (relativeVelocity.x > 0 && relativeVelocity.y < 0)
	{
		//BLtoTR
		hitPattern = CollisionBLtoTR(mOwner->GetPosition(), ownCol, objPos, objCol, indexX, indexY);
	}
	else if (relativeVelocity.x < 0 && relativeVelocity.y > 0)
	{
		//TRtoBL
		hitPattern = CollisionTRtoBL(mOwner->GetPosition(), ownCol, objPos, objCol, indexX, indexY);
	}
	else if (relativeVelocity.x < 0 && relativeVelocity.y < 0)
	{
		//BRtoTL
		hitPattern = CollisionBRtoTL(mOwner->GetPosition(), ownCol, objPos, objCol, indexX, indexY);
	}
	else if (relativeVelocity.x > 0 && relativeVelocity.y == 0)
	{
		//LtoR
		if (mOwner->GetScene()->GetStageManager()->GetStageData(StageManager::eStageObjectLayerBlock,indexX - 1, indexY) <= 0)
		{
			hitPattern = eHitRight;
		}
	}
	else if (relativeVelocity.x < 0 && relativeVelocity.y == 0)
	{
		//RtoL
		if (mOwner->GetScene()->GetStageManager()->GetStageData(StageManager::eStageObjectLayerBlock, indexX + 1, indexY) <= 0)
		{
			hitPattern = eHitLeft;
		}
	}
	else if (relativeVelocity.x == 0 && relativeVelocity.y < 0)
	{
		//BtoT
		if (mOwner->GetScene()->GetStageManager()->GetStageData(StageManager::eStageObjectLayerBlock, indexX , indexY + 1) <= 0)
		{
			hitPattern = eHitTop;
		}
	}
	else if (relativeVelocity.x == 0 && relativeVelocity.y > 0)
	{
		//TtoB
		if (mOwner->GetScene()->GetStageManager()->GetStageData(StageManager::eStageObjectLayerBlock, indexX, indexY - 1) <= 0)
		{
			hitPattern = eHitBottom;
		}
	}
	else
	{

	}
	return hitPattern;
}

// ---------------------------------------------------------------------------
// 関数名: int ComponentMove::CollisionABtoCD(Vector2 ownPos, Vector2 ownCol, Vector2 objPos, Vector2 objCol, int indexX, int indexY, int stageDataID)
// 引数:   Vector2 ownPos コンポーネント所持者の座標
// 引数:   Vector2 ownCol コンポーネント所持者のサイズ
// 引数:   Vector2 objPos ブロックの位置
// 引数:   Vector2 objCol ブロックのサイズ
// 引数:   int indexX ブロックの配列位置
// 引数:   int indexY ブロックの配列位置
// 引数:   int stageDataID ブロックが属する配列の番号
// 戻値:   0:重なっていない、1:左から右に衝突している、2:右から左に衝突している、3:下から上に衝突している、4:上から下に衝突している
// 説明:   AB方向からCD方向への重なり判定を行う関数(T...上、B...下、L...左、R...右)
// ---------------------------------------------------------------------------
int ComponentMove::CollisionTLtoBR(Vector2 ownPos, Vector2 ownCol, Vector2 objPos, Vector2 objCol, int indexX, int indexY)
{
	//左・上方向に自分が移動できる空間が存在するか確認
	int leftID = mOwner->GetScene()->GetStageManager()->GetStageData(StageManager::eStageObjectLayerBlock, indexX - 1, indexY);
	int topID = mOwner->GetScene()->GetStageManager()->GetStageData(StageManager::eStageObjectLayerBlock, indexX, indexY - 1);
	if (leftID <= 0 && topID <= 0)
	{	//どちらも空白
		float xUmari = (ownPos.x + ownCol.x) - (objPos.x - objCol.x);
		float yUmari = (ownPos.y + ownCol.y) - (objPos.y - objCol.y);
		if (xUmari > yUmari)
		{
			return eHitBottom;
		}
		else
		{
			return eHitRight;
		}
	}	//左だけが空白
	else if (leftID <= 0)
	{
		return eHitRight;
	}
	else if (topID <= 0)
	{	//上だけが空白
		return eHitBottom;
	}
	else
	{
		return eNoHit;
	}
}
int ComponentMove::CollisionTRtoBL(Vector2 ownPos, Vector2 ownCol, Vector2 objPos, Vector2 objCol, int indexX, int indexY)
{
	int rightID = mOwner->GetScene()->GetStageManager()->GetStageData(StageManager::eStageObjectLayerBlock, indexX + 1, indexY);
	int topID = mOwner->GetScene()->GetStageManager()->GetStageData(StageManager::eStageObjectLayerBlock, indexX, indexY - 1);
	if (rightID <= 0 && topID <= 0)
	{
		float xUmari = (objPos.x + objCol.x) - (ownPos.x - ownCol.x);
		float yUmari = (ownPos.y + ownCol.y) - (objPos.y - objCol.y);
		if (xUmari > yUmari)
		{
			return eHitBottom;
		}
		else
		{
			return eHitLeft;
		}
	}
	else if (rightID <= 0)
	{
		return eHitLeft;
	}
	else if (topID <= 0)
	{
		return eHitBottom;
	}
	else
	{
		return eNoHit;
	}
}
int ComponentMove::CollisionBLtoTR(Vector2 ownPos, Vector2 ownCol, Vector2 objPos, Vector2 objCol, int indexX, int indexY)
{
	int leftID = mOwner->GetScene()->GetStageManager()->GetStageData(StageManager::eStageObjectLayerBlock, indexX - 1, indexY);
	int bottomID = mOwner->GetScene()->GetStageManager()->GetStageData(StageManager::eStageObjectLayerBlock, indexX, indexY + 1);
	if (leftID <= 0 && bottomID <= 0)
	{
		float xUmari = (ownPos.x + ownCol.x) - (objPos.x - objCol.x);
		float yUmari = (objPos.y + objCol.y) - (ownPos.y - ownCol.y);
		if (xUmari > yUmari)
		{
			return eHitTop;
		}
		else
		{
			return eHitRight;
		}
	}
	else if (leftID <= 0)
	{
		return eHitRight;
	}
	else if (bottomID <= 0)
	{
		return eHitTop;
	}
	else
	{
		return eNoHit;
	}
}
int ComponentMove::CollisionBRtoTL(Vector2 ownPos, Vector2 ownCol, Vector2 objPos, Vector2 objCol, int indexX, int indexY)
{
	int rightID = mOwner->GetScene()->GetStageManager()->GetStageData(StageManager::eStageObjectLayerBlock, indexX + 1, indexY);
	int bottomID = mOwner->GetScene()->GetStageManager()->GetStageData(StageManager::eStageObjectLayerBlock, indexX, indexY + 1);
	if (rightID <= 0 && bottomID <= 0)
	{
		float xUmari = (objPos.x + objCol.x) - (ownPos.x - ownCol.x);
		float yUmari = (objPos.y + objCol.y) - (ownPos.y - ownCol.y);
		if (xUmari > yUmari)
		{
			return eHitTop;
		}
		else
		{
			return eHitLeft;
		}
	}
	else if (rightID <= 0)
	{
		return eHitLeft;
	}
	else if (bottomID <= 0)
	{
		return eHitTop;
	}
	else
	{
		return eNoHit;
	}
}
