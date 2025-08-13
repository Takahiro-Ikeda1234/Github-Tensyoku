#pragma once
#include"Object.h"
class StagePicture : public Object
{
public:
	StagePicture(class Scene* scene, int id, int stageID, int stageWidth, int stageHeight);
	~StagePicture() {}
	virtual void UpdateObject(float deltaTime) override {}
	void ReloadTexture();
private:
	int mID;
};