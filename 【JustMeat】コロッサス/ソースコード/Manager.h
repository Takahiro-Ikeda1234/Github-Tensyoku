//=============================================================================
//
// [Manager.h] �}�l�[�W���[���ʏ���
//
//=============================================================================
#ifndef _MANAGER_H_
#define _MANAGER_H_

//�N���X��`
class Manager
{
public:
	Manager(class Scene* scene);
	virtual void Update(float deltaTime) = 0;
protected:
	class Scene* mScene;
};
#endif