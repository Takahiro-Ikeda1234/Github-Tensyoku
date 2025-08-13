//=============================================================================
//
// [Object.h] オブジェクト共通処理
//
//=============================================================================
#ifndef _UIOBJECT_H_
#define _UIOBJECT_H_

//インクルードファイル
#include<vector>
#include"Vector2.h"
#include"AbstractObject.h"
#include"textureData.h"
#include"Counter.h"

class ComponentClickEvent;
class ComponentUISprite;
class Scene;
//クラス定義
class UIObject : public AbstractObject
{
public:
	UIObject(Scene* scene);
	virtual ~UIObject();
	bool Clicked();
	bool OuterClicked();
	void SetVisible();
	void SetInisible();
	virtual void UpdateUIObject(float deltaTime) = 0;
	virtual void UpdateObject(float deltaTime) final override;
	virtual ComponentUISprite* GetCSprite() { return mUISprite; }
	static float ToHeightSize(float ySize);
	static float ToWidthSize(float ySize);
	void SetBlink(float blink, float blinkTime = 0);

protected:
	ComponentClickEvent* mClick;
	ComponentUISprite* mUISprite;
	LoopCounter mBlink;
	Counter mBlinkTime;
};


class UIBox : public UIObject
{
public:
	UIBox(Scene* scene, float xPos, float yPos, float xSize, float ySize, TextureTag tag, int drawPriority);
	virtual ~UIBox() {}
	virtual void UpdateUIObject(float deltaTime) override {}
};

class UIBoxFade : public UIBox
{
public:
	UIBoxFade(Scene* scene, float xPos, float yPos, float xSize, float ySize, TextureTag tag, int drawPriority) :UIBox(scene, xPos, yPos, xSize, ySize, tag, drawPriority){}
	virtual ~UIBoxFade() {}
	virtual void UpdateUIObject(float deltaTime) override;
	void SetShowTime(float fadeinTime, float showTime, float fadeoutTime, float fadeinDelay);
private:
	Counter mFadeinDelay;
	Counter mFadeinTime;
	Counter mShowTime;
	Counter mFadeoutTime;
};

class UICircleButton : public UIObject
{
public:
	UICircleButton(Scene* scene, float xPos, float yPos, float radius, TextureTag tag, int drawPriority);
	virtual ~UICircleButton(){}
	virtual void UpdateUIObject(float deltaTime) override;
};

class UIRectButton : public UIObject
{
public:
	UIRectButton(Scene* scene, float xPos, float yPos, float xSize, float ySize, TextureTag tag, int drawPriority);
	virtual ~UIRectButton() {}
	virtual void UpdateUIObject(float deltaTime) override;
};

class UIRectSwitch : public UIRectButton
{
public:
	UIRectSwitch(Scene* scene, float xPos, float yPos, float xSize, float ySize, TextureTag tag, int drawPriority) : UIRectButton(scene, xPos, yPos, xSize, ySize, tag, drawPriority), mIsSwitchActive(false) {}
	virtual ~UIRectSwitch() {}
	void UpdateUIObject(float deltaTime) override;
	bool IsSwitchActive() { return mIsSwitchActive; }
	void SetSwitchActive(bool active) { mIsSwitchActive = active; }
	virtual void Active(float deltaTime);
private:
	bool mIsSwitchActive;
};

class UIRectNumBox : public UIRectButton
{
public:
	UIRectNumBox(Scene* scene, int defaultNum, int min, int max, float xPos, float yPos, float xSize, float ySize, TextureTag tag, int drawPriority);
	virtual ~UIRectNumBox() {}
	virtual void UpdateUIObject(float deltaTime) override;
	int GetNum() { return mCurrentNum; }
	void SetNum(int num) { mCurrentNum = num; }
private:
	int mMaxDigit = 3;
	int mDigit;
	char mCurrentNumString[4];
	int mCurrentNum;
	const int mMax;
	const int mMin;
	bool mIsBoxActive;
};

#endif