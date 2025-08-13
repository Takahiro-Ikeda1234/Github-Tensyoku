//=============================================================================
//
// [obj_Background.cpp] 背景オブジェクト
//
//=============================================================================

//インクルードファイル
#include"Scene.h"
#include"ComponentAnimationSprite.h"
#include"TextureData.h"
#include"myMath.h"
#include"obj_screenBackground.h"
#include"ini.h"
#include"CameraManager.h"

// ---------------------------------------------------------------------------
// 関数名: Background::Background(TextureTag tag, Scene* scene)
// 引数:   TextureTag tag 利用する画像のタグ
// 引数:   Scene* scene シーンのポインタ
// 説明:   Backgroundのコンストラクタ
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
