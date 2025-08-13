#pragma once
#include "Scene.h"
#include "Counter.h"
class SceneCredit : public Scene
{
public:
	SceneCredit(SceneManager* sceneManager, Parameter param);
	~SceneCredit() {}
	virtual void Update(float deltaTime)override;
	virtual void DrawUI() override {}
private:
	void LoadMessage();
	std::vector<std::string> mTexts;
	std::vector<int> mTextEvent;	//0 í èÌ 1 â¸ÉyÅ[ÉW
	std::vector<class UIObject*> mTextObjects;
	int mTotalMessage = 0;
	int mCurrentMessage = 0;
	float mTotalHeight = 0;
	bool NextText();
	float mScrollTime = 30.0f;
	Counter mEndingTime = 42.0f;
	class UIObject* mTitle;
	class UIObject* mLogo;
};