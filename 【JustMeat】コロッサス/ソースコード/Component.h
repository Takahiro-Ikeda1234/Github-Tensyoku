//=============================================================================
//
// [Component.h] �R���|�[�l���g���ʓ���
//
//=============================================================================
#ifndef _COMPONENT_H_
#define _COMPONENT_H_


#define UPDATE_PRIORITY_OBJECT 100

//�C���N���[�h�t�@�C��

//�N���X��`
class Component
{
public:
	Component(class AbstractObject* owner, int updatePriority = UPDATE_PRIORITY_OBJECT);
	virtual ~Component();
	virtual void Update(float deltaTime) = 0;
	int GetUpdatePriority() const { return mUpdatePriority; }
	bool GetResistPause() { return mResistPause; }
	bool SetResistPause(bool resist) { mResistPause = resist; }
protected:
	class AbstractObject* mOwner;	//���L��
	int mUpdatePriority;	//�X�V�D��x
	bool mResistPause = false;	//�ꎞ��~����
};
#endif