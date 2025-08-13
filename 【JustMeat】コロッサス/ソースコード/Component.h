//=============================================================================
//
// [Component.h] コンポーネント共通動作
//
//=============================================================================
#ifndef _COMPONENT_H_
#define _COMPONENT_H_


#define UPDATE_PRIORITY_OBJECT 100

//インクルードファイル

//クラス定義
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
	class AbstractObject* mOwner;	//所有者
	int mUpdatePriority;	//更新優先度
	bool mResistPause = false;	//一時停止無効
};
#endif