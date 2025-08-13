//=============================================================================
//
// [SceneTitle.h] �^�C�g�����
//
//=============================================================================
#ifndef _SCENE_TITLE_H_
#define _SCENE_TITLE_H_

//�C���N���[�h�t�@�C��
#include"Scene.h"
#include"Counter.h"
#include"TextureData.h"

//�}�N����`

//�N���X��`
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