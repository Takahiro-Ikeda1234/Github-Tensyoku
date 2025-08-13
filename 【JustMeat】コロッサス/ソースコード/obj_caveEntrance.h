#pragma once
#include"obj_AbstractForegroundWall.h"
class CaveEntrance : public AbstractForegroundWall
{
public:
	CaveEntrance(Scene* scene, int indexX, int indexY, bool isRight);
	CaveEntrance(Scene* scene, Vector2 pos, bool isRight);
	virtual ~CaveEntrance() {}
	virtual void UpdateObject(float deltaTime);
private:
	bool mIsRight;	//“´ŒA‚Ì•ûŒü‚ª‰E
	bool mIsPlayerEntering = false;
};