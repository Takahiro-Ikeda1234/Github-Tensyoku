//=============================================================================
//
// [CameraManager.cpp] �J��������E�J�����h�炵����
//
//=============================================================================

//�C���N���[�h�t�@�C��
#include"CameraManager.h"
#include"Scene.h"
#include"myMath.h"
#include"ini.h"
#include"obj_player.h"
#include"keyboard.h"
#include"mouse.h"
#include"SceneManager.h"
#include"StageManager.h"
#include"EventManager.h"
// ---------------------------------------------------------------------------
// �֐���: CameraManager::CameraManager(Scene* scene)
// ����:   Scene* scene �V�[���̃|�C���^
// ����:   CameraManager�̃R���X�g���N�^
// ---------------------------------------------------------------------------

CameraManager::CameraManager(Scene* scene)
	: Manager(scene)
	, mPos(0, 0)
	, mCameraShakeTime(0)
	, mShakeX(0)
	, mShakeY(0)
	, mBaseShakeX(0)
	, mBaseShakeY(0)
	, mStartStay(4)
	, mFollow(nullptr)
	, mModeWASD(false)
	, mCameraScale(0.6f)
	, mOffsetZoomBackup(0.6f)
	, mCanZoom(false)
	, mScrollSpeed(0, 0)

{
	mWindowHeight = GetConfigInt("Windows", "Height");
	mWindowWidth = GetConfigInt("Windows", "Width");
	SetRotation(0);
	SetTextureScale(1.0f / mCameraScale);
	ResetLimit();

}

// ---------------------------------------------------------------------------
// �֐���: void CameraManager::Update(float deltaTime)
// ����:   float deltaTime 
// ����:   �J�����̍X�V����
// ---------------------------------------------------------------------------
void CameraManager::Update(float deltaTime)
{
	if (mCanZoom)
	{
		if (Mouse::Inst()->GetWheelMove() <= -120)
			Zoom(1);
		if (Mouse::Inst()->GetWheelMove() >= 120)
			Zoom(-1);
	}
	if (mModeWASD)
	{
		UpdateModeWASD(deltaTime);
		return;
	}
	//SetPosition(GetPosition() + mScrollSpeed * deltaTime);
	mOffsetSeekTime -= deltaTime;
	if (mOffsetSeekTime.IsEnd()) mOffsetStopTime -= deltaTime;
	if (mOffsetStopTime.IsEnd()) mOffsetReturnTime -= deltaTime;

	if (mFollow)
	{
		UpdateLimit();
		if (!mOffsetSeekTime.IsEnd())
		{
			Vector2 move = mOffset * (1 - mOffsetSeekTime.Percentage());
			SetPosition(move + Vector2(mFollow->GetPosition().x, mFollow->GetPosition().y));
			mCameraScale = mOffsetZoomBackup + (mOffsetZoom - mOffsetZoomBackup) * (1 - mOffsetSeekTime.Percentage());
		}
		else if (!mOffsetStopTime.IsEnd())
		{
			Vector2 move = mOffset;
			SetPosition(move + Vector2(mFollow->GetPosition().x, mFollow->GetPosition().y));
			mCameraScale = mOffsetZoom;
		}
		else if (!mOffsetReturnTime.IsEnd())
		{
			Vector2 move = mOffset * mOffsetReturnTime.Percentage();
			SetPosition(move + Vector2(mFollow->GetPosition().x, mFollow->GetPosition().y));
			mCameraScale = mOffsetZoomBackup + (mOffsetZoom - mOffsetZoomBackup) * mOffsetReturnTime.Percentage();
		}
		else
		{
			mCameraScale = mOffsetZoomBackup;
			Rect rect;
			const float TOLERANT_X = GetCameraWidth() * 0.15f;
			const float TOLETANT_Y = GetCameraHeight() * 0.15f;
			rect.left = mPos.x - TOLERANT_X;
			rect.right = mPos.x + TOLERANT_X;
			rect.top = mPos.y - TOLETANT_Y;
			rect.bottom = mPos.y + TOLETANT_Y;
			float x = mFollow->GetPosition().x;
			float y = mFollow->GetPosition().y;
			if (!InRect(rect, x, y))
			{
				if (x < rect.left)
				{
					float diff = x - rect.left;
					Translate(diff, 0);
				}
				else if (x > rect.right)
				{
					float diff = x - rect.right;
					Translate(diff, 0);
				}
				if (y < rect.top)
				{
					float diff = y - rect.top;
					Translate(0, diff);
				}
				else if (y > rect.bottom)
				{
					float diff = y - rect.bottom;
					Translate(0, diff);
				}
			}
			if (mFollowCenter)
			{
				Vector2 diff = (mFollow->GetPosition() - mPos);
				float dist = diff.Length();
				const float MAXSPEED = BLOCKSIZE * 10 * deltaTime;
				if (dist > MAXSPEED)
				{
					diff = diff / dist * MAXSPEED;
				}
				Translate(diff.x, diff.y);
			}
		}
	}
	SetTextureScale(1.0f / mCameraScale);
	if (mLimitLeft < mLimitRight)
	{
		if (GetPosition().x < mLimitLeft)
			SetPositionX(mLimitLeft);
		if (GetPosition().x > mLimitRight)
			SetPositionX(mLimitRight);
	}
	else
	{
		SetPositionX((mLimitLeft + mLimitRight) / 2);
	}
	if (mLimitTop < mLimitBottom)
	{
		if (GetPosition().y < mLimitTop)
			SetPositionY(mLimitTop);
		if (GetPosition().y > mLimitBottom)
			SetPositionY(mLimitBottom);
	}
	else
	{
		SetPositionY((mLimitTop + mLimitBottom) / 2);
	}
	//�h�ꏈ��
	if (!mCameraShakeTime.IsEnd())
	{
		mShakeX = randomFloat() * mBaseShakeX;/* degCos(90 * mCameraShakeTime.Percentage());*/
		mShakeY = randomFloat() * mBaseShakeY; /* degCos(90 * mCameraShakeTime.Percentage());*/
	}
	else
	{
		mBaseShakeX = 0;
		mBaseShakeY = 0;
		mShakeX = 0;
		mShakeY = 0;
	}

	mCameraShakeTime -= deltaTime;
	mStartStay -= deltaTime;
	//SetPosition(Vector2((int)GetPosition().x, (int)GetPosition().y));

}

// ---------------------------------------------------------------------------
// �֐���: Vector2 CameraManager::ToScreenPosition(Vector2 worldPos)
// ����:   Vector2 worldPos 
// �ߒl:   �ϊ����ꂽ���W
// ����:   ���[���h���W���X�N���[�����W�ւ̕ϊ�
// ---------------------------------------------------------------------------
Vector2 CameraManager::ToScreenPosition(Vector2 worldPos)
{
	Vector2 relational((worldPos.x - (mPos.x + mShakeX)) / GetCameraWidth() * mWindowWidth, (worldPos.y - (mPos.y + mShakeY)) / GetCameraHeight() * mWindowHeight);
	Vector2 screenPos;
	screenPos.x = relational.x * mCos - relational.y * mSin;
	screenPos.y = relational.x * mSin + relational.y * mCos;
	return screenPos;
}
// ---------------------------------------------------------------------------
// �֐���: Vector2 CameraManager::ToWorldPosition(Vector2 screenPos)
// ����:   Vector2 screenPos 
// �ߒl:   �ϊ����ꂽ���W
// ����:   �X�N���[�����W�����[���h���W�ւ̕ϊ�
// ---------------------------------------------------------------------------
Vector2 CameraManager::ToWorldPosition(Vector2 screenPos)
{
	Vector2 screenToCamera((screenPos.x - mWindowWidth / 2) / mWindowWidth * GetCameraWidth(), (screenPos.y - mWindowHeight / 2) / mWindowHeight * GetCameraHeight());
	Vector2 cameraToRelational(screenToCamera.x * mCos_360MinusTheta - screenToCamera.y * mSin_360MinusTheta, screenToCamera.x * mSin_360MinusTheta + screenToCamera.y * mCos_360MinusTheta);
	Vector2 relationalToWorld(cameraToRelational.x + mPos.x + mShakeX, cameraToRelational.y + mPos.y + mShakeY);
	return relationalToWorld;
}

// ---------------------------------------------------------------------------
// �֐���: int CameraManager::InCamera(const Vector2* worldPos, const Vector2* size)
// ����:   const Vector2* worldPos ���肵�������S���W
// ����:   const Vector2* size ���肵�����I�u�W�F�N�g�̃T�C�Y
// �ߒl:   �J�������ɂ����0�A�J�������E�ł����1�A���܂��͏�A���ł����-1
// ����:   �Ώۂ̃I�u�W�F�N�g���J�����ɉf���Ă��邩�̔���
// ---------------------------------------------------------------------------
int CameraManager::InCamera(const Vector2* worldPos, const Vector2* size)
{
	//����
	if (worldPos->x + size->x < GetCameraRect().left + mPos.x)
	{
		return -1;
	}
	//�E��
	if (GetCameraRect().right + mPos.x < worldPos->x - size->x)
	{
		return 1;
	}
	//�㉺
	if (worldPos->y + size->y < GetCameraRect().top + mPos.y || GetCameraRect().bottom + mPos.y < worldPos->y - size->y)
	{
		return -1;
	}
	return 0;
}

float CameraManager::GetCameraWidth()
{
	return gDefaultWidth * mCameraScale;
}

float CameraManager::GetCameraHeight()
{
	return gDefaultHeight * mCameraScale;
}
void CameraManager::Zoom(float zoom)
{
	if(mCameraScale <= 1)
		mCameraScale += zoom / 10;
	else if(mCameraScale <= 5)
		mCameraScale += zoom / 5;
	else if (mCameraScale <= 10)
		mCameraScale += zoom / 2.5f;
	
	if (mCameraScale < 0.1f) mCameraScale = 0.1f;
	if (mCameraScale > 10.0f) mCameraScale = 10.0f;
	SetTextureScale(1.0f / mCameraScale);
}
void CameraManager::ResetLimit()
{
	if (mScene->GetStageManager())
	{
		mLimitLeft = -GetCameraWidth() / 2;
		mLimitTop = -GetCameraHeight() / 2;
	}
	else
	{
		mLimitLeft = - BLOCKSIZE * 1000;
		mLimitTop = - BLOCKSIZE * 1000;
	}
	if (mScene->GetStageManager())
	{
		mLimitRight = mScene->GetStageManager()->LimitX();
		mLimitBottom = mScene->GetStageManager()->LimitY();
	}
	else
	{
		mLimitRight = BLOCKSIZE * 1000;
		mLimitBottom = BLOCKSIZE * 1000;
	}
}
void CameraManager::UpdateLimit()
{
	mLimitBottom = (mScene->GetStageManager()->LimitY() - BLOCKSIZE * 1) - GetCameraHeight() / 2;
}
CameraManager::Rect CameraManager::GetCameraRect()
{
	return Rect{ -GetCameraWidth() / 2, GetCameraWidth() / 2, -GetCameraHeight() / 2, GetCameraHeight() / 2 };
}
void CameraManager::UpdateModeWASD(float deltaTime)
{
	float moveX = 0;
	float moveY = 0;
	if (Keyboard::Inst()->KeyDown(DIK_W) > 0)
	{
		moveY -= 5000;
	}
	if (Keyboard::Inst()->KeyDown(DIK_S) > 0)
	{
		moveY += 5000;
	}
	if (Keyboard::Inst()->KeyDown(DIK_A) > 0)
	{
		moveX -= 5000;
	}
	if (Keyboard::Inst()->KeyDown(DIK_D) > 0)
	{
		moveX += 5000;
	}
	Translate(moveX * deltaTime, moveY * deltaTime);
}
