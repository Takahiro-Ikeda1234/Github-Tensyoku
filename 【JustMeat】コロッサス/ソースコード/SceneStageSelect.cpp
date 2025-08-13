#include "SceneStageSelect.h"
#include "obj_screenBackground.h"
#include"ui_stageSelect.h"
#include"EventManager.h"
#include "ComponentAnimationSprite.h"
#include "AudioData.h"
SceneStageSelect::SceneStageSelect(SceneManager * sceneManager, Parameter & param)
	: Scene(sceneManager)
{
	SetVolumeBGM(6, 0);
	auto it = new ScreenBackground(this, TextureTag::Title);
	it->GetCASprite()->SetColorRate(0.3f);
	float fValue = 0;
	param.GetF("フェードイン時間", &fValue);
	mEventManager->Fadein(fValue, 0);
	

	new ScreenBackground(this, TextureTag::Invisible);
	mUIStageSelect = new UIStageSelect(this);

	int value = 0;
	param.Get("ステージカーソル", &value);
	mUIStageSelect->SetStageSelectCursor(value);

	if (param.Get("前シーン", &value))
	{
		if (value != SceneManager::SceneID::eTitle)
		{
			PlayBGM(BGM_TITLE, 0, 100);
		}
	}
	else
	{
		PlayBGM(BGM_TITLE, 0, 100);
	}
}

SceneStageSelect::~SceneStageSelect()
{
	delete mUIStageSelect;
}

void SceneStageSelect::Update(float deltaTime)
{
	UpdateObjects(deltaTime);
	UpdateManagers(deltaTime);
	mUIStageSelect->Update(deltaTime);
}

void SceneStageSelect::DrawUI()
{
}
