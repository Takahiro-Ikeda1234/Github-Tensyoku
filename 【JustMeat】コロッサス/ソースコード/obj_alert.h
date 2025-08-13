#pragma once
#include"object.h"
#include"UIObject.h"
#include"Counter.h"
class AlertUI;
class AlertArrow;
class Alert : public Object
{
public:
	Alert(Scene* scene, Vector2 origin, Vector2 rect, Vector2 armStart, float lifetime, float blinktime = 1, unsigned color = 0xFFFF0000);
	~Alert();
	void UpdateObject(float deltaTime) override;
	virtual void LoadCheckPointData()override;
private:
	Counter mLifetime;
	LoopCounter mBlink;
	AlertUI* mUI;
	AlertArrow* mUIArrow;
};

class AlertUI : public UIObject
{
public:
	AlertUI(Scene* scene, Vector2 armStart, Vector2 alertCenter);
	~AlertUI(){}
	virtual void UpdateUIObject(float deltaTime) override;
private:
	Vector2 mArmStart;
	Vector2 mAlertCenter;
};

class AlertArrow : public UIObject
{
public:
	AlertArrow(Scene* scene, Vector2 armStart, Vector2 alertCenter);
	~AlertArrow() {}
	virtual void UpdateUIObject(float deltaTime) override;
private:
	Vector2 mArmStart;
	Vector2 mAlertCenter;
	ComponentUISprite* mUISprite2;
	LoopCounter mBlink;
};