//=============================================================================
//
// [ComponentAnimationSprite.cpp] アニメーションするスプライトコンポーネント
//
//=============================================================================

//インクルードファイル
#include"ComponentAnimationSprite.h"
#include"Object.h"

// ---------------------------------------------------------------------------
// 関数名: ComponentAnimationSprite::ComponentAnimationSprite(AbstractObject* owner, TextureTag tag, int drawPriority)
// 引数:   AbstractObject* owner 所持者のポインタ
// 引数:   TextureTag tag 利用する画像のタグ
// 引数:   int drawPriority 描画優先度。低いほど前に表示される
// 説明:   ComponentAnimationSpriteのコンストラクタ
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
// 関数名: void ComponentAnimationSprite::SetAnimation(int startL, int startR, int totalFrame, float animSpeed, bool isLoop)
// 引数:   int startL 左向きの場合のアニメーション開始位置(縦方向分割)
// 引数:   int startR 右向きの場合のアニメーション開始位置(縦方向分割)
// 引数:   int totalFrame 総フレーム数
// 引数:   float animSpeed アニメーション速度(totalFrame = animSpeedで1秒)
// 引数:   bool isLoop falseなら最後のフレームでアニメーション終了
// 説明:   描画するアニメーションを設定する
// ---------------------------------------------------------------------------
void ComponentAnimationSprite::SetAnimation(int startL, int startR, int totalFrame, float animSpeed, bool isLoop)
{
	//既に設定されているアニメーションと同じであれば無視
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
		mUseID = mStartID_R + (int)mNowFrame;	//キャストでfloatの誤差を避ける
	}
	else
	{
		mUseID = mStartID_L + (int)mNowFrame;	//キャストでfloatの誤差を避ける
	}
}

// ---------------------------------------------------------------------------
// 関数名: void ComponentAnimationSprite::Update(float deltaTime)
// 引数:   float deltaTime 
// 説明:   アニメーション更新処理
// ---------------------------------------------------------------------------
void ComponentAnimationSprite::Update(float deltaTime)
{
	if (!mIsActive)return;
	AbstractObject::Dir isRight = mOwner->GetDirection();
	if (isRight == AbstractObject::Dir::eRight)
	{
		mUseID = mStartID_R + (int)mNowFrame;	//キャストでfloatの誤差を避ける
	}
	else
	{
		mUseID = mStartID_L + (int)mNowFrame;	//キャストでfloatの誤差を避ける
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
