#pragma once
#include<unordered_map>
#define TEXTURE_FOLDER "resources/textures/"
#define TEXTURE_EXTENSION ".png"
enum class TextureTag
{
	Plane,
	PlaneRed,
	PlaneGreen,
	Invisible,
	Title,
	TitleEnding,
	TitleBG,
	TitleEndingBG,
	Block,
	Button,
	Banner,
	EditorObjectWindow,
	Border,
	WeedS,
	WeedM,
	WeedL,
	Climber,
	Wall,
	Stamina,
	TriggerButton,
	TriggerButtonRot,
	GhostCat,
	Fog,
	Cloud,
	CloudGeneratorTop,
	CloudGeneratorBottom,
	HorizontalArm,
	VerticalArm,
	HorizontalArmArmor,
	VerticalArmArmor,
	Spark,
	Smoke,
	Entrance,
	PushBlock,
	PushWall,
	MoveWall,
	MoveWallStage2,
	MoveWallStage3,
	MoveWall0,
	MoveWall1,
	MoveWall2,
	FallRock,
	Prime1,
	Prime2,
	Prime3,
	Prime4,
	DummyPlayer,
	WallClimbable,
	NotClimbableWall,
	GroundBlock,
	GroundBlockCorner,
	GroundBlockStage2,
	GroundBlockCornerStage2,
	GroundBlockStage3,
	GroundBlockCornerStage3,
	RemainsBlock,
	RemainsBlockStage2,
	RemainsBlockStage3,
	SoilBlock,
	SoilBlockCorner,
	MoveGroundBlockSize3,
	MoveGroundBlockSize3Stage2,
	MoveGroundBlockSize3Stage3,
	MoveGroundBlockSize3Horizontal,
	MoveGroundBlockSize3HorizontalStage2,
	MoveGroundBlockSize3HorizontalStage3,
	MoveGroundBlockSize9,
	Core,
	CatSkill,
	Laser,
	Control,
	KeyWASD,
	KeyR,
	KeyEnter,
	KeyEsc,
	Pause,
	Warning,
	Arrow,
	UI9Sliced,
	Paper,
	Credit,
	Fin,
	Justmeat,
	Colossus,
	CatIcon,
	Crystal,
	Star,
	Marker,
	FragileRock1,
	FragileRock2,
	FragileRock3,
	Fragment1,
	Fragment2,
	Fragment3,
	Fragment4,
	Grass,
	Flower,
	TutorialMove,
	TutorialClimb,
	TutorialJump,
	TutorialEnter,
	TutorialDash,
	StageSelectArm,
	StageSelectText,
	PauseText,
	PauseSelect,
	TitleFrame,
	TitleButton,
	Stage0_0,
	Stage0_1,
	Stage0_2,
	Stage0_3,
	Stage1_0,
	Stage1_1,
	Stage1_2,
	Stage1_3,
	Stage2_0,
	Stage2_1,
	Stage2_2,
	Stage2_3,
	Stage3_0,
	Stage3_1,
	Stage3_2,
	Stage3_3,
	Stage4_0,
	Stage4_1,
	Stage4_2,
	Stage4_3,
	Stage5_0,
	Stage5_1,
	Stage5_2,
	Stage5_3,
	Stage6_0,
	Stage6_1,
	Stage6_2,
	Stage6_3,
	Stage7_0,
	Stage7_1,
	Stage7_2,
	Stage7_3,
	Stage8_0,
	Stage8_1,
	Stage8_2,
	Stage8_3,
};



class TextureData
{
public:
	TextureData(std::string filename, int width, int height, int divX, int divY);
	~TextureData() {}
public:
	const std::string mFilename;
	const int mWidth;
	const int mHeight;
	const int mDivX;
	const int mDivY;
	int mUseCount;
	void* mTexture;
};


class TextureSystem
{
public:
	static TextureSystem* Get();
	TextureSystem();
	~TextureSystem();
	void Init();
	void Uninit();
	void* AcquireTexture(TextureTag tag);	//所有権の取得
	void ReleaseTexture(TextureTag tag);	//所有権の放棄
	int GetWidth(TextureTag tag) { return mTextures.at(tag)->mWidth; };
	int GetHeight(TextureTag tag) { return mTextures.at(tag)->mHeight; };
	int GetDivX(TextureTag tag) { return mTextures.at(tag)->mDivX; };
	int GetDivY(TextureTag tag) { return mTextures.at(tag)->mDivY; };
	void ReloadTexture(TextureTag tag);
private:
	void* LoadTexture(std::string filepath, int width, int height);		//テクスチャの利用者数が0→1になるとき呼ぶ
	void UnloadTexture(void* texture);	//テクスチャの利用者数が1→0になるとき呼ぶ
	std::unordered_map<TextureTag, class TextureData*> mTextures;
};


class Texture
{
public:
	Texture(TextureTag tag);
	~Texture() {
		TextureSystem::Get()->ReleaseTexture(mTag);
	}
	void* GetTexture() { return ((TextureData*)mTexture)->mTexture; }
	TextureTag GetTag() { return mTag; }

public:
	const int mDivX;
	const int mDivY;
private:
	TextureTag mTag;
	void* mTexture;
};