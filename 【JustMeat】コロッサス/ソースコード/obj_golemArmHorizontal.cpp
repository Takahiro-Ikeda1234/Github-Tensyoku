#include "obj_golemArmHorizontal.h"
#include"StageManager.h"
#include "ComponentAnimationSprite.h"
#include"obj_player.h"
GolemArmHorizontal::GolemArmHorizontal(Scene * scene, Vector2 pos, bool isWall, bool isRight)
	: AbstractGolemArm(scene, pos, isWall, isRight, true)
{
	mCASprite = new ComponentAnimationSprite(this, TextureTag::HorizontalArm, 5);
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
	SetSize(Vector2(BLOCKSIZE * 30, BLOCKSIZE * 5));
}

GolemArmHorizontal::GolemArmHorizontal(Scene * scene, int indexX, int indexY, bool isWall, bool isRight)
	: GolemArmHorizontal(scene, StageManager::ToPosition(indexX, indexY), isWall, isRight)
{
}

GolemArmHorizontal::~GolemArmHorizontal()
{
}

void GolemArmHorizontal::CreateBlock(int indexX, int indexY)
{
	int x = indexX;
	int y = indexY;
	mBlockData[y][x] = 1;
	if (mIsWall)
	{
		mBlocks.push_back(new GolemWall(this, GetScene(), GetPosition() + Vector2(BLOCKSIZE * (-14.5f + x), BLOCKSIZE * (-3 + y)), x, y));
		mBlocks.back()->SetActive();
	}
	else
	{
		mBlocks.push_back(new GolemBlock(this, GetScene(), GetPosition() + Vector2(BLOCKSIZE * (-14.5f + x), BLOCKSIZE * (-3 + y)), x, y));
		mBlocks.back()->SetActive();
	}
}

int GolemArmHorizontal::IndexToGolemIndexX(int indexX)
{
	return indexX - StageManager::ToIndexX(GetPosition().x - BLOCKSIZE * 14.5f);
}
int GolemArmHorizontal::IndexToGolemIndexY(int indexY)
{
	return indexY - StageManager::ToIndexY(GetPosition().y - BLOCKSIZE * 3);
}

GolemArmHorizontalArmored::GolemArmHorizontalArmored(Scene * scene, Vector2 pos, bool isWall, bool isRight)
	: GolemArmHorizontal(scene, pos, isWall, isRight)
{
	Object::SetObjectType(Type::eWall);
	mSub[0] = new GolemArmHorizontalArmoredSub(scene, pos + Vector2(0, 0) * BLOCKSIZE, false, isRight, this);
	mSub[1] = new GolemArmHorizontalArmoredSub(scene, pos + Vector2(0, 0) * BLOCKSIZE, true, isRight, this);
	mSub[2] = new GolemArmHorizontalArmoredSub(scene, pos + Vector2(0, 0) * BLOCKSIZE, false, isRight, this);
	SetArmoredTexture();
}

GolemArmHorizontalArmored::GolemArmHorizontalArmored(Scene * scene, int indexX, int indexY, bool isWall, bool isRight)
	:GolemArmHorizontalArmored(scene, StageManager::ToPosition(indexX, indexY), isWall, isRight)
{
}

void GolemArmHorizontalArmored::SetObjectType(Type type)
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

void GolemArmHorizontalArmored::Punch(int startX, int startY, int move, float moveTime, float stopTime, float backSpeed)
{
	AbstractGolemArm::Punch(startX, startY, move, moveTime, stopTime, backSpeed);
	mSub[0]->Punch(startX - 10, startY, move, moveTime, stopTime, backSpeed);
	mSub[1]->Punch(startX + 0, startY, move, moveTime, stopTime, backSpeed);
	mSub[2]->Punch(startX + 10, startY, move, moveTime, stopTime, backSpeed);
	mSub[0]->SetObjectType(Type::eNone);
	mSub[2]->SetObjectType(Type::eNone);
}

bool GolemArmHorizontalArmored::IsHit(Vector2 pos, Vector2 col)
{
	return (AbstractGolemArm::IsHit(pos, col) || mSub[0]->IsHit(pos, col) || mSub[1]->IsHit(pos, col) || mSub[2]->IsHit(pos, col));
}

GolemArmHorizontalArmoredSub::GolemArmHorizontalArmoredSub(Scene * scene, Vector2 pos, bool isWall, bool isRight, GolemArmHorizontalArmored* parent)
	: AbstractGolemArm(scene, pos, isWall, isRight, true, true)
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
	SetSize(Vector2(BLOCKSIZE * 10, BLOCKSIZE * 5));
	mCASprite->SetScale(1, 1);

	if (isWall)
	{
		mCASprite->SetColorRate(0);
	}
	SetObjectType(Type::eNone);
}

GolemArmHorizontalArmoredSub::GolemArmHorizontalArmoredSub(Scene * scene, int indexX, int indexY, bool isWall, bool isRight, GolemArmHorizontalArmored* parent)
	: GolemArmHorizontalArmoredSub(scene, StageManager::ToPosition(indexX, indexY), isWall, isRight, parent)
{
}

void GolemArmHorizontalArmoredSub::SetObjectType(Type type)
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
