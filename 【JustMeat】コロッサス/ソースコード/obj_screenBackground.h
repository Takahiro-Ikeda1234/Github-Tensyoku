//=============================================================================
//
// [obj_Background.h] 背景オブジェクト
//
//=============================================================================
#ifndef _OBJ_SCREEN_BACKGROUND_H_
#define _OBJ_SCREEN_BACKGROUND_H_

//インクルードファイル
#include"Object.h"
#include<string>
#include"TextureData.h"
class Scene;
class ComponentSprite;

//クラス定義
class ScreenBackground : public Object
{
public:
	ScreenBackground(Scene* scene, TextureTag tag);
	~ScreenBackground() {}
	void UpdateObject(float deltaTime);
};

#endif