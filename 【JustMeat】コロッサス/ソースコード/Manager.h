//=============================================================================
//
// [Manager.h] マネージャー共通処理
//
//=============================================================================
#ifndef _MANAGER_H_
#define _MANAGER_H_

//クラス定義
class Manager
{
public:
	Manager(class Scene* scene);
	virtual void Update(float deltaTime) = 0;
protected:
	class Scene* mScene;
};
#endif