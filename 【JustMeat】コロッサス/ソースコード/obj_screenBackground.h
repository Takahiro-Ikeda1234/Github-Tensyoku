//=============================================================================
//
// [obj_Background.h] �w�i�I�u�W�F�N�g
//
//=============================================================================
#ifndef _OBJ_SCREEN_BACKGROUND_H_
#define _OBJ_SCREEN_BACKGROUND_H_

//�C���N���[�h�t�@�C��
#include"Object.h"
#include<string>
#include"TextureData.h"
class Scene;
class ComponentSprite;

//�N���X��`
class ScreenBackground : public Object
{
public:
	ScreenBackground(Scene* scene, TextureTag tag);
	~ScreenBackground() {}
	void UpdateObject(float deltaTime);
};

#endif