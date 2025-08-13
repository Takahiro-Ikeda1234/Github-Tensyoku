#pragma once
#include"MoveFloorObject.h"
class MoveWall : public MoveFloorObject
{
public:
	MoveWall(Scene* scene, Vector2 pos, int id);
	MoveWall(Scene* scene, int indexX, int indexY, int id);
	~MoveWall() {}
	void UpdateStageObject(float deltaTime) override;
private:

	//ˆÚ“®•Çî•ñ
private:
};