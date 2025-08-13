#include "ui_editorObjectWindow.h"
#include "SceneEditor.h"
#include "Windows_core.h"
#include "StageManager.h"
#include "obj_prop.h"
#include "mouse.h"
#include"keyboard.h"
#include "CameraManager.h"
#include "ComponentClickEvent.h"
#include "ComponentAnimationSprite.h"
#include "ComponentUISprite.h"
UIEditorObjectWindow::UIEditorObjectWindow(SceneEditor * scene, float xPos, float yPos, float xSize, float ySize)
	: UIBox(scene, xPos, yPos, xSize, ySize, TextureTag::EditorObjectWindow, 5)
	, mXPos(xPos)
	, mYPos(yPos)
	, mXSize(xSize)
	, mYSize(ySize)
	, mIconSize(xSize * 2 * 0.18f)
	, mIconOffset(xSize * 2 * 0.13f)
	, mIconDistance(xSize * 2 * 0.10f)
	, mCurrentTab(EDITOR_TAB_MAX - 1)
	, mCurrentLayer(EDITOR_LAYER_NUM - 1)
{
	float x = xPos - xSize;
	float y = yPos - ToWidthSize(ySize);
	for (int i = 0; i < OBJECT_ID_MAX; i++)
	{
		if (OBJECT_DATA[i].id == ID_NOUSE) continue;
		int index = OBJECT_DATA[i].layer;

		mObjectIcons[index][mCount[index]] = new UIObjectIcon(GetScene(), x, y, mIconSize / 2, mIconSize / 2, OBJECT_DATA[i].texTag, 4, &OBJECT_DATA[i]);
		mObjectIcons[index][mCount[index]]->SetInisible();
		mObjectIcons[index][mCount[index]]->Translate(GetWindowWidth() * (mIconOffset + mIconSize / 2 + ((mCount[index] % 3) * (mIconSize + mIconDistance))), 0);
		mObjectIcons[index][mCount[index]]->Translate(0, GetWindowWidth() * (mIconOffset + mIconSize / 2 + ((mCount[index] / 3) * (mIconSize + mIconDistance))));
		mCount[index]++;
	}
	for (int i = 0; i < EDITOR_TAB_MAX; i++)
	{
		mTabButton[i]= new UIRectSwitch(GetScene(), x, y, xSize * 2 * 0.15f / 2, xSize * 2 * 0.10f, TextureTag::Banner, 6);
		mTabButton[i]->Translate(GetWindowWidth() * (xSize * 2 * 0.05f + xSize * 2 * 0.15f * (i + 0.5f)), 0);
	}
	for (int i = 0; i < EDITOR_LAYER_NUM; i++)
	{
		mLayerButton[i] = new UIRectSwitch(GetScene(), 0.1f, 0.98f, xSize * 2 * 0.30f / 2, xSize * 2 * 0.10f, TextureTag::Banner, 6);
		mLayerButton[i]->Translate(GetWindowWidth() * (xSize * 2 * 0.05f + xSize * 2 * 0.30f * (i + 0.5f)), 0);
	}
	ChangeTab(0);
	ChangeLayer(0);
}

void UIEditorObjectWindow::UpdateUIObject(float deltaTime)
{
	for (int i = 0; i < EDITOR_TAB_MAX; i++)
	{
		if (i == mCurrentTab) continue;
		if (mTabButton[i]->IsSwitchActive())
			ChangeTab(i);
	}
	for (int i = 0; i < EDITOR_LAYER_NUM; i++)
	{
		if (i == mCurrentLayer) continue;
		if (mLayerButton[i]->IsSwitchActive())
			ChangeLayer(i);
	}

	bool mHitPageDown = Keyboard::Inst()->KeyHit(DIK_PRIOR);
	bool mHitPageUp = Keyboard::Inst()->KeyHit(DIK_NEXT);
	bool mHitLeft = Keyboard::Inst()->KeyHit(DIK_HOME);
	bool mHitRight = Keyboard::Inst()->KeyHit(DIK_END);

	if ((mHitPageDown || mHitPageUp) && !(mHitPageDown && mHitPageUp))
	{
		auto editorObjects = GetScene()->FindObjects(nullptr, Object::Type::eEditorProp);
		for (auto it : editorObjects)
		{
			((EditorProp*)it)->Offset(0, -mHitPageDown + mHitPageUp);
		}
		((SceneEditor*)GetScene())->OffsetStageData(0, -mHitPageDown + mHitPageUp);
	}

	if ((mHitLeft || mHitRight) && !(mHitLeft && mHitRight))
	{
		auto editorObjects = GetScene()->FindObjects(nullptr, Object::Type::eEditorProp);
		for (auto it : editorObjects)
		{
			((EditorProp*)it)->Offset(-mHitLeft + mHitRight, 0);
		}
		((SceneEditor*)GetScene())->OffsetStageData(-mHitLeft + mHitRight, 0);
	}

	if (Mouse::Inst()->GetWheelMove() >= 120 && Keyboard::Inst()->KeyDown(DIK_LSHIFT))
	{
		for (int i = 0; i < mCount[mCurrentTab]; i++)
		{
			mObjectIcons[mCurrentTab][i]->Translate(Vector2(0, BLOCKSIZE * 0.5f));
		}
	}
	if (Mouse::Inst()->GetWheelMove() <= -120 && Keyboard::Inst()->KeyDown(DIK_LSHIFT))
	{
		for (int i = 0; i < mCount[mCurrentTab]; i++)
		{
			mObjectIcons[mCurrentTab][i]->Translate(Vector2(0, -BLOCKSIZE * 0.5f));
		}
	}
}

void UIEditorObjectWindow::CreateObject(int id, int layer, int indexX, int indexY, int ch)
{
	new EditorProp((SceneEditor*)GetScene(), OBJECT_DATA[id].texTag, indexX, indexY, OBJECT_DATA[id].xSize, OBJECT_DATA[id].ySize, -1 + layer * 2, layer, id, ch);
}

void UIEditorObjectWindow::ChangeLayer(int layerID)
{
	if (mCurrentLayer == layerID) return;

	if (mCurrentLayer != layerID)
	{
		mLayerButton[mCurrentLayer]->SetSwitchActive(false);
		mCurrentLayer = layerID;
	}
	mLayerButton[mCurrentLayer]->SetSwitchActive(true);
	if(mCurrentLayer != mCurrentTab)
	{
		if (layerID >= 3)
			ChangeTab(layerID - 1);
		else
			ChangeTab(mCurrentLayer);
	}	
}

void UIEditorObjectWindow::ChangeTab(int tabID)
{
	if (mCurrentTab == tabID) return;

	if (tabID >= EDITOR_TAB_MAX) tabID = EDITOR_TAB_MAX - 1;
	if (mCurrentTab != tabID)
	{
		mTabButton[mCurrentTab]->SetSwitchActive(false);
		for (int i = 0; i < mCount[mCurrentTab]; i++)
		{
			mObjectIcons[mCurrentTab][i]->SetInisible();
		}
		mCurrentTab = tabID;
	}
	mTabButton[mCurrentTab]->SetSwitchActive(true);
	for (int i = 0; i < mCount[mCurrentTab]; i++)
	{
		mObjectIcons[mCurrentTab][i]->SetVisible();
	}

	if (tabID >= 3)
	{
		ChangeLayer(tabID + 1);
	}
	else if (!(tabID == 2 && mCurrentLayer == 3))
	{
		ChangeLayer(tabID);
	}
}

UIObjectIcon::UIObjectIcon(Scene * scene, float xPos, float yPos, float xSize, float ySize, TextureTag tag, int drawPriority, const ObjectData * data)
	: UIRectButton(scene, xPos, yPos, xSize, ySize, tag, drawPriority)
	, mData(data)
{
	mUISprite->SetStringColor(0xFF000000);
	mUISprite->SetString(data->name);
	if (data->prime & 0b0001) mUISpritePrime[0] = new ComponentUISprite(this, TextureTag::Prime1, drawPriority - 1);
	if (data->prime & 0b0010) mUISpritePrime[1] = new ComponentUISprite(this, TextureTag::Prime2, drawPriority - 1);
	if (data->prime & 0b0100) mUISpritePrime[2] = new ComponentUISprite(this, TextureTag::Prime3, drawPriority - 1);
	if (data->prime & 0b1000) mUISpritePrime[3] = new ComponentUISprite(this, TextureTag::Prime4, drawPriority - 1);
}

void UIObjectIcon::UpdateUIObject(float deltaTime)
{
	mUISprite->SetStringInfo(10, GetPosition() + Vector2(0, GetWindowWidth()*0.03f), Vector2(12, 12), true);
	if (mClick->OnClicked())
	{
		((SceneEditor*)GetScene())->CreateCursor(new EditorCursor(GetScene(), mData->texTag, mData));
	}
}

EditorCursor::EditorCursor(Scene * scene, TextureTag tag, const ObjectData* data)
	: Object(scene, scene->GetCameraManager()->ToWorldPosition(Mouse::Inst()->GetPos()))
	, mData(data)
	, mTab(((SceneEditor*)scene)->GetCurrentTab())
{
	mCASprite = new ComponentAnimationSprite(this, tag, 0);
	mClick = new ComponentClickEventObject(this);
	mCASprite->SetAlpha(0x4F);
	UpdateObject(0);

	if (mData->useCursorCh)
	{
		mCh = 1;
		char buff[5];
		sprintf(buff, "%d", mCh);
		mCASprite->SetString(buff);
	}
}

void EditorCursor::UpdateObject(float deltaTime)
{
	SetPosition(GetScene()->GetStageManager()->AdjustPos(GetScene()->GetCameraManager()->ToWorldPosition(Mouse::Inst()->GetPos())));
	mCASprite->SetStringInfo(3, GetPosition(), Vector2(16, 16), true);
	if (mClick->OnMouse() && Mouse::Inst()->MouseDown(MOUSE_LEFT) > 0&& Mouse::Inst()->GetPosX() < GetWindowWidth()* 0.79f)
	{
		//エディタで生成
		int idxx = GetScene()->GetStageManager()->ToIndexX(GetPosition().x);
		int idxy = GetScene()->GetStageManager()->ToIndexY(GetPosition().y);
		for (int i = -mBrushSize; i <= mBrushSize; i++)
		{
			for (int j = -mBrushSize; j <= mBrushSize; j++)
			{
				((SceneEditor*)GetScene())->CreateEditorObject(mData->id, idxx + i, idxy + j, mCh);
			}
		}
	}
	if (mClick->OnHitRight())
	{
		((SceneEditor*)GetScene())->CreateCursor(nullptr);
	}
	if (((SceneEditor*)GetScene())->GetCurrentTab() != mTab)
	{
		((SceneEditor*)GetScene())->CreateCursor(nullptr);
	}
	if (Keyboard::Inst()->KeyHit(DIK_DOWN))
	{
		if (mBrushSize > 0)
			mBrushSize--;
	}
	if (Keyboard::Inst()->KeyHit(DIK_UP))
	{
		if (mBrushSize < 5)
			mBrushSize++;
	}

	if (Keyboard::Inst()->KeyHit(DIK_RIGHT) && mData->useCursorCh)
	{
		mCh++;
		if (mCh >= MAX_OBJECT_CH)
			mCh = MAX_OBJECT_CH - 1;

		char buff[5];
		sprintf(buff, "%d", mCh);
		mCASprite->SetString(buff);
	}
	if (Keyboard::Inst()->KeyHit(DIK_LEFT) && mData->useCursorCh)
	{
		mCh--;
		if (mCh < 1)
			mCh = 1;

		char buff[5];
		sprintf(buff, "%d", mCh);
		mCASprite->SetString(buff);
	}
}
