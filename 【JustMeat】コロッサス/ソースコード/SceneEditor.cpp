#include "SceneEditor.h"
#include "UIObject.h"
#include"SceneManager.h"
#include"ui_editorObjectWindow.h"
#include"StageManager.h"
#include"CameraManager.h"
#include"draw.h"
#include"obj_screenBackground.h"
#include"ComponentSprite.h"
#include"StageData.h"
#include"mouse.h"
#include"ComponentUISprite.h"
#include "ui_title.h"
#include"DX9_printf.h"
SceneEditor::SceneEditor(SceneManager * sceneManager, Parameter & param)
	: Scene(sceneManager)
{
	mStageID = 0;
	param.Get("ステージ", &mStageID);
	mBorderLine = new ScreenBackground(this, TextureTag::Block);
	mCameraManager->SetWASDMode(true);
	mCameraManager->SetCanZoom(true);

	StageData::Inst()->CreateEditorStage(mStageData, &mWidth, &mHeight, mStageID);
	Resize(mWidth, mHeight);
	mBorderLine->SetSize(Vector2(GetStageManager()->LimitX() * 0.999f, GetStageManager()->LimitY()) * 0.998f);
	mBorderLine->SetPosition(Vector2(GetStageManager()->LimitX() / 2, GetStageManager()->LimitY() / 2));
	mBorderLine->SetDrawMode((int)ComponentSprite::DrawMode::eLine);
	mBorderLine->SetColor(0xFF00FF00);

	mLeaveSceneButton = new UICircleButton(this, 0.05f, 0.1f, 0.025f, TextureTag::Button, 0);
	mLeaveSceneButton->GetCSprite()->SetString("戻る");
	mLeaveSceneButton->GetCSprite()->SetStringInfo(5, mLeaveSceneButton->GetPosition(), mLeaveSceneButton->GetColSize() / 2, true);
	mSaveButton = new UIRectButton(this, 0.20f, 0.1f, 0.05f, 0.0125f, TextureTag::Banner, 0);
	mMainEditor = new UIEditorObjectWindow(this, 0.89f, 0.55f, 0.10f, 0.25f);

	for (int i = 0; i < EDITOR_LAYER_NUM; i++)
	{
		for (int x = 0; x < mWidth; x++)
		{
			for (int y = 0; y < mHeight; y++)
			{
				int id = mStageData[i][x + y * mWidth].id;
				if (id != -1)
				{
					mMainEditor->CreateObject(id, i, x, y, 0);
				}

			}
		}
	}
}

SceneEditor::~SceneEditor()
{
	for (int i = 0; i < EDITOR_LAYER_NUM; i++)
	{
		delete[] mStageData[i];
	}
}

void SceneEditor::Update(float deltaTime)
{
	UpdateObjects(deltaTime);
	UpdateManagers(deltaTime);
	if (mLeaveSceneButton->Clicked())
	{
		Parameter nextParam;
		nextParam.Set("メニュー状態", (int)UITitle::TitleState::eMenuSelect);
		mSceneManager->SceneChange(SceneManager::SceneID::eTitle, &nextParam, 0, 0, 0);
	}
	if (mSaveButton->Clicked())
	{
		StageData::Inst()->SaveStageFile(mStageData, mWidth, mHeight, mStageID);
	}
}

void SceneEditor::DrawUI()
{
	char buff[50];
	Vector2 cameraPos = mCameraManager->ToWorldPosition(Mouse::Inst()->GetPos());
	sprintf(buff, "x %d : %f", StageManager::ToIndexX(cameraPos.x), cameraPos.x);
	DX9printf(50, 50, buff);
	sprintf(buff, "y %d : %f", StageManager::ToIndexY(cameraPos.y), cameraPos.y);
	DX9printf(50, 100, buff);
}

void SceneEditor::Resize(int width, int height)
{
	for (int i = 0; i < EDITOR_LAYER_NUM; i++)
	{
		SceneEditor::StageObjectData *newData = new SceneEditor::StageObjectData[width * height];
		for (int x = 0; x < width; x++)
		{
			for (int y = 0; y < height; y++)
			{
				newData[y * width + x].id = -1;
			}
		}
		for (int x = 0; x < width && x < mWidth; x++)
		{
			for (int y = 0; y < height && y < mHeight; y++)
			{
				newData[y * width + x] = mStageData[i][y * mWidth + x];
			}
		}
		delete[] mStageData[i];
		mStageData[i] = newData;
	}

	mWidth = width;
	mHeight = height;
	GetStageManager()->SetWidth(mWidth);
	GetStageManager()->SetHeight(mHeight);

	mBorderLine->SetSize(Vector2(GetStageManager()->LimitX() * 0.999f, GetStageManager()->LimitY()) * 0.998f);
}

int SceneEditor::GetCurrentLayer()
{
	return mMainEditor->GetCurrentLayer();
}

int SceneEditor::GetCurrentTab()
{
	return mMainEditor->GetCurrentTab();
}

void SceneEditor::CreateEditorObject(int id, int indexX, int indexY, int ch)
{
	if (mStageData[mMainEditor->GetCurrentLayer()][indexY * mWidth + indexX].id > -1) return;
	if (indexX < 0 || indexX >= mWidth || indexY < 0 || indexY >= mHeight) return;
	mMainEditor->CreateObject(id, mMainEditor->GetCurrentLayer(), indexX, indexY, ch);
}

void SceneEditor::CreateCursor(Object * obj)
{
	if (mCursor)
	{
		mCursor->SetState(AbstractObject::State::eDead);
	}
	mCursor = obj;
}

void SceneEditor::OffsetStageData(int offsetX, int offsetY)
{
	for (int i = 0; i < EDITOR_LAYER_NUM; i++)
	{
		SceneEditor::StageObjectData *copyStageData = new SceneEditor::StageObjectData[mWidth * mHeight];
		memcpy(copyStageData, mStageData[i], mWidth * mHeight * sizeof(StageObjectData));
		for (int y = 0; y < mHeight; y++)
		{
			for (int x = 0; x < mWidth; x++)
			{
				int idxx = x - offsetX;
				int idxy = y - offsetY;
				if (0 <= idxx && idxx < mWidth && 0 <= idxy && idxy < mHeight)
				{
					mStageData[i][y * mWidth + x] = copyStageData[idxy * mWidth + idxx];
				}
				else
				{
					mStageData[i][y * mWidth + x].id = -1;
				}
			}
		}
		delete[] copyStageData;
	}
}
