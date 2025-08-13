//=============================================================================
//
// [ComponentUISprite.h] �A�j���[�V��������X�v���C�g�R���|�[�l���g
//
//=============================================================================
#ifndef _COMPONENT_CIRCLE_UI_SPRITE_H_
#define _COMPONENT_CIRCLE_UI_SPRITE_H_

//�C���N���[�h�t�@�C��
#include"ComponentCircleSprite.h"

//�N���X��`
class ComponentCircleUISprite : public ComponentCircleSprite
{
public:
	ComponentCircleUISprite(class AbstractObject* owner, TextureTag tag, int drawPriority = LAYER_OBJECT) : ComponentCircleSprite(owner, tag, drawPriority) {}
	~ComponentCircleUISprite() {}
	void Draw() override;
protected:
	void Mask() override;
};
#endif