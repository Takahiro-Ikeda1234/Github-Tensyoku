//=============================================================================
//
// [ComponentAnimationSprite.cpp] �A�j���[�V��������X�v���C�g�R���|�[�l���g
//
//=============================================================================

//�C���N���[�h�t�@�C��
#include"ComponentAnimationSprite.h"
#include"Object.h"

// ---------------------------------------------------------------------------
// �֐���: ComponentAnimationSprite::ComponentAnimationSprite(AbstractObject* owner, TextureTag tag, int drawPriority)
// ����:   AbstractObject* owner �����҂̃|�C���^
// ����:   TextureTag tag ���p����摜�̃^�O
// ����:   int drawPriority �`��D��x�B�Ⴂ�قǑO�ɕ\�������
// ����:   ComponentAnimationSprite�̃R���X�g���N�^
// ---------------------------------------------------------------------------
ComponentAnimationSprite::ComponentAnimationSprite(AbstractObject* owner, TextureTag tag, int drawPriority)
	: ComponentSprite(owner, tag, drawPriority)
	, mStartID_L(0)
	, mStartID_R(0)
	, mTotalFrame(1)
	, mAnimSpeed(0)
	, mNowFrame(0)
	, mIsLoop(false)
{

}

// ---------------------------------------------------------------------------
// �֐���: void ComponentAnimationSprite::SetAnimation(int startL, int startR, int totalFrame, float animSpeed, bool isLoop)
// ����:   int startL �������̏ꍇ�̃A�j���[�V�����J�n�ʒu(�c��������)
// ����:   int startR �E�����̏ꍇ�̃A�j���[�V�����J�n�ʒu(�c��������)
// ����:   int totalFrame ���t���[����
// ����:   float animSpeed �A�j���[�V�������x(totalFrame = animSpeed��1�b)
// ����:   bool isLoop false�Ȃ�Ō�̃t���[���ŃA�j���[�V�����I��
// ����:   �`�悷��A�j���[�V������ݒ肷��
// ---------------------------------------------------------------------------
void ComponentAnimationSprite::SetAnimation(int startL, int startR, int totalFrame, float animSpeed, bool isLoop)
{
	//���ɐݒ肳��Ă���A�j���[�V�����Ɠ����ł���Ζ���
	if (totalFrame == mTotalFrame && startL * mTexX == mStartID_L && startR * mTexX == mStartID_R && isLoop == mIsLoop)return;

	mStartID_L = startL * mTexX;
	mStartID_R = startR * mTexX;
	mTotalFrame = totalFrame;
	mAnimSpeed = animSpeed;
	mNowFrame = 0;
	mIsLoop = isLoop;

	AbstractObject::Dir isRight = mOwner->GetDirection();
	if (isRight == AbstractObject::Dir::eRight)
	{
		mUseID = mStartID_R + (int)mNowFrame;	//�L���X�g��float�̌덷�������
	}
	else
	{
		mUseID = mStartID_L + (int)mNowFrame;	//�L���X�g��float�̌덷�������
	}
}

// ---------------------------------------------------------------------------
// �֐���: void ComponentAnimationSprite::Update(float deltaTime)
// ����:   float deltaTime 
// ����:   �A�j���[�V�����X�V����
// ---------------------------------------------------------------------------
void ComponentAnimationSprite::Update(float deltaTime)
{
	if (!mIsActive)return;
	AbstractObject::Dir isRight = mOwner->GetDirection();
	if (isRight == AbstractObject::Dir::eRight)
	{
		mUseID = mStartID_R + (int)mNowFrame;	//�L���X�g��float�̌덷�������
	}
	else
	{
		mUseID = mStartID_L + (int)mNowFrame;	//�L���X�g��float�̌덷�������
	}

	mNowFrame += mAnimSpeed * deltaTime;
	if ((int)mNowFrame > mTotalFrame - 1)
	{
		if (mIsLoop)
		{
			mNowFrame = 0;
		}
		else
		{
			mNowFrame = mTotalFrame - 1;
		}
	}
}

ComponentAnimationSprite& ComponentAnimationSprite::operator=(ComponentAnimationSprite& copy)
{
	mStartID_L = copy.mStartID_L;
	mStartID_R = copy.mStartID_R;
	mTotalFrame = copy.mTotalFrame;
	mNowFrame = copy.mNowFrame;
	mAnimSpeed = copy.mAnimSpeed;
	mIsLoop = copy.mIsLoop;
	return *this;
}
