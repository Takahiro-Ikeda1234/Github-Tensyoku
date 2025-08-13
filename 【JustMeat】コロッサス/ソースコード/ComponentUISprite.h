//=============================================================================
//
// [ComponentUISprite.h] アニメーションするスプライトコンポーネント
//
//=============================================================================
#ifndef _COMPONENT_UI_SPRITE_H_
#define _COMPONENT_UI_SPRITE_H_

//インクルードファイル
#include"ComponentAnimationSprite.h"

//クラス定義
class ComponentUISprite : public ComponentAnimationSprite
{
public:
	ComponentUISprite(class AbstractObject* owner, TextureTag tag, int drawPriority = LAYER_OBJECT) : ComponentAnimationSprite(owner, tag, drawPriority){}
	~ComponentUISprite() {}
	void Draw() override;
};
#endif