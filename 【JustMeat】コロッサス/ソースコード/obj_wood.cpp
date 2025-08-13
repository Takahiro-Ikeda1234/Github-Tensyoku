//#include "obj_wood.h"
//#include "StageManager.h"
//#include "ComponentAnimationSprite.h"
//
//#include"obj_player.h"
//Wood::Wood(Scene * scene, Vector2 pos)
//	: Object(scene, pos)
//{
//	mCASprite = new ComponentAnimationSprite(this, TextureTag::Block, LAYER_OBJECT);
//	SetSize(Vector2(4, 1)  * BLOCKSIZE);
//	Translate(Vector2(0.5f, 0) * BLOCKSIZE);
//}
//
//Wood::Wood(Scene * scene, int indexX, int indexY)
//	: Wood(scene, StageManager::ToPosition(indexX, indexY))
//{
//}
//
//void Wood::UpdateObject(float deltaTime)
//{
//	Vector2 playerPos = GetScene()->GetPlayer()->GetPosition();
//	Vector2 playerColSize = GetScene()->GetPlayer()->GetColSize();
//	if (Vector2::RectCollision(playerPos, playerColSize, GetPosition(), GetColSize()))
//	{
//		GetScene()->GetPlayer()->WoodJump();
//	}
//}
