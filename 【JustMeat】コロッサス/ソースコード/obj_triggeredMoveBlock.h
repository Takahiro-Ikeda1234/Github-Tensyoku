#pragma once
#include"MoveFloorObject.h"
class TriggeredMoveBlock : public MoveFloorObject
{
public:
	TriggeredMoveBlock(Scene* scene, Vector2 pos, int id);
	TriggeredMoveBlock(Scene* scene, int indexX, int indexY, int id);
	~TriggeredMoveBlock() {}
	void UpdateStageObject(float deltaTime) override;
	virtual void SaveCheckPointData()override;
	virtual void LoadCheckPointData()override;
private:
	class ComponentTriggered* mTriggered;
	bool mIsActive = false;

	bool mCPIsActive = false;
};