#pragma once
#include"Singleton.h"
#include<vector>
#include"file.h"
#include"SceneEditor.h"
#include"StageManager.h"
class StageManager;
class StageData : public Singleton<StageData>
{
public:
	friend Singleton<StageData>;
	StageData() : Singleton() { LoadStageList(); LoadStageTitle(); }
	void CreateStage(StageManager* stageManager, int stageID);
	void CreateEditorStage(SceneEditor::StageObjectData** editorStageData, int* width, int* height, int stageID);
	void SaveStageFile(SceneEditor::StageObjectData** editorStageData, int width, int height, int stageID);

	void StageSizeFromStageFile(int id, int* x, int* y);
public:
	void GetStageTitle(int id, std::string* title1, std::string* title2) { *title1 = mStageTitle[id][0]; *title2 = mStageTitle[id][1]; }
private:
	void LoadStageList();
	void LoadStageTitle();

	void CreateObjects(StageManager *stageManager, int id, const char str[], int layer);
	void CreateEditorObjects(SceneEditor::StageObjectData * editorStageData, int id, const char str[], int width, int height);
	void CreateObjectsCh(StageManager *stageManager, int id, const char str[], int layer);
	void CreateEditorObjectsCh(SceneEditor::StageObjectData * editorStageData, int id, const char str[], int width, int height);
	std::vector<std::string> mStageName;
	int mStageCount;
	std::string mStageTitle[STAGE_MAX][2];
};