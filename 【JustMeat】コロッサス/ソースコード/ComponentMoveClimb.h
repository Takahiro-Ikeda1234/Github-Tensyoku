#pragma once
#include"ComponentMove.h"
class ComponentMoveClimb : public ComponentMove
{
public:
	ComponentMoveClimb(Object* owner, bool useGravity = true, int gravitySpeed = GRAVITY, int updatePriority = LAYER_OBJECT);
	~ComponentMoveClimb() {}
	void Update(float deltaTime) override;
	bool Collision() override;
	void SetMoveNoWall(bool moveNoWall) { mCanMoveNoWall = moveNoWall; }
private:
	bool mCanMoveNoWall = false;
	//unsigned CollisionCustom() override;
};