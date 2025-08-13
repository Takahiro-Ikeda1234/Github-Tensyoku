//=============================================================================
//
// [SceneManager.h] �V�[���J�ڊǗ�
//
//=============================================================================
#ifndef _SCENE_MANAGER_H_
#define _SCENE_MANAGER_H_

//�C���N���[�h�t�@�C��
#include <string>
#include"Parameter.h"

//�N���X��`
class SceneManager
{
public:
	enum SceneID	//�V�[��
	{
		eDefault,	//�J�ڂȂ�
		eExit,
		eTitle,
		eGame,
		eStageSelect,
		eEditor,
		eOpening,
		eEnding,
		eCredit,
		eFin,
		eLogo,
	};
	SceneManager();
	~SceneManager();
	bool SceneChange(SceneID nextScene, Parameter* param, float fadeoutDelay, float fadeoutTime, float fadeinTime, bool whiteFade = false);	//�V�[���ύX�̗v���B���ۂ̃V�[���ύX�̓t���[���̏����̍Ō�Ɏ��s�����
	bool CanSceneChange() const;
	SceneID GetNextScene() { return mNextScene; }
	SceneID GetSceneID() { return mCurrentScene; }
	void LoadScene(SceneID id);
	void UnloadScene();
	void Update(float deltaTime);
	void Draw();
	class Scene* GetScene() { return mScene; }
	void ChangeShowUI() { mShowUI = !mShowUI; }
	bool GetShowUI() { return mShowUI; }
	//bool GetDebugMode() { return mDebugMode; }
	bool GetDebugMode() { return false; }
private:
	SceneID mNextScene;	//���V�[��
	SceneID mCurrentScene;	//���݃V�[��
	Parameter mNextParam;	//���V�[���ɓn���p�����[�^
	class Scene* mScene;	//�V�[���̃|�C���^
	bool mShowUI;	//UI��\�����邩�ǂ���
	virtual void DrawUI() {}
	bool mDebugMode;
	bool mWhiteFade = false;
};
#endif