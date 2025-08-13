#pragma once
#include"StageObject.h"
class FragileFallRock : public StageObject
{
public:
	FragileFallRock(Scene* scene, int indexX, int indexY);
	~FragileFallRock() {}
	void UpdateStageObject(float deltaTime) override;
	virtual void EnterGolemArm()override;
	virtual void LoadCheckPointData()override;
private:
	Counter mFallDelay;
	Vector2 mSpawnPos;
	class ComponentMove* mMove;
	void GenerateFragments();
	void GenerateSmoke();
	void GenerateSpark();
};