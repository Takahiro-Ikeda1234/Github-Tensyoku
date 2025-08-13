//=============================================================================
//
// [Component.cpp] �R���|�[�l���g���ʏ���
//
//=============================================================================

//�C���N���[�h�t�@�C��
#include"Scene.h"
#include"AbstractObject.h"
#include"Component.h"

Component::Component(AbstractObject* owner, int updatePriority)
	: mOwner(owner)
	, mUpdatePriority(updatePriority)
{
	//owner�̃R���|�[�l���g�ɒǉ�
	mOwner->AddComponent(this);
}

Component::~Component()
{
	//owner�̃R���|�[�l���g�ꗗ���珜��
	mOwner->RemoveComponent(this);
}
