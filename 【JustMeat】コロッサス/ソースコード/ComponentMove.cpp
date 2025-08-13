//=============================================================================
//
// [ComponentMove.cpp] �ړ��p�R���|�[�l���g
//
//=============================================================================

//�C���N���[�h�t�@�C��
#include"ComponentMove.h"
#include"Object.h"
#include"Scene.h"
#include"Manager.h"
#include<vector>
#include"config.h"
#include"StageManager.h"
#include"obj_abstractBlock.h"

//�}�N����`
#define FRAME_XSPEED_MAX BLOCKSIZE	//�t���[�����Ƃ̑��x���
#define FRAME_YSPEED_MAX BLOCKSIZE	//�t���[�����Ƃ̑��x���

//�Փ˔���p
enum Hit
{
	eNoHit = 0,	//�������Ă��Ȃ�
	eHitRight,	//�E�̃u���b�N�ɓ�������
	eHitLeft,	//���̃u���b�N�ɓ�������
	eHitTop,	//��̃u���b�N�ɓ�������
	eHitBottom,	//���̃u���b�N�ɓ�������
};

// ---------------------------------------------------------------------------
// �֐���: ComponentMove::ComponentMove(Object* owner, bool useGravity, int gravitySpeed, int updatePriority)
// ����:   Object* owner �����҂̃|�C���^
// ����:   bool useGravity true�Ȃ�d�͂𗘗p����
// ����:   int gravitySpeed �d�͂̋���
// ����:   int updatePriority �X�V�D��x�B�Ⴂ�قǑ��̃R���|�[�l���g������ɏ��������
// ����:   ComponentMove�̃R���X�g���N�^
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
// �֐���: void ComponentMove::Update(float deltaTime)
// ����:   float deltaTime 
// ����:   �ړ��X�V����
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

	//�u���b�N��ʂ蔲���Ȃ����߂̑��x���
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
// �֐���: bool ComponentMove::Collision()
// �ߒl:   false = �������ĂȂ�
// �ߒl:   true  = ��������
// ����:   �u���b�N�Ƃ̏Փˏ����B�u���b�N�Əd�Ȃ�Ȃ��ʒu�Ɉړ�����
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
		for (int i = 0; i < findCount; i++)	//�d�Ȃ��Ă���S�Ẵu���b�N�Ƃ̔���
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

	//���t���[���ŕǂɐڐG���Ă��Ȃ��̂ɐڕǔ��肾�����ꍇ�A���̕����Ƀu���b�N�����邩�m�F����
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
	//���t���[���Őڒn���Ă��Ȃ��̂ɐڒn���肾�����ꍇ�A���Ƀu���b�N�����邩�m�F����
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
// �֐���: unsigned ComponentMove::CollisionCustom()
// �ߒl:   0b0000�̌`���Ԃ��B�����珇�ɁA���œ������Ă����1�A��œ������Ă����1�A���œ������Ă����1�A�E�œ������Ă����1
// ����:   �u���b�N�Ƃ̏Փ˔��肾���s���֐�
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
	//���t���[���Őڒn���Ă��Ȃ��̂ɐڒn���肾�����ꍇ�A���Ƀu���b�N�����邩�m�F����
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
{	//�����o������
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
// �֐���: int ComponentMove::ColPattern(Object* obj, Vector2 ownCol, Vector2 objCol, Vector2 objPos, int indexX, int indexY, int stageDataID)
// ����:   Object* obj �R���|�[�l���g�����҂̃|�C���^
// ����:   Vector2 ownCol �R���|�[�l���g�����҂̃T�C�Y
// ����:   Vector2 objCol �u���b�N�̃T�C�Y
// ����:   Vector2 objPos �u���b�N�̈ʒu
// ����:   int indexX �u���b�N�̔z��ʒu
// ����:   int indexY �u���b�N�̔z��ʒu
// ����:   int stageDataID �u���b�N��������z��̔ԍ�
// �ߒl:   0:�d�Ȃ��Ă��Ȃ��A1:������E�ɏՓ˂��Ă���A2:�E���獶�ɏՓ˂��Ă���A3:�������ɏՓ˂��Ă���A4:�ォ�牺�ɏՓ˂��Ă���
// ����:   �d�Ȃ蔻��������Ȃ��֐�
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
// �֐���: int ComponentMove::CollisionABtoCD(Vector2 ownPos, Vector2 ownCol, Vector2 objPos, Vector2 objCol, int indexX, int indexY, int stageDataID)
// ����:   Vector2 ownPos �R���|�[�l���g�����҂̍��W
// ����:   Vector2 ownCol �R���|�[�l���g�����҂̃T�C�Y
// ����:   Vector2 objPos �u���b�N�̈ʒu
// ����:   Vector2 objCol �u���b�N�̃T�C�Y
// ����:   int indexX �u���b�N�̔z��ʒu
// ����:   int indexY �u���b�N�̔z��ʒu
// ����:   int stageDataID �u���b�N��������z��̔ԍ�
// �ߒl:   0:�d�Ȃ��Ă��Ȃ��A1:������E�ɏՓ˂��Ă���A2:�E���獶�ɏՓ˂��Ă���A3:�������ɏՓ˂��Ă���A4:�ォ�牺�ɏՓ˂��Ă���
// ����:   AB��������CD�����ւ̏d�Ȃ蔻����s���֐�(T...��AB...���AL...���AR...�E)
// ---------------------------------------------------------------------------
int ComponentMove::CollisionTLtoBR(Vector2 ownPos, Vector2 ownCol, Vector2 objPos, Vector2 objCol, int indexX, int indexY)
{
	//���E������Ɏ������ړ��ł����Ԃ����݂��邩�m�F
	int leftID = mOwner->GetScene()->GetStageManager()->GetStageData(StageManager::eStageObjectLayerBlock, indexX - 1, indexY);
	int topID = mOwner->GetScene()->GetStageManager()->GetStageData(StageManager::eStageObjectLayerBlock, indexX, indexY - 1);
	if (leftID <= 0 && topID <= 0)
	{	//�ǂ������
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
	}	//����������
	else if (leftID <= 0)
	{
		return eHitRight;
	}
	else if (topID <= 0)
	{	//�ゾ������
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
