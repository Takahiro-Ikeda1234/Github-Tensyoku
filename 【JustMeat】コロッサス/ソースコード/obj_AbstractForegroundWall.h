#pragma once
#include"Object.h"
#include"Counter.h"
static const float FOREGROUND_FADESPEED = 1.0f;
class AbstractForegroundWall : public Object
{
public:
	AbstractForegroundWall(Scene* scene, int indexX, int indexY);
	AbstractForegroundWall(Scene* scene, Vector2 pos);
	virtual ~AbstractForegroundWall() {}
	virtual void UpdateObject(float deltaTime);
protected:
private:
	float mFloatColor = 0x01;
};