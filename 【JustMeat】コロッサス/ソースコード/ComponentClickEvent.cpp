#include "ComponentClickEvent.h"
#include "mouse.h"
#include "CameraManager.h"
#include "Object.h"
#include "Scene.h"

ComponentClickEvent::ComponentClickEvent(AbstractObject * owner)
	:Component(owner)
{

}


bool ComponentClickEvent::OnHit()
{
	if (OnMouse() && LeftHit()) return true;
	return false;
}

bool ComponentClickEvent::Released()
{
	if (mClickTime.GetLeftTime() > 0 && Mouse::Inst()->MouseUp(MOUSE_LEFT)) return true;
	return false;
}

bool ComponentClickEvent::OnClicked()
{
	if (mClickTime.GetLeftTime() > 0 && Released())
	{
		return true;
	}
	return false;
}

bool ComponentClickEvent::OnHitRight()
{
	if (OnMouse() && RightHit()) return true;
	return false;
}

bool ComponentClickEvent::ReleasedRight()
{
	if (mRightClickTime.GetLeftTime() > 0 && Mouse::Inst()->MouseUp(MOUSE_RIGHT)) return true;
	return false;
}

bool ComponentClickEvent::OnClickedRight()
{
	if (mRightClickTime.GetLeftTime() > 0 && ReleasedRight())
	{
		return true;
	}
	return false;
}

bool ComponentClickEvent::OuterHit()
{
	if (!OnMouse() && LeftHit()) return true;
	return false;
}

bool ComponentClickEvent::OuterClicked()
{
	if (mOuterClickTime.GetLeftTime() > 0 && Released())
	{
		return true;
	}
	return false;
}

bool ComponentClickEvent::OuterHitRight()
{
	if (!OnMouse() && RightHit()) return true;
	return false;
}

bool ComponentClickEvent::OuterClickedRight()
{
	if (mOuterRightClickTime.GetLeftTime() > 0 && Released())
	{
		return true;
	}
	return false;
}

bool ComponentClickEvent::LeftHit()
{
	return Mouse::Inst()->MouseHit(MOUSE_LEFT);
}

bool ComponentClickEvent::RightHit()
{
	return Mouse::Inst()->MouseHit(MOUSE_RIGHT);
}

Vector2 ComponentClickEvent::GetWorldPos()
{
	return Vector2(mOwner->GetScene()->GetCameraManager()->ToWorldPosition(Mouse::Inst()->GetPos()));
}

void ComponentClickEvent::Update(float deltaTime)
{
	mClickTime -= deltaTime;
	mRightClickTime -= deltaTime;
	mOuterClickTime -= deltaTime;
	mOuterRightClickTime -= deltaTime;
	if (OnHit())
	{
		mClickTime = CLICKTIME;
	}
	if (OnHitRight())
	{
		mRightClickTime = CLICKTIME;
	}
	if (OuterHit())
	{
		mOuterClickTime = CLICKTIME;
	}
	if (OuterHitRight())
	{
		mOuterRightClickTime = CLICKTIME;
	}
}

ComponentClickEventCircle::ComponentClickEventCircle(AbstractObject * owner, float radius)
	: ComponentClickEvent(owner)
	, mRadius(radius)
{
}

bool ComponentClickEventCircle::OnMouse()
{
	Vector2 mouseWorldPos = GetWorldPos();
	float dist = Vector2::Distance(mOwner->GetPosition(), mouseWorldPos);
	if (dist <= mRadius) return true;
	return false;
}

ComponentClickEventObject::ComponentClickEventObject(AbstractObject * owner)
	: ComponentClickEvent(owner)
{
}

bool ComponentClickEventObject::OnMouse()
{
	Vector2 mouseWorldPos = GetWorldPos();
	Vector2 ownerPos = mOwner->GetPosition();
	Vector2 ownerCol = mOwner->GetColSize();
	if (ownerPos.x - ownerCol.x <= mouseWorldPos.x && mouseWorldPos.x <= ownerPos.x + ownerCol.x)
	{
		if (ownerPos.y - ownerCol.y <= mouseWorldPos.y && mouseWorldPos.y <= ownerPos.y + ownerCol.y)
		{
			return true;
		}
	}
	return false;
}


ComponentClickEventRectUI::ComponentClickEventRectUI(AbstractObject * owner)
	: ComponentClickEvent(owner)
{
}

bool ComponentClickEventRectUI::OnMouse()
{
	Vector2 mouseScreenPos = Mouse::Inst()->GetPos();
	if (mOwner->GetPosition().x - mOwner->GetColSize().x <= mouseScreenPos.x &&mouseScreenPos.x <= mOwner->GetPosition().x + mOwner->GetColSize().x)
	{
		if (mOwner->GetPosition().y - mOwner->GetColSize().y <= mouseScreenPos.y &&mouseScreenPos.y <= mOwner->GetPosition().y + mOwner->GetColSize().y)
		{
			return true;
		}
	}
	return false;
}

ComponentClickEventCircleUI::ComponentClickEventCircleUI(AbstractObject * owner, float radius)
	: ComponentClickEvent(owner)
	, mRadius(radius)
{
}

bool ComponentClickEventCircleUI::OnMouse()
{
	Vector2 mouseScreenPos = Mouse::Inst()->GetPos();
	float dist = Vector2::Distance(mOwner->GetPosition(), mouseScreenPos);
	if (dist <= mRadius) return true;
	return false;
}
