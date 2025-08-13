#pragma once
#include "Scene.h"
#include "Counter.h"
class SceneFin : public Scene
{
public:
	SceneFin(SceneManager* sceneManager, Parameter param);
	~SceneFin() {}
	virtual void Update(float deltaTime)override;
	virtual void DrawUI() override {}
private:
	Counter mFinTime = 20.0f;
};