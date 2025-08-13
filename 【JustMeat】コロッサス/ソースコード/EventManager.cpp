#include "EventManager.h"
#include "ComponentUISprite.h"
#include"keyboard.h"
#include"Scene.h"
#include"obj_filter.h"
#include"file.h"
#include"obj_player.h"
#include"TextureData.h"

EventData EventManager::mCatEvent[STAGE_MAX][EV_MAX];	//ステージ数
EventData EventManager::mGolemEvent[STAGE_MAX][EV_MAX];	//ステージ数
bool EventManager::mAlreadyLoadEventFile = false;

EventManager::EventManager(Scene * scene)
	: Manager(scene)
{
	mFade = new Filter(scene, TextureTag::Plane);
	mFade->GetCSprite()->SetColorRate(0);

	if (mScene->GetSceneManager()->GetSceneID() == SceneManager::SceneID::eGame && mScene->GetStageManager()->GetCurrentStageID() == 0)
	{
		mFog = new Filter(scene, TextureTag::Fog);
		mFog->GetCSprite()->SetAlphaRate(0.5f);
		mFog->GetCSprite()->SetDrawMode(ComponentSprite::DrawMode::eMultiple);
		mFog->GetCSprite()->SetActive(false);
	}
	LoadEventFile();
}

void EventManager::Update(float deltaTime)
{
	UpdateFade(deltaTime);
	UpdateSlow(deltaTime);
	mHitstopDelay -= deltaTime;
	if (mHitstopDelay.IsEnd()) mHitstopTime -= deltaTime;


	///洞窟内判定一旦削除
	if (mScene->GetPlayer())
	{
		if (mScene->GetPlayer()->InCave())
		{
			mFog->GetCSprite()->SetActive(true);
		}
		else
		{
			mFog->GetCSprite()->SetActive(false);
		}
	}
}


bool EventManager::IsFadeinEnd()
{
	return mFadeinTime.IsEnd();
}

bool EventManager::IsFadeoutEnd()
{
	return mFadeoutDelay.IsEnd() && mFadeoutTime.IsEnd();
}

void EventManager::SetWhiteFade()
{
	mFade->GetCSprite()->SetColorRate(1);
}

void EventManager::UpdateSlow(float deltaTime)
{
	mSlowTime -= deltaTime;
	mSlowResetTime -= deltaTime;
	if(!mSlowTime.IsEnd())
	{
		mCurrentSlowSpeed += mIncreaseSlowSpeedPerSecond * deltaTime;
	}
	if (mSlowResetTime.IsFinish())
	{
		mCurrentSlowSpeed = 1;
	}
}

void EventManager::UpdateFade(float deltaTime)
{
	mFadeoutDelay -= deltaTime;
	mFadeinDelay -= deltaTime;
	if (mFadeoutDelay.IsEnd()) mFadeoutTime -= deltaTime;
	if (mFadeinDelay.IsEnd()) mFadeinTime -= deltaTime;

	switch (mFadeMode)
	{
	case -1:
		mFade->GetCSprite()->SetAlphaRate(1 - mFadeoutTime.Percentage());
		break;
	case 0:
		break;
	case 1:
		mFade->GetCSprite()->SetAlphaRate(mFadeinTime.Percentage());
		break;
	}

	if (mFadeoutTime.IsEnd() && mFadeMode == -1)
	{
		mFade->GetCSprite()->SetAlphaRate(1);
		mFadeMode = 0;
	}
	if (mFadeinTime.IsEnd() && mFadeMode == 1)
	{
		mFade->GetCSprite()->SetAlphaRate(0);
		mFadeMode = 0;
		mFade->GetCSprite()->SetColorRate(0);
	}
}

void EventManager::LoadEventFile()
{
	if (mAlreadyLoadEventFile) return;
	mAlreadyLoadEventFile = true;
	std::vector<std::string> fileData;
	FILE* fp = fopen("resources/event.txt", "r");
	loadFileAllData(fp, &fileData);


	EventData((*pEventArea)[EV_MAX]) = mCatEvent;

	int row = fileData.size();
	for (int ct = 0; ct < row; ct++)
	{
		if (IsSplit(fileData[ct][0])) continue;

		int index = 0;
		int stage = 0;
		int eventID = 0;
		std::string temp = NextData(&fileData[ct], &index);
		if (temp == "Cat")
		{
			pEventArea = mCatEvent;
		}
		else if (temp == "Golem")
		{
			pEventArea = mGolemEvent;
		}
		temp = NextData(&fileData[ct], &index);
		stage = atoi(temp.c_str());
		temp = NextData(&fileData[ct], &index);
		eventID = atoi(temp.c_str());
		temp = NextData(&fileData[ct], &index);
		pEventArea[stage][eventID].area.left = atoi(temp.c_str());
		temp = NextData(&fileData[ct], &index);
		pEventArea[stage][eventID].area.right = atoi(temp.c_str());
		temp = NextData(&fileData[ct], &index);
		pEventArea[stage][eventID].area.top = atoi(temp.c_str());
		temp = NextData(&fileData[ct], &index);
		pEventArea[stage][eventID].area.bottom = atoi(temp.c_str());
		temp = NextData(&fileData[ct], &index);
		pEventArea[stage][eventID].x = atoi(temp.c_str());
		temp = NextData(&fileData[ct], &index);
		pEventArea[stage][eventID].y = atoi(temp.c_str());

	}
	fclose(fp);
}



bool InRect(CameraManager::Rect& rect, int x, int y)
{
	if (rect.left <= x && x <= rect.right && rect.top <= y && y <= rect.bottom)
	{
		return true;
	}
	return false;
}
