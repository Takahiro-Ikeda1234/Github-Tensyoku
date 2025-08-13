#pragma once
#include<vector>
#include"Counter.h"
#include"StageObject.h"
class GhostCat : public StageObject
{
public:
	enum class State
	{
		Wait,
		Jumping,
		Hiding,
		Thrust,
		Invisible,
		FollowPlayer,
		Sit,
		Dummy,

	};
	GhostCat(Scene* scene, Vector2 position);
	~GhostCat() {}
	virtual void UpdateStageObject(float deltaTime) override;
	void StateFollowPlayer(float deltaTime);
	void CatEventManager(float deltaTime);
	const float MAXSPEED = BLOCKSIZE * 10;
	virtual void SaveCheckPointData() override;
	virtual void LoadCheckPointData() override;
	void StartDummy(bool isClimb, Dir dir);
	void EndDummy(bool makeEffect);
	void WarpGate();
	virtual void EnterGolemArm() override;
	void UpdateMoveFloor();
private:
	void CreateDummy();
	Counter mDummyDelay = 0;
	bool mIsDummyClimb;
	Dir mDummyDir;
	Vector2 mDummyPos;
	class ComponentAnimationSprite* mCASpriteDummy;
	class ComponentMessageBox* mMessage;
	class UIBox* mMessageIcon;
	bool mCatEventID[MAX_OBJECT_CH];
	State mCatState = State::FollowPlayer;
	Counter mEventCounter;

	bool mCPEventID[MAX_OBJECT_CH];
	State mCPCatState = State::FollowPlayer;
	Counter mCPEventCounter = 0;
	void GenerateSmoke(Vector2 position);
	void GenerateStar(Vector2 position);
	void GenerateWarp(Vector2 position);

	bool mIsClimb = false;


	void LoadMessageFile();
	static bool mAlreadyLoadFile;
	typedef struct
	{
		int eventType;	//0:メッセージ表示,1:メッセージ削除
		std::string message;
		float autoDisappearTime;	//0なら表示し続ける
		int coloredStart;
		int coloredCount;
	}MessageEventData;
	static MessageEventData mData[STAGE_MAX][MAX_OBJECT_CH];
};