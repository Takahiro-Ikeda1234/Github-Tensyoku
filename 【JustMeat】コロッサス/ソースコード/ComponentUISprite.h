//=============================================================================
//
// [ComponentUISprite.h] �A�j���[�V��������X�v���C�g�R���|�[�l���g
//
//=============================================================================
#ifndef _COMPONENT_UI_SPRITE_H_
#define _COMPONENT_UI_SPRITE_H_

//�C���N���[�h�t�@�C��
#include"ComponentAnimationSprite.h"

//�N���X��`
class ComponentUISprite : public ComponentAnimationSprite
{
public:
	ComponentUISprite(class AbstractObject* owner, TextureTag tag, int drawPriority = LAYER_OBJECT) : ComponentAnimationSprite(owner, tag, drawPriority){}
	~ComponentUISprite() {}
	void Draw() override;
};
#endif