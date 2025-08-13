#pragma once
#include"obj_AbstractGolemArm.h"
class GolemArmHorizontalArmoredSub;
class GolemArmHorizontal : public AbstractGolemArm
{
public:
	GolemArmHorizontal(Scene* scene, Vector2 pos, bool isWall, bool isRight);
	GolemArmHorizontal(Scene* scene, int indexX, int indexY, bool isWall, bool isRight);
	virtual ~GolemArmHorizontal();
	void CreateBlock (int indexX, int indexY)override;
	int IndexToGolemIndexX(int indexX) override;
	int IndexToGolemIndexY(int indexY) override;
	void SetBlockData(int id, int indexX, int indexY) { mBlockData[indexY][indexX] = id; }
	int GetBlockData(int indexX, int indexY) override { return mBlockData[indexY][indexX]; }
private:
};

class GolemArmHorizontalArmored : public GolemArmHorizontal
{
public:
	GolemArmHorizontalArmored(Scene* scene, Vector2 pos, bool isWall, bool isRight);
	GolemArmHorizontalArmored(Scene* scene, int indexX, int indexY, bool isWall, bool isRight);
	virtual ~GolemArmHorizontalArmored() {}
	virtual void SetObjectType(Type type);
	virtual void Punch(int startX, int startY, int move, float moveTime, float stopTime, float backSpeed) override;
	virtual bool IsHit(Vector2 pos, Vector2 col);
private:
	GolemArmHorizontalArmoredSub* mSub[3];
};

class GolemArmHorizontalArmoredSub : public AbstractGolemArm
{
public:
	GolemArmHorizontalArmoredSub(Scene* scene, Vector2 pos, bool isWall, bool isRight, GolemArmHorizontalArmored* parent);
	GolemArmHorizontalArmoredSub(Scene* scene, int indexX, int indexY, bool isWall, bool isRight, GolemArmHorizontalArmored* parent);
	virtual ~GolemArmHorizontalArmoredSub() {}
	int IndexToGolemIndexX(int indexX) override { return 0; }
	int IndexToGolemIndexY(int indexY) override { return 0; }
	void SetBlockData(int id, int indexX, int indexY) { mBlockData[indexY][indexX] = id; }
	int GetBlockData(int indexX, int indexY) override { return mBlockData[indexY][indexX]; }
	virtual void SetObjectType(Type type);
	GolemArmHorizontalArmored* GetParent()const { return mParent; }
private:
	GolemArmHorizontalArmored* mParent;
	Type mBaseType;
};