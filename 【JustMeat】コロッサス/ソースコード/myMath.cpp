//=============================================================================
//
// [myMath.cpp] 計算処理用
//
//=============================================================================

//インクルードファイル
#include<math.h>
#include"myMath.h"
#include<stdlib.h>
#define PI 3.1416f

float randomFloat()
{
	return (float)rand() / RAND_MAX;
}
float randomRange(float min, float max)
{
	if (min > max)
	{
		float temp = min;
		min = max;
		max = temp;
	}
	return randomFloat() * (max - min) + min;
}
float degToRad(float deg)
{
	return deg * (PI / 180);
}
float radToDeg(float rad)
{
	return rad / (PI / 180);
}
float degSin(float deg)
{
	return sinf(degToRad(deg));
}
float degCos(float deg)
{
	return cosf(degToRad(deg));
}
float degTan(float deg)
{
	if(deg) return tanf(degToRad(deg));
	return 0;
}
float degArctan(float x, float y)
{
	return radToDeg(atan2(y, x));
}
float sigmoid(float gain, float x)
{
	return 1.0f / (1.0f + exp(-gain * x));
}
float QuadraticFormula(float a, float b, float c)
{
	if (a == 0) return 0;
	float sqrtbb4ac = sqrtf(b * b - 4 * a * c);
	return (-b - sqrtbb4ac) / (2 * a);
}