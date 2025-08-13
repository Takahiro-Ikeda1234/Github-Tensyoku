//=============================================================================
//
// [obj_prop.h] 飾りオブジェクト
//
//=============================================================================
#ifndef _OBJ_PROP_H_
#define _OBJ_PROP_H_

//インクルードファイル
#include"Object.h"
#include<string>
#include"TextureData.h"
class Scene;
class SceneEditor;
class ComponentSprite;
class ComponentClickEvent;

//クラス定義
class Prop : public Object
{
public:
	Prop(Scene* scene, int indexX, int indexY, int drawPriority, int id);
	Prop(Scene* scene, TextureTag tag, int indexX, int indexY, int scaleX, int scaleY, int drawPriority, float animSpeed = 1);
	virtual ~Prop() {}
	virtual void UpdateObject(float deltaTime) override {}
};
class EditorProp : public Prop
{
public:
	EditorProp(SceneEditor* scene, TextureTag tag, int indexX, int indexY, int scaleX, int scaleY, int drawPriority, int layer, int id, int ch);
	virtual ~EditorProp() {}
	virtual void UpdateObject(float deltaTime) override;
	int GetLayer() { return mLayer; }
	void Offset(int indexX, int indexY);
private:
	int mLayer;
	int mIndexX;
	int mIndexY;
	ComponentClickEvent* mClick;
	class ComponentAnimationSprite* mCASpritePrime[4];
	int mID;
	bool mUseCh;
	int mCh = 0;

};
#endif