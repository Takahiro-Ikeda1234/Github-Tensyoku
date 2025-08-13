#pragma once
#include"Object.h"
#include"Counter.h"
class Laser;
class ParticleSystem;
class LaserCannon : public Object
{
public:
	static const int LASER_MAX_LENGTH = 30;
	static const int LASER_LAYER = 6;
	LaserCannon(Scene* scene, int indexX, int indexY, int dir);
	~LaserCannon();
	virtual void UpdateObject(float deltaTime) override;
	void UpdateLaserCollision(float deltaTime);
	void GenerateLaser();
	void UpdateTerminal(Object* laser);
	void GenerateParticle(Vector2 pos);
	void GenerateParticlePlayerHit(Vector2 pos);
	
private:
	Laser* mLaser[LASER_LAYER][LASER_MAX_LENGTH];
	int mDir;
	int mIndexX;
	int mIndexY;
	int mLaserLength;
	LoopCounter mUpdateInterval;
	bool mFirstUpdate = true;
	Vector2 mTerminalPos;
};

class Laser : public Object
{
public:
	Laser(Scene* scene, Vector2 pos, int texID, int dir, int layer);
	~Laser() {}
	virtual void UpdateObject(float deltaTime) override;
};