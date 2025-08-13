#pragma once
#include"Object.h"
#include"Counter.h"
class CheckPoint : public Object
{
public:
	CheckPoint(Scene* scene, int indexX, int indexY, int id);
	CheckPoint(Scene* scene, Vector2 pos, int id);
	~CheckPoint();
	virtual void UpdateObject(float deltaTime) override;
	static void ActiveCP(int id);
	static void InactiveCP(int id);
	virtual void LoadCheckPointData()override;
private:
	int mID;
	static CheckPoint* mCP[MAX_OBJECT_CH];
	class ParticleSystem* mParticle;
};