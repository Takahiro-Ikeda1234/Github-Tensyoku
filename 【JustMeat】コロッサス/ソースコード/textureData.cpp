#include"TextureData.h"
#include"DX9_texture.h"
#include"StageManager.h"
#include"StageData.h"

TextureSystem* TextureSystem::Get()
{
	static TextureSystem instance;
	return &instance;
}

void TextureSystem::Init()
{
	//mTextures[TextureTag::TextureTag名] = new TextureData("ファイル名", 幅, 高さ, 横方向分割数, 縦方向分割数);
	mTextures[TextureTag::Plane] = new TextureData("objects/plane", 64, 64, 1, 1);
	mTextures[TextureTag::PlaneRed] = new TextureData("editor/planeRed", 64, 64, 1, 1);
	mTextures[TextureTag::PlaneGreen] = new TextureData("editor/planeGreen", 64, 64, 1, 1);
	mTextures[TextureTag::Invisible] = new TextureData("objects/invisible", 64, 64, 1, 1);
	mTextures[TextureTag::Title] = new TextureData("UI/title", 1280, 720, 1, 1);
	mTextures[TextureTag::TitleEnding] = new TextureData("UI/title2", 1280, 720, 1, 1);
	mTextures[TextureTag::TitleBG] = new TextureData("UI/title_bg", 1280, 720, 1, 1);
	mTextures[TextureTag::TitleEndingBG] = new TextureData("UI/title2_bg", 1280, 720, 1, 1);
	mTextures[TextureTag::Block] = new TextureData("editor/block", 64, 64, 1, 1);
	mTextures[TextureTag::Button] = new TextureData("editor/button", 64, 64, 1, 1);
	mTextures[TextureTag::Banner] = new TextureData("editor/banner", 384, 64, 1, 1);
	mTextures[TextureTag::EditorObjectWindow] = new TextureData("editor/window", 360, 900, 1, 1);
	mTextures[TextureTag::Border] = new TextureData("editor/border", 64, 64, 1, 1);
	mTextures[TextureTag::WeedS] = new TextureData("props/weed_small", 64, 16, 4, 1);
	mTextures[TextureTag::WeedM] = new TextureData("props/weed_medium", 64, 16, 4, 1);
	mTextures[TextureTag::WeedL] = new TextureData("props/weed_large", 64, 16, 4, 1);
	mTextures[TextureTag::Grass] = new TextureData("props/grass", 128, 32, 4, 1);
	mTextures[TextureTag::Flower] = new TextureData("props/flower", 32, 32, 1, 1);
	mTextures[TextureTag::Climber] = new TextureData("objects/player", 192, 1216, 6, 19);
	mTextures[TextureTag::Wall] = new TextureData("editor/wall", 16, 16, 1, 1);
	mTextures[TextureTag::Stamina] = new TextureData("ui/stamina", 64, 256, 1, 4);
	mTextures[TextureTag::GhostCat] = new TextureData("objects/witchCat", 96, 64, 3, 2);
	mTextures[TextureTag::Fog] = new TextureData("UI/fog", 1920, 1080, 1, 1);
	mTextures[TextureTag::Cloud] = new TextureData("objects/cloud", 64, 192, 1, 6);
	mTextures[TextureTag::CloudGeneratorTop] = new TextureData("editor/kumo1", 64, 64, 1, 1);
	mTextures[TextureTag::CloudGeneratorBottom] = new TextureData("editor/kumo2", 64, 64, 1, 1);
	mTextures[TextureTag::HorizontalArm] = new TextureData("objects/hArm", 960, 1280, 1, 8);
	mTextures[TextureTag::VerticalArm] = new TextureData("objects/vArm", 160, 7680, 1, 8);
	mTextures[TextureTag::HorizontalArmArmor] = new TextureData("objects/hArmArmor", 960, 1280, 1, 8);
	mTextures[TextureTag::VerticalArmArmor] = new TextureData("objects/vArmArmor", 160, 7680, 1, 8);
	mTextures[TextureTag::Spark] = new TextureData("particles/spark", 4, 4, 1, 1);
	mTextures[TextureTag::Smoke] = new TextureData("particles/smoke", 32, 32, 1, 1);
	mTextures[TextureTag::Entrance] = new TextureData("objects/woodframe", 64, 192, 1, 3);
	mTextures[TextureTag::MoveWall] = new TextureData("objects/moveWall", 96, 96, 1, 1);
	mTextures[TextureTag::MoveWallStage2] = new TextureData("objects/moveWall_st2", 96, 96, 1, 1);
	mTextures[TextureTag::MoveWallStage3] = new TextureData("objects/moveWall_st3", 96, 96, 1, 1);

	mTextures[TextureTag::MoveWall0] = new TextureData("objects/Block_Climbable_0", 64, 64, 1, 1);
	mTextures[TextureTag::MoveWall1] = new TextureData("objects/Block_Climbable_1", 96, 128, 1, 1);
	mTextures[TextureTag::MoveWall2] = new TextureData("objects/Block_Climbable_0", 64, 64, 1, 1);
	mTextures[TextureTag::FallRock] = new TextureData("objects/fall_rock", 64, 96, 1, 1);
	mTextures[TextureTag::Prime1] = new TextureData("editor/0001", 32, 32, 1, 1);
	mTextures[TextureTag::Prime2] = new TextureData("editor/0010", 32, 32, 1, 1);
	mTextures[TextureTag::Prime3] = new TextureData("editor/0100", 32, 32, 1, 1);
	mTextures[TextureTag::Prime4] = new TextureData("editor/1000", 32, 32, 1, 1);
	mTextures[TextureTag::WallClimbable] = new TextureData("blocks/Wall_Climbable", 32, 32, 1, 1);
	mTextures[TextureTag::NotClimbableWall] = new TextureData("blocks/Wall_Not_Climbable", 32, 32, 1, 1);
	mTextures[TextureTag::GroundBlock] = new TextureData("blocks/block_groundBlock", 128, 256, 4, 8);
	mTextures[TextureTag::GroundBlockCorner] = new TextureData("blocks/block_corner_groundBlock", 128, 256, 4, 8);
	mTextures[TextureTag::GroundBlockStage2] = new TextureData("blocks/block_groundBlock_st2", 128, 256, 4, 8);
	mTextures[TextureTag::GroundBlockCornerStage2] = new TextureData("blocks/block_corner_groundBlock_st2", 128, 256, 4, 8);
	mTextures[TextureTag::GroundBlockStage3] = new TextureData("blocks/block_groundBlock_st3", 128, 256, 4, 8);
	mTextures[TextureTag::GroundBlockCornerStage3] = new TextureData("blocks/block_corner_groundBlock_st3", 128, 256, 4, 8);
	mTextures[TextureTag::RemainsBlock] = new TextureData("blocks/Iseki_block", 128, 256, 4, 8);
	mTextures[TextureTag::RemainsBlockStage2] = new TextureData("blocks/Iseki_block_st2", 128, 256, 4, 8);
	mTextures[TextureTag::RemainsBlockStage3] = new TextureData("blocks/Iseki_block_st3", 128, 256, 4, 8);
	mTextures[TextureTag::SoilBlock] = new TextureData("blocks/block_soilBlock", 128, 256, 4, 8);
	mTextures[TextureTag::SoilBlockCorner] = new TextureData("blocks/block_corner_soilBlock", 128, 256, 4, 8);
	mTextures[TextureTag::MoveGroundBlockSize3] = new TextureData("blocks/moveblock_groundBlock", 32, 96, 1, 1);
	mTextures[TextureTag::MoveGroundBlockSize3Stage2] = new TextureData("blocks/moveblock_groundBlock_st2", 32, 96, 1, 1);
	mTextures[TextureTag::MoveGroundBlockSize3Stage3] = new TextureData("blocks/moveblock_groundBlock_st3", 32, 96, 1, 1);
	mTextures[TextureTag::MoveGroundBlockSize3Horizontal] = new TextureData("blocks/moveblock_groundBlock_h", 96, 32, 1, 1);
	mTextures[TextureTag::MoveGroundBlockSize3HorizontalStage2] = new TextureData("blocks/moveblock_groundBlock_h_st2", 96, 32, 1, 1);
	mTextures[TextureTag::MoveGroundBlockSize3HorizontalStage3] = new TextureData("blocks/moveblock_groundBlock_h_st3", 96, 32, 1, 1);
	mTextures[TextureTag::MoveGroundBlockSize9] = new TextureData("blocks/moveblock_groundBlock_3x3", 96, 96, 1, 1);
	mTextures[TextureTag::CatSkill] = new TextureData("UI/dummy", 64, 128, 1, 2);
	mTextures[TextureTag::Laser] = new TextureData("objects/laser", 32, 224, 1, 7);
	mTextures[TextureTag::KeyWASD] = new TextureData("UI/WASD", 192, 128, 1, 1);
	mTextures[TextureTag::KeyR] = new TextureData("UI/keyR", 64, 64, 1, 1);
	mTextures[TextureTag::KeyEnter] = new TextureData("UI/Enter", 96, 128, 1, 1);
	mTextures[TextureTag::KeyEsc] = new TextureData("UI/ESC", 64, 64, 1, 1);
	mTextures[TextureTag::Control] = new TextureData("UI/control", 1920, 108, 1, 1);
	mTextures[TextureTag::Pause] = new TextureData("UI/pause", 128, 192, 1, 1);
	mTextures[TextureTag::Warning] = new TextureData("UI/warning", 64, 64, 1, 1);
	mTextures[TextureTag::Arrow] = new TextureData("UI/arrow", 32, 32, 1, 1);
	mTextures[TextureTag::UI9Sliced] = new TextureData("UI/nineSlice", 96, 96, 3, 3);
	mTextures[TextureTag::Fin] = new TextureData("UI/fin", 512, 256, 1, 1);
	mTextures[TextureTag::Credit] = new TextureData("UI/credit", 960, 1080, 1, 1);
	mTextures[TextureTag::Justmeat] = new TextureData("UI/justmeat", 512, 512, 1, 1);
	mTextures[TextureTag::Colossus] = new TextureData("UI/colossus", 1280, 640, 1, 1);
	mTextures[TextureTag::Paper] = new TextureData("UI/paper", 1920, 1080, 1, 1);
	mTextures[TextureTag::TriggerButton] = new TextureData("objects/button", 128, 480, 4, 3);
	mTextures[TextureTag::TriggerButtonRot] = new TextureData("objects/buttonRot", 640, 96, 4, 3);
	mTextures[TextureTag::FragileRock1] = new TextureData("objects/fragile_rock_1", 160, 192, 1, 1);
	mTextures[TextureTag::FragileRock2] = new TextureData("objects/fragile_rock_2", 160, 160, 1, 1);
	mTextures[TextureTag::FragileRock3] = new TextureData("objects/fragile_rock_3", 224, 160, 1, 1);
	mTextures[TextureTag::Fragment1] = new TextureData("particles/fragment_1", 32, 32, 1, 1);
	mTextures[TextureTag::Fragment2] = new TextureData("particles/fragment_2", 32, 32, 1, 1);
	mTextures[TextureTag::Fragment3] = new TextureData("particles/fragment_3", 32, 32, 1, 1);
	mTextures[TextureTag::Fragment4] = new TextureData("particles/fragment_4", 32, 32, 1, 1);
	mTextures[TextureTag::PushBlock] = new TextureData("objects/push_rock", 160, 160, 1, 1);
	mTextures[TextureTag::PushWall] = new TextureData("objects/push_wall", 160, 160, 1, 1);
	mTextures[TextureTag::Crystal] = new TextureData("objects/crystal", 128, 128, 1, 1);
	mTextures[TextureTag::Core] = new TextureData("objects/core", 160, 160, 1, 1);
	mTextures[TextureTag::TutorialMove] = new TextureData("UI/tutorialMove", 128, 128, 1, 1);
	mTextures[TextureTag::TutorialClimb] = new TextureData("UI/tutorialClimb", 128, 128, 1, 1);
	mTextures[TextureTag::TutorialJump] = new TextureData("UI/tutorialJump", 128, 128, 1, 1);
	mTextures[TextureTag::TutorialEnter] = new TextureData("UI/tutorialEnter", 128, 128, 1, 1);
	mTextures[TextureTag::TutorialDash] = new TextureData("UI/tutorialDash", 128, 128, 1, 1);
	mTextures[TextureTag::DummyPlayer] = new TextureData("objects/dummyPlayer", 32, 192, 1, 3);
	mTextures[TextureTag::Star] = new TextureData("particles/star", 32, 32, 1, 1);
	mTextures[TextureTag::Marker] = new TextureData("UI/marker", 256, 32, 8, 1);
	mTextures[TextureTag::CatIcon] = new TextureData("UI/catIcon", 192, 192, 1, 1);

	mTextures[TextureTag::StageSelectArm] = new TextureData("UI/stages", 1288, 5680, 1, 16);
	mTextures[TextureTag::StageSelectText] = new TextureData("UI/stageselectText", 599, 77, 1, 1);
	mTextures[TextureTag::PauseText] = new TextureData("UI/pauseText", 492, 106, 1, 1);
	mTextures[TextureTag::PauseSelect] = new TextureData("UI/armIcon", 309, 85, 1, 1);
	mTextures[TextureTag::TitleFrame] = new TextureData("UI/titleFrame", 512, 272, 1, 1);
	mTextures[TextureTag::TitleButton] = new TextureData("UI/titleButton", 265, 80, 1, 2);

	int stageSizeX[STAGE_MAX];
	int stageSizeY[STAGE_MAX];
	for (int i = 0; i < STAGE_MAX; i++)
	{
		StageData::Inst()->StageSizeFromStageFile(i, &stageSizeX[i], &stageSizeY[i]);
		stageSizeX[i] *= 16;
		stageSizeY[i] *= 16;
	}

	mTextures[TextureTag::Stage0_0] = new TextureData("stage/0/stageBlock_0_0", stageSizeX[0], stageSizeY[0], 1, 1);
	mTextures[TextureTag::Stage0_1] = new TextureData("stage/0/stageBlock_0_1", stageSizeX[0], stageSizeY[0], 1, 1);
	mTextures[TextureTag::Stage0_2] = new TextureData("stage/0/stageBlock_0_2", stageSizeX[0], stageSizeY[0], 1, 1);
	mTextures[TextureTag::Stage0_3] = new TextureData("stage/0/stageBlock_0_3", stageSizeX[0], stageSizeY[0], 1, 1);
	mTextures[TextureTag::Stage1_0] = new TextureData("stage/1/stageBlock_1_0", stageSizeX[1], stageSizeY[1], 1, 1);
	mTextures[TextureTag::Stage1_1] = new TextureData("stage/1/stageBlock_1_1", stageSizeX[1], stageSizeY[1], 1, 1);
	mTextures[TextureTag::Stage1_2] = new TextureData("stage/1/stageBlock_1_2", stageSizeX[1], stageSizeY[1], 1, 1);
	mTextures[TextureTag::Stage1_3] = new TextureData("stage/1/stageBlock_1_3", stageSizeX[1], stageSizeY[1], 1, 1);
	mTextures[TextureTag::Stage2_0] = new TextureData("stage/2/stageBlock_2_0", stageSizeX[2], stageSizeY[2], 1, 1);
	mTextures[TextureTag::Stage2_1] = new TextureData("stage/2/stageBlock_2_1", stageSizeX[2], stageSizeY[2], 1, 1);
	mTextures[TextureTag::Stage2_2] = new TextureData("stage/2/stageBlock_2_2", stageSizeX[2], stageSizeY[2], 1, 1);
	mTextures[TextureTag::Stage2_3] = new TextureData("stage/2/stageBlock_2_3", stageSizeX[2], stageSizeY[2], 1, 1);
	mTextures[TextureTag::Stage3_0] = new TextureData("stage/3/stageBlock_3_0", stageSizeX[3], stageSizeY[3], 1, 1);
	mTextures[TextureTag::Stage3_1] = new TextureData("stage/3/stageBlock_3_1", stageSizeX[3], stageSizeY[3], 1, 1);
	mTextures[TextureTag::Stage3_2] = new TextureData("stage/3/stageBlock_3_2", stageSizeX[3], stageSizeY[3], 1, 1);
	mTextures[TextureTag::Stage3_3] = new TextureData("stage/3/stageBlock_3_3", stageSizeX[3], stageSizeY[3], 1, 1);
	mTextures[TextureTag::Stage4_0] = new TextureData("stage/4/stageBlock_4_0", stageSizeX[4], stageSizeY[4], 1, 1);
	mTextures[TextureTag::Stage4_1] = new TextureData("stage/4/stageBlock_4_1", stageSizeX[4], stageSizeY[4], 1, 1);
	mTextures[TextureTag::Stage4_2] = new TextureData("stage/4/stageBlock_4_2", stageSizeX[4], stageSizeY[4], 1, 1);
	mTextures[TextureTag::Stage4_3] = new TextureData("stage/4/stageBlock_4_3", stageSizeX[4], stageSizeY[4], 1, 1);
	mTextures[TextureTag::Stage5_0] = new TextureData("stage/5/stageBlock_5_0", stageSizeX[5], stageSizeY[5], 1, 1);
	mTextures[TextureTag::Stage5_1] = new TextureData("stage/5/stageBlock_5_1", stageSizeX[5], stageSizeY[5], 1, 1);
	mTextures[TextureTag::Stage5_2] = new TextureData("stage/5/stageBlock_5_2", stageSizeX[5], stageSizeY[5], 1, 1);
	mTextures[TextureTag::Stage5_3] = new TextureData("stage/5/stageBlock_5_3", stageSizeX[5], stageSizeY[5], 1, 1);
	mTextures[TextureTag::Stage6_0] = new TextureData("stage/6/stageBlock_6_0", stageSizeX[6], stageSizeY[6], 1, 1);
	mTextures[TextureTag::Stage6_1] = new TextureData("stage/6/stageBlock_6_1", stageSizeX[6], stageSizeY[6], 1, 1);
	mTextures[TextureTag::Stage6_2] = new TextureData("stage/6/stageBlock_6_2", stageSizeX[6], stageSizeY[6], 1, 1);
	mTextures[TextureTag::Stage6_3] = new TextureData("stage/6/stageBlock_6_3", stageSizeX[6], stageSizeY[6], 1, 1);
	mTextures[TextureTag::Stage7_0] = new TextureData("stage/7/stageBlock_7_0", stageSizeX[7], stageSizeY[7], 1, 1);
	mTextures[TextureTag::Stage7_1] = new TextureData("stage/7/stageBlock_7_1", stageSizeX[7], stageSizeY[7], 1, 1);
	mTextures[TextureTag::Stage7_2] = new TextureData("stage/7/stageBlock_7_2", stageSizeX[7], stageSizeY[7], 1, 1);
	mTextures[TextureTag::Stage7_3] = new TextureData("stage/7/stageBlock_7_3", stageSizeX[7], stageSizeY[7], 1, 1);
	mTextures[TextureTag::Stage8_0] = new TextureData("stage/8/stageBlock_8_0", stageSizeX[8], stageSizeY[8], 1, 1);
	mTextures[TextureTag::Stage8_1] = new TextureData("stage/8/stageBlock_8_1", stageSizeX[8], stageSizeY[8], 1, 1);
	mTextures[TextureTag::Stage8_2] = new TextureData("stage/8/stageBlock_8_2", stageSizeX[8], stageSizeY[8], 1, 1);
	mTextures[TextureTag::Stage8_3] = new TextureData("stage/8/stageBlock_8_3", stageSizeX[8], stageSizeY[8], 1, 1);
}
void TextureSystem::Uninit()
{
	while (!mTextures.empty())
	{
		UnloadTexture(mTextures.begin()->second->mTexture);
		mTextures.erase(mTextures.begin());
	}
}

void* TextureSystem::AcquireTexture(TextureTag tag)
{
	//ここでエラーが発生したら存在しないファイルを要求している(ファイル名を間違えている)
	if (mTextures.at(tag)->mUseCount == 0)
	{
		char filepath[64];
		sprintf(filepath, "%s%s%s", TEXTURE_FOLDER, mTextures.at(tag)->mFilename.c_str(), TEXTURE_EXTENSION);
		mTextures.at(tag)->mTexture = LoadTexture(filepath, mTextures.at(tag)->mWidth, mTextures.at(tag)->mHeight);
	}
	mTextures.at(tag)->mUseCount++;
	return mTextures.at(tag);
}

void TextureSystem::ReleaseTexture(TextureTag tag)
{
	mTextures.at(tag)->mUseCount--;
	if (mTextures.at(tag)->mUseCount == 0)
	{
		UnloadTexture(mTextures.at(tag)->mTexture);
		mTextures.at(tag)->mTexture = nullptr;
	}
}
TextureSystem::TextureSystem()
{
	Init();
}
TextureSystem::~TextureSystem()
{
	//呼ばれない
}

void TextureSystem::ReloadTexture(TextureTag tag)
{
	if (mTextures.at(tag)->mUseCount == 0)return;

	char filepath[64];
	sprintf(filepath, "%s%s%s", TEXTURE_FOLDER, mTextures.at(tag)->mFilename.c_str(), TEXTURE_EXTENSION);
	UnloadTexture(mTextures.at(tag)->mTexture);
	mTextures.at(tag)->mTexture = LoadTexture(filepath, mTextures.at(tag)->mWidth, mTextures.at(tag)->mHeight);
}

void* TextureSystem::LoadTexture(std::string filepath, int width, int height)
{
	//DX9
	return LoadTextureDX9(filepath, width, height);
}

void TextureSystem::UnloadTexture(void* texture)
{
	//DX9
	if (texture == nullptr)return;
	UnloadTextureDX9(texture);
}



TextureData::TextureData(std::string filename, int width, int height, int divX, int divY)
	: mFilename(filename)
	, mWidth(width)
	, mHeight(height)
	, mDivX(divX)
	, mDivY(divY)
	, mUseCount(0)
	, mTexture(nullptr)
{

}

Texture::Texture(TextureTag tag)
	: mDivX(TextureSystem::Get()->GetDivX(tag))
	, mDivY(TextureSystem::Get()->GetDivY(tag))
	, mTag(tag)
{
	mTexture = TextureSystem::Get()->AcquireTexture(mTag);
}

