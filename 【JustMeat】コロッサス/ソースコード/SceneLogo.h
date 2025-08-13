#pragma once
#include "Scene.h"
#include "Counter.h"
class SceneLogo : public Scene
{
public:
	SceneLogo(SceneManager* sceneManager, Parameter param);
	~SceneLogo() {}
	virtual void Update(float deltaTime)override;
	virtual void DrawUI() override {}
private:
	Counter mLogoTime = 2.0f;
};