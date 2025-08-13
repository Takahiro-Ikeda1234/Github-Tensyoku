//=============================================================================
//
// [ComponentUISprite.h] アニメーションするスプライトコンポーネント
//
//=============================================================================
#ifndef _COMPONENT_CIRCLE_UI_SPRITE_H_
#define _COMPONENT_CIRCLE_UI_SPRITE_H_

//インクルードファイル
#include"ComponentCircleSprite.h"

//クラス定義
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