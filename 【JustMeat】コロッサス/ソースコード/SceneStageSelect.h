//=============================================================================
//
// [SceneTitle.h] タイトル画面
//
//=============================================================================
#ifndef _SCENE_STAGESELECT_H_
#define _SCENE_STAGESELECT_H_

//インクルードファイル
#include"Scene.h"
#include"Counter.h"
#include"TextureData.h"

//クラス定義
class SceneStageSelect : public Scene
{
public:
	SceneStageSelect(class SceneManager* sceneManager, Parameter& param);
	~SceneStageSelect();
	void Update(float deltaTime) override;
	void DrawUI() override;
private:
	class UIStageSelect* mUIStageSelect;
};
#endif