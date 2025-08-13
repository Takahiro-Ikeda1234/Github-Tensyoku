#pragma once
#include"AbstractObject.h"
#include"Counter.h"
class Particle : public AbstractObject
{
public:
	Particle(class Scene* scene, Vector2 pos, class ParticleInfo* info, float deg);
	~Particle() {}
	void UpdateObject(float deltaTime) override;
	float GetLifetimePercentage() { return mLifetime.Percentage(); }

	Vector2 GetBaseVector() { return baseSpeed * Vector2(sin, cos); }
	Vector2 GetGravity() { return gravity; }
	float GetSpeed() { return baseSpeed; }
private:
	float GetAlphaRate() { return baseAlphaRate; }
	//float GetAlphaRate() { return baseColorRate; }
	float GetRedRate() { return baseRedRate; }
	float GetGreenRate() { return baseGreenRate; }
	float GetBlueRate() { return baseBlueRate; }
	float GetRotationSpeed() { return baseRotationSpeed; }
	void UpdateParameter();
private:
	class ParticleInfo* mInfo;
	class ComponentAnimationSprite* mCASprite;
	Counter mLifetime;

	float baseScale;
	float baseDeg;
	float sin;
	float cos;
	float baseSpeed;
	float baseRotationSpeed;


	float baseAlphaRate = 1;
	//float baseColorRate = 1;
	float baseRedRate = 1;
	float baseGreenRate = 1;
	float baseBlueRate = 1;

	Vector2 gravity = { 0,0 };
};