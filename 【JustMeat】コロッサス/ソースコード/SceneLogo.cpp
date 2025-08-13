#include "SceneLogo.h"
#include "obj_screenBackground.h"
#include "ComponentAnimationSprite.h"
#include "ComponentUISprite.h"
#include "EventManager.h"
#include "file.h"
#include "UIObject.h"
#include "keyboard.h"
#include "AudioData.h"
SceneLogo::SceneLogo(SceneManager * sceneManager, Parameter param)
	: Scene(sceneManager)
{
	SetVolumeBGM(6, 0);
	float fValue = 0;
	int value = 0;
	param.GetF("フェードイン時間", &fValue);
	mEventManager->Fadein(fValue, 0);

	auto bg = new ScreenBackground(this, TextureTag::Plane);
	auto it = new UIBox(this, 0.5f, 0.5f, 0.1f, 0.1f, TextureTag::Justmeat, 100);
}

void SceneLogo::Update(float deltaTime)
{
	UpdateObjects(deltaTime);
	UpdateManagers(deltaTime);

	if (Keyboard::Inst()->KeyHit(DIK_RETURN) || mLogoTime.IsEnd())
	{
		Parameter param;
		GetSceneManager()->SceneChange(SceneManager::SceneID::eTitle, &param, 0, 0.5f, 0.5f);
	}
	mLogoTime -= deltaTime;
}