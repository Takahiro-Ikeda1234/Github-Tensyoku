#include "obj_wall.h"
#include "StageManager.h"
#include "ComponentAnimationSprite.h"
Wall::Wall(Scene * scene, int indexX, int indexY)
	: Object(scene, StageManager::ToPosition(indexX, indexY))
	, mIndexX(indexX)
	, mIndexY(indexY)
{
	SetObjectType(Type::eWall);
	mCASprite = new ComponentAnimationSprite(this, TextureTag::WallClimbable, LAYER_OBJECT + 9);
	mCASprite->SetScale(1.02f, 1.02f);
	GetScene()->GetStageManager()->SetStageData(StageManager::ObjectLayer::eStageObjectLayerWall, 1, mIndexX, mIndexY);
}

Wall::Wall(Scene * scene, Vector2 pos)
	: Wall(scene, StageManager::ToIndexX(pos.x), StageManager::ToIndexY(pos.y))
{
}

Wall::~Wall()
{
	GetScene()->GetStageManager()->SetStageData(StageManager::ObjectLayer::eStageObjectLayerWall, 0, mIndexX, mIndexY);
}
