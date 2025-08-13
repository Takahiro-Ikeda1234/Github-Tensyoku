//=============================================================================
//
// [Component.cpp] コンポーネント共通処理
//
//=============================================================================

//インクルードファイル
#include"Scene.h"
#include"AbstractObject.h"
#include"Component.h"

Component::Component(AbstractObject* owner, int updatePriority)
	: mOwner(owner)
	, mUpdatePriority(updatePriority)
{
	//ownerのコンポーネントに追加
	mOwner->AddComponent(this);
}

Component::~Component()
{
	//ownerのコンポーネント一覧から除去
	mOwner->RemoveComponent(this);
}
