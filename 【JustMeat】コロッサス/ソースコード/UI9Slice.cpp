#include "UI9Slice.h"
#include "ComponentUI9Slice.h"
#include "Windows_core.h"

UI9Slice::UI9Slice(Scene * scene, float xPos, float yPos, float xSize, float ySize, TextureTag tag, int drawPriority)
	:UIBox(scene, xPos, yPos, xSize, ySize, TextureTag::Invisible, drawPriority)
{
	delete mUISprite;
	mUISprite = new  ComponentUI9Slice(this, tag, drawPriority);
}