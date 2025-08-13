//=============================================================================
//
// [mainProcess.cpp] 主処理
//
//=============================================================================

//インクルードファイル
#include"gameProcess.h"
#include<time.h>
#include<stdlib.h>
#include"mytime.h"
#include"mouse.h"
#include"keyboard.h"
#include"gamepad.h"
#include"DX9_core.h"
#include"DX9_sound.h"
#include"DX9_printf.h"
#include"DX9_glitch.h"
#include"draw.h"
#include"TextureData.h"
#include"SceneManager.h"
#include"StageData.h"
#include"scene.h"
#include"StageManager.h"
#include"EventManager.h"
#include"obj_player.h"

GameProcess::GameProcess()
{
	srand((unsigned)time(NULL));
	InitSound();
	InitDraw();
	StageData::Inst();
	mSceneManager = new SceneManager;
}

GameProcess::~GameProcess()
{
	UninitSound();
	TextureSystem::Get()->Uninit();
}

bool GameProcess::Update()
{
	MyTime::Inst()->Update();
	float deltaTime = MyTime::Inst()->GetDeltaTime();
	Keyboard::Inst()->Update(deltaTime);
	Mouse::Inst()->Update(deltaTime);
	Gamepad::Inst()->Update(deltaTime);


	int r = 0, g = 0, b = 0;
	bool inCave = false;
	if (mSceneManager->GetScene()->GetPlayer()) inCave = mSceneManager->GetScene()->GetPlayer()->InCave();
	switch (mSceneManager->GetScene()->GetStageManager()->GetCurrentStageID())
	{
	case 0:
	case 1:
	case 2:
		r = 4;
		g = 17;
		b = 44;
		if (inCave)
		{
			r = 0;
			g = 0;
			b = 23;
		}
		break;
	case 3:
	case 4:
	case 5:
		r = 53;
		g = 36;
		b = 60;
		if (inCave)
		{
			r = 24;
			g = 0;
			b = 7;
		}
		break;
	case 6:
	case 7:
	case 8:
		r = 166;
		g = 213;
		b = 223;
		if (inCave)
		{
			r = 0;
			g = 0;
			b = 0;
		}
		break;
	}
	DrawStart(r, g, b);


	mSceneManager->Update(deltaTime);
	mSceneManager->Draw();

	SceneManager::SceneID next = mSceneManager->GetNextScene();
	if (next != SceneManager::eDefault && mSceneManager->GetScene()->GetEventManager()->IsFadeoutEnd())
	{
		if (next == SceneManager::eExit)
		{
			delete mSceneManager;
			return true;
		}
		else
		{
			mSceneManager->LoadScene(next);
			mSceneManager->Update(0);
			mSceneManager->Draw();
		}
	}
	UpdateSound(deltaTime);

	if (!mSceneManager->GetScene()->IsPause())DrawGlitch(mSceneManager->GetScene()->GetCameraManager(), deltaTime);
	SwapBuffer();

	return false;
}

