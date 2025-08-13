#pragma once
#include "Scene.h"
class SceneOpening : public Scene
{
public:
	SceneOpening(SceneManager* sceneManager, Parameter param);
	~SceneOpening() {}
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