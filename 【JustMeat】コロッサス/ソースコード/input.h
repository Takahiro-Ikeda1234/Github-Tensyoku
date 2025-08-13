#pragma once
#include"Singleton.h"

class Input : public Singleton<Input>
{
private:
	friend Singleton<Input>;
	Input() : Singleton() {}
	void Update(float deltaTime);
	bool Hit();
	float Down();
	bool Up();
public:
private:
};