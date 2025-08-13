//=============================================================================
//
// [Parameter.cpp] �p�����[�^�N���X
//
//=============================================================================

//�C���N���[�h�t�@�C��
#include "Parameter.h"

// ---------------------------------------------------------------------------
// �֐���: bool Parameter::Get(std::string key, int* result)
// ����:   std::string key
// ����:   int* result �L�[�ɑΉ������l
// �ߒl:   true = �ǂݍ��ݐ��� false = �ǂݍ��ݎ��s
// ����:   �ʃV�[���Őݒ肳�ꂽ�l�̎擾
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
