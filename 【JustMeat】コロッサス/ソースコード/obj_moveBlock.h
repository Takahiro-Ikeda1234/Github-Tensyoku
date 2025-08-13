#pragma once
#include"MoveFloorObject.h"
class MoveBlock : public MoveFloorObject
{
public:
	MoveBlock(Scene* scene, Vector2 pos, int id);
	MoveBlock(Scene* scene, int indexX, int indexY, int id);
	~MoveBlock() {}
	void UpdateStageObject(float deltaTime) override;
	//virtual void LoadCheckPointData(float deltaTime);
private:
	Vector2 mStartPoint;
	Vector2 mStartMove;
};