//=============================================================================
//
// [Vector2.h] 平面クラス
//
//=============================================================================
#ifndef _VECTOR2_H_
#define _VECTOR2_H_

//インクルードファイル
#include<math.h>
#include"myMath.h"

//クラス定義
class Vector2
{
public:
	Vector2() : x(0), y(0){}
	Vector2(float _x, float _y);
	float x;
	float y;
	static float Distance(const Vector2& a, const Vector2& b)
	{
		return sqrtf((a.x - b.x)*(a.x - b.x) + (a.y - b.y)*(a.y - b.y));
	}
	float Length() { return sqrtf(x*x + y * y); }
	Vector2& operator=(Vector2 copy) { x = copy.x; y = copy.y;  return *this; }
	bool operator==(const Vector2& a) { if (x == a.x && y == a.y) return true; return false; }
	Vector2 operator*(const Vector2& a) { Vector2 temp(x * a.x, y * a.y); return temp; }
	Vector2 operator*(const float a) { Vector2 temp(x * a, y * a); return temp; }
	Vector2 operator/(const Vector2& a) { Vector2 temp(x / a.x, y / a.y); return temp; }
	Vector2 operator/(const float a) { Vector2 temp(x / a, y / a); return temp; }
	Vector2 operator-(const Vector2& a) { Vector2 temp(x - a.x, y - a.y); return temp; }
	Vector2 operator+(const Vector2& a) { Vector2 temp(x + a.x, y + a.y); return temp; }
	static bool RectCollision(const Vector2& aPos, const Vector2& aSize, const Vector2& bPos, const Vector2& bSize);
	static bool LineAndLineCollision(const Vector2& lineA1, const Vector2& lineA2, const Vector2& lineB1, const Vector2& lineB2);
	static bool LineAndRectCollision(const Vector2& linePos, float mag, float deg, const Vector2& rectPos, const Vector2& rectSize);
	static float degArctan(const Vector2& origin, const Vector2& target);
	static float degArctan(const Vector2& vec);
	float deg() { return Vector2::degArctan(*this); };
	static Vector2 degVector(float deg) { return Vector2(degCos(deg), degSin(deg)); }
	Vector2 Rotation(float deg) { Vector2 temp(x * degCos(deg) - y * degSin(deg), x * degSin(deg) + y * degCos(deg)); return temp; }
};


const Vector2 operator*(const float a, const Vector2& v);
const Vector2 operator*(const Vector2& v, const float a);
const Vector2 operator+(const Vector2& v1, const Vector2& v2);
#endif