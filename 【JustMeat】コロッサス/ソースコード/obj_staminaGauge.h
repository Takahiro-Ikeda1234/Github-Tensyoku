//=============================================================================
//
// [obj_Background.h] �w�i�I�u�W�F�N�g
//
//=============================================================================
#ifndef _OBJ_STAMINA_GAUGE_H_
#define _OBJ_STAMINA_GAUGE_H_

//�C���N���[�h�t�@�C��
#include"UIObject.h"
#include<string>
#include"TextureData.h"
class Scene;
class ComponentCircleUISprite;
class ComponentCircleSprite;
class ComponentAnimationSprite;
//�N���X��`
class StaminaGauge : public UIObject
{
public:
	StaminaGauge(Scene* scene, Vector2 pos);
	~StaminaGauge() {}
	void UpdateUIObject(float deltaTime);
	void SetFill(float percentage);
	void SetAlphaRate(float rate);
	//ComponentCircleUISprite* GetCCUSprite() { return mCCUSprite; }
private:
	//ComponentCircleUISprite* mCCUSprite;
	ComponentCircleSprite* mCCSprite;
	ComponentAnimationSprite* mCASprite;
	Counter mAlphaTime;
	bool mAppear = false;
};

#endif