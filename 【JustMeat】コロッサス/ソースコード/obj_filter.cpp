#include "obj_filter.h"
#include "ComponentUISprite.h"
Filter::Filter(Scene * scene, TextureTag tag, int drawPriority)
	: UIBox(scene, 0.5f, 0.5f, 0.5f, UIObject::ToHeightSize(0.5f), tag, drawPriority)
{
	mUISprite->SetAlpha(0xFF);
	mUISprite->SetAlphaRate(0);
}