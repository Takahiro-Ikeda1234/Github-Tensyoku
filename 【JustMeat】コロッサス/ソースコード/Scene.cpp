//=============================================================================
//
// [Scene.cpp] ��ʋ��ʏ���
//
//=============================================================================

//�C���N���[�h�t�@�C��
#include"Scene.h"
#include"Object.h"
#include"UIObject.h"
#include"ComponentSprite.h"
#include"CameraManager.h"
#include"StageManager.h"
#include "EventManager.h"
#include"Parameter.h"
#include"obj_player.h"
#include"SceneManager.h"
// ---------------------------------------------------------------------------
// �֐���: Scene::Scene(SceneManager* sceneManager)
// ����:   SceneManager* sceneManager �V�[���}�l�[�W���̃|�C���^
// ����:   Scene�̃R���X�g���N�^
// ---------------------------------------------------------------------------
Scene::Scene(SceneManager* sceneManager)
	: mSceneManager(sceneManager)
	, mIsUpdating(false)
	, mIsSleep(false)
	, mTextColor(0xFFFFFFFF)
{
	mCameraManager = new CameraManager(this);
	mStageManager = new StageManager(this);
	mEventManager = new EventManager(this);

}
// ---------------------------------------------------------------------------
// �֐���: Scene::~Scene()
// ����:   Scene�̃f�X�g���N�^
// ---------------------------------------------------------------------------
Scene::~Scene()
{
	//�V�[�������[�h���Ă���v�f��S�폜
	while (!mObjects.empty())
	{
		delete mObjects.back();
	}
	while (!mUIObjects.empty())
	{
		delete mUIObjects.back();
	}
	delete mCameraManager;
	delete mStageManager;
	delete mEventManager;
}
// ---------------------------------------------------------------------------
// �֐���: void Scene::Draw()
// ����:   �`�揈��
// ---------------------------------------------------------------------------
void Scene::Draw()
{
	for (auto it : mSprites)
	{
		it->Draw();
	}
}
// ---------------------------------------------------------------------------
// �֐���: void Scene::AddObject(Object* object)
// ����:   Object* object 
// ����:   �I�u�W�F�N�g�̒ǉ�
// ---------------------------------------------------------------------------
void Scene::AddObject(Object* object)
{
	if (mIsUpdating)
	{
		mNewObjects.emplace_back(object);
	}
	else
	{
		mObjects.emplace_back(object);
	}
}
// ---------------------------------------------------------------------------
// �֐���: void Scene::RemoveObject(Object* object)
// ����:   Object* object 
// ����:   �I�u�W�F�N�g�̍폜
// ---------------------------------------------------------------------------
void Scene::RemoveObject(Object* object)
{
	//�ǉ��҂��I�u�W�F�N�g�̍폜
	auto itr1 = std::find(mNewObjects.begin(), mNewObjects.end(), object);
	if (itr1 != mNewObjects.end())
	{
		std::iter_swap(itr1, mNewObjects.end() - 1);
		mNewObjects.pop_back();
	}

	//�I�u�W�F�N�g�̍폜
	auto itr2 = std::find(mObjects.begin(), mObjects.end(), object);
	if (itr2 != mObjects.end())
	{
		std::iter_swap(itr2, mObjects.end() - 1);
		mObjects.pop_back();
	}
}
void Scene::AddUIObject(UIObject * object)
{
	if (mIsUpdating)
	{
		mNewUIObjects.emplace_back(object);
	}
	else
	{
		mUIObjects.emplace_back(object);
	}
}
void Scene::RemoveUIObject(UIObject * object)
{
	//�ǉ��҂��I�u�W�F�N�g�̍폜
	auto itr1 = std::find(mNewUIObjects.begin(), mNewUIObjects.end(), object);
	if (itr1 != mNewUIObjects.end())
	{
		std::iter_swap(itr1, mNewUIObjects.end() - 1);
		mNewUIObjects.pop_back();
	}

	//�I�u�W�F�N�g�̍폜
	auto itr2 = std::find(mUIObjects.begin(), mUIObjects.end(), object);
	if (itr2 != mUIObjects.end())
	{
		std::iter_swap(itr2, mUIObjects.end() - 1);
		mUIObjects.pop_back();
	}
}
// ---------------------------------------------------------------------------
// �֐���: void Scene::AddSprite(ComponentSprite* sprite)
// ����:   ComponentSprite* sprite 
// ����:   �X�v���C�g�̒ǉ�
// ---------------------------------------------------------------------------
void Scene::AddSprite(ComponentSprite* sprite)
{
	int priority = sprite->GetDrawPriority();
	if (priority > LAYER_OBJECT)
	{
		auto itr = mSprites.begin();
		auto end = mSprites.end();
		for (; itr != end; ++itr)
		{
			if (priority > (*itr)->GetDrawPriority() - 1)
			{
				break;
			}
		}
		mSprites.insert(itr, sprite);
	}
	else
	{
		if (mSprites.size() > 0)
		{
			auto itr = mSprites.end() - 1;
			auto end = mSprites.begin();
			for (; ; --itr)
			{
				if (priority < (*itr)->GetDrawPriority() + 1)
				{
					mSprites.insert(itr + 1, sprite);
					break;
				}
				else if (itr == end)
				{
					mSprites.insert(itr, sprite);
					break;
				}
			}
			
		}
		else
		{
			mSprites.insert(mSprites.begin(), sprite);
		}
	}

}// ---------------------------------------------------------------------------
// �֐���: void Scene::RemoveSprite(ComponentSprite* sprite)
// ����:   ComponentSprite* sprite 
// ����:   �X�v���C�g�̍폜
// ---------------------------------------------------------------------------
void Scene::RemoveSprite(ComponentSprite* sprite)
{
	auto itr = std::find(mSprites.begin(), mSprites.end(), sprite);

	if (itr != mSprites.end())
	{
		mSprites.erase(itr);
	}
}
void Scene::AddMoveFloor(MoveFloorObject * object)
{
	mMoveFloors.push_back(object);
}
void Scene::RemoveMoveFloor(MoveFloorObject * object)
{
	auto itr = std::find(mMoveFloors.begin(), mMoveFloors.end(), object);

	if (itr != mMoveFloors.end())
	{
		mMoveFloors.erase(itr);
	}
}
void Scene::AddTriggeredMoveFloor(TriggeredMoveFloorObject * object)
{
	mTriggeredMoveFloors.push_back(object);
}
void Scene::RemoveTriggeredMoveFloor(TriggeredMoveFloorObject * object)
{
	auto itr = std::find(mTriggeredMoveFloors.begin(), mTriggeredMoveFloors.end(), object);

	if (itr != mTriggeredMoveFloors.end())
	{
		mTriggeredMoveFloors.erase(itr);
	}
}
void Scene::AddTrigger(ComponentTrigger * object)
{
	mTriggers.push_back(object);
}
void Scene::RemoveTrigger(ComponentTrigger * object)
{
	auto itr = std::find(mTriggers.begin(), mTriggers.end(), object);

	if (itr != mTriggers.end())
	{
		mTriggers.erase(itr);
	}
}
// ---------------------------------------------------------------------------
// �֐���: void Scene::UpdateObjects(float deltaTime)
// ����:   float deltaTime 
// ����:   �S�I�u�W�F�N�g�̍X�V
// ---------------------------------------------------------------------------
void Scene::UpdateObjects(float deltaTime)
{

	if (mIsSleep)return;


	mIsUpdating = true;
	float slow = mEventManager->GetSlow();
	const float objectDeltaTime = slow * deltaTime;

	if (!mIsPause)
	{
		for (auto it : mObjects)
		{
			it->Update(objectDeltaTime);
		}
	}
	for (auto it : mUIObjects)
	{
		it->Update(deltaTime);
	}
	mIsUpdating = false;

	for (auto it : mNewObjects)
	{
		mObjects.emplace_back(it);
	}
	for (auto it : mNewUIObjects)
	{
		mUIObjects.emplace_back(it);
	}
	mNewObjects.clear();
	mNewUIObjects.clear();

	std::vector<Object*> destroyObjects;
	std::vector<UIObject*> destroyUIObjects;
	for (auto it : mObjects)
	{
		if (it->GetState() == AbstractObject::State::eDead)
		{
			destroyObjects.emplace_back(it);
		}
	}
	for (auto it : mUIObjects)
	{
		if (it->GetState() == AbstractObject::State::eDead)
		{
			destroyUIObjects.emplace_back(it);
		}
	}
	for (auto it : destroyObjects)
	{
		delete it;
	}
	for (auto it : destroyUIObjects)
	{
		delete it;
	}
	if (GetPlayer())
	{
		if (GetPlayer()->IsDead() && GetSceneManager()->CanSceneChange())
		{
			AllObjectLoadCheckPointData();
		}
	}

}
// ---------------------------------------------------------------------------
// �֐���: void Scene::UpdateManagers(float deltaTime)
// ����:   float deltaTime 
// ����:   �S�}�l�[�W���̍X�V
// ---------------------------------------------------------------------------
void Scene::UpdateManagers(float deltaTime)
{
	mCameraManager->Update(deltaTime);
	mEventManager->Update(deltaTime);
}
// ---------------------------------------------------------------------------
// �֐���: std::vector<Object*>* Scene::OnCollision(Vector2 pos, Vector2 col, Object* object, Object::Type objectType)
// ����:   Vector2 pos �ʒu
// ����:   Vector2 col �T�C�Y
// ����:   Object* object ���肷��I�u�W�F�N�g
// ����:   Object::Type objectType ��������I�u�W�F�N�g�^�C�v
// �ߒl:   �ڐG���Ă���S�I�u�W�F�N�g
// ����:   �d�Ȃ�I�u�W�F�N�g�擾�֐�
// ---------------------------------------------------------------------------
std::vector<Object*> Scene::OnCollision(Vector2 pos, Vector2 col, Object* object, Object::Type objectType)
{
	std::vector<Object*>  findObjects;
	for (auto it : mObjects)
	{
		if (it->GetObjectType() != objectType) continue;
		float range = it->GetActiveRange() + object->GetActiveRange();
		float x = fabsf(it->GetPosition().x - pos.x);
		if (x > range)continue;
		float y = fabsf(it->GetPosition().y - pos.y);
		if (y > range)continue;
		if (object == it)continue;

		Vector2 objCol = col;
		Vector2 itCol = it->GetColSize();
		if (x < (objCol.x + itCol.x) && y < (objCol.y + itCol.y))
		{
			findObjects.push_back(it);
		}
	}
	return findObjects;
}
// ---------------------------------------------------------------------------
// �֐���: std::vector<Object*>* Scene::FindObjects(Object* object, Object::Type objectType)
// ����:   Object* object ���肷��I�u�W�F�N�g
// ����:   Object::Type objectType ��������I�u�W�F�N�g�^�C�v
// �ߒl:   �Ώۂ̑S�I�u�W�F�N�g
// ����:   �S�I�u�W�F�N�g�擾�֐�
// ---------------------------------------------------------------------------
std::vector<Object*> Scene::FindObjects(Object* object, Object::Type objectType)
{
	std::vector<Object*>  findObjects;

	for (auto it : mObjects)
	{
		if (it->GetObjectType() != objectType)continue;
		float range = it->GetActiveRange() /* + object->GetActiveRange()*/;
		findObjects.push_back(it);
	}
	return findObjects;
}


void Scene::AllObjectSaveCheckPointData()
{
	mIsUpdating = true;
	for (auto it : mObjects)
	{
		it->SaveCheckPointData();
	}
	mIsUpdating = false;
}

void Scene::AllObjectLoadCheckPointData()
{
	mIsUpdating = true;
	for (int i = 0; i < mObjects.size(); i++)
	{
		mObjects[i]->LoadCheckPointData();
	}
	mIsUpdating = false;
}