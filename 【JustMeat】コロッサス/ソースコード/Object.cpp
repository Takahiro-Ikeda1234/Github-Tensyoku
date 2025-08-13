//=============================================================================
//
// [Object.cpp] �I�u�W�F�N�g���ʏ���
//
//=============================================================================

//�C���N���[�h�t�@�C��
#include"Scene.h"
#include<vector>
#include"Object.h"
#include"Component.h"
#include"ComponentAnimationSprite.h"
// ---------------------------------------------------------------------------
// �֐���: Object::Object(Scene * scene, Vector2 position)
// ����:   Scene* scene �V�[���̃|�C���^
// ����:   Vector2 position �����ʒu
// ����:   Object�̃R���X�g���N�^
// ---------------------------------------------------------------------------
Object::Object(Scene * scene, Vector2 position)
	: AbstractObject(scene)
	, mCASprite(nullptr)
	, mIsForeground(false)
{
	GetScene()->AddObject(this);
	SetPosition(position);
}
// ---------------------------------------------------------------------------
// �֐���: Object::~Object()
// ����:   Object�̃f�X�g���N�^
// ---------------------------------------------------------------------------
Object::~Object()
{
	//�V�[�����玩�g��Remove
	GetScene()->RemoveObject(this);
}

void Object::SetDrawMode(int drawMode)
{
	if (mCASprite)
	{
		mCASprite->SetDrawMode((ComponentSprite::DrawMode)drawMode);
	}
}

void Object::SetColor(unsigned color)
{
	if (mCASprite)
	{
		mCASprite->SetColor(color);
	}
}

void Object::SceneChangeEvent()
{
	SetState(State::eDead);
}

