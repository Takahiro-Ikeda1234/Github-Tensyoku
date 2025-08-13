#include "obj_abstractBlock.h"
#include "ComponentAnimationSprite.h"
#include "StageManager.h"
#include "ComponentClickEvent.h"


//id -1: 透明ブロック 0: ブロック 1: 土ブロック 2~3(ブロックステージ別)  4:遺跡ブロック 5~6(遺跡ステージ別)
//+2している箇所はid-1の無ブロックに対応するため
AbstractBlock::AbstractBlock(Scene * scene, int indexX, int indexY, int id)
	: Object(scene, StageManager::ToPosition(indexX, indexY))
	, mIndexX(indexX)
	, mIndexY(indexY)
	, mID(id)
{
	SetObjectType(Type::eSolid);
	mIsBlock = true;
	mIsExportTexture = true;
	if (mID == 0)
	{
		if (3 <= GetScene()->GetStageManager()->GetCurrentStageID() && GetScene()->GetStageManager()->GetCurrentStageID() <= 5)
		{
			mID = 2;
		}
		if (6 <= GetScene()->GetStageManager()->GetCurrentStageID() && GetScene()->GetStageManager()->GetCurrentStageID() <= 8)
		{
			mID = 3;
		}
	}
	if (mID == 4)
	{
		if (3 <= GetScene()->GetStageManager()->GetCurrentStageID() && GetScene()->GetStageManager()->GetCurrentStageID() <= 5)
		{
			mID = 5;
		}
		if (6 <= GetScene()->GetStageManager()->GetCurrentStageID() && GetScene()->GetStageManager()->GetCurrentStageID() <= 8)
		{
			mID = 6;
		}
	}
	GetScene()->GetStageManager()->SetStageData(StageManager::eStageObjectLayerBlock, mID + 2, mIndexX, mIndexY);

	if (id == -1)
	{
		SetState(State::eDead);
	}


}

AbstractBlock::AbstractBlock(Scene * scene, Vector2 pos, int id)
	: AbstractBlock(scene, StageManager::ToIndexX(pos.x), StageManager::ToIndexY(pos.y), id)
{

}

AbstractBlock::~AbstractBlock()
{
	if (mID != -1)
	{
		GetScene()->GetStageManager()->SetStageData(StageManager::eStageObjectLayerBlock, 0, mIndexX, mIndexY);
	}
}

void AbstractBlock::UpdateBlockTexture()
{	//画像更新
	if (mBlockSprite == nullptr) return;
	unsigned pattern = 0b00000;
	if (mIndexY == 0 || mIndexY == 11) pattern |= 0b10000;
	int t = GetScene()->GetStageManager()->GetStageData(StageManager::ObjectLayer::eStageObjectLayerBlock, mIndexX, mIndexY - 1);
	int b = GetScene()->GetStageManager()->GetStageData(StageManager::ObjectLayer::eStageObjectLayerBlock, mIndexX, mIndexY + 1);
	int l = GetScene()->GetStageManager()->GetStageData(StageManager::ObjectLayer::eStageObjectLayerBlock, mIndexX - 1, mIndexY);
	int r = GetScene()->GetStageManager()->GetStageData(StageManager::ObjectLayer::eStageObjectLayerBlock, mIndexX + 1, mIndexY);

	if (t == mID + 2 || t == 1) pattern |= 0b01000;
	if (b == mID + 2 || b == 1) pattern |= 0b00100;
	if (l == mID + 2 || l == 1) pattern |= 0b00010;
	if (r == mID + 2 || r == 1) pattern |= 0b00001;
	mBlockSprite->SetUseID(pattern);

	if (mCornerSprite == nullptr)return;
	unsigned pattern_corner = 0b1111;
	if ((pattern & 0b1010) == 0b1010)
	{
		int tl = GetScene()->GetStageManager()->GetStageData(StageManager::ObjectLayer::eStageObjectLayerBlock, mIndexX - 1, mIndexY - 1);
		if (tl != mID + 2 && tl != 1) pattern_corner ^= 0b0001;
	}

	if ((pattern & 0b1001) == 0b1001)
	{
		int tr = GetScene()->GetStageManager()->GetStageData(StageManager::ObjectLayer::eStageObjectLayerBlock, mIndexX + 1, mIndexY - 1);
		if (tr != mID + 2 && tr != 1) pattern_corner ^= 0b0010;
	}

	if ((pattern & 0b0110) == 0b0110)
	{
		int bl = GetScene()->GetStageManager()->GetStageData(StageManager::ObjectLayer::eStageObjectLayerBlock, mIndexX - 1, mIndexY + 1);
		if (bl != mID + 2 && bl != 1) pattern_corner ^= 0b0100;
	}

	if ((pattern & 0b0101) == 0b0101)
	{
		int br = GetScene()->GetStageManager()->GetStageData(StageManager::ObjectLayer::eStageObjectLayerBlock, mIndexX + 1, mIndexY + 1);
		if (br != mID + 2 && br != 1) pattern_corner ^= 0b1000;
	}
	mCornerSprite->SetUseID(pattern_corner);
}

void AbstractBlock::ExportObjectTexture()
{
	TextureTag tex[] = {
		TextureTag::GroundBlock,
		TextureTag::SoilBlock,
		TextureTag::GroundBlockStage2,
		TextureTag::GroundBlockStage3,
		TextureTag::RemainsBlock,
		TextureTag::RemainsBlockStage2,
		TextureTag::RemainsBlockStage3,
	};
	TextureTag texCorner[] = {
		TextureTag::GroundBlockCorner,
		TextureTag::SoilBlockCorner,
		TextureTag::GroundBlockCornerStage2,
		TextureTag::GroundBlockCornerStage3,
		TextureTag::Invisible,
		TextureTag::Invisible,
		TextureTag::Invisible,
	};
	if (mID >= 0)
	{
		mBlockSprite = new ComponentSprite(this, tex[mID], LAYER_OBJECT + 1);
		if (texCorner[mID] != TextureTag::Invisible)
		{
			mCornerSprite = new ComponentSprite(this, texCorner[mID], LAYER_OBJECT);
		}
	}
	UpdateBlockTexture();

	if (mBlockSprite) mBlockSprite->ExportObjectTexture();
	if (mCornerSprite)mCornerSprite->ExportObjectTexture();

	delete mCASprite;
	delete mCornerSprite;
}
