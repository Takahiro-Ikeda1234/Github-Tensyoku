//=============================================================================
//
// [SceneManager.cpp] シーン遷移管理
//
//=============================================================================

//インクルードファイル
#include"SceneManager.h"
#include"SceneGame.h"
#include"SceneTitle.h"
#include"SceneEditor.h"
#include"SceneStageSelect.h"
#include"Scene.h"
#include"CameraManager.h"
#include"Vector2.h"
#include"DX9_texture.h"
#include"DX9_draw.h"
#include"keyboard.h"
#include"ini.h"
#include"SceneOpening.h"
#include"SceneEnding.h"
#include"SceneCredit.h"
#include"SceneFin.h"
#include"SceneLogo.h"
#include"EventManager.h"

// ---------------------------------------------------------------------------
// 関数名: SceneManager::SceneManager()
// 説明:   SceneManagerのコンストラクタ
// ---------------------------------------------------------------------------
SceneManager::SceneManager()
	: mNextScene(eLogo)
	, mCurrentScene(eDefault)
	, mScene(NULL)
	, mShowUI(true)
{
	LoadScene(eLogo);
	mNextParam.Clear();
	mDebugMode = GetConfigInt("System", "Debug");
}
SceneManager::~SceneManager()
{
	UnloadScene();
}
void SceneManager::UnloadScene()
{
	delete mScene;
}
// ---------------------------------------------------------------------------
// 関数名: void SceneManager::LoadScene(SceneID id)
// 引数:   SceneID id 読み込むシーンのタグ
// 説明:   シーン読込処理
// ---------------------------------------------------------------------------
void SceneManager::LoadScene(SceneID id)
{
	if (mScene != NULL)
	{
		UnloadScene();
	}
	mCurrentScene = mNextScene;
	switch (id)
	{
	case eExit:
		break;
	case eTitle:
		mScene = new SceneTitle(this, mNextParam);
		break;
	case eGame:
		mScene = new SceneGame(this, mNextParam);
		break;
	case eEditor:
		mScene = new SceneEditor(this, mNextParam);
		break;
	case eStageSelect:
		mScene = new SceneStageSelect(this, mNextParam);
		break;
	case eOpening:
		mScene = new SceneOpening(this, mNextParam);
		break;
	case eEnding:
		mScene = new SceneEnding(this, mNextParam);
		break;
	case eCredit:
		mScene = new SceneCredit(this, mNextParam);
		break;
	case eFin:
		mScene = new SceneFin(this, mNextParam);
		break;
	case eLogo:
		mScene = new SceneLogo(this, mNextParam);
		break;
	}
	mNextScene = eDefault;
	mNextParam.Clear();

	if (mWhiteFade)
	{
		mScene->GetEventManager()->SetWhiteFade();
		mWhiteFade = false;
	}
}
// ---------------------------------------------------------------------------
// 関数名: void SceneManager::Update(float deltaTime)
// 引数:   float deltaTime 
// 説明:   更新処理
// ---------------------------------------------------------------------------
void SceneManager::Update(float deltaTime)
{
	mScene->Update(deltaTime);
	if (Keyboard::Inst()->KeyHit(DIK_F1))
	{
		mDebugMode = !mDebugMode;
	}
}
// ---------------------------------------------------------------------------
// 関数名: void SceneManager::Draw()
// 説明:   描画処理
// ---------------------------------------------------------------------------
void SceneManager::Draw()
{
	mScene->Draw();
	mScene->DrawUI();
}
// ---------------------------------------------------------------------------
// 関数名: void SceneManager::SceneChange(SceneID nextScene, Parameter& param)
// 引数:   SceneID nextScene 次のシーンのタグ
// 引数:   Parameter& param パラメータ
// 説明:   シーン変更関数
// ---------------------------------------------------------------------------
bool SceneManager::SceneChange(SceneID nextScene, Parameter* param, float fadeoutDelay, float fadeoutTime, float fadeinTime, bool whiteFade)
{
	if (!CanSceneChange())return false;
	param->SetF("フェードイン時間", fadeinTime);
	mNextScene = nextScene;
	mNextParam = *param;
	mNextParam.Set("前シーン", mCurrentScene);
	mScene->GetEventManager()->Fadeout(fadeoutTime, fadeoutDelay);
	param->Clear();
	if (whiteFade)
	{
		mWhiteFade = true;
		mScene->GetEventManager()->SetWhiteFade();
	}
	return true;
}

bool SceneManager::CanSceneChange() const
{
	if (mNextScene != SceneID::eDefault)return false;
	return true;
}
