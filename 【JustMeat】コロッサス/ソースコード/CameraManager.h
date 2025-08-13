//=============================================================================
//
// [CameraManager.h] �J��������E�J�����h�炵����
//
//=============================================================================
#ifndef _CAMERA_MANAGER_H_
#define _CAMERA_MANAGER_H_

//�C���N���[�h�t�@�C��
#include"Manager.h"
#include"Vector2.h"
#include"Counter.h"
#include"Windows_core.h"
#include"myMath.h"
#include"draw.h"
#include"Object.h"
#include"config.h"
class Object;

//�N���X��`
class CameraManager : public Manager
{
public:
	CameraManager(class Scene* scene);
	virtual ~CameraManager() {};
	virtual void Update(float deltaTime) override;
	Vector2 GetPosition() { return mPos; }
	void SetPosition(Vector2 pos) { mPos = pos; }
	void SetPositionX(float x) { mPos.x = x; }
	void SetPositionY(float y) { mPos.y = y; }
	Vector2 ToScreenPosition(Vector2 worldPos);
	Vector2 ToWorldPosition(Vector2 screenPos);
	int InCamera(const Vector2* worldPos, const Vector2* size);
	void SetCameraShake(float shakeX, float shakeY, float shaketime) { if (mCameraShakeTime.GetLeftTime() > shaketime) return;  mCameraShakeTime = shaketime; mBaseShakeX = shakeX; mBaseShakeY = shakeY; };
	float GetShakeX() { return mBaseShakeX; }
	float GetShakeY() { return mBaseShakeY; }
	float GetShakeTime() { return mCameraShakeTime.GetLeftTime(); }
	bool GetStartStay() { return !mStartStay.IsEnd(); }
	void SetFollowObject(Object* object) { mFollow = object; SetPosition(mFollow->GetPosition()); }
	void SetWASDMode(bool mode) { mModeWASD = mode; }
	float GetRotation() { return mRotation; }
	void SetRotation(float rotation) { mRotation = rotation; mSin = degSin(mRotation); mCos = degCos(mRotation); mSin_360MinusTheta = -mSin; mCos_360MinusTheta = degCos(360 - mRotation); }
	void Translate(float x, float y) { mPos.x += x; mPos.y += y; }
	float GetCameraWidth();
	float GetCameraHeight();
	void SetCanZoom(bool canZoom) { mCanZoom = canZoom; }
	void Zoom(float zoom);
	float GetZoom() { return mCameraScale; }
	void SetOffset(float zoom, Vector2 offset, float seekTime, float stopTime, float returnTime) { mOffsetZoomBackup = mCameraScale; mOffsetZoom = zoom;  mOffset = offset; mOffsetSeekTime = seekTime; mOffsetStopTime = stopTime; mOffsetReturnTime = returnTime; }
	void ResetLimit();
	void SetLimitTop(float y) { mLimitTop = y + GetCameraHeight() / 2; }
	void SetLimitBottom(float y) { mLimitBottom = y - GetCameraHeight() / 2; }
	void SetLimitLeft(float x) { mLimitLeft = x + GetCameraWidth() / 2; }
	void SetLimitRight(float x) { mLimitRight = x - GetCameraWidth() / 2; }
	void SetFollowCenter(bool flag) { mFollowCenter = flag; }
	static const int MAX_SPEED = BLOCKSIZE * 50;
	void UpdateLimit();
	typedef struct
	{
		float left;
		float right;
		float top;
		float bottom;
	}Rect;
private:
	Rect GetCameraRect();
	Vector2 mPos;	//�J�����ʒu
	Vector2 mOffset;	//���炵����
	Counter mOffsetSeekTime;	//���炵����
	Counter mOffsetStopTime;	//��������
	Counter mOffsetReturnTime;	//���炵(�߂�)����
	float mOffsetZoomBackup;
	float mOffsetZoom;

	int mWindowWidth;
	int mWindowHeight;

	Counter mCameraShakeTime;	//�h�ꎞ��
	float mBaseShakeX;	//��{�ƂȂ�h��l
	float mBaseShakeY;	//��{�ƂȂ�h��l
	float mShakeX;	//���݂̗h��l
	float mShakeY;	//���݂̗h��l
	Counter mStartStay;	//�X�e�[�W�J�n���̃J�����Î~����
	Object* mFollow;
	bool mFollowCenter = false;
	float mRotation;	//�J�����̉�]�p�x
	float mSin;	//degSin(mRotation)�̒l
	float mCos;	//degCos(mRotation)�̒l
	float mSin_360MinusTheta;	//degSin(360 - mRotation)�̒l
	float mCos_360MinusTheta;	//degCos(360 - mRotation)�̒l
	float mCameraScale;	//�J�����̊g�嗦

	bool mModeWASD;	//WASD�ŃJ�������삷����
	void UpdateModeWASD(float deltaTime);

	bool mCanZoom;

	float mLimitLeft;
	float mLimitRight;
	float mLimitTop;
	float mLimitBottom;

public:
	void SetScrollSpeed(Vector2 speed) { mScrollSpeed = speed; }
private:
	Vector2 mScrollSpeed;

};
#endif