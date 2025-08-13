//=============================================================================
//
// [Vector2.cpp] ���ʃN���X
//
//=============================================================================

//�C���N���[�h�t�@�C��
#include"Vector2.h"
#include"myMath.h"

Vector2::Vector2(float _x, float _y)
	: x(_x)
	, y(_y)
{

}

// ---------------------------------------------------------------------------
// �֐���: bool Vector2::RectCollision(const Vector2& aPos, const Vector2& aSize, const Vector2& bPos, const Vector2& bSize)
// ����:   const Vector2& aPos A�̈ʒu
// ����:   const Vector2& aSize A�̃T�C�Y
// ����:   const Vector2& bPos B�̈ʒu
// ����:   const Vector2& bSize B�̃T�C�Y
// �ߒl:   false = �d�Ȃ��Ă��Ȃ�
// �ߒl:   true  = �d�Ȃ��Ă���
// ����:   2�̋�`���d�Ȃ��Ă��邩����
// ---------------------------------------------------------------------------
bool Vector2::RectCollision(const Vector2& aPos, const Vector2& aSize, const Vector2& bPos, const Vector2& bSize)
{
	if (fabs(aPos.x - bPos.x) < aSize.x + bSize.x &&fabs(aPos.y - bPos.y) < aSize.y + bSize.y)
	{
		return true;
	}
	return false;
}


// ---------------------------------------------------------------------------
// �֐���: bool Vector2::LineAndLineCollision(const Vector2& lineA1, const Vector2& lineA2, const Vector2& lineB1, const Vector2& lineB2)
// ����:   const Vector2& lineA1 ��A�̓_1
// ����:   const Vector2& lineA2 ��A�̓_2
// ����:   const Vector2& lineB1 ��B�̓_1
// ����:   const Vector2& lineB2 ��B�̓_2
// �ߒl:   false = �������Ă��Ȃ�
// �ߒl:   true  = �������Ă���
// ����:   2�̐����������Ă��邩����
// ---------------------------------------------------------------------------
bool Vector2::LineAndLineCollision(const Vector2& lineA1, const Vector2& lineA2, const Vector2& lineB1, const Vector2& lineB2)
{
	float xA = lineA2.x - lineA1.x;
	float yA = lineA2.y - lineA1.y;
	float xB1 = lineB1.x - lineA1.x;
	float xB2 = lineB2.x - lineA1.x;
	float yB1 = lineB1.y - lineA1.y;
	float yB2 = lineB2.y - lineA1.y;
	const float crossB1 = xA * yB1 - yA * xB1;
	const float crossB2 = xA * yB2 - yA * xB2;
	if (crossB1*crossB2 > 0)return false;

	float xB = lineB2.x - lineB1.x;
	float yB = lineB2.y - lineB1.y;
	float xA1 = lineA1.x - lineB1.x;
	float xA2 = lineA2.x - lineB1.x;
	float yA1 = lineA1.y - lineB1.y;
	float yA2 = lineA2.y - lineB1.y;
	const float crossA1 = xB * yA1 - yB * xA1;
	const float crossA2 = xB * yA2 - yB * xA2;
	if (crossA1*crossA2 > 0)return false;

	return true;
}

// ---------------------------------------------------------------------------
// �֐���: bool Vector2::LineAndRectCollision(const Vector2& linePos, float mag, float deg, const Vector2& rectPos, const Vector2& rectSize)
// ����:   const Vector2& linePos ���̎n�_
// ����:   float mag ���̒���
// ����:   float deg ���̌X��
// ����:   const Vector2& rectPos ��`�̈ʒu
// ����:   const Vector2& rectSize ��`�̃T�C�Y
// �ߒl:   false = �ڐG���Ă��Ȃ�
// �ߒl:   true  = �ڐG���Ă���
// ����:   ������`�ɐڐG���Ă��邩����
// ---------------------------------------------------------------------------
bool Vector2::LineAndRectCollision(const Vector2& linePos, float mag, float deg, const Vector2& rectPos, const Vector2& rectSize)
{
	Vector2 linePosB(linePos.x + mag * degCos(deg), linePos.y + mag * degSin(deg));
	Vector2 a(rectPos.x - rectSize.x / 2, rectPos.y - rectSize.y / 2);
	Vector2 b(rectPos.x + rectSize.x / 2, rectPos.y - rectSize.y / 2);
	Vector2 c(rectPos.x - rectSize.x / 2, rectPos.y + rectSize.y / 2);
	Vector2 d(rectPos.x + rectSize.x / 2, rectPos.y + rectSize.y / 2);

	if (LineAndLineCollision(linePos, linePosB, a, b)) return true;
	if (LineAndLineCollision(linePos, linePosB, b, c)) return true;
	if (LineAndLineCollision(linePos, linePosB, c, d)) return true;
	if (LineAndLineCollision(linePos, linePosB, d, a)) return true;

	return false;
}

float Vector2::degArctan(const Vector2& origin, const Vector2& target)
{
	return ::degArctan(target.x - origin.x, target.y - origin.y);
}

float Vector2::degArctan(const Vector2 & vec)
{
	return ::degArctan(vec.x, vec.y);
}

const Vector2 operator*(const float a, const Vector2& v)
{
	return Vector2(v.x * a, v.y * a);
}

const Vector2 operator*(const Vector2 & v, const float a)
{
	return Vector2(v.x * a, v.y * a);
}

const Vector2 operator+(const Vector2 & v1, const Vector2 & v2)
{
	return Vector2(v1.x + v2.x, v1.y + v2.y);
}
