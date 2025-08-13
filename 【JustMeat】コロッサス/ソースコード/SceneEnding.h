#pragma once
#include "Scene.h"
class SceneEnding : public Scene
{
public:
	SceneEnding(SceneManager* sceneManager, Parameter param);
	~SceneEnding() {}
	virtual void Update(float deltaTime)override;
	virtual void DrawUI() override {}
private:
	void LoadMessage();
	std::vector<std::vector<std::string>> mTexts;
	std::vector<int> mTextEvent;	//0 ’Êí 1 ‰üƒy[ƒW
	std::vector<class UIObject*> mTextObjects;
	int mTotalMessage = 0;
	int mCurrentMessage = 0;
	float mTotalHeight = 0;
	bool NextText();
};