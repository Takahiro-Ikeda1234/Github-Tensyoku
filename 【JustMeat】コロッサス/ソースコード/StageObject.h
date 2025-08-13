#pragma once
#include"Object.h"

#include "StageManager.h"
#include "ComponentAnimationSprite.h"

class StageObject : public Object
{
public:
	StageObject(Scene* scene, Vector2 pos);
	StageObject(Scene* scene, int indexX, int indexY);
	~StageObject() {}
	virtual void SaveCheckPointData() override;
	virtual void LoadCheckPointData() override;
	virtual void UpdateObject(float deltaTime) final;
	virtual void UpdateStageObject(float deltaTime) = 0;
	void Completed() { mDone = true; }	//�M�~�b�N���I�������炱������s����(���ꂽ��ԂȂǂŌŒ肳���)
	virtual void EnterGolemArm() {}
	virtual void OnGolemArm() {}
	virtual void ExitGolemArm() {}
	bool IsEnterGolemArm() { if (mDone) return false; return mEnterGolemArm; }
	bool IsOnGolemArm() { if (mDone) return false; return mOnGolemArm; }
	bool IsExitGolemArm() { if (mDone) return false; return mExitGolemArm; }

	class AbstractGolemArm* GetHitGolemArm() { return mHitGolemArm; }
private:
	bool mEnterGolemArm = false;
	bool mOnGolemArm = false;
	bool mExitGolemArm = false;
	class AbstractGolemArm* mHitGolemArm;

	bool mDone = false;	//true�Ȃ�`�F�b�N�|�C���g�ɂ�郍�[�h���s��Ȃ�

	//�`�F�b�N�|�C���g���̃f�[�^
	bool mCPDone = false;
	float mCPRotationDeg;	//��]�p�x
	Vector2 mCPPosition;	//���݈ʒu
	State mCPState;			//���݂̏��
	Vector2 mCPSize;		//�T�C�Y
	Type mCPType;
};