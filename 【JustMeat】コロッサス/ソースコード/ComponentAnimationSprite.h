//=============================================================================
//
// [ComponentAnimationSprite.h] �A�j���[�V��������X�v���C�g�R���|�[�l���g
//
//=============================================================================
#ifndef _COMPONENT_ANIMATION_SPRITE_H_
#define _COMPONENT_ANIMATION_SPRITE_H_

//�C���N���[�h�t�@�C��
#include"ComponentSprite.h"

//�N���X��`
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
	int mStartID_L;	//�������̊J�n�t���[��
	int mStartID_R;	//�E�����̊J�n�t���[��
	int mTotalFrame;	//���t���[����
	float mNowFrame;	//���݂̃t���[��
	float mAnimSpeed;	//1�b�Ԃ�����̃t���[������
	bool mIsLoop;		//�Ō�܂œ��B���Ƀ��[�v���邩
};
#endif