//=============================================================================
//
// [ComponentMove.h] 移動用コンポーネント
//
//=============================================================================
#ifndef _COMPONENT_MOVE_H_
#define _COMPONENT_MOVE_H_

//インクルードファイル
#include"Component.h"
#include"Vector2.h"
#include"ComponentSprite.h"
#include"config.h"

//マクロ定義
class Object;
//クラス定義
class ComponentMove : public Component
{
public:
	enum class TouchWall
	{
		eFalse,
		eLeft,
		eRight,
	};
	ComponentMove(Object* owner, bool useGravity = true, int gravitySpeed = GRAVITY, int updatePriority = LAYER_OBJECT);
	~ComponentMove() {}
	virtual void Update(float deltaTime) override;
	void SetSpeed(Vector2 speed) { mSpeed = speed; }
	void SetSpeedX(float xsp) { mSpeed.x = xsp; }
	void SetSpeedY(float ysp) { mSpeed.y = ysp; }
	void AddSpeed(float xsp, float ysp) { mSpeed.x += xsp; mSpeed.y += ysp; }
	Vector2 GetSpeed() { return mSpeed; }
	void SetPosition(Vector2 pos) { mPos = pos; }
	Vector2 GetPosition() { return mPos; }
	Vector2 GetPrevPosition() { return mPrevPos; }
	void SetPrevPosition(Vector2 pos) { mPrevPos = pos; }
	virtual bool Collision();
	virtual unsigned CollisionCustom();
	bool IsLanding() { return mIsLanding; }
	TouchWall IsTouchWall() { return mIsTouchWall; }
	bool CanJump() { return mCanJump; }
	void SetLanding(bool tf) { mIsLanding = tf; }
	void SetCanJump(bool tf) { mCanJump = tf;}
	void SetUseGravity(bool tf) { mUseGravity = tf; }
	bool GetUseGravity() { return mUseGravity; }
	void SetTouchWall(TouchWall touchwall) { mIsTouchWall = touchwall; }
	Vector2 GetCollisionMove() { return mCollisionMove; }
	void PushObject();
	void SetNoCollision(bool set) { mNoCollision = set; }
protected:
	int ColPattern(Object* obj, Vector2 ownCol, Vector2 objCol, Vector2 objPos, int indexX, int indexY);
	bool mIsLanding;	//接地判定
	TouchWall mIsTouchWall;	//接壁判定
	bool mCanJump;		//ジャンプ可能判定
	bool mNoCollision = false;

	bool mUseGravity;	//重力使用
	float mGravitySpeed;	//重力加速度
	Vector2 mPrevPrevPos;
	Vector2 mPrevPos;	//前フレームの位置
	Vector2 mPos;	//計算中の位置
	Vector2 mSpeed;	//1秒あたりの速度
	Vector2 mSpeedDeltaTime;
	Vector2 mCollisionMove;	//ブロックとの衝突時に衝突しない位置まで移動させた距離

	int CollisionTLtoBR(Vector2 ownPos, Vector2 ownCol, Vector2 objPos, Vector2 objCol, int indexX, int indexY);
	int CollisionTRtoBL(Vector2 ownPos, Vector2 ownCol, Vector2 objPos, Vector2 objCol, int indexX, int indexY);
	int CollisionBLtoTR(Vector2 ownPos, Vector2 ownCol, Vector2 objPos, Vector2 objCol, int indexX, int indexY);
	int CollisionBRtoTL(Vector2 ownPos, Vector2 ownCol, Vector2 objPos, Vector2 objCol, int indexX, int indexY);
};
#endif