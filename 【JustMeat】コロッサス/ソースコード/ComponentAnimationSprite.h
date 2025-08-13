//=============================================================================
//
// [ComponentAnimationSprite.h] アニメーションするスプライトコンポーネント
//
//=============================================================================
#ifndef _COMPONENT_ANIMATION_SPRITE_H_
#define _COMPONENT_ANIMATION_SPRITE_H_

//インクルードファイル
#include"ComponentSprite.h"

//クラス定義
class ComponentAnimationSprite : public ComponentSprite
{
public:
	ComponentAnimationSprite(class AbstractObject* owner, TextureTag tag, int drawPriority = LAYER_OBJECT);
	virtual ~ComponentAnimationSprite() {}
	void SetAnimation(int startL, int startR, int totalFrame, float animSpeed, bool isLoop = true);
	void Update(float deltaTime);
	void SetNowFrame(float frame) { mNowFrame = frame; }
	float GetNowFrame() { return mNowFrame; }
	void SetAnimationSpeed(float animSpeed) { mAnimSpeed = animSpeed; }
	virtual ComponentAnimationSprite& operator=(ComponentAnimationSprite& copy);
private:
	int mStartID_L;	//左向きの開始フレーム
	int mStartID_R;	//右向きの開始フレーム
	int mTotalFrame;	//総フレーム数
	float mNowFrame;	//現在のフレーム
	float mAnimSpeed;	//1秒間あたりのフレーム枚数
	bool mIsLoop;		//最後まで到達時にループするか
};
#endif