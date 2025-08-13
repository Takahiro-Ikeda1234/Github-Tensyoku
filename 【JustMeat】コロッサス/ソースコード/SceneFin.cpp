#include "SceneFin.h"
#include "obj_screenBackground.h"
#include "ComponentAnimationSprite.h"
#include "ComponentUISprite.h"
#include "EventManager.h"
#include "file.h"
#include "UIObject.h"
#include "keyboard.h"
#include "AudioData.h"
SceneFin::SceneFin(SceneManager * sceneManager, Parameter param)
	: Scene(sceneManager)
{
	SetVolumeBGM(6, 0);
	float fValue = 0;
	int value = 0;
	param.GetF("フェードイン時間", &fValue);
	mEventManager->Fadein(fValue, 0);

	auto bg = new ScreenBackground(this, TextureTag::TitleEnding);
	auto it = new UIBox(this, 0.8f, 0.85f, 0.05f, 0.025f, TextureTag::Fin, 100);
	//it->GetCSprite()->SetColor(0xFFFFFFFF);
}

void SceneFin::Update(float deltaTime)
{
	UpdateObjects(deltaTime);
	UpdateManagers(deltaTime);

	if (Keyboard::Inst()->KeyHit(DIK_RETURN) || mFinTime.IsEnd())
	{
		Parameter param;
		SetFadeout(0, 20, 0);
		GetSceneManager()->SceneChange(SceneManager::SceneID::eLogo, &param, 0.0f, 5.0f, 0.5f, true);
	}
	mFinTime -= deltaTime;
}