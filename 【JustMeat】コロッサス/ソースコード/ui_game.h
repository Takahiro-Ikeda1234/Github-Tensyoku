#pragma once
class Scene;
class ComponentCircleUISprite;
#include"UIObject.h"
#include"SceneManager.h"
class UIGame
{
public:
	UIGame(class SceneGame *scene);
	~UIGame() {}
	void Update(float deltaTime);
	void SetAlphaPauseMenu(bool flag);
	void UpdatePauseMenu(float deltaTime);
private:
	void SetMenuSelectCursor(int cursor);
	int mMenuSelectCursorY;
	static const int MENU_CURSOR_MAX = 4;

	class SceneGame* mScene;

	class Filter* mFilter;
	class UIBox* mBox;
	class UIBox* mPause;
	class UIBox* mControl;
	class UIBox* mWASD;
	class UIBox* mESC;
	class UIBox* mEnter;
	class UIBox* mUIMenuSelect[4];
	class UIBox* mUIMenuCursor[4];

	static const float LIMITTIME;
	Counter mKeyDownLimit;

	class UIBoxFade* mDepthBG;
	class UIBoxFade* mDepth;
	class UIBoxFade* mStageTitleBG;
	class UIBoxFade* mStageTitle;
	
	Counter mRetryTime;
};

class DummyUI : public UIObject
{
public:
	DummyUI(Scene* scene);
	~DummyUI() {}
	void UpdateUIObject(float deltaTime);
	ComponentCircleUISprite* GetCCUSprite() { return mCCUSprite; }
	void SetAlphaRate(float rate);
private:
	ComponentCircleUISprite* mCCUSprite;
	bool mIsBlink = false;
};
