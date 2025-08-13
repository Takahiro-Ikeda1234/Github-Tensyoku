//=============================================================================
//
// [SceneManager.h] シーン遷移管理
//
//=============================================================================
#ifndef _SCENE_MANAGER_H_
#define _SCENE_MANAGER_H_

//インクルードファイル
#include <string>
#include"Parameter.h"

//クラス定義
class SceneManager
{
public:
	enum SceneID	//シーン
	{
		eDefault,	//遷移なし
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
	bool SceneChange(SceneID nextScene, Parameter* param, float fadeoutDelay, float fadeoutTime, float fadeinTime, bool whiteFade = false);	//シーン変更の要求。実際のシーン変更はフレームの処理の最後に実行される
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
	SceneID mNextScene;	//次シーン
	SceneID mCurrentScene;	//現在シーン
	Parameter mNextParam;	//次シーンに渡すパラメータ
	class Scene* mScene;	//シーンのポインタ
	bool mShowUI;	//UIを表示するかどうか
	virtual void DrawUI() {}
	bool mDebugMode;
	bool mWhiteFade = false;
};
#endif