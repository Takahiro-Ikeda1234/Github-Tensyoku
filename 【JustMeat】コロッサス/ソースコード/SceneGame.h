//=============================================================================
//
// [SceneGame.h] �Q�[�����
//
//=============================================================================
#ifndef _SCENE_GAME_H_
#define _SCENE_GAME_H_

//�C���N���[�h�t�@�C��
#include"Scene.h"
#include"Counter.h"
#include"TextureData.h"

//�N���X��`
class SceneGame : public Scene
{
public:
	SceneGame(class SceneManager* sceneManager, Parameter& param);
	virtual ~SceneGame();
	virtual void Update(float deltaTime) override;
	virtual void DrawUI() override;
	void UpdateManagers(float deltaTime)override;
	void NextStage();
protected:
	Counter mScrollStay;
	class UIGame* mUIGame;
	class StagePicture* mStagePicture[4];
};

#endif