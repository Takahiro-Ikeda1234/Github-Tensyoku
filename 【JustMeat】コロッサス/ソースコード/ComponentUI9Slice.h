#pragma once
//�C���N���[�h�t�@�C��
#include"ComponentUISprite.h"

//�N���X��`
class ComponentUI9Slice : public ComponentUISprite
{
public:
	ComponentUI9Slice(class AbstractObject* owner, TextureTag tag, int drawPriority = LAYER_OBJECT) : ComponentUISprite(owner, tag, drawPriority) {}
	~ComponentUI9Slice() {}
	void Draw() override;
};