#pragma once
#include"Component.h"
#include"Counter.h"
#include"Vector2.h"
class AbstractObject;
#define CLICKTIME 0.2f	//クリック押下→離すがこの時間以内におこなわれたらクリックと判定する
class ComponentClickEvent : public Component
{
public:
	ComponentClickEvent(AbstractObject* owner);
	virtual ~ComponentClickEvent() {}
	virtual bool OnMouse() = 0;		//マウスがオブジェクトと重なっている
	bool OnHit();	//マウスがオブジェクトと重なっている状態で左クリックが押された
	bool Released();	//マウスがオブジェクトと重なっている状態でクリックされ、任意の場所で離された
	bool OnClicked();		//マウスがオブジェクトを左クリックした

	bool OnHitRight();	//マウスがオブジェクトと重なっている状態で左クリックが押された
	bool ReleasedRight();	//マウスがオブジェクトと重なっている状態でクリックされ、任意の場所で離された
	bool OnClickedRight();	//マウスがオブジェクトを右クリックした

	bool OuterHit();
	bool OuterClicked();

	bool OuterHitRight();
	bool OuterClickedRight();

	bool LeftHit();		//マウス左ボタンが任意の場所で押された
	bool RightHit();	//マウス右ボタンが任意の場所で押された
	Vector2 GetWorldPos();	//マウスのゲーム座標を取得
	void Update(float deltaTime);
private:
	Counter mClickTime;	//クリックの判定時間
	Counter mRightClickTime;	//クリックの判定時間
	Counter mOuterClickTime;
	Counter mOuterRightClickTime;
};

//オブジェクトの中心から一定範囲
class ComponentClickEventCircle : public ComponentClickEvent
{
public:
	ComponentClickEventCircle(AbstractObject* owner, float radius);
	~ComponentClickEventCircle() {}
	bool OnMouse() override;
private:
	float mRadius;
};

//オブジェクトの当たり判定を利用する
//範囲はコリジョンと同じ矩形
class ComponentClickEventObject : public ComponentClickEvent
{
public:
	ComponentClickEventObject(AbstractObject* owner);
	~ComponentClickEventObject() {}
	bool OnMouse() override;
};

class ComponentClickEventRectUI : public ComponentClickEvent
{
public:
	ComponentClickEventRectUI(AbstractObject* owner);
	~ComponentClickEventRectUI() {}
	bool OnMouse() override;
};

class ComponentClickEventCircleUI : public ComponentClickEvent
{
public:
	ComponentClickEventCircleUI(AbstractObject* owner, float radius);
	~ComponentClickEventCircleUI() {}
	bool OnMouse() override;
private:
	float mRadius;
};