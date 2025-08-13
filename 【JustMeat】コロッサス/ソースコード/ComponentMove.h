//=============================================================================
//
// [ComponentMove.h] �ړ��p�R���|�[�l���g
//
//=============================================================================
#ifndef _COMPONENT_MOVE_H_
#define _COMPONENT_MOVE_H_

//�C���N���[�h�t�@�C��
#include"Component.h"
#include"Vector2.h"
#include"ComponentSprite.h"
#include"config.h"

//�}�N����`
class Object;
//�N���X��`
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
	bool mIsLanding;	//�ڒn����
	TouchWall mIsTouchWall;	//�ڕǔ���
	bool mCanJump;		//�W�����v�\����
	bool mNoCollision = false;

	bool mUseGravity;	//�d�͎g�p
	float mGravitySpeed;	//�d�͉����x
	Vector2 mPrevPrevPos;
	Vector2 mPrevPos;	//�O�t���[���̈ʒu
	Vector2 mPos;	//�v�Z���̈ʒu
	Vector2 mSpeed;	//1�b������̑��x
	Vector2 mSpeedDeltaTime;
	Vector2 mCollisionMove;	//�u���b�N�Ƃ̏Փˎ��ɏՓ˂��Ȃ��ʒu�܂ňړ�����������

	int CollisionTLtoBR(Vector2 ownPos, Vector2 ownCol, Vector2 objPos, Vector2 objCol, int indexX, int indexY);
	int CollisionTRtoBL(Vector2 ownPos, Vector2 ownCol, Vector2 objPos, Vector2 objCol, int indexX, int indexY);
	int CollisionBLtoTR(Vector2 ownPos, Vector2 ownCol, Vector2 objPos, Vector2 objCol, int indexX, int indexY);
	int CollisionBRtoTL(Vector2 ownPos, Vector2 ownCol, Vector2 objPos, Vector2 objCol, int indexX, int indexY);
};
#endif