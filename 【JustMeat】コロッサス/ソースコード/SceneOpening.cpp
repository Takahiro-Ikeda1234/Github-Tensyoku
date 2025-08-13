#include "SceneOpening.h"
#include "obj_screenBackground.h"
#include "ComponentAnimationSprite.h"
#include "ComponentUISprite.h"
#include "EventManager.h"
#include "file.h"
#include "UIObject.h"
#include "keyboard.h"
#include "AudioData.h"
SceneOpening::SceneOpening(SceneManager * sceneManager, Parameter param)
	: Scene(sceneManager)
{
	SetVolumeBGM(6, 0);
	float fValue = 0;
	int value = 0;
	param.GetF("フェードイン時間", &fValue);
	mEventManager->Fadein(fValue, 0);

	auto it = new ScreenBackground(this, TextureTag::TitleBG);
	it->GetCASprite()->SetColorRate(0.4f);
	LoadMessage();
	NextText();


	auto mControl = new UIBox(this, 0.5f, 0.96f, 0.5f, 0.04f, TextureTag::Control, -101);
	auto mEnter = new UIBox(this, 0.8f, 0.96f, 0.0145f, 0.0185f, TextureTag::KeyEnter, -102);
	mEnter->GetCSprite()->SetString("次へ");
	mEnter->GetCSprite()->SetStringInfo(5, mEnter->GetPosition() + Vector2(GetWindowWidth() * 0.02f, GetWindowHeight() * -0.01f), Vector2(20, 20), false);

}

void SceneOpening::Update(float deltaTime)
{
	UpdateObjects(deltaTime);
	UpdateManagers(deltaTime);

	if (Keyboard::Inst()->KeyHit(DIK_RETURN))
	{
		if (NextText())
		{
			Parameter param;
			GetSceneManager()->SceneChange(SceneManager::SceneID::eGame, &param, 0, 1.0f, 1.0f);
		}
	}
}

void SceneOpening::LoadMessage()
{
	FILE* fp = fopen("resources/opening.txt", "r");
	std::vector<std::string> mRawData;
	int row = loadFileAllData(fp, &mRawData);
	int count = 0;
	mTexts.push_back({});
	for (int i = 0; i < row; i++)
	{

		int length = mRawData[i].size();
		if (length > 0 && mRawData[i][length - 1] == '@')
		{
			int end = mRawData[i].find_last_of('@');
			int start = mRawData[i].substr(0, end).find_last_of('@');
			std::string str = mRawData[i].substr(start + 1, end - start - 1);
			mRawData[i].erase(start, end - start + 1);
			mTexts[count].push_back(mRawData[i]);
			mTexts.push_back({});
			mTextEvent.push_back(std::stoi(str));
			count++;
		}
		else
		{
			mTexts[count].push_back(mRawData[i]);
		}
	}
	mTotalMessage = mTextEvent.size();
	fclose(fp);
}

bool SceneOpening::NextText()
{
	if (mCurrentMessage == mTotalMessage) return true;

	if (mCurrentMessage != 0)
	{
		switch (mTextEvent[mCurrentMessage - 1])
		{
		case 1:
			mTotalHeight = 0;
			while (!mTextObjects.empty())
			{
				mTextObjects.back()->GetCSprite()->SetStringAlpha(0x00);
				mTextObjects.back()->SetState(Object::State::eDead);
				mTextObjects.pop_back();
			}
			break;
		}
	}

	float height = ComponentSprite::GetStringHeight("1行の高さ", Vector2(32, 32), 30);
	for (int i = 0; i < mTexts[mCurrentMessage].size(); i++)
	{
		auto text = new UIBox(this, 0.5f, 0.1f + (mTotalHeight + height / 2.0f) / GetWindowHeight(), 0.1f, 0.1f, TextureTag::Invisible, 100);
		mTextObjects.push_back(text);
		text->GetCSprite()->SetString(mTexts[mCurrentMessage][i]);
		text->GetCSprite()->SetStringInfo(30, text->GetPosition(), Vector2(32, 32), true);
		mTotalHeight += height;
	}
	mCurrentMessage++;
	return false;
}
