#pragma once
#include"ComponentUISprite.h"
class ComponentSystemMessage : public ComponentUISprite
{
public:
	ComponentSystemMessage(class AbstractObject* owner, TextureTag tag, int drawPriority = LAYER_OBJECT)
		: ComponentUISprite(owner, tag, drawPriority) {}
	~ComponentSystemMessage() {}
	virtual void Draw() override;
	void Appear(float messageTime) { mAppearTime = 0.5f; mMessageTime = messageTime; mShow = true; }
	void Disappear() { mAppearTime = 0; mMessageTime = 0; mDisappearTime = 0.25f; mShow = false; }
	virtual void Update(float deltaTime)override;
	virtual void SetString(std::string text, int coloredStart, int coloredCount);
	void UpdateMessageColor(float alphaRate);
private:
	Counter mAppearTime;
	Counter mDisappearTime;
	Counter mMessageTime;
	Vector2 mStringOffset;
	bool mShow = false;
	unsigned* mColorArray;
	int mColoredStart = 0;
	int mColoredCount = 0;
};