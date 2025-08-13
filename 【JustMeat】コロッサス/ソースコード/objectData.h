#pragma once
#include "textureData.h"

static const int MAX_OBJECT_CH = 50;

enum ObjectID
{
	ID_NOUSE = -1,	//�G�f�B�^������������ꍇ�͂���ɂ���
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
	unsigned char prime = 0b0000;	//����̉摜�̂��̂��������邽�߂̂���
	bool useCh = false;
	bool useCursorCh = false;
	float animSpeed = 1;
	int xSize = 1;
	int ySize = 1;
}ObjectData;

static const ObjectData OBJECT_DATA[] = {
	{ID_NOUSE},
	{ID_BLOCK,"�u���b�N", TextureTag::Block },
	{ID_NOUSE},
	{ID_NOUSE},
	{ID_NOUSE},
	{ID_NOUSE},
	{ID_CLIMBER,"�v���C���[", TextureTag::Climber},
	{ID_WALL,"�o����", TextureTag::Wall, EDITOR_TAB_WALL },
	{ID_TRIGGERED_MOVEWALL,"������(�A��)", TextureTag::MoveWall, EDITOR_TAB_ETC, 0b0010, true},
	{ID_TRIGGERED_MOVEBLOCK,"�����u��(�A��)", TextureTag::Block, EDITOR_TAB_ETC, 0b0010, true},
	{ID_TRIGGER_BUTTON_RIGHT, "�{�^���E", TextureTag::TriggerButton, EDITOR_TAB_ETC, 0b0000, true},
	{ID_TRIGGER_BUTTON_LEFT, "�{�^����", TextureTag::TriggerButton, EDITOR_TAB_ETC, 0b0001, true},
	{ID_CLOUD_GENERATOR_TOP,"��[�̉_", TextureTag::CloudGeneratorTop, EDITOR_TAB_PROP},
	{ID_CLOUD_GENERATOR_BOTTOM,"���[�̉_", TextureTag::CloudGeneratorBottom, EDITOR_TAB_PROP},
	{ID_NOUSE},
	{ID_PROP_WALL, "����E��", TextureTag::NotClimbableWall, EDITOR_TAB_PROP, 0b0000, false, false, 0},
	{ID_FRAGILEBLOCK, "�Ƃ��u��5x5", TextureTag::FragileRock1},
	{ID_PUSHBLOCK ,"�����u���b�N", TextureTag::PushBlock, EDITOR_TAB_ETC},
	{ID_PUSHWALL, "������", TextureTag::PushWall, EDITOR_TAB_ETC },
	{ID_NOUSE},
	{ID_NOUSE},
	{ID_ROCKFALL,"����", TextureTag::FallRock, EDITOR_TAB_OBJECT},
	{ID_ENTRANCE,"����" ,TextureTag::Entrance, EDITOR_TAB_ETC, 0b0000, true},
	{ID_MOVEWALL,"������" ,TextureTag::MoveWall, EDITOR_TAB_ETC, 0b0001, true},
	{ID_MOVEBLOCK,"�����u��" ,TextureTag::Block, EDITOR_TAB_ETC, 0b0001, true},
	{ID_GROUNDBLOCK,"�u���b�N", TextureTag::GroundBlock},
	{ID_SOILBLOCK,"�u���b�N", TextureTag::SoilBlock},
	{ID_GOLEMZONE,"�U���̈�", TextureTag::Plane, EDITOR_TAB_GOLEM, 0b0000, true, true},
	{ID_LASER_RIGHT,"���U�E", TextureTag::Laser, EDITOR_TAB_OBJECT, 0b0001},
	{ID_LASER_LEFT,"���U��", TextureTag::Laser, EDITOR_TAB_OBJECT, 0b1000},
	{ID_LASER_UP,"���U��", TextureTag::Laser, EDITOR_TAB_OBJECT, 0b0010},
	{ID_LASER_DOWN,"���U��", TextureTag::Laser, EDITOR_TAB_OBJECT, 0b0100},
	{ID_MESSAGEZONE,"���b�Z�[�W", TextureTag::Plane, EDITOR_TAB_MESSAGE, 0b0000, true, true},
	{ID_CHECKPOINTZONE,"CP�͈�", TextureTag::PlaneGreen, EDITOR_TAB_GOLEM, 0b0000, true, true},
	{ID_CHECKPOINTCENTER,"�����n�_", TextureTag::PlaneRed, EDITOR_TAB_GOLEM, 0b0000, true, true},
	{ID_FRAGILEBLOCK6x5, "�Ƃ��u��6x5", TextureTag::FragileRock1, EDITOR_TAB_OBJECT, 0b0001},
	{ID_FRAGILEBLOCK5x7, "�Ƃ��u��5x7", TextureTag::FragileRock1, EDITOR_TAB_OBJECT, 0b0010},
	{ID_GOLEMCORE, "�R�A", TextureTag::Block},
	{ID_ENTRANCE_LEFT,"�o��(��)" ,TextureTag::Entrance, EDITOR_TAB_ETC, 0b0001, true},
	{ID_ENTRANCE_RIGHT,"�o��(�E)" ,TextureTag::Entrance, EDITOR_TAB_ETC, 0b0010, true},
	{ID_REMAINS_BLOCK,"��Ճu���b�N", TextureTag::RemainsBlock },
	{ID_TRIGGER_BUTTON_TOP, "�{�^����", TextureTag::TriggerButton, EDITOR_TAB_ETC, 0b0010, true},
	{ID_TRIGGER_BUTTON_BOTTOM, "�{�^����", TextureTag::TriggerButton, EDITOR_TAB_ETC, 0b0100, true},
	{ID_GRASS,"�����E��", TextureTag::Grass, EDITOR_TAB_PROP, 0b0000, false, false, 2 },
	{ID_FLOWER,"�����E��", TextureTag::Flower, EDITOR_TAB_PROP, 0b0000, false, false, 3 },
	{ID_TUTORIAL_MOVE,"�ړ�", TextureTag::TutorialMove, EDITOR_TAB_PROP, 0b0000, false, false, 1, 3, 3  },
	{ID_TUTORIAL_CLIMB,"�o��", TextureTag::TutorialClimb, EDITOR_TAB_PROP, 0b0000, false, false, 1, 3, 3  },
	{ID_TUTORIAL_JUMP,"�W�����v", TextureTag::TutorialJump, EDITOR_TAB_PROP, 0b0000, false, false, 1, 3, 3  },
	{ID_TUTORIAL_ENTER,"����", TextureTag::TutorialEnter, EDITOR_TAB_PROP, 0b0000, false, false, 1, 3, 3 } ,
	{ID_TUTORIAL_DASH,"����", TextureTag::TutorialDash, EDITOR_TAB_PROP, 0b0000, false, false, 1, 3, 3 } ,
	{ID_CHECKPOINT_GATE,"�����n�_(�����E)", TextureTag::PlaneRed, EDITOR_TAB_GOLEM, 0b1111, true, true},
	{ID_MARKER,"���ڈ�", TextureTag::Marker},
};