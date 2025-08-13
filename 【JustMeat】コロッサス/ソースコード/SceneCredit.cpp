#include "SceneCredit.h"
#include "obj_screenBackground.h"
#include "ComponentAnimationSprite.h"
#include "ComponentUISprite.h"
#include "EventManager.h"
#include "file.h"
#include "UIObject.h"
#include "keyboard.h"
#include "AudioData.h"
SceneCredit::SceneCredit(SceneManager * sceneManager, Parameter param)
	: Scene(sceneManager)
{
	SetVolumeBGM(6, 0);
	float fValue = 0;
	int value = 0;
	param.GetF("フェードイン時間", &fValue);
	mEventManager->Fadein(fValue, 0);
	mEventManager->SetWhiteFade();

	auto bg = new ScreenBackground(this, TextureTag::TitleEndingBG);
	auto back = new UIBox(this, 0.5f, 0.5f, 0.25f, UIObject::ToHeightSize(0.5f), TextureTag::Credit, 101);
	LoadMessage();

	mTitle = new UIBox(this, 0.5f, 1.25f + mTotalHeight / GetWindowHeight(), 0.2f, 0.1f, TextureTag::Colossus, 100);
	mTextObjects.push_back(mTitle);

	NextText();

	mLogo = new UIBox(this, 0.5f, 2.5f + mTotalHeight / GetWindowHeight(), 0.1f, 0.1f, TextureTag::Justmeat, 100);
	mTextObjects.push_back(mLogo);
}

void SceneCredit::Update(float deltaTime)
{
	UpdateObjects(deltaTime);
	UpdateManagers(deltaTime);
	float h = mTotalHeight + GetWindowHeight();
	if (mLogo->GetPosition().y > GetWindowHeight() * 0.5f)
	{
		for (auto it : mTextObjects)
		{
			it->Translate(Vector2(0, -h / mScrollTime * deltaTime));
			it->GetCSprite()->SetStringInfo(30, it->GetPosition(), Vector2(32, 32), true);
		}
	}

	//if (mEndingTime.IsEnd())
	//{
	//	for (auto it : mTextObjects)
	//	{
	//		it->SetState(Object::State::eDead);
	//	}
	//	mTextObjects.clear();
	//}

	mEndingTime -= deltaTime;

	if (Keyboard::Inst()->KeyHit(DIK_RETURN) || mEndingTime.IsEnd())
	{
		if (NextText())
		{
			Parameter param;
			GetSceneManager()->SceneChange(SceneManager::SceneID::eFin, &param, 0, 2.0f, 2.0f, true);
		}
	}
}

void SceneCredit::LoadMessage()
{
	FILE* fp = fopen("resources/credit.txt", "r");
	std::vector<std::string> mRawData;
	int row = loadFileAllData(fp, &mRawData);
	int count = 0;
	for (int i = 0; i < row; i++)
	{

		int length = mRawData[i].size();
		if (length > 0 && mRawData[i][length - 1] == '@')
		{
			int end = mRawData[i].find_last_of('@');
			int start = mRawData[i].substr(0, end).find_last_of('@');
			std::string str = mRawData[i].substr(start + 1, end - start - 1);
			mRawData[i].erase(start, end - start + 1);
			mTexts.push_back(mRawData[i]);
			mTextEvent.push_back(std::stoi(str));
		}
		else
		{
			mTexts.push_back(mRawData[i]);
		}
	}
	mTotalMessage = 1;
	fclose(fp);
}

bool SceneCredit::NextText()
{
	if (mCurrentMessage == mTotalMessage) return true;
	mTotalHeight = 0.5f * GetWindowHeight();
	float height = ComponentSprite::GetStringHeight("1行の高さ", Vector2(32, 32), 30);
	for (int i = 0; i < mTexts.size(); i++)
	{
		auto text = new UIBox(this, 0.5f, 1.5f + (mTotalHeight + height / 2.0f) / GetWindowHeight(), 0.1f, 0.1f, TextureTag::Invisible, 100);

		mTextObjects.push_back(text);
		text->GetCSprite()->SetString(mTexts[i]);
		text->GetCSprite()->SetStringInfo(30, text->GetPosition(), Vector2(32, 32), true);
		text->GetCSprite()->SetStringColor(0xFFFFFFFF);
		mTotalHeight += height;
	}
	mCurrentMessage++;
	return false;
}
