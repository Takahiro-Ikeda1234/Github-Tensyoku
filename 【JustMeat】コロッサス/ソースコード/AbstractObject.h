//=============================================================================
//
// [Object.h] �I�u�W�F�N�g���ʏ���
//
//=============================================================================
#ifndef _ABSTRACT_OBJECT_H_
#define _ABSTRACT_OBJECT_H_

//�C���N���[�h�t�@�C��
#include<vector>
#include"Vector2.h"
#include"objectData.h"
#include"config.h"

class Scene;

//�N���X��`
class AbstractObject
{
public:
	enum class State	//�I�u�W�F�N�g�̏��
	{
		eActive = 0,	//�A�N�e�B�u
		eInactive,	//��A�N�e�B�u
		eDead,	//�폜�҂�
	};

	enum class Type	//�I�u�W�F�N�g�̎��
	{
		eNone = 0,
		eSolid,
		ePlayer,
		eEnemy,
		eBullet,
		eItem,
		eBackground,
		eWall,
		eForeground,
		eEditorProp,
		eWarpGate,
		eLaser,
	};
	enum class Dir	//�I�u�W�F�N�g�̌���
	{
		eLeft = -1,
		eRight = 1,
	};
	AbstractObject(Scene* scene);
	virtual ~AbstractObject();

	void Update(float deltaTime);
	void UpdateComponents(float deltaTime);
	virtual void UpdateObject(float deltaTime) = 0;

	void AddComponent(class Component* component);
	void RemoveComponent(class Component* component);

	Scene* GetScene() { return mScene; }
	Vector2 GetPosition() { return mPosition; }
	Vector2 GetSize() { return mSize * mScale; }
	float GetRotation() { return mRotationDeg; }
	Vector2 GetColSize() { return mColSize * mScale; }
	float GetActiveRange() { return mActiveRange; }
	State GetState() { return mState; }
	virtual void SetPosition(Vector2 pos) { mPosition = pos; }
	void SetSize(Vector2 size) { mSize = size; SetColSize(size); }
	void SetRotation(float rotation) { mRotationDeg = rotation; }
	void Rotation(float rotation) { mRotationDeg += rotation; }
	void SetColSize(Vector2 colSize) { mColSize.x = colSize.x / 2; mColSize.y = colSize.y / 2; if (mColSize.x > mColSize.y) { mActiveRange = mColSize.x * 2; } else mActiveRange = mColSize.y * 2; }
	void SetScale(Vector2 scale) { if (scale.x < 0.1f) { scale.x = 0.1f; }if (scale.y < 0.1f) { scale.y = 0.1f; } mScale = scale; }
	void SetScale(float scale) { if (scale < 0.1f) { scale = 0.1f; } mScale.x = scale; mScale.y = scale; }
	void SetState(State state) { mState = state; }
	virtual void Translate(float x, float y) { mPosition.x += x, mPosition.y += y; }
	virtual void Translate(Vector2 v) { mPosition.x += v.x, mPosition.y += v.y; }

	Type GetObjectType() { return mObjectType; }
	virtual void SetObjectType(Type type) { mObjectType = type; }
	Dir GetDirection() { return mDir; }
	void SetDirection(Dir dir) { mDir = dir; }
	bool GetResistPause() { return mResistPause; }
	void SetResistPause(bool resist) { mResistPause = resist; }
	void ResetMoved() { mMoved = Vector2(0, 0); }

	virtual Vector2 GetVelocity() { return mMoved; }
	virtual void SaveCheckPointData() {};
	virtual void LoadCheckPointData() {};
	void ResetPrevPos() { mPrevPos = mPosition; mMoved = Vector2(0, 0); }
private:
	float mRotationDeg;	//��]�p�x
	Vector2 mPosition;	//���݈ʒu
	Vector2 mPrevPos;
	Vector2 mMoved = Vector2(0, 0);
	State mState;	//���݂̏��
	Scene* mScene;	//�V�[���̃|�C���^
	Vector2 mSize;	//�O���T�C�Y
	Vector2 mColSize;	//�����蔻��T�C�Y
	Vector2 mScale;	//�傫���␳
	float mActiveRange;	//����������Ȃ�����
	std::vector<class Component*> mComponents;	//���L�R���|�[�l���g�z��
	Dir mDir;	//���݂̌���
	Type mObjectType;	//�I�u�W�F�N�g�̎��
	bool mResistPause = false;
};
#endif