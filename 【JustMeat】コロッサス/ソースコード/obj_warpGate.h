#pragma once
#include"Object.h"	//Object���p������̂ɕK�v
#include"StageManager.h"
#include"Scene.h"
class WarpGate : public Object
{
public:
	WarpGate(Scene* scene, Vector2 pos, int ID, bool enter = true, bool right = false);	//���g��ID�A�ڑ���ID
	WarpGate(Scene* scene, int indexX, int indexY, int ID, bool enter = true, bool right = false);
	~WarpGate() {}
	void UpdateObject(float deltaTime) override {}
	WarpGate* GetConnect() { return mConnect; }
	int GetID() { return mID; }
	WarpGate* CreateConnection(WarpGate* obj) { mConnect = obj; return this; };
	bool CanEnter()const { return mCanEnter; }
private:
	int mID;
	WarpGate* mConnect;
	bool mCanEnter;
	//���[�v�Q�[�g�ڑ����
private:
};