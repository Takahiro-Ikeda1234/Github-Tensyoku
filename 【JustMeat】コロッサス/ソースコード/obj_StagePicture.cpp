#include "obj_StagePicture.h"
#include"StageManager.h"
#include"ComponentAnimationSprite.h"
StagePicture::StagePicture(Scene * scene, int id, int stageID, int stageWidth, int stageHeight)
	: Object(scene, Vector2(0, 0))
	, mID(id)
{
	float posX;
	float posY;
	float sizeX;
	float sizeY;
	switch (id)
	{
	case 0:	//¶ã
		sizeX = stageWidth / 2;
		sizeY = stageHeight / 2;
		posX = sizeX / 2;
		posY = sizeY / 2;
		break;
	case 1:	//‰Eã
		sizeX = stageWidth / 2 + stageWidth % 2;
		sizeY = stageHeight / 2;
		posX = (stageWidth / 2) + sizeX / 2;
		posY = sizeY / 2;
		break;
	case 2:	//¶‰º
		sizeX = stageWidth / 2;
		sizeY = stageHeight / 2 + stageHeight % 2;
		posX = sizeX / 2;
		posY = (stageHeight / 2) + sizeY / 2;
		break;
	case 3:	//‰E‰º
		sizeX = stageWidth / 2 + stageWidth % 2;
		sizeY = stageHeight / 2 + stageHeight % 2;
		posX = (stageWidth / 2) + sizeX / 2;
		posY = (stageHeight / 2) + sizeY / 2;
		break;
	}
	SetPosition(BLOCKSIZE * Vector2(posX, posY - 0.01f));
	SetSize(BLOCKSIZE * Vector2(sizeX, sizeY));

	static const TextureTag tag[][4] = {
		{TextureTag::Stage0_0,TextureTag::Stage0_1,TextureTag::Stage0_2,TextureTag::Stage0_3},
		{TextureTag::Stage1_0,TextureTag::Stage1_1,TextureTag::Stage1_2,TextureTag::Stage1_3},
		{TextureTag::Stage2_0,TextureTag::Stage2_1,TextureTag::Stage2_2,TextureTag::Stage2_3},
		{TextureTag::Stage3_0,TextureTag::Stage3_1,TextureTag::Stage3_2,TextureTag::Stage3_3},
		{TextureTag::Stage4_0,TextureTag::Stage4_1,TextureTag::Stage4_2,TextureTag::Stage4_3},
		{TextureTag::Stage5_0,TextureTag::Stage5_1,TextureTag::Stage5_2,TextureTag::Stage5_3},
		{TextureTag::Stage6_0,TextureTag::Stage6_1,TextureTag::Stage6_2,TextureTag::Stage6_3},
		{TextureTag::Stage7_0,TextureTag::Stage7_1,TextureTag::Stage7_2,TextureTag::Stage7_3},
		{TextureTag::Stage8_0,TextureTag::Stage8_1,TextureTag::Stage8_2,TextureTag::Stage8_3},
	};
	mCASprite = new ComponentAnimationSprite(this, tag[stageID][id], LAYER_OBJECT + 10);
}

void StagePicture::ReloadTexture()
{
	static const TextureTag tag[][4] = {
	{TextureTag::Stage0_0,TextureTag::Stage0_1,TextureTag::Stage0_2,TextureTag::Stage0_3},
	{TextureTag::Stage1_0,TextureTag::Stage1_1,TextureTag::Stage1_2,TextureTag::Stage1_3},
	{TextureTag::Stage2_0,TextureTag::Stage2_1,TextureTag::Stage2_2,TextureTag::Stage2_3},
	{TextureTag::Stage3_0,TextureTag::Stage3_1,TextureTag::Stage3_2,TextureTag::Stage3_3},
	{TextureTag::Stage4_0,TextureTag::Stage4_1,TextureTag::Stage4_2,TextureTag::Stage4_3},
	{TextureTag::Stage5_0,TextureTag::Stage5_1,TextureTag::Stage5_2,TextureTag::Stage5_3},
	{TextureTag::Stage6_0,TextureTag::Stage6_1,TextureTag::Stage6_2,TextureTag::Stage6_3},
	{TextureTag::Stage7_0,TextureTag::Stage7_1,TextureTag::Stage7_2,TextureTag::Stage7_3},
	{TextureTag::Stage8_0,TextureTag::Stage8_1,TextureTag::Stage8_2,TextureTag::Stage8_3},
	};
	TextureSystem::Get()->ReloadTexture(tag[GetScene()->GetStageManager()->GetCurrentStageID()][mID]);
}
