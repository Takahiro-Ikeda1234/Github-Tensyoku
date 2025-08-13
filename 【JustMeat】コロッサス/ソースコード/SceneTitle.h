//=============================================================================
//
// [SceneTitle.h] タイトル画面
//
//=============================================================================
#ifndef _SCENE_TITLE_H_
#define _SCENE_TITLE_H_

//インクルードファイル
#include"Scene.h"
#include"Counter.h"
#include"TextureData.h"

//マクロ定義

//クラス定義
class SceneTitle : public Scene
{
public:
	SceneTitle(class SceneManager* sceneManager, Parameter& param);
	~SceneTitle();
	void Update(float deltaTime) override;
	void DrawUI() override;
private:
	class UITitle* mUITitle;
};
#endif