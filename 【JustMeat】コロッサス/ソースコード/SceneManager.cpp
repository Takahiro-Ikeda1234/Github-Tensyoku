//=============================================================================
//
// [SceneManager.cpp] �V�[���J�ڊǗ�
//
//=============================================================================

//�C���N���[�h�t�@�C��
#include"SceneManager.h"
#include"SceneGame.h"
#include"SceneTitle.h"
#include"SceneEditor.h"
#include"SceneStageSelect.h"
#include"Scene.h"
#include"CameraManager.h"
#include"Vector2.h"
#include"DX9_texture.h"
#include"DX9_draw.h"
#include"keyboard.h"
#include"ini.h"
#include"SceneOpening.h"
#include"SceneEnding.h"
#include"SceneCredit.h"
#include"SceneFin.h"
#include"SceneLogo.h"
#include"EventManager.h"

// ---------------------------------------------------------------------------
// �֐���: SceneManager::SceneManager()
// ����:   SceneManager�̃R���X�g���N�^
// ---------------------------------------------------------------------------
SceneManager::SceneManager()
	: mNextScene(eLogo)
	, mCurrentScene(eDefault)
	, mScene(NULL)
	, mShowUI(true)
{
	LoadScene(eLogo);
	mNextParam.Clear();
	mDebugMode = GetConfigInt("System", "Debug");
}
SceneManager::~SceneManager()
{
	UnloadScene();
}
void SceneManager::UnloadScene()
{
	delete mScene;
}
// ---------------------------------------------------------------------------
// �֐���: void SceneManager::LoadScene(SceneID id)
// ����:   SceneID id �ǂݍ��ރV�[���̃^�O
// ����:   �V�[���Ǎ�����
// ---------------------------------------------------------------------------
void SceneManager::LoadScene(SceneID id)
{
	if (mScene != NULL)
	{
		UnloadScene();
	}
	mCurrentScene = mNextScene;
	switch (id)
	{
	case eExit:
		break;
	case eTitle:
		mScene = new SceneTitle(this, mNextParam);
		break;
	case eGame:
		mScene = new SceneGame(this, mNextParam);
		break;
	case eEditor:
		mScene = new SceneEditor(this, mNextParam);
		break;
	case eStageSelect:
		mScene = new SceneStageSelect(this, mNextParam);
		break;
	case eOpening:
		mScene = new SceneOpening(this, mNextParam);
		break;
	case eEnding:
		mScene = new SceneEnding(this, mNextParam);
		break;
	case eCredit:
		mScene = new SceneCredit(this, mNextParam);
		break;
	case eFin:
		mScene = new SceneFin(this, mNextParam);
		break;
	case eLogo:
		mScene = new SceneLogo(this, mNextParam);
		break;
	}
	mNextScene = eDefault;
	mNextParam.Clear();

	if (mWhiteFade)
	{
		mScene->GetEventManager()->SetWhiteFade();
		mWhiteFade = false;
	}
}
// ---------------------------------------------------------------------------
// �֐���: void SceneManager::Update(float deltaTime)
// ����:   float deltaTime 
// ����:   �X�V����
// ---------------------------------------------------------------------------
void SceneManager::Update(float deltaTime)
{
	mScene->Update(deltaTime);
	if (Keyboard::Inst()->KeyHit(DIK_F1))
	{
		mDebugMode = !mDebugMode;
	}
}
// ---------------------------------------------------------------------------
// �֐���: void SceneManager::Draw()
// ����:   �`�揈��
// ---------------------------------------------------------------------------
void SceneManager::Draw()
{
	mScene->Draw();
	mScene->DrawUI();
}
// ---------------------------------------------------------------------------
// �֐���: void SceneManager::SceneChange(SceneID nextScene, Parameter& param)
// ����:   SceneID nextScene ���̃V�[���̃^�O
// ����:   Parameter& param �p�����[�^
// ����:   �V�[���ύX�֐�
// ---------------------------------------------------------------------------
bool SceneManager::SceneChange(SceneID nextScene, Parameter* param, float fadeoutDelay, float fadeoutTime, float fadeinTime, bool whiteFade)
{
	if (!CanSceneChange())return false;
	param->SetF("�t�F�[�h�C������", fadeinTime);
	mNextScene = nextScene;
	mNextParam = *param;
	mNextParam.Set("�O�V�[��", mCurrentScene);
	mScene->GetEventManager()->Fadeout(fadeoutTime, fadeoutDelay);
	param->Clear();
	if (whiteFade)
	{
		mWhiteFade = true;
		mScene->GetEventManager()->SetWhiteFade();
	}
	return true;
}

bool SceneManager::CanSceneChange() const
{
	if (mNextScene != SceneID::eDefault)return false;
	return true;
}
