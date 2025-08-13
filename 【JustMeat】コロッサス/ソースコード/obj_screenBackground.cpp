//=============================================================================
//
// [obj_Background.cpp] �w�i�I�u�W�F�N�g
//
//=============================================================================

//�C���N���[�h�t�@�C��
#include"Scene.h"
#include"ComponentAnimationSprite.h"
#include"TextureData.h"
#include"myMath.h"
#include"obj_screenBackground.h"
#include"ini.h"
#include"CameraManager.h"

// ---------------------------------------------------------------------------
// �֐���: Background::Background(TextureTag tag, Scene* scene)
// ����:   TextureTag tag ���p����摜�̃^�O
// ����:   Scene* scene �V�[���̃|�C���^
// ����:   Background�̃R���X�g���N�^
// ---------------------------------------------------------------------------
ScreenBackground::ScreenBackground(Scene* scene, TextureTag tag)
	: Object(scene, Vector2(0, 0))
{
	mCASprite = new ComponentAnimationSprite(this, tag, LAYER_BACKGROUND);
	float cameraWidth = GetScene()->GetCameraManager()->GetCameraWidth();
	float cameraHeight = GetScene()->GetCameraManager()->GetCameraHeight();
	SetSize(Vector2(cameraWidth, cameraHeight));
}

void ScreenBackground::UpdateObject(float deltaTime)
{
	//SetPosition(GetScene()->GetCameraManager()->GetPosition());
}
