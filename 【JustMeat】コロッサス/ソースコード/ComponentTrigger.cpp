#include "ComponentTrigger.h"
#include "AbstractObject.h"
#include "Scene.h"
ComponentTrigger::ComponentTrigger(AbstractObject * owner, int ch, int updatePriority)
	: Component(owner, updatePriority)
	, mCh(ch)
{
	mOwner->GetScene()->AddTrigger(this);
}
ComponentTrigger::~ComponentTrigger()
{
	mOwner->GetScene()->RemoveTrigger(this);
}

void ComponentTriggered::Update(float deltaTime)
{
	if (mTrigger == nullptr)
	{
		auto objects = mOwner->GetScene()->GetTriggers();
		for (auto it : objects)
		{
			if (it->GetCh() == mCh)
			{
				mTrigger = it;
				break;
			}
		}
	}
}

ComponentTriggered::ComponentTriggered(AbstractObject * owner, int ch, int updatePriority)
	: Component(owner, updatePriority)
	, mCh(ch)
{
}
