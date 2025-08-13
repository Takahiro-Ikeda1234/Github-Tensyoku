//=============================================================================
//
// [Parameter.h] パラメータクラス
//
//=============================================================================
#ifndef _PARAMETER_H_
#define _PARAMETER_H_

//インクルードファイル
#include<unordered_map>

//クラス定義
class Parameter
{
public:
	Parameter() {};
	~Parameter() {};

	void Set(std::string key, int value) { mParam[key] = value; }
	void SetF(std::string key, float value) { mFParam[key] = value; }
	bool Get(std::string key, int* result);
	bool GetF(std::string key, float* result);
	void Clear() { mParam.clear(); }
private:
	std::unordered_map<std::string, int> mParam;
	std::unordered_map<std::string, float> mFParam;
};

#endif