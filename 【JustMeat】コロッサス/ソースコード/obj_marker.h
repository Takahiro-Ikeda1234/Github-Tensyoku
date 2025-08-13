#pragma once
#include"StageObject.h"
class Marker : public StageObject
{
public:
	Marker(Scene* scene, int indexX, int indexY);
	~Marker(){}
	virtual void UpdateStageObject(float deltaTime)override;
	virtual void EnterGolemArm() override;
	virtual void ExitGolemArm() override;
private:
	Counter mFade;
	bool mIsFadein = false;
};