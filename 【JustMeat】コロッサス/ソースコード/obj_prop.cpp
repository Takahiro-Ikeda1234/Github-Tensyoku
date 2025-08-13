#include "obj_prop.h"
#include "stageManager.h"
#include "ComponentAnimationSprite.h"
#include "ComponentClickEvent.h"
#include "SceneEditor.h"
#include "mouse.h"
#include "keyboard.h"
Prop::Prop(Scene* scene, TextureTag tag, int indexX, int indexY, int scaleX, int scaleY, int drawPriority, float animSpeed)
	: Object(scene, StageManager::ToPosition(indexX, indexY))
{
	mCASprite = new ComponentAnimationSprite(this, tag, LAYER_OBJECT - 1 + 2 * (5 - drawPriority));
	mCASprite->SetAnimation(0, 0, TextureSystem::Get()->GetDivX(tag), animSpeed * randomRange(0.9f, 1.1f));
	if (scene->GetSceneManager()->GetSceneID() == SceneManager::SceneID::eGame)
	{
		mCASprite->SetScale(1.02f, 1.02f);
	}
}

Prop::Prop(Scene * scene, int indexX, int indexY, int drawPriority, int id)
	: Prop(scene, OBJECT_DATA[id].texTag, indexX, indexY, OBJECT_DATA[id].xSize, OBJECT_DATA[id].ySize, drawPriority, OBJECT_DATA[id].animSpeed)
{
	SetSize(Vector2(OBJECT_DATA[id].xSize, OBJECT_DATA[id].ySize) * BLOCKSIZE);
}

bool IsSameEdtiorCh(SceneEditor* scene, int ch, int layer, int indexX, int indexY)
{
	if (indexX < 0)return true;
	if (indexY < 0)return true;
	if (indexX >= scene->GetStageManager()->GetWidth())return true;
	if (indexY >= scene->GetStageManager()->GetHeight())return true;

	int id = scene->GetStageChData(layer, indexX, indexY);
	if (id == -1)return true;
	if (id != ch)return false;
	return true;
}

static SceneEditor::StageObjectData g_CloudAround[EDITOR_LAYER_NUM][21][21];

EditorProp::EditorProp(SceneEditor* scene, TextureTag tag, int indexX, int indexY, int scaleX, int scaleY, int drawPriority, int layer, int id, int ch)
	: Prop(scene, tag, indexX, indexY, scaleX, scaleY, drawPriority)
	, mLayer(layer)
	, mIndexX(indexX)
	, mIndexY(indexY)
	, mCh(ch)
	, mID(id)
{
	SetObjectType(Type::eEditorProp);
	mClick = new ComponentClickEventObject(this);
	mCASprite->SetAnimation(0, 0, 1, 1);
	mCASprite->SetStringInfo(3, GetPosition(), Vector2(16, 16), true);
	((SceneEditor*)GetScene())->SetStageData(id, layer, indexX, indexY);

	if (OBJECT_DATA[id].prime & 0b0001) mCASpritePrime[0] = new ComponentAnimationSprite(this, TextureTag::Prime1, drawPriority - 1);
	if (OBJECT_DATA[id].prime & 0b0010) mCASpritePrime[1] = new ComponentAnimationSprite(this, TextureTag::Prime2, drawPriority - 1);
	if (OBJECT_DATA[id].prime & 0b0100) mCASpritePrime[2] = new ComponentAnimationSprite(this, TextureTag::Prime3, drawPriority - 1);
	if (OBJECT_DATA[id].prime & 0b1000) mCASpritePrime[3] = new ComponentAnimationSprite(this, TextureTag::Prime4, drawPriority - 1);

	mUseCh = OBJECT_DATA[id].useCh;
	if (mUseCh)
	{
		if (ch == 0)
		{
			mCh = ((SceneEditor*)GetScene())->GetStageChData(layer, indexX, indexY);
			if (mCh < 0)mCh = 0;
		}

		char buff[5];
		sprintf(buff, "%d", mCh);
		mCASprite->SetString(buff);
	}


	if (OBJECT_DATA[mID].texTag == TextureTag::CloudGeneratorBottom || OBJECT_DATA[mID].texTag == TextureTag::CloudGeneratorTop)
	{
		mCASpritePrime[0] = new ComponentAnimationSprite(this, TextureTag::Plane);
		mCASpritePrime[0]->SetAlphaRate(0.2f);
		mCASpritePrime[0]->SetScale(21, 21);
	}
}

void EditorProp::UpdateObject(float deltaTime)
{
	if (mLayer == ((SceneEditor*)GetScene())->GetCurrentLayer())
	{
		mCASprite->SetAlpha(0x7F);
		mCASprite->SetStringAlphaRate(1);
		if (mClick->OnMouse() && Mouse::Inst()->MouseDown(MOUSE_RIGHT))
		{
			SetState(State::eDead);
			((SceneEditor*)GetScene())->SetStageData(-1, mLayer, mIndexX, mIndexY);
		}

		if (mUseCh)
		{
			if (mClick->OnMouse() && Mouse::Inst()->MouseHit(MOUSE_LEFT))
			{
				mCh++;
				if (mCh >= MAX_OBJECT_CH) mCh = MAX_OBJECT_CH - 1;
				char buff[5];
				sprintf(buff, "%d", mCh);
				mCASprite->SetString(buff);
				((SceneEditor*)GetScene())->SetStageChData(mCh, mLayer, mIndexX, mIndexY);
			}
			if (mClick->OnMouse() && Mouse::Inst()->MouseHit(MOUSE_MIDDLE))
			{
				mCh--;
				if (mCh < 0)mCh = 0;
				char buff[5];
				sprintf(buff, "%d", mCh);
				mCASprite->SetString(buff);
				((SceneEditor*)GetScene())->SetStageChData(mCh, mLayer, mIndexX, mIndexY);
			}


			((SceneEditor*)GetScene())->SetStageChData(mCh, mLayer, mIndexX, mIndexY);
			if (!IsSameEdtiorCh((SceneEditor*)GetScene(), mCh, mLayer, mIndexX + 1, mIndexY + 0) ||
				!IsSameEdtiorCh((SceneEditor*)GetScene(), mCh, mLayer, mIndexX - 1, mIndexY + 0) ||
				!IsSameEdtiorCh((SceneEditor*)GetScene(), mCh, mLayer, mIndexX + 0, mIndexY + 1) ||
				!IsSameEdtiorCh((SceneEditor*)GetScene(), mCh, mLayer, mIndexX + 0, mIndexY - 1))
			{
				mCASprite->SetStringColor(0xFFFF00FF);
			}
			else
			{
				mCASprite->SetStringColor(0xFFFFFFFF);
			}
		}
	}
	else
	{
		mCASprite->SetAlpha(0x4F);
		if (((SceneEditor*)GetScene())->GetCurrentLayer() > EDITOR_TAB_GOLEM)
		{
			mCASprite->SetStringAlphaRate(0);
		}
		else
		{
			mCASprite->SetStringAlphaRate(1);
		}
		if (mLayer == EDITOR_TAB_GOLEM + 1)
		{
			mCASprite->SetAlpha(0x00);
			mCASprite->SetStringAlphaRate(0);
		}
		if (mLayer == EDITOR_TAB_MESSAGE + 1)
		{
			mCASprite->SetAlpha(0x00);
			mCASprite->SetStringAlphaRate(0);
		}
	}



	for (int i = 0; i < 4; i++)
	{
		if (mCASpritePrime[i])
		{
			mCASpritePrime[i]->SetAlpha(mCASprite->GetAlpha());
		}
	}

	//if (OBJECT_DATA[mID].texTag == TextureTag::CloudGeneratorTop && Keyboard::Inst()->KeyHit(DIK_F2))
	//{
	//	for (int i = 0; i < EDITOR_LAYER_NUM; i++)
	//	{
	//		for (int x = -10; x <= 10; x++ )
	//		{
	//			for (int y = -10; y <= 10; y++)
	//			{
	//				int id = ((SceneEditor*)GetScene())->GetStageData(i, mIndexX + x, mIndexY + y);
	//				int ch = ((SceneEditor*)GetScene())->GetStageChData(i, mIndexX + x, mIndexY + y);
	//				if (id == ID_CLOUD_GENERATOR_TOP)
	//				{
	//					id = ID_CLOUD_GENERATOR_BOTTOM;
	//				}
	//				g_CloudAround[i][y + 10][x + 10] = { id, ch };
	//			}
	//		}
	//	}
	//}

	//if (OBJECT_DATA[mID].texTag == TextureTag::CloudGeneratorBottom && Keyboard::Inst()->KeyHit(DIK_F3))
	//{
	//	for (int i = 0; i < EDITOR_LAYER_NUM; i++)
	//	{
	//		for (int x = -10; x <= 10; x++)
	//		{
	//			for (int y = -10; y <= 10; y++)
	//			{
	//				((SceneEditor*)GetScene())->SetStageData(g_CloudAround[i][y + 10][x + 10].id, i, mIndexX + x, mIndexY + y);
	//				((SceneEditor*)GetScene())->SetStageChData(g_CloudAround[i][y + 10][x + 10].ch, i, mIndexX + x, mIndexY + y);
	//			}
	//		}
	//	}
	//}


	//if (OBJECT_DATA[mID].texTag == TextureTag::CloudGeneratorBottom && Keyboard::Inst()->KeyHit(DIK_F5))
	//{
	//	for (int i = 0; i < EDITOR_LAYER_NUM; i++)
	//	{
	//		for (int x = -10; x <= 10; x++)
	//		{
	//			for (int y = -10; y <= 10; y++)
	//			{
	//				if (((SceneEditor*)GetScene())->GetStageData(i, mIndexX + x, mIndexY + y) == ID_CLOUD_GENERATOR_BOTTOM)continue;
	//				((SceneEditor*)GetScene())->SetStageData(-1, i, mIndexX + x, mIndexY + y);
	//				((SceneEditor*)GetScene())->SetStageChData(-1, i, mIndexX + x, mIndexY + y);
	//			}
	//		}
	//	}
	//}
}

void EditorProp::Offset(int indexX, int indexY)
{
	mIndexX += indexX;
	mIndexY += indexY;
	Translate(BLOCKSIZE * Vector2(indexX, indexY));
	int w = ((SceneEditor*)GetScene())->GetWidth();
	int h = ((SceneEditor*)GetScene())->GetHeight();
	if (mIndexX < 0 || mIndexY < 0 || mIndexX >= w || mIndexY >= h)
	{
		SetState(State::eDead);
	}
}
