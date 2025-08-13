//=============================================================================
//
// [Object.h] �I�u�W�F�N�g���ʏ���
//
//=============================================================================
#ifndef _OBJECT_H_
#define _OBJECT_H_

//�C���N���[�h�t�@�C��
#include<vector>
#include"Vector2.h"
#include"AbstractObject.h"
//�N���X��`
class Object : public AbstractObject
{
public:
	Object(Scene* scene, Vector2 position);
	virtual ~Object();
	void SetDrawMode(int drawMode);
	void SetColor(unsigned color);
	void SceneChangeEvent();
	class ComponentAnimationSprite* GetCASprite() { return mCASprite; }
	bool IsBlock() { return mIsBlock; }
	bool IsExportTexture() { return mIsExportTexture; }
protected:
	class ComponentAnimationSprite* mCASprite;
	bool mIsForeground;
	bool mIsBlock = false;
	bool mIsExportTexture = false;
};
#endif