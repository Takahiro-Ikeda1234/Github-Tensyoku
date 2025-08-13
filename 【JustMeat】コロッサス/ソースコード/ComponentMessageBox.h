#pragma once
#include"ComponentUI9Slice.h"
class ComponentMessageBox : public ComponentUI9Slice
{
public:
	ComponentMessageBox(class AbstractObject* owner, TextureTag tag, int drawPriority = LAYER_OBJECT)
		: ComponentUI9Slice(owner, tag, drawPriority) {}
	~ComponentMessageBox() {}
	virtual void Draw() override;
	void Appear(float messageTime) { mAppearTime = 0.5f; mMessageTime = messageTime; mShow = true; }
	void Disappear() { mAppearTime = 0; mMessageTime = 0; mDisappearTime = 0.25f; mShow = false; }
	virtual void Update(float deltaTime)override;
	virtual void SetString(std::string text, int coloredStart, int coloredCount);
	Vector2 GetPosition();
	Vector2 GetSize();
private:
	Counter mAppearTime;
	Counter mDisappearTime;
	Counter mMessageTime;
	Vector2 mStringOffset;
	bool mShow = false;
	unsigned* mColorArray;
};