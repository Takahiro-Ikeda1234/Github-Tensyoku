#pragma once
#include"ComponentAnimationSprite.h"
class ComponentCircleSprite : public ComponentAnimationSprite
{
public:
	ComponentCircleSprite(class AbstractObject* owner, TextureTag tag, int drawPriority = LAYER_OBJECT);
	virtual ~ComponentCircleSprite() {}
	void SetFill(float fill) { mFill = fill; }	//0Å`1
	float GetFill() { return mFill; }
	virtual void Update(float deltaTime) override;
	virtual void Draw() override;
protected:
	virtual void Mask();
	float mFill;
};