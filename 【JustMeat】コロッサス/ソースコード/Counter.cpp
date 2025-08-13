//=============================================================================
//
// [Counter.cpp] ���ԃJ�E���g�p�N���X
//
//=============================================================================

//�C���N���[�h�t�@�C��
#include"Counter.h"

// ---------------------------------------------------------------------------
// �֐���: bool Counter::IsEnd()
// �ߒl:   true  = �c�莞�Ԃ�0
// �ߒl:   false = 0�ȊO
// ����:   �c�莞�Ԃ�0�ł����true��Ԃ��֐�
// ---------------------------------------------------------------------------
bool Counter::IsEnd()
{
	if (mMax == 0)
	{
		return true;
	}
	return false;
}

// ---------------------------------------------------------------------------
// �֐���: bool Counter::IsFinish()
// �ߒl:   true  = �c�莞�Ԃ�0�ɂȂ����u��
// �ߒl:   false = ����ȊO
// ����:   �c�莞�Ԃ�0�ɂȂ���1�񂾂�true��Ԃ��֐�
// ---------------------------------------------------------------------------
bool Counter::IsFinish()
{
	if (mIsFinish && mMax == 0)
	{
		mIsFinish = false;
		return true;
	}
	return false;
}

//-=�̉��Z�q�Ŏc�莞�Ԃ����点��悤�ɂ���
Counter& Counter::operator-=(float deltaTime)
{
	if (mIsFinish) mIsFinish = false;
	mLeft -= deltaTime;
	if (mLeft <= 0)
	{
		if (mMax != 0) mIsFinish = true;
		mLeft = mMax = 0;
	}
	return *this;
}

Counter & Counter::operator+=(float deltaTime)
{
	mLeft += deltaTime;
	if (mLeft >= mMax)
	{
		mLeft = mMax;
	}
	return *this;
}

LoopCounter& LoopCounter::operator-=(float deltaTime)
{
	if (mIsFinish) mIsFinish = false;
	mLeft -= deltaTime;
	if (mLeft <= 0)
	{
		if (mMax != 0) mIsFinish = true;
		mLeft += mMax;
	}
	return *this;
}
