#pragma once
#include"Object.h"
#include"obj_golemEvent.h"

#include"MoveFloorObject.h"
class GolemCrystal;
class AbstractGolemArm : public MoveFloorObject
{
public:
	enum class State
	{
		eReady,
		ePunchStart,
		ePunch,
		ePunchStop,
		ePunchEnd,
		eMove,
	};
	AbstractGolemArm(Scene* scene, Vector2 pos, bool isWall, bool isRight, bool isHorizontal, bool isSub = false);
	AbstractGolemArm(Scene* scene, int indexX, int indexY, bool isWall, bool isRight, bool isHorizontal, bool isSub = false);
	virtual ~AbstractGolemArm();
	virtual void UpdateStageObject(float deltaTime) override;
	bool IsReady() { if (mState == State::eReady) return true; return false; }
	virtual bool IsHit(Vector2 pos, Vector2 col);
	virtual void Punch(int startX, int startY, int move, float moveTime, float stopTime, float backSpeed);
	virtual void StateReady(float deltaTime);
	virtual void StatePunchStart(float deltaTime);
	virtual void StatePunch(float deltaTime);
	virtual void StatePunchStop(float deltaTime);
	virtual void StatePunchEnd(float deltaTime);
	virtual void StateMove(float deltaTime);
	void StartBlockMode();
	void EndBlockMode();
	Vector2 FrontPosition();
	virtual void CreateBlock(int indexX, int indexY) {}
	virtual int IndexToGolemIndexX(int indexX) = 0;
	virtual int IndexToGolemIndexY(int indexY) = 0;
	virtual void SetBlockData(int id, int indexX, int indexY) = 0;
	virtual int GetBlockData(int indexX, int indexY) = 0;
	virtual void Translate(Vector2 v) override;
	virtual void SetPosition(Vector2 v) override;
	virtual void LoadCheckPointData() override;
	State GetGolemState() { return mState; }
	bool IsHorizontal()const { return mIsHorizontal; }
	bool IsRight()const { return mIsRight; }
	void SetArmoredTexture();
protected:
	bool mIsSub = false;
	int mSize = 30;
	State mState = State::eReady;
	Counter mNextMotionTime;
	std::vector<GolemObject*> mBlocks;
	bool mIsRight;
	bool mIsWall;
	bool mIsHorizontal;

	//çUåÇóp
	int mStartX;
	int mStartY;
	int mMoveX;
	int mMoveY;
	float mBackSpeedX;
	float mBackSpeedY;

	int mBlockData[30][30];

	float mAttackMoveTime;
	float mAttackStopTime;

	class ParticleSystem* mSpark;
	void GenerateSpark();

	void GenerateFragments();
	void GenerateSmoke();
	void GenerateSmoke1();
	void GenerateSmoke2();
	class GolemCrystal* mCrystal;
	static bool mFirstAttack;
public:
	void SetMove(int moveX, int moveY, float time, float delay) { mState = State::eMove; mPrevPercentage = 1; mMove_MoveX = moveX; mMove_MoveY = moveY; mMoveTime = time; mMoveDelay = delay; }
private:
	Counter mMoveDelay;
	Counter mMoveTime;
	float mMove_MoveX;
	float mMove_MoveY;
	float mPrevPercentage;

};

class GolemBlock : public GolemObject
{
public:
	GolemBlock(AbstractGolemArm * owner, Scene* scene, int indexX, int indexY, int golemIndexX, int golemIndexY);
	GolemBlock(AbstractGolemArm * owner, Scene* scene, Vector2 pos, int golemIndexX, int golemIndexY);
	virtual ~GolemBlock() {}
	virtual void UpdateStageObject(float deltaTime) override {}
	virtual void Translate(Vector2 v) override;
	virtual void SetActive() override;
	virtual void SetInactive() override;

	int GetIndexX() { return mIndexX; }
	int GetIndexY() { return mIndexY; }
private:
	int mIndexX;
	int mIndexY;
};

class GolemWall : public GolemObject
{
public:
	GolemWall(AbstractGolemArm * owner, Scene* scene, int indexX, int indexY, int golemIndexX, int golemIndexY);
	GolemWall(AbstractGolemArm * owner, Scene* scene, Vector2 pos, int golemIndexX, int golemIndexY);
	virtual ~GolemWall() {}
	virtual void UpdateStageObject(float deltaTime) override {}

	virtual void SetActive() override;
	virtual void SetInactive() override;

	int GetIndexX() { return mIndexX; }
	int GetIndexY() { return mIndexY; }
private:
	int mIndexX;
	int mIndexY;
};

class GolemBuildable : public GolemObject
{
public:
	GolemBuildable(AbstractGolemArm * owner, Scene* scene, int indexX, int indexY, int golemIndexX, int golemIndexY);
	GolemBuildable(AbstractGolemArm * owner, Scene* scene, Vector2 pos, int golemIndexX, int golemIndexY);
	virtual ~GolemBuildable() {}
	virtual void UpdateStageObject(float deltaTime) override {}

	virtual void SetActive() override;
	virtual void SetInactive() override;

	int GetIndexX() { return mIndexX; }
	int GetIndexY() { return mIndexY; }
private:
	int mIndexX;
	int mIndexY;
};

class GolemCrystal : public Object
{
public:
	GolemCrystal(Scene* scene, Vector2 pos, float rotation);
	~GolemCrystal() {}
	virtual void UpdateObject(float deltaTime) override {}
private:
};