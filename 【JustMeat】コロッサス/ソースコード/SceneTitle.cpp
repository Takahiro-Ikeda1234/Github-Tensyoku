//=============================================================================
//
// [SceneTitle.cpp] �^�C�g�����
//
//=============================================================================

//�C���N���[�h�t�@�C��
#include"SceneTitle.h"
#include"SceneManager.h"
#include"myMath.h"
#include"Draw.h"
#include"TextureData.h"
#include"keyboard.h"
#include"obj_screenBackground.h"
#include"CameraManager.h"
#include"EventManager.h"
#include"UIObject.h"
#include"ComponentUISprite.h"
#include"ui_title.h"
#include"AudioData.h"
// ---------------------------------------------------------------------------
// �֐���: SceneTitle::SceneTitle(class SceneManager* sceneManager, Parameter& param)
// ����:   SceneManager* sceneManager �V�[���}�l�[�W���̃|�C���^
// ����:   Parameter& param �p�����[�^
// ����:   SceneTitle�̃R���X�g���N�^
// ---------------------------------------------------------------------------
SceneTitle::SceneTitle(SceneManager* sceneManager, Parameter& param)
	: Scene(sceneManager)
{
	SetVolumeBGM(6, 0);
	new ScreenBackground(this, TextureTag::Title);
	mUITitle = new UITitle(this);
	int value;
	if (param.Get("���j���[���", &value))
	{
		mUITitle->SetState((UITitle::TitleState)value);
	}
	if (param.Get("�J�[�\���ʒu", &value))
	{
		mUITitle->SetMenuSelectCursor(value);
	}

	if (param.Get("�O�V�[��", &value))
	{
		if (value != SceneManager::SceneID::eStageSelect)
		{
			PlayBGM(BGM_TITLE, 0, 100);
		}
	}
	else
	{
		PlayBGM(BGM_TITLE, 0, 100);
	}

	float fValue = 0;
	if (param.GetF("�t�F�[�h�C������", &fValue))
	{
		mEventManager->Fadein(fValue, 0);
	}



	//UIObject* obj = new UIBox(this, 0.5f, 0.5f, 0.25f, 0.1f, TextureTag::Invisible, 50);
	//obj->GetCSprite()->SetString("Enter�ŊJ�n");
	//obj->GetCSprite()->SetStringInfo(20, obj->GetPosition(), Vector2(60, 60), true);
	//obj->SetBlink(1);
}
// ---------------------------------------------------------------------------
// �֐���: SceneTitle::~SceneTitle()
// ����:   SceneTitle�̃f�X�g���N�^
// ---------------------------------------------------------------------------
SceneTitle::~SceneTitle()
{
	delete mUITitle;
}
// ---------------------------------------------------------------------------
// �֐���: void SceneTitle::Update(float deltaTime)
// ����:   float deltaTime 
// ����:   �X�V����
// ---------------------------------------------------------------------------
void SceneTitle::Update(float deltaTime)
{
	UpdateObjects(deltaTime);
	UpdateManagers(deltaTime);
	mUITitle->Update(deltaTime);

}
// ---------------------------------------------------------------------------
// �֐���: void SceneTitle::DrawUI()
// ����:   UI�`�揈��
// ---------------------------------------------------------------------------
void SceneTitle::DrawUI()
{
}