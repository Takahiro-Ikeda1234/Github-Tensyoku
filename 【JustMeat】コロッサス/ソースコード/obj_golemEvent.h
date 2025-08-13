#pragma once
#include"Object.h"
#include"Counter.h"
#include"StageManager.h"
#include"MoveFloorObject.h"
class Scene;
class ComponentMove;
class GolemObject;
class GolemEvent : public Object
{
public:
	GolemEvent(Scene* scene, Vector2 pos);
	GolemEvent(Scene* scene, int indexX, int indexY);
	virtual ~GolemEvent() {}
	virtual void UpdateObject(float deltaTime) override;
	void CreateBlock(int x, int y);
	void SetActiveArm(bool isRight, bool isHorizontal, class AbstractGolemArm* arm) { mIsActiveArmRight = isRight; mIsActiveArmHorizontal = isHorizontal; mActiveArm = arm; }
	class AbstractGolemArm* HitGolemArm(const Vector2& pos, const Vector2& col);
private:
	void GolemAttackZone(float deltaTime);
	void Stage0GolemArmEvent(float deltaTime);
	void Stage1GolemArmEvent(float deltaTime) {}
	void Stage2GolemArmEvent(float deltaTime) {}
	void Stage3GolemArmEvent(float deltaTime) {}
	void Stage4GolemArmEvent(float deltaTime) {}
	void ColorUpdate(float deltaTime);
	bool AllArmReady();
	class AbstractGolemArm* mLeftVerticalWallArm;
	class AbstractGolemArm* mLeftVerticalBlockArm;
	class AbstractGolemArm* mLeftHorizontalWallArm;
	class AbstractGolemArm* mLeftHorizontalBlockArm;
	class AbstractGolemArm* mRightVerticalWallArm;
	class AbstractGolemArm* mRightVerticalBlockArm;
	class AbstractGolemArm* mRightHorizontalWallArm;
	class AbstractGolemArm* mRightHorizontalBlockArm;
	class AbstractGolemArm* mActiveArm;
	float mFloatColor = 0xFF;
	bool mIsActiveArmRight;
	bool mIsActiveArmHorizontal;

	unsigned char mEventFlag = 0b00000000;


	typedef struct
	{
		int armType;
		int moveIndex;
		int stopIndex;
		float moveTime;
		float stopTime;
		int targetType;
		int targetIndexX;
		int targetIndexY;
		float backSpeed;

	}GolemAttackData;
	static void LoadSettingFile();
	static GolemAttackData mGolemAttackData[STAGE_MAX][MAX_OBJECT_CH];
	static bool mAlreadyLoadFile;
};

class GolemObject : public MoveFloorObject
{
public:
	GolemObject(class AbstractGolemArm * mOwner, Scene* scene, int indexX, int indexY, int golemIndexX, int golemIndexY);
	GolemObject(class AbstractGolemArm * mOwner, Scene* scene, Vector2 pos, int golemIndexX, int golemIndexY);
	virtual ~GolemObject();
	virtual void UpdateStageObject(float deltaTime) override {}
	virtual void SetActive() = 0 {};
	virtual void SetInactive() = 0 {};
	int GetGolemIndexX() { return mGolemIndexX; }
	int GetGolemIndexY() { return mGolemIndexY; }
protected:
	class AbstractGolemArm* mOwner;
private:
	int mGolemIndexX;
	int mGolemIndexY;
};

