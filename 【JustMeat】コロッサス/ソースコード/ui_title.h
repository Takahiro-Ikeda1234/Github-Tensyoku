#pragma once
#include"UIObject.h"
#include"Object.h"
#include<string>
#include "SceneManager.h"
class UITitle
{
public:
	UITitle(class SceneTitle *scene);
	~UITitle() {}
	void Update(float deltaTime);
	enum class TitleState
	{
		ePressAnyKey,
		eMenuSelect,
		//eSetting,
	};
public:
	TitleState GetState() { return mState; }
	void SetState(TitleState state);
	void SetMenuSelectCursor(int cursor);
private:
	static const int MENU_CURSOR_MAX = 3;
	class SceneTitle* mScene;
	TitleState mState = TitleState::ePressAnyKey;

	UIRectButton* mUIPressAnyKey;
	const std::string mTextPressAnyKey = "Press enter key";
	void StatePressAnyKey(float deltaTime);
	void HidePressAnyKey();
	void ShowPressAnyKey();

	class UIBox* mUIFrame;
	class UIBox* mUIMenuSelect[MENU_CURSOR_MAX];
	const std::string mTextMenuSelect[MENU_CURSOR_MAX] = 	{
		"開始",
		"ステージセレクト",
		"終了"
	};
	int mMenuSelectCursorY = 0;
	void StateMenuSelect(float deltaTime);
	void HideMenuSelect();
	void ShowMenuSelect();

	static const float LIMITTIME;
	Counter mKeyDownLimit;

	UIBox* mBox;
	UIBox* mControl;
	UIBox* mWASD;
	UIBox* mEnter;
	UIBox* mESC;
};