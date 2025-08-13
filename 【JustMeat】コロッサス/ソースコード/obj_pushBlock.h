#pragma once
#include"MoveFloorObject.h"
class PushBlock :public MoveFloorObject
{
public:
	PushBlock(class Scene* scene, int indexX, int indexY);
	virtual ~PushBlock() {}
	virtual void UpdateStageObject(float deltaTime)override;
private:
};