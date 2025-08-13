//=============================================================================
//
// [mouse.h] マウス入力
//
//=============================================================================
#ifndef _MOUSE_H_
#define _MOUSE_H_
#include"Singleton.h"
#include"Vector2.h"

#define MOUSE_LEFT 0
#define MOUSE_RIGHT 1
#define MOUSE_MIDDLE 2

class Mouse : public Singleton<Mouse>
{
private:
	friend Singleton<Mouse>;
	Mouse() : Singleton() {}

public:
	void Update(float deltaTime);
	bool MouseHit(int mousecode) const;
	float MouseDown(int mousecode) const;
	bool MouseUp(int mousecode) const;
	float GetPosX() const { return mMouseState.pos.x; }
	float GetPosY() const { return mMouseState.pos.y; }
	Vector2 GetPos() const { return mMouseState.pos; }
	int GetWheelMove() const { return mMouseState.wheelMove; }
	
private:
	typedef struct
	{
		float mouseDownTime[3];
		int wheelMove;	//値はホイールを手前に1段階動かすと-120、奥に1段階動かすと120
		Vector2 pos;
	}MouseState;

	MouseState mMouseState;
	MouseState mPrevMouseState;
};
#endif