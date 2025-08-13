//=============================================================================
//
// [Object.cpp] オブジェクト共通処理
//
//=============================================================================

//インクルードファイル
#include"Scene.h"
#include<vector>
#include"Object.h"
#include"Component.h"
#include"ComponentAnimationSprite.h"
// ---------------------------------------------------------------------------
// 関数名: Object::Object(Scene * scene, Vector2 position)
// 引数:   Scene* scene シーンのポインタ
// 引数:   Vector2 position 生成位置
// 説明:   Objectのコンストラクタ
// ---------------------------------------------------------------------------
Object::Object(Scene * scene, Vector2 position)
	: AbstractObject(scene)
	, mCASprite(nullptr)
	, mIsForeground(false)
{
	GetScene()->AddObject(this);
	SetPosition(position);
}
// ---------------------------------------------------------------------------
// 関数名: Object::~Object()
// 説明:   Objectのデストラクタ
// ---------------------------------------------------------------------------
Object::~Object()
{
	//シーンから自身をRemove
	GetScene()->RemoveObject(this);
}

void Object::SetDrawMode(int drawMode)
{
	if (mCASprite)
	{
		mCASprite->SetDrawMode((ComponentSprite::DrawMode)drawMode);
	}
}

void Object::SetColor(unsigned color)
{
	if (mCASprite)
	{
		mCASprite->SetColor(color);
	}
}

void Object::SceneChangeEvent()
{
	SetState(State::eDead);
}

