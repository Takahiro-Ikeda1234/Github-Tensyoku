#pragma once
#include"Object.h"
#include"UIObject.h"
#include<vector>
#include"SceneEditor.h"
class UIObjectIcon;
class ComponentClickEvent;

class UIEditorObjectWindow : public UIBox
{
public:
	UIEditorObjectWindow(SceneEditor* scene, float xPos, float yPos, float xSize, float ySize);
	~UIEditorObjectWindow() { }
	void UpdateUIObject(float deltaTime) override;
	void CreateObject(int id, int layer, int indexX, int indexY, int ch);
	int GetCurrentLayer() { return mCurrentLayer; }
	int GetCurrentTab() { return mCurrentTab; }
private:
	void ChangeLayer(int layerID);
	void ChangeTab(int tabID);
	int mCurrentTab;	//0-3 オブジェクト,背景(壁),小物,前景
	int mCurrentLayer;	//0-4 オブジェクト,小物(2-3,値が小さいほど手前)
	UIRectSwitch* mTabButton[EDITOR_TAB_MAX];
	UIRectSwitch* mLayerButton[EDITOR_LAYER_NUM];
	UIObjectIcon* mObjectIcons[EDITOR_TAB_MAX][OBJECT_ID_MAX];
	int mCount[EDITOR_TAB_MAX];
	float mXPos;
	float mYPos;
	float mXSize;
	float mYSize;
	float mIconSize;
	float mIconOffset;
	float mIconDistance;
};

class UIObjectIcon : public UIRectButton
{
public:
	UIObjectIcon(Scene* scene, float xPos, float yPos, float xSize, float ySize, TextureTag tag, int drawPriority, const ObjectData* data);
	virtual ~UIObjectIcon() {}
	virtual void UpdateUIObject(float deltaTime) override;
private:
	const ObjectData* const mData;
	class ComponentUISprite* mUISpritePrime[4];
};

class EditorCursor : public Object
{
public:
	EditorCursor(Scene* scene, TextureTag tag, const ObjectData* data);
	virtual ~EditorCursor() {}
	virtual void UpdateObject(float deltaTime) override;
private:
	ComponentClickEvent* mClick;
	const ObjectData* mData;
	int mTab;
	int mBrushSize = 0;
	int mCh = 0;
};