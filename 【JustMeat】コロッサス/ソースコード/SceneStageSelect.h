//=============================================================================
//
// [SceneTitle.h] �^�C�g�����
//
//=============================================================================
#ifndef _SCENE_STAGESELECT_H_
#define _SCENE_STAGESELECT_H_

//�C���N���[�h�t�@�C��
#include"Scene.h"
#include"Counter.h"
#include"TextureData.h"

//�N���X��`
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