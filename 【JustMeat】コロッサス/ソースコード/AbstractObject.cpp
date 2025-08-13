//=============================================================================
//
// [Object.cpp] オブジェクト共通処理
//
//=============================================================================

//インクルードファイル
#include"Scene.h"
#include<vector>
#include"Component.h"
#include"AbstractObject.h"
#include"config.h"
// ---------------------------------------------------------------------------
// 関数名: Object::Object(Scene* scene)
// 引数:   Scene* scene シーンのポインタ
// 説明:   AbstractObjectのコンストラクタ
// ---------------------------------------------------------------------------
AbstractObject::AbstractObject(Scene* scene)
	: mScene(scene)
	, mPosition(Vector2(0, 0))
	, mRotationDeg(0)
	, mSize(Vector2(0, 0))
	, mColSize(Vector2(0, 0))
	, mActiveRange(0)
	, mScale(Vector2(1, 1))
	, mState(State::eActive)
	, mObjectType(Type::eNone)
	, mDir(Dir::eRight)
{
	SetSize(Vector2(BLOCKSIZE, BLOCKSIZE));
}
// ---------------------------------------------------------------------------
// 関数名: AbstractObject::~AbstractObject()
// 説明:   AbstractObjectのデストラクタ
// ---------------------------------------------------------------------------
AbstractObject::~AbstractObject()
{
	//全部なくなるまでコンポーネントのdelete
	while (!mComponents.empty())
	{
		delete mComponents.back();
	}
}
// ---------------------------------------------------------------------------
// 関数名: void AbstractObject::Update(float deltaTime)
// 引数:   float deltaTime 
// 説明:   更新処理
// ---------------------------------------------------------------------------
void AbstractObject::Update(float deltaTime)
{
	if (mState == State::eActive)
	{
		mPrevPos = mPosition;
		UpdateComponents(deltaTime);
		if (!GetScene()->IsPause() || mResistPause)
		{
			UpdateObject(deltaTime);
		}
		if (deltaTime != 0) mMoved = (mPosition - mPrevPos) / deltaTime;
	}
}
// ---------------------------------------------------------------------------
// 関数名: void AbstractObject::UpdateComponents(float deltaTime)
// 引数:   float deltaTime 
// 説明:   コンポーネントの更新
// ---------------------------------------------------------------------------
void AbstractObject::UpdateComponents(float deltaTime)
{
	for (auto it : mComponents)
	{
		if (!GetScene()->IsPause() || it->GetResistPause())
		{
			it->Update(deltaTime);
		}
	}
}
// ---------------------------------------------------------------------------
// 関数名: void AbstractObject::AddComponent(Component* component)
// 引数:   Component* component 
// 説明:   コンポーネントの追加
// ---------------------------------------------------------------------------
void AbstractObject::AddComponent(Component* component)
{
	int priority = component->GetUpdatePriority();
	auto itr = mComponents.begin();
	for (; itr != mComponents.end(); ++itr)
	{
		if (priority < (*itr)->GetUpdatePriority())
		{
			break;
		}
	}
	mComponents.insert(itr, component);
}
// ---------------------------------------------------------------------------
// 関数名: void AbstractObject::RemoveComponent(Component* component)
// 引数:   Component* component 
// 説明:   コンポーネントの削除
// ---------------------------------------------------------------------------
void AbstractObject::RemoveComponent(Component* component)
{
	auto itr = std::find(mComponents.begin(), mComponents.end(), component);
	if (itr != mComponents.end())
	{
		mComponents.erase(itr);
	}
}