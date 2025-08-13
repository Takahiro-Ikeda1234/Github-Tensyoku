#pragma once
#include"MoveFloorObject.h"
class PushWall :public MoveFloorObject
{
public:
	PushWall(class Scene* scene, int indexX, int indexY);
	virtual ~PushWall() {}
	virtual void UpdateStageObject(float deltaTime)override;
private:
};