//=============================================================================
//
// [Counter.cpp] 時間カウント用クラス
//
//=============================================================================

//インクルードファイル
#include"Counter.h"

// ---------------------------------------------------------------------------
// 関数名: bool Counter::IsEnd()
// 戻値:   true  = 残り時間が0
// 戻値:   false = 0以外
// 説明:   残り時間が0であればtrueを返す関数
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
// 関数名: bool Counter::IsFinish()
// 戻値:   true  = 残り時間が0になった瞬間
// 戻値:   false = それ以外
// 説明:   残り時間が0になって1回だけtrueを返す関数
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

//-=の演算子で残り時間を減らせるようにする
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
