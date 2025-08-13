//=============================================================================
//
// [SceneTitle.cpp] タイトル画面
//
//=============================================================================

//インクルードファイル
#include"SceneTitle.h"
#include"SceneManager.h"
#include"myMath.h"
#include"Draw.h"
#include"TextureData.h"
#include"keyboard.h"
#include"obj_screenBackground.h"
#include"CameraManager.h"
#include"EventManager.h"
#include"UIObject.h"
#include"ComponentUISprite.h"
#include"ui_title.h"
#include"AudioData.h"
// ---------------------------------------------------------------------------
// 関数名: SceneTitle::SceneTitle(class SceneManager* sceneManager, Parameter& param)
// 引数:   SceneManager* sceneManager シーンマネージャのポインタ
// 引数:   Parameter& param パラメータ
// 説明:   SceneTitleのコンストラクタ
// ---------------------------------------------------------------------------
SceneTitle::SceneTitle(SceneManager* sceneManager, Parameter& param)
	: Scene(sceneManager)
{
	SetVolumeBGM(6, 0);
	new ScreenBackground(this, TextureTag::Title);
	mUITitle = new UITitle(this);
	int value;
	if (param.Get("メニュー状態", &value))
	{
		mUITitle->SetState((UITitle::TitleState)value);
	}
	if (param.Get("カーソル位置", &value))
	{
		mUITitle->SetMenuSelectCursor(value);
	}

	if (param.Get("前シーン", &value))
	{
		if (value != SceneManager::SceneID::eStageSelect)
		{
			PlayBGM(BGM_TITLE, 0, 100);
		}
	}
	else
	{
		PlayBGM(BGM_TITLE, 0, 100);
	}

	float fValue = 0;
	if (param.GetF("フェードイン時間", &fValue))
	{
		mEventManager->Fadein(fValue, 0);
	}



	//UIObject* obj = new UIBox(this, 0.5f, 0.5f, 0.25f, 0.1f, TextureTag::Invisible, 50);
	//obj->GetCSprite()->SetString("Enterで開始");
	//obj->GetCSprite()->SetStringInfo(20, obj->GetPosition(), Vector2(60, 60), true);
	//obj->SetBlink(1);
}
// ---------------------------------------------------------------------------
// 関数名: SceneTitle::~SceneTitle()
// 説明:   SceneTitleのデストラクタ
// ---------------------------------------------------------------------------
SceneTitle::~SceneTitle()
{
	delete mUITitle;
}
// ---------------------------------------------------------------------------
// 関数名: void SceneTitle::Update(float deltaTime)
// 引数:   float deltaTime 
// 説明:   更新処理
// ---------------------------------------------------------------------------
void SceneTitle::Update(float deltaTime)
{
	UpdateObjects(deltaTime);
	UpdateManagers(deltaTime);
	mUITitle->Update(deltaTime);

}
// ---------------------------------------------------------------------------
// 関数名: void SceneTitle::DrawUI()
// 説明:   UI描画処理
// ---------------------------------------------------------------------------
void SceneTitle::DrawUI()
{
}