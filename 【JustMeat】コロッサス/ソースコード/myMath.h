//=============================================================================
//
// [myMath.h] �v�Z�����p
//
//=============================================================================
#ifndef _MYMATH_H_
#define _MYMATH_H_

//�C���N���[�h�t�@�C��

//�}�N����`
#define MIN(a, b) (a < b ? a : b)
#define MAX(a, b) (a < b ? b : a)
#define LIMIT(a, min, max) (MIN(max, MAX(min, a)))

//�v���g�^�C�v�錾
float randomFloat();
float randomRange(float min, float max);
float degToRad(float deg);
float radToDeg(float rad);
float degSin(float deg);
float degCos(float deg);
float degTan(float deg);
float degArctan(float x, float y);
float sigmoid(float gain, float x);
float QuadraticFormula(float a, float b, float c);
#endif