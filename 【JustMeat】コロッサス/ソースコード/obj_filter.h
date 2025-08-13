#pragma once
#include"UIObject.h"
class Filter : public UIBox
{
public:
	Filter(Scene* scene, TextureTag tag, int drawPriority = -10000);
	~Filter() {}
	ComponentUISprite* mMask;
};