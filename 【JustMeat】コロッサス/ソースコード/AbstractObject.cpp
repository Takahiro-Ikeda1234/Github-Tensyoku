//=============================================================================
//
// [Object.cpp] �I�u�W�F�N�g���ʏ���
//
//=============================================================================

//�C���N���[�h�t�@�C��
#include"Scene.h"
#include<vector>
#include"Component.h"
#include"AbstractObject.h"
#include"config.h"
// ---------------------------------------------------------------------------
// �֐���: Object::Object(Scene* scene)
// ����:   Scene* scene �V�[���̃|�C���^
// ����:   AbstractObject�̃R���X�g���N�^
// ---------------------------------------------------------------------------
AbstractObject::AbstractObject(Scene* scene)
	: mScene(scene)
	, mPosition(Vector2(0, 0))
	, mRotationDeg(0)
	, mSize(Vector2(0, 0))
	, mColSize(Vector2(0, 0))
	, mActiveRange(0)
	, mScale(Vector2(1, 1))
	, mState(State::eActive)
	, mObjectType(Type::eNone)
	, mDir(Dir::eRight)
{
	SetSize(Vector2(BLOCKSIZE, BLOCKSIZE));
}
// ---------------------------------------------------------------------------
// �֐���: AbstractObject::~AbstractObject()
// ����:   AbstractObject�̃f�X�g���N�^
// ---------------------------------------------------------------------------
AbstractObject::~AbstractObject()
{
	//�S���Ȃ��Ȃ�܂ŃR���|�[�l���g��delete
	while (!mComponents.empty())
	{
		delete mComponents.back();
	}
}
// ---------------------------------------------------------------------------
// �֐���: void AbstractObject::Update(float deltaTime)
// ����:   float deltaTime 
// ����:   �X�V����
// ---------------------------------------------------------------------------
void AbstractObject::Update(float deltaTime)
{
	if (mState == State::eActive)
	{
		mPrevPos = mPosition;
		UpdateComponents(deltaTime);
		if (!GetScene()->IsPause() || mResistPause)
		{
			UpdateObject(deltaTime);
		}
		if (deltaTime != 0) mMoved = (mPosition - mPrevPos) / deltaTime;
	}
}
// ---------------------------------------------------------------------------
// �֐���: void AbstractObject::UpdateComponents(float deltaTime)
// ����:   float deltaTime 
// ����:   �R���|�[�l���g�̍X�V
// ---------------------------------------------------------------------------
void AbstractObject::UpdateComponents(float deltaTime)
{
	for (auto it : mComponents)
	{
		if (!GetScene()->IsPause() || it->GetResistPause())
		{
			it->Update(deltaTime);
		}
	}
}
// ---------------------------------------------------------------------------
// �֐���: void AbstractObject::AddComponent(Component* component)
// ����:   Component* component 
// ����:   �R���|�[�l���g�̒ǉ�
// ---------------------------------------------------------------------------
void AbstractObject::AddComponent(Component* component)
{
	int priority = component->GetUpdatePriority();
	auto itr = mComponents.begin();
	for (; itr != mComponents.end(); ++itr)
	{
		if (priority < (*itr)->GetUpdatePriority())
		{
			break;
		}
	}
	mComponents.insert(itr, component);
}
// ---------------------------------------------------------------------------
// �֐���: void AbstractObject::RemoveComponent(Component* component)
// ����:   Component* component 
// ����:   �R���|�[�l���g�̍폜
// ---------------------------------------------------------------------------
void AbstractObject::RemoveComponent(Component* component)
{
	auto itr = std::find(mComponents.begin(), mComponents.end(), component);
	if (itr != mComponents.end())
	{
		mComponents.erase(itr);
	}
}