//=============================================================================
//
// [Scene.h] 画面共通処理
//
//=============================================================================
#ifndef _SCENE_H_
#define _SCENE_H_

//インクルードファイル
#include<vector>
#include<unordered_map>
#include<string>
#include"Vector2.h"
#include"Object.h"
#include"UIObject.h"
#include"Parameter.h"
#include"SceneManager.h"
//マクロ定義
#define PREVSCENE_KEY "Scene_Prev"


//クラス定義
class Scene
{
public:
	Scene(class SceneManager* sceneManager);
	virtual ~Scene();
	virtual void Update(float deltaTime) = 0;
	void UpdateObjects(float deltaTime);
	virtual void UpdateManagers(float deltaTime);
	void Draw();
	virtual void DrawUI() = 0;

	void AddObject(Object* object);
	void RemoveObject(Object* object);
	void AddUIObject(UIObject* object);
	void RemoveUIObject(UIObject* object);
	void AddSprite(class ComponentSprite* sprite);
	void RemoveSprite(class ComponentSprite* sprite);
	void AddMoveFloor(class MoveFloorObject* object);
	void RemoveMoveFloor(class MoveFloorObject* object);
	void AddTriggeredMoveFloor(class TriggeredMoveFloorObject* object);
	void RemoveTriggeredMoveFloor(class TriggeredMoveFloorObject* object);
	void AddTrigger(class ComponentTrigger* object);
	void RemoveTrigger(class ComponentTrigger* object);

	class SceneManager* GetSceneManager() { return mSceneManager; }
	std::vector<Object*> OnCollision(Vector2 pos, Vector2 col, Object* object, Object::Type objectType);
	std::vector<Object*> FindObjects(Object* object, Object::Type objectType);
	std::vector<class MoveFloorObject*> GetMoveFloors() { return mMoveFloors; }
	std::vector<class TriggeredMoveFloorObject*> GetTriggeredMoveFloors() { return mTriggeredMoveFloors; }
	std::vector<class ComponentTrigger*>GetTriggers() { return mTriggers; }
	class CameraManager* GetCameraManager() { return mCameraManager; }
	class StageManager* GetStageManager() { return mStageManager; }
	class EventManager* GetEventManager() { return mEventManager; }


	void SetPlayer(class Player* player) { mPlayer = player; }
	void SetClimber(class Climber* climber) { mClimber = climber; }
	class Player* GetPlayer() { return mPlayer; }
	class GhostCat* GetCat() { return mCat; }
	class Climber* GetClimber() { return mClimber; }
	class GolemEvent* GetGolem() { return mGolem; }
	unsigned GetTextColor() { return mTextColor; }
	bool IsPause() { return mIsPause; }
	void SetPause(bool pause) { mIsPause = pause; }
	void SetInCave(int inCave) { mInCave = inCave; }
	int InCave() { return mInCave; }

	void AllObjectSaveCheckPointData();
	void AllObjectLoadCheckPointData();
protected:
	unsigned mTextColor;
	bool mIsSleep;	//メニュー表示中など、データは消去しないが処理もしないとき
	bool mIsUpdating;
	class SceneManager* mSceneManager;

	class CameraManager* mCameraManager;
	class StageManager* mStageManager;
	class EventManager* mEventManager;
	std::vector<Object*> mObjects;	//追加済みオブジェクト
	std::vector<Object*> mNewObjects;	//追加待ちオブジェクト
	std::vector<UIObject*> mUIObjects;
	std::vector<UIObject*> mNewUIObjects;
	std::vector<class MoveFloorObject*> mMoveFloors;	//移動する壁・床
	std::vector<class TriggeredMoveFloorObject*> mTriggeredMoveFloors;	//移動する壁・床
	std::vector<class ComponentTrigger*> mTriggers;
	std::vector<class ComponentSprite*> mSprites;	//表示する画像

	class Player* mPlayer;
	class GhostCat* mCat;
	class GolemEvent* mGolem;
	class Climber* mClimber;

	bool mIsPause = false;

	int mInCave = -1;
};
#endif