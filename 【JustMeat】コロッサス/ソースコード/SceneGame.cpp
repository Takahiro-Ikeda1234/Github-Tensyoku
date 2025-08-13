//=============================================================================
//
// [SceneGame.cpp] ゲーム画面
//
//=============================================================================

//インクルードファイル
#include"SceneGame.h"
#include"SceneManager.h"
#include"Object.h"
#include"Manager.h"
#include"CameraManager.h"
#include"StageManager.h"
#include "EventManager.h"
#include"TextureData.h"
#include"Draw.h"
#include"myMath.h"
#include"keyboard.h"
#include"mouse.h"
#include"obj_screenBackground.h"
#include"obj_player.h"
#include"obj_abstractBlock.h"
#include"obj_golemEvent.h"
#include"obj_ghostCat.h"
#include"obj_golemHead.h"
#include"ui_game.h"
#include"CloudManager.h"
#include"obj_wood.h"
#include"obj_moveWall.h"
#include"obj_moveBlock.h"
#include"obj_StagePicture.h"
#include"obj_wall.h"
#include"DX9_printf.h"
#include"DX9_texture.h"
#include"mytime.h"
#include"obj_AbstractGolemArm.h"
#include"AudioData.h"
static int stageBGMArray[] = { 0,1,1,2,2,2,3,3,4 };
// ---------------------------------------------------------------------------
// 関数名: SceneGame::SceneGame(SceneManager* sceneManager)
// 引数:   SceneManager* sceneManager シーンマネージャのポインタ
// 説明:   SceneGameのコンストラクタ
// ---------------------------------------------------------------------------
SceneGame::SceneGame(SceneManager* sceneManager, Parameter& param)
	: Scene(sceneManager)
{
	CloudManager::Inst()->Reset();
	float fValue = 0;
	int value = 0;
	param.GetF("フェードイン時間", &fValue);
	mEventManager->Fadein(fValue, 0);

	value = 0;
	param.Get("ステージ", &value);
	mStageManager->LoadStage(value);
	mCameraManager->ResetLimit();
	int stageID = value;
	mGolem = new GolemEvent(this, 0, 0);

	std::string stageBGM = BGM_STAGE;
	stageBGM += '0' + stageBGMArray[stageID];
	StopBGM(0);
	if (stageBGMArray[stageID] != 1)StopBGM(1);
	if (stageBGMArray[stageID] != 2)StopBGM(2);
	if (stageBGMArray[stageID] != 3)StopBGM(3);
	if (stageBGMArray[stageID] != 4)StopBGM(4);

	int prevScene = SceneManager::SceneID::eDefault;
	param.Get("前シーン", &prevScene);

	if (value == 0)
	{
		PlayBGM(stageBGM.c_str(), stageBGMArray[stageID], 100);
	}
	else if (stageID != 0)
	{
		if (prevScene != SceneManager::SceneID::eGame)
		{
			PlayBGM(stageBGM.c_str(), stageBGMArray[stageID], 100);
		}
		else if (stageBGMArray[stageID] != stageBGMArray[stageID - 1])
		{
			PlayBGM(stageBGM.c_str(), stageBGMArray[stageID], 100);
		}
	}


	mUIGame = new UIGame(this);
	AllObjectSaveCheckPointData();


	value = 0;
	param.Get("継続", &value);
	if (value)
	{
		mPlayer->StageChangeLoad();
		GetCameraManager()->SetPosition(mPlayer->GetPosition());
		mCameraManager->SetOffset(1.2f, Vector2(0, -12) * BLOCKSIZE, 2.5f, 3, 2.5f);
	}
	else
	{
		CloudManager::Inst()->Update(this, 1.0f / 60);
		CloudManager::Inst()->ClearClouds();
	}
	mCat = new GhostCat(this, GetPlayer()->GetPosition());

	mStagePicture[0] = new StagePicture(this, 0, mStageManager->GetCurrentStageID(), mStageManager->GetWidth(), mStageManager->GetHeight());
	mStagePicture[1] = new StagePicture(this, 1, mStageManager->GetCurrentStageID(), mStageManager->GetWidth(), mStageManager->GetHeight());
	mStagePicture[2] = new StagePicture(this, 2, mStageManager->GetCurrentStageID(), mStageManager->GetWidth(), mStageManager->GetHeight());
	mStagePicture[3] = new StagePicture(this, 3, mStageManager->GetCurrentStageID(), mStageManager->GetWidth(), mStageManager->GetHeight());
}
// ---------------------------------------------------------------------------
// 関数名: SceneGame::~SceneGame()
// 説明:   SceneGameのデストラクタ
// ---------------------------------------------------------------------------
SceneGame::~SceneGame()
{
	delete mUIGame;
}
// ---------------------------------------------------------------------------
// 関数名: void SceneGame::Update(float deltaTime)
// 引数:   float deltaTime 
// 説明:   更新処理
// ---------------------------------------------------------------------------
void SceneGame::Update(float deltaTime)
{
	CloudManager::Inst()->Update(this, deltaTime);
	if (!mEventManager->IsHitstop())
	{
		UpdateObjects(deltaTime);
	}
	UpdateManagers(deltaTime);
	mUIGame->Update(deltaTime);


	if (mScrollStay.IsEnd())
	{
		GetCameraManager()->SetScrollSpeed(Vector2(0, -BLOCKSIZE));
	}

	mScrollStay -= deltaTime;
	if (Keyboard::Inst()->KeyHit(DIK_F11) && GetSceneManager()->GetDebugMode())
	{
		StartExportStageData(mStageManager->GetWidth(), mStageManager->GetHeight());
		auto blocks = FindObjects(nullptr, AbstractObject::Type::eSolid);
		for (auto it : blocks)
		{
			if (it->IsExportTexture())
			{
				if (it->IsBlock())
				{
					((AbstractBlock*)it)->ExportObjectTexture();
				}
			}
		}

		char buff[100];
		sprintf(buff, "resources/textures/stage/%d/stageBlock_%d", mStageManager->GetCurrentStageID(), mStageManager->GetCurrentStageID());
		EndExportStageData(buff);
		for (int i = 0; i < 4; i++)
		{
			mStagePicture[i]->ReloadTexture();
		}
	}

	auto hitGolemArm = mGolem->HitGolemArm(mPlayer->GetPosition(), mPlayer->GetColSize());
	if (hitGolemArm != nullptr)
	{
		if (hitGolemArm->GetObjectType() != Object::Type::eNone && hitGolemArm->GetGolemState() == AbstractGolemArm::State::ePunch && mPlayer->GetGolemCount() <= 3)
		{
			mPlayer->HitGolemArm(0);
			hitGolemArm->SetObjectType(Object::Type::eNone);
			hitGolemArm->EndBlockMode();
		}
	}
}
// ---------------------------------------------------------------------------
// 関数名: void SceneGame::UpdateManagers(float deltaTime)
// 引数:   float deltaTime 
// 説明:   全マネージャの更新
// ---------------------------------------------------------------------------
void SceneGame::UpdateManagers(float deltaTime)
{
	if (!IsPause())
	{
		mCameraManager->Update(deltaTime);
	}
	
	mStageManager->Update(deltaTime);
	mEventManager->Update(deltaTime);
}

void SceneGame::NextStage()
{
	mPlayer->StageChangeSave();
	Parameter param;
	param.Set("ステージ", mStageManager->GetCurrentStageID() + 1);
	param.Set("継続", 1);
	mSceneManager->SceneChange(SceneManager::SceneID::eGame, &param, 0, 0, 0);
}

// ---------------------------------------------------------------------------
// 関数名: void SceneGameMain::DrawUI()
// 説明:   UI描画処理
// ---------------------------------------------------------------------------
void SceneGame::DrawUI()
{
	if (GetSceneManager()->GetDebugMode())
	{
		char buff[50];
		Vector2 cameraPos = mCameraManager->ToWorldPosition(Mouse::Inst()->GetPos());
		sprintf(buff, "F1でデバッグモード解除", StageManager::ToIndexY(cameraPos.y), cameraPos.y);
		DX9printf(50, 50, buff);
		sprintf(buff, "x %d : %f", StageManager::ToIndexX(cameraPos.x), cameraPos.x);
		DX9printf(50, 100, buff);
		sprintf(buff, "y %d : %f", StageManager::ToIndexY(cameraPos.y), cameraPos.y);
		DX9printf(50, 150, buff);
		sprintf(buff, "FPS : %f", MyTime::Inst()->GetFPSCount());
		DX9printf(50, 200, buff);
		sprintf(buff, "SOUND : %d", GetSoundPosition(0));
		DX9printf(50, 250, buff);
	}
}