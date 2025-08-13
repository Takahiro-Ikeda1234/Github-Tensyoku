#pragma once
#include"StageObject.h"
class TriggerButton : public StageObject
{
public:
	TriggerButton(Scene* scene, Vector2 pos, int ch, int type);	//0 âE, 1 ç∂, 2 è„, 3 â∫
	TriggerButton(Scene* scene, int indexX, int indexY, int ch, int type);
	~TriggerButton() {};
	virtual void UpdateStageObject(float deltaTime)override;
	virtual void EnterGolemArm() override;
	virtual void ExitGolemArm() override;

private:
	class ComponentTrigger* mCTrigger;
};