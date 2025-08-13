//#include "obj_golemHead.h"
//#include "StageManager.h"
//#include "ComponentAnimationSprite.h"
//#include "Scene.h"
//#include "SceneManager.h"
//#include "obj_player.h"
//#include "AudioData.h"
////–¢Žg—pB
//GolemHead::GolemHead(Scene * scene, Vector2 pos)
//	: Object(scene, pos)
//{
//	mCASprite = new ComponentAnimationSprite(this, TextureTag::Head);
//	mCore = new GolemHeadCore(scene, GetPosition() + Vector2(0, BLOCKSIZE * -3.5f));
//	SetSize(Vector2(BLOCKSIZE, BLOCKSIZE) * 7);
//}
//
//GolemHead::GolemHead(Scene * scene, int indexX, int indexY)
//	: GolemHead(scene, StageManager::ToPosition(indexX, indexY))
//{
//
//}
//
//void GolemHead::UpdateObject(float deltaTime)
//{
//	if (mCore->HitPlayer() && !mIsDestroyed)
//	{
//
//		mIsDestroyed = true;
//		Parameter param;
//		SetFadeout(3, 100, 0.5f);
//		GetScene()->GetSceneManager()->SceneChange(SceneManager::SceneID::eEnding, &param, 0.5f, 1.0f, 1.0f);
//
//	}
//}
//
//GolemHeadCore::GolemHeadCore(Scene * scene, Vector2 pos)
//	: Object(scene, pos)
//{
//	mCASprite = new ComponentAnimationSprite(this, TextureTag::HeadButton);
//}
//
//bool GolemHeadCore::HitPlayer()
//{
//	Vector2 playerPos = GetScene()->GetPlayer()->GetPosition();
//	Vector2 playerCol = GetScene()->GetPlayer()->GetColSize();
//	if (Vector2::RectCollision(playerPos, playerCol, GetPosition(), GetColSize()))
//	{
//		return true;
//	}
//	return false;
//}
