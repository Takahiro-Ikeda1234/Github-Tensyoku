#pragma once
#include"Object.h"	//Object���p������̂ɕK�v
class NewObject : public Object
{
public:
	NewObject(Scene* scene, Vector2 pos);
	NewObject(Scene* scene, int indexX, int indexY);
	~NewObject() {}	// Component�͎����I�ɍ폜�����̂�Component��delete�͕s�v
	void UpdateObject(float deltaTime) override;	//�������z�֐��Ȃ̂ŕK�����
private:
};