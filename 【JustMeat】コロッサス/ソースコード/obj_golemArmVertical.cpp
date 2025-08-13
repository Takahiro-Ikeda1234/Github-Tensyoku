#include "obj_golemArmVertical.h"
#include "StageManager.h"
#include"ComponentAnimationSprite.h"
#include "obj_player.h"
GolemArmVertical::GolemArmVertical(Scene * scene, Vector2 pos, bool isWall, bool isRight)
	: AbstractGolemArm(scene, pos, isWall, isRight, false)
{
	mCASprite = new ComponentAnimationSprite(this, TextureTag::VerticalArm, 5);
	if (isWall)
	{
		mCASprite->SetAnimation(0, 1, 1, 1, false);
	}
	else
	{
		int stage = scene->GetStageManager()->GetCurrentStageID();
		if (stage < 3) mCASprite->SetAnimation(2, 3, 1, 1, false);
		else if (stage < 6) mCASprite->SetAnimation(4, 5, 1, 1, false);
		else mCASprite->SetAnimation(6, 7, 1, 1, false);
	}
	if (isRight)
	{
		SetDirection(Dir::eRight);
	}
	else
	{
		SetDirection(Dir::eLeft);
	}
	SetSize(Vector2(BLOCKSIZE * 5, BLOCKSIZE * 30));
	//for (int y = 0; y < 10; y++)
	//{
	//	mBlockData[y][0] = 0;
	//	mBlockData[y][1] = 1;
	//	mBlockData[y][2] = 1;
	//	mBlockData[y][3] = 1;
	//	mBlockData[y][4] = 1;
	//	mBlockData[y][5] = 1;
	//	mBlockData[y][6] = 0;
	//}
	//for (int y = 20; y < 30; y++)
	//{
	//	mBlockData[y][0] = 0;
	//	mBlockData[y][1] = 1;
	//	mBlockData[y][2] = 1;
	//	mBlockData[y][3] = 1;
	//	mBlockData[y][4] = 1;
	//	mBlockData[y][5] = 1;
	//	mBlockData[y][6] = 0;
	//}

	for (int y = 0; y < 30; y++)
	{
		for (int x = 0; x < 7; x++)
		{
			if (mBlockData[y][x] == 1)
			{
				mBlocks.push_back(new GolemBlock(this, GetScene(), GetPosition() + Vector2(BLOCKSIZE * (-3 + x), BLOCKSIZE * (-14.5f + y)), x, y));
			}
		}
	}
}

GolemArmVertical::GolemArmVertical(Scene * scene, int indexX, int indexY, bool isWall, bool isRight)
	:GolemArmVertical(scene, StageManager::ToPosition(indexX, indexY), isWall, isRight)
{
}

GolemArmVertical::~GolemArmVertical()
{
}

void GolemArmVertical::CreateBlock(int indexX, int indexY)
{

	int x = indexX;
	int y = indexY;
	mBlockData[y][x] = 1;

	if (mIsWall)
	{
		mBlocks.push_back(new GolemWall(this, GetScene(), GetPosition() + Vector2(BLOCKSIZE * (-3 + x), BLOCKSIZE * (-14.5f + y)), x, y));
		mBlocks.back()->SetActive();
	}
	else
	{
		if (y >= 27)
		{

			mBlocks.push_back(new GolemBlock(this, GetScene(), GetPosition() + Vector2(BLOCKSIZE * (-3 + x), BLOCKSIZE * (-14.5f + y)), x, y));
			mBlocks.back()->SetActive();
		}
		else
		{
			mBlocks.push_back(new GolemWall(this, GetScene(), GetPosition() + Vector2(BLOCKSIZE * (-3 + x), BLOCKSIZE * (-14.5f + y)), x, y));
			mBlocks.back()->SetActive();
		}
	}
}

int GolemArmVertical::IndexToGolemIndexX(int indexX)
{
	return indexX - StageManager::ToIndexX(GetPosition().x - BLOCKSIZE * 3);
}
int GolemArmVertical::IndexToGolemIndexY(int indexY)
{
	return indexY - StageManager::ToIndexY(GetPosition().y - BLOCKSIZE * 14.5f);
}



GolemArmVerticalArmored::GolemArmVerticalArmored(Scene * scene, Vector2 pos, bool isWall, bool isRight)
	: GolemArmVertical(scene, pos, isWall, isRight)
{
	Object::SetObjectType(Type::eWall);
	mSub[0] = new GolemArmVerticalArmoredSub(scene, pos + Vector2(0, 0) * BLOCKSIZE, false, isRight, this);
	mSub[1] = new GolemArmVerticalArmoredSub(scene, pos + Vector2(0, 0) * BLOCKSIZE, true, isRight, this);
	mSub[2] = new GolemArmVerticalArmoredSub(scene, pos + Vector2(0, 0) * BLOCKSIZE, false, isRight, this);
	SetArmoredTexture();
}

GolemArmVerticalArmored::GolemArmVerticalArmored(Scene * scene, int indexX, int indexY, bool isWall, bool isRight)
	:GolemArmVerticalArmored(scene, StageManager::ToPosition(indexX, indexY), isWall, isRight)
{
}

void GolemArmVerticalArmored::SetObjectType(Type type)
{
	if (type == Type::eNone)
	{
		Object::SetObjectType(Type::eNone);
		mSub[0]->SetObjectType(Type::eNone);
		mSub[1]->SetObjectType(Type::eNone);
		mSub[2]->SetObjectType(Type::eNone);
	}
	else
	{
		Object::SetObjectType(type);
	}
}

void GolemArmVerticalArmored::Punch(int startX, int startY, int move, float moveTime, float stopTime, float backSpeed)
{
	AbstractGolemArm::Punch(startX, startY, move, moveTime, stopTime, backSpeed);
	mSub[0]->Punch(startX, startY - 10, move, moveTime, stopTime, backSpeed);
	mSub[1]->Punch(startX, startY + 0, move, moveTime, stopTime, backSpeed);
	mSub[2]->Punch(startX, startY + 10, move, moveTime, stopTime, backSpeed);
	mSub[0]->SetObjectType(Type::eSolid);
	mSub[2]->SetObjectType(Type::eSolid);
}

bool GolemArmVerticalArmored::IsHit(Vector2 pos, Vector2 col)
{
	return (AbstractGolemArm::IsHit(pos, col) || mSub[0]->IsHit(pos, col) || mSub[1]->IsHit(pos, col) || mSub[2]->IsHit(pos, col));
}

GolemArmVerticalArmoredSub::GolemArmVerticalArmoredSub(Scene * scene, Vector2 pos, bool isWall, bool isRight, GolemArmVerticalArmored* parent)
	: AbstractGolemArm(scene, pos, isWall, isRight, false, true)
	, mParent(parent)
{
	if (isWall) mBaseType = Type::eNone;
	else mBaseType = Type::eSolid;

	mCASprite = new ComponentAnimationSprite(this, TextureTag::Invisible, 150);
	if (isRight)
	{
		SetDirection(Dir::eRight);
	}
	else
	{
		SetDirection(Dir::eLeft);
	}
	SetSize(Vector2(BLOCKSIZE * 5, BLOCKSIZE * 10));
	mCASprite->SetScale(1, 1);

	if (isWall)
	{
		mCASprite->SetColorRate(0);
	}
	SetObjectType(Type::eNone);
}

GolemArmVerticalArmoredSub::GolemArmVerticalArmoredSub(Scene * scene, int indexX, int indexY, bool isWall, bool isRight, GolemArmVerticalArmored* parent)
	: GolemArmVerticalArmoredSub(scene, StageManager::ToPosition(indexX, indexY), isWall, isRight, parent)
{
}

void GolemArmVerticalArmoredSub::SetObjectType(Type type)
{
	if (type == Type::eNone)
	{
		Object::SetObjectType(Type::eNone);
	}
	else
	{
		Object::SetObjectType(mBaseType);
	}

	if (GetScene()->GetPlayer()->IsDying())Object::SetObjectType(Type::eNone);
}
