#pragma once
#include"Object.h"	//Objectを継承するのに必要
class NewObject : public Object
{
public:
	NewObject(Scene* scene, Vector2 pos);
	NewObject(Scene* scene, int indexX, int indexY);
	~NewObject() {}	// Componentは自動的に削除されるのでComponentのdeleteは不要
	void UpdateObject(float deltaTime) override;	//純粋仮想関数なので必ず作る
private:
};