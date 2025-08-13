#pragma once
#include "StageObject.h"
class GolemCore : public StageObject
{
public:
	GolemCore(Scene*scene, int indexX, int indexY);
	~GolemCore(){}
	virtual void UpdateStageObject(float deltaTime) override;
	virtual void EnterGolemArm() override;
private:
	void Explosion1(Vector2 pos, float scale);
	void Explosion2(Vector2 pos, float scale);
	void Explosion3(Vector2 pos, float scale);
	Counter mExplosionTime;
	int mExplosionCount = 0;
};