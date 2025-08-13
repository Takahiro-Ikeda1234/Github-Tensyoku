#pragma once
#include "textureData.h"

static const int MAX_OBJECT_CH = 50;

enum ObjectID
{
	ID_NOUSE = -1,	//エディタから消したい場合はこれにする
	ID_none,	//NOUSE
	ID_BLOCK,
	ID_WEED_S,
	ID_WEED_M,
	ID_WEED_L,
	ID_TREE,
	ID_CLIMBER,
	ID_WALL,
	ID_TRIGGERED_MOVEWALL,
	ID_TRIGGERED_MOVEBLOCK,
	ID_TRIGGER_BUTTON_RIGHT,
	ID_TRIGGER_BUTTON_LEFT,
	ID_CLOUD_GENERATOR_TOP,
	ID_CLOUD_GENERATOR_BOTTOM,
	ID_WOOD,
	ID_PROP_WALL,
	ID_FRAGILEBLOCK,
	ID_PUSHBLOCK,
	ID_PUSHWALL,
	ID_CHECKPOINT,
	ID_SAND,
	ID_ROCKFALL,
	ID_ENTRANCE,
	ID_MOVEWALL,
	ID_MOVEBLOCK,
	ID_GROUNDBLOCK,
	ID_SOILBLOCK,
	ID_GOLEMZONE,
	ID_LASER_RIGHT,
	ID_LASER_LEFT,
	ID_LASER_UP,
	ID_LASER_DOWN,
	ID_MESSAGEZONE,
	ID_CHECKPOINTZONE,
	ID_CHECKPOINTCENTER,
	ID_FRAGILEBLOCK6x5,
	ID_FRAGILEBLOCK5x7,
	ID_GOLEMCORE,
	ID_ENTRANCE_LEFT,
	ID_ENTRANCE_RIGHT,
	ID_REMAINS_BLOCK,
	ID_TRIGGER_BUTTON_TOP,
	ID_TRIGGER_BUTTON_BOTTOM,
	ID_GRASS,
	ID_FLOWER,
	ID_TUTORIAL_MOVE,
	ID_TUTORIAL_CLIMB,
	ID_TUTORIAL_JUMP,
	ID_TUTORIAL_ENTER,
	ID_TUTORIAL_DASH,
	ID_CHECKPOINT_GATE,
	ID_MARKER,
	OBJECT_ID_MAX,
};

enum ObjectLayer
{
	EDITOR_TAB_OBJECT = 0,
	EDITOR_TAB_WALL,
	EDITOR_TAB_PROP,
	EDITOR_TAB_ETC,
	EDITOR_TAB_GOLEM,
	EDITOR_TAB_MESSAGE,
	EDITOR_TAB_MAX,
};

typedef struct
{
	ObjectID id;
	std::string name = "-";
	TextureTag texTag;
	ObjectLayer layer = EDITOR_TAB_OBJECT;
	unsigned char prime = 0b0000;	//同一の画像のものを見分けるためのもの
	bool useCh = false;
	bool useCursorCh = false;
	float animSpeed = 1;
	int xSize = 1;
	int ySize = 1;
}ObjectData;

static const ObjectData OBJECT_DATA[] = {
	{ID_NOUSE},
	{ID_BLOCK,"ブロック", TextureTag::Block },
	{ID_NOUSE},
	{ID_NOUSE},
	{ID_NOUSE},
	{ID_NOUSE},
	{ID_CLIMBER,"プレイヤー", TextureTag::Climber},
	{ID_WALL,"登れる壁", TextureTag::Wall, EDITOR_TAB_WALL },
	{ID_TRIGGERED_MOVEWALL,"動く壁(連動)", TextureTag::MoveWall, EDITOR_TAB_ETC, 0b0010, true},
	{ID_TRIGGERED_MOVEBLOCK,"動くブロ(連動)", TextureTag::Block, EDITOR_TAB_ETC, 0b0010, true},
	{ID_TRIGGER_BUTTON_RIGHT, "ボタン右", TextureTag::TriggerButton, EDITOR_TAB_ETC, 0b0000, true},
	{ID_TRIGGER_BUTTON_LEFT, "ボタン左", TextureTag::TriggerButton, EDITOR_TAB_ETC, 0b0001, true},
	{ID_CLOUD_GENERATOR_TOP,"上端の雲", TextureTag::CloudGeneratorTop, EDITOR_TAB_PROP},
	{ID_CLOUD_GENERATOR_BOTTOM,"下端の雲", TextureTag::CloudGeneratorBottom, EDITOR_TAB_PROP},
	{ID_NOUSE},
	{ID_PROP_WALL, "飾り・壁", TextureTag::NotClimbableWall, EDITOR_TAB_PROP, 0b0000, false, false, 0},
	{ID_FRAGILEBLOCK, "脆いブロ5x5", TextureTag::FragileRock1},
	{ID_PUSHBLOCK ,"押すブロック", TextureTag::PushBlock, EDITOR_TAB_ETC},
	{ID_PUSHWALL, "押す壁", TextureTag::PushWall, EDITOR_TAB_ETC },
	{ID_NOUSE},
	{ID_NOUSE},
	{ID_ROCKFALL,"落石", TextureTag::FallRock, EDITOR_TAB_OBJECT},
	{ID_ENTRANCE,"入口" ,TextureTag::Entrance, EDITOR_TAB_ETC, 0b0000, true},
	{ID_MOVEWALL,"動く壁" ,TextureTag::MoveWall, EDITOR_TAB_ETC, 0b0001, true},
	{ID_MOVEBLOCK,"動くブロ" ,TextureTag::Block, EDITOR_TAB_ETC, 0b0001, true},
	{ID_GROUNDBLOCK,"ブロック", TextureTag::GroundBlock},
	{ID_SOILBLOCK,"ブロック", TextureTag::SoilBlock},
	{ID_GOLEMZONE,"攻撃領域", TextureTag::Plane, EDITOR_TAB_GOLEM, 0b0000, true, true},
	{ID_LASER_RIGHT,"レザ右", TextureTag::Laser, EDITOR_TAB_OBJECT, 0b0001},
	{ID_LASER_LEFT,"レザ左", TextureTag::Laser, EDITOR_TAB_OBJECT, 0b1000},
	{ID_LASER_UP,"レザ上", TextureTag::Laser, EDITOR_TAB_OBJECT, 0b0010},
	{ID_LASER_DOWN,"レザ下", TextureTag::Laser, EDITOR_TAB_OBJECT, 0b0100},
	{ID_MESSAGEZONE,"メッセージ", TextureTag::Plane, EDITOR_TAB_MESSAGE, 0b0000, true, true},
	{ID_CHECKPOINTZONE,"CP範囲", TextureTag::PlaneGreen, EDITOR_TAB_GOLEM, 0b0000, true, true},
	{ID_CHECKPOINTCENTER,"復活地点", TextureTag::PlaneRed, EDITOR_TAB_GOLEM, 0b0000, true, true},
	{ID_FRAGILEBLOCK6x5, "脆いブロ6x5", TextureTag::FragileRock1, EDITOR_TAB_OBJECT, 0b0001},
	{ID_FRAGILEBLOCK5x7, "脆いブロ5x7", TextureTag::FragileRock1, EDITOR_TAB_OBJECT, 0b0010},
	{ID_GOLEMCORE, "コア", TextureTag::Block},
	{ID_ENTRANCE_LEFT,"出口(左)" ,TextureTag::Entrance, EDITOR_TAB_ETC, 0b0001, true},
	{ID_ENTRANCE_RIGHT,"出口(右)" ,TextureTag::Entrance, EDITOR_TAB_ETC, 0b0010, true},
	{ID_REMAINS_BLOCK,"遺跡ブロック", TextureTag::RemainsBlock },
	{ID_TRIGGER_BUTTON_TOP, "ボタン上", TextureTag::TriggerButton, EDITOR_TAB_ETC, 0b0010, true},
	{ID_TRIGGER_BUTTON_BOTTOM, "ボタン下", TextureTag::TriggerButton, EDITOR_TAB_ETC, 0b0100, true},
	{ID_GRASS,"装飾・草", TextureTag::Grass, EDITOR_TAB_PROP, 0b0000, false, false, 2 },
	{ID_FLOWER,"装飾・花", TextureTag::Flower, EDITOR_TAB_PROP, 0b0000, false, false, 3 },
	{ID_TUTORIAL_MOVE,"移動", TextureTag::TutorialMove, EDITOR_TAB_PROP, 0b0000, false, false, 1, 3, 3  },
	{ID_TUTORIAL_CLIMB,"登る", TextureTag::TutorialClimb, EDITOR_TAB_PROP, 0b0000, false, false, 1, 3, 3  },
	{ID_TUTORIAL_JUMP,"ジャンプ", TextureTag::TutorialJump, EDITOR_TAB_PROP, 0b0000, false, false, 1, 3, 3  },
	{ID_TUTORIAL_ENTER,"入る", TextureTag::TutorialEnter, EDITOR_TAB_PROP, 0b0000, false, false, 1, 3, 3 } ,
	{ID_TUTORIAL_DASH,"走る", TextureTag::TutorialDash, EDITOR_TAB_PROP, 0b0000, false, false, 1, 3, 3 } ,
	{ID_CHECKPOINT_GATE,"復活地点(半分右)", TextureTag::PlaneRed, EDITOR_TAB_GOLEM, 0b1111, true, true},
	{ID_MARKER,"乗り目印", TextureTag::Marker},
};