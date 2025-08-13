#pragma once
#include"Component.h"
class ComponentTrigger : public Component
{
public:
	ComponentTrigger(class AbstractObject* owner, int ch, int updatePriority = UPDATE_PRIORITY_OBJECT);
	~ComponentTrigger();
	int GetCh() { return mCh; }
	void SetActive(bool active) { mIsActive = active; }
	bool IsActive() { return mIsActive; }
	virtual void Update(float deltaTime)override {}
private:
	int mCh;
	bool mIsActive = false;
};

class ComponentTriggered : public Component
{
public:
	ComponentTriggered(class AbstractObject* owner, int ch, int updatePriority = UPDATE_PRIORITY_OBJECT);
	~ComponentTriggered() {}
	virtual void Update(float deltaTime) override;
	bool IsActive() { if (mTrigger == nullptr) { return false; }return mTrigger->IsActive(); }
private:
	int mCh;
	ComponentTrigger* mTrigger;
};