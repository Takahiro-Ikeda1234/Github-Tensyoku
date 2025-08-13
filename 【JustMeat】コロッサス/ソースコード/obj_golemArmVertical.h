#pragma once
#include"obj_AbstractGolemArm.h"

class GolemArmVertical : public AbstractGolemArm
{
public:
	GolemArmVertical(Scene* scene, Vector2 pos, bool isWall, bool isRight);
	GolemArmVertical(Scene* scene, int indexX, int indexY, bool isWall, bool isRight);
	virtual ~GolemArmVertical();
	void CreateBlock(int indexX, int indexY)override;
	int IndexToGolemIndexX(int indexX) override;
	int IndexToGolemIndexY(int indexY) override;
	void SetBlockData(int id, int indexX, int indexY) override { mBlockData[indexY][indexX] = id; }
	int GetBlockData(int indexX, int indexY) override { return mBlockData[indexY][indexX]; }
private:
};
class GolemArmVerticalArmoredSub;
class GolemArmVerticalArmored : public GolemArmVertical
{
public:
	GolemArmVerticalArmored(Scene* scene, Vector2 pos, bool isWall, bool isRight);
	GolemArmVerticalArmored(Scene* scene, int indexX, int indexY, bool isWall, bool isRight);
	virtual ~GolemArmVerticalArmored() {}
	virtual void SetObjectType(Type type);
	virtual void Punch(int startX, int startY, int move, float moveTime, float stopTime, float backSpeed) override;
	virtual bool IsHit(Vector2 pos, Vector2 col);
private:
	GolemArmVerticalArmoredSub* mSub[3];
};

class GolemArmVerticalArmoredSub : public AbstractGolemArm
{
public:
	GolemArmVerticalArmoredSub(Scene* scene, Vector2 pos, bool isWall, bool isRight, GolemArmVerticalArmored* parent);
	GolemArmVerticalArmoredSub(Scene* scene, int indexX, int indexY, bool isWall, bool isRight, GolemArmVerticalArmored* parent);
	virtual ~GolemArmVerticalArmoredSub() {}
	int IndexToGolemIndexX(int indexX) override { return 0; }
	int IndexToGolemIndexY(int indexY) override { return 0; }
	void SetBlockData(int id, int indexX, int indexY) { mBlockData[indexY][indexX] = id; }
	int GetBlockData(int indexX, int indexY) override { return mBlockData[indexY][indexX]; }
	virtual void SetObjectType(Type type);
	GolemArmVerticalArmored* GetParent()const { return mParent; }
private:
	GolemArmVerticalArmored* mParent;
	Type mBaseType;
};