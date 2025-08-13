#pragma once
#include"StageObject.h"
class FragileBlock : public StageObject
{
public:
	FragileBlock(class Scene* scene, int indexX, int indexY, int type);
	~FragileBlock() {}
	virtual void UpdateStageObject(float deltaTime) override {}
	virtual void EnterGolemArm() override;
private:
	void GenerateFragments();
	void GenerateSmoke();
	void GenerateSpark();
};