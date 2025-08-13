#pragma once
#include"MoveFloorObject.h"
class TriggeredMoveWall : public MoveFloorObject
{
public:
	TriggeredMoveWall(Scene* scene, Vector2 pos, int id);
	TriggeredMoveWall(Scene* scene, int indexX, int indexY, int id);
	~TriggeredMoveWall() {}
	void UpdateStageObject(float deltaTime) override;
	virtual void SaveCheckPointData()override;
	virtual void LoadCheckPointData()override;
private:
	class ComponentTriggered* mTriggered;
	bool mIsActive = false;

	bool mCPIsActive = false;
};