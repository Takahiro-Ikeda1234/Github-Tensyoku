//=============================================================================
//
// [Parameter.cpp] パラメータクラス
//
//=============================================================================

//インクルードファイル
#include "Parameter.h"

// ---------------------------------------------------------------------------
// 関数名: bool Parameter::Get(std::string key, int* result)
// 引数:   std::string key
// 引数:   int* result キーに対応した値
// 戻値:   true = 読み込み成功 false = 読み込み失敗
// 説明:   別シーンで設定された値の取得
// ---------------------------------------------------------------------------
bool Parameter::Get(std::string key, int* result)
{
	auto it = mParam.find(key);
	if (it != mParam.end())
	{
		*result =  it->second;
		return true;
	}
	return false;
}

bool Parameter::GetF(std::string key, float *result)
{
	{
		auto it = mFParam.find(key);
		if (it != mFParam.end())
		{
			*result = it->second;
			return true;
		}
	}

	{
		auto it = mParam.find(key);
		if (it != mParam.end())
		{
			*result = it->second;
			return true;
		}
	}
	return false;
}
