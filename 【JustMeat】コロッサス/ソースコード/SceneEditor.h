//=============================================================================
//
// [SceneEditor.h] �G�f�B�^���
//
//=============================================================================
#ifndef _SCENE_EDITOR_H_
#define _SCENE_EDITOR_H_

//�C���N���[�h�t�@�C��
#include"Scene.h"
#include"Counter.h"
#include"TextureData.h"
class UIEditorObjectWindow;
#define EDITOR_LAYER_NUM 7
//�N���X��`
class SceneEditor : public Scene
{
public:
	SceneEditor(class SceneManager* sceneManager, Parameter& param);
	virtual ~SceneEditor();
	virtual void Update(float deltaTime) override;
	virtual void DrawUI() override;
	void Resize(int width, int height);
	int GetCurrentLayer();
	int GetCurrentTab();
	void SetStageData(int id, int layer, int indexX, int indexY) { if (!InArea(indexX, indexY))return; mStageData[layer][indexY * mWidth + indexX].id = id; }
	int GetStageData(int layer, int indexX, int indexY) { if (!InArea(indexX, indexY))return -1; return mStageData[layer][indexY * mWidth + indexX].id; }
	void SetStageChData(int ch, int layer, int indexX, int indexY) { if (!InArea(indexX, indexY))return; mStageData[layer][indexY * mWidth + indexX].ch = ch; }
	int GetStageChData(int layer, int indexX, int indexY) { if (!InArea(indexX, indexY))return -1; return mStageData[layer][indexY * mWidth + indexX].ch; }
	void CreateEditorObject(int id, int indexX, int indexY, int ch);
	void CreateCursor(Object* obj);
	int GetWidth() { return mWidth; }
	int GetHeight() { return mHeight; }
	bool InArea(int indexX, int indexY) { if (indexX < 0 || indexX >= GetWidth() || indexY < 0 || indexY >= GetHeight()) return false; return true; }
	void OffsetStageData(int offsetX, int offsetY);
	typedef struct
	{
		int id = -1;	//�I�u�W�F�N�gID
		int ch = -1;	//�ڑ�ID
	}StageObjectData;
private:
	StageObjectData *mStageData[EDITOR_LAYER_NUM];
	int mWidth;
	int mHeight;
	int mStageID;
	Object* mCursor;

	//�ȉ�UI�p
public:
private:
	Object* mBorderLine;
	//����p
	UICircleButton* mLeaveSceneButton;
	UIRectButton* mSaveButton;
	//�E�p
	UIEditorObjectWindow* mMainEditor;
};

#endif