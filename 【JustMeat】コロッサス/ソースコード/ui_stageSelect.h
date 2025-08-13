#pragma once
#include"UIObject.h"
#include"Object.h"
#include<string>
#include "SceneManager.h"
class UIStageSelect
{
public:
	UIStageSelect(class SceneStageSelect *scene);
	~UIStageSelect() {}
	void Update(float deltaTime);
	void StateStageSelect(float deltaTime);
	enum class StageSelectState
	{
		eStageSelect,
	};
public:
	StageSelectState GetState() { return mState; }
	void SetStageSelectCursor(int cursor);
private:
	class SceneStageSelect* mScene;
	StageSelectState mState = StageSelectState::eStageSelect;

	static const int SELECTABLE_STAGE_NUM = 8;
	int mStageSelectCursor = 0;
	UIBox* mUIStages[SELECTABLE_STAGE_NUM];

	static const float LIMITTIME;
	Counter mKeyDownLimit;
};