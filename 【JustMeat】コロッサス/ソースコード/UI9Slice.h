#pragma once
#include"UIObject.h"
class UI9Slice : public UIBox
{
public:
	UI9Slice(Scene* scene, float xPos, float yPos, float xSize, float ySize, TextureTag tag, int drawPriority);
	virtual ~UI9Slice() {}
	virtual void UpdateUIObject(float deltaTime) override {}
private:
};