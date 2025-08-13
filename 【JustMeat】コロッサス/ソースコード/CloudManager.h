#pragma once
#include"Singleton.h"
#include"Vector2.h"
typedef struct
{
	int useTextureID;
	Vector2 pos;	//���S����̑��Έʒu
	float scale;
	float xSpeed;
	float currentLifetime;
	float maxLifetime;
	int layer;
	bool isBottom;
} CloudParameter;

class CloudGenerator
{
public:
	//static const int CLOUD_HEIGHT_MIN = 3;	//	�_1�̌����ŏ��l(�u���b�N)
	//static const int CLOUD_HEIGHT_MAX = 5;	//	�_1�̌����ő�l(�u���b�N)
	//static const int CLOUDS_TOTAL_HEIGHT = 20;	//�_�̑w�̌���(�u���b�N)

	static const int CLOUD_LAYER_NUM = 5;	//�_�̑w�̐�
	static const int CLOUD_PER_LAYER = 40;
	static const int CLOUD_MOVE = 50;	//�_�����ł���܂łɈړ����鋗��
	static const int CLOUD_OFFSET_X = 25;	//�����ʒu�̒��S����̋���
	static const int LOWER_CLOUD = 2;	//�_�̑w�̂����V�[���ύX�̍ۂɐ���Ȃ��_�̑w�̐�
public:
	CloudGenerator();
	void SaveClouds(int count[], CloudParameter clouds[][CLOUD_PER_LAYER]);
	void LoadClouds(class Scene* scene, int cloudCount[], CloudParameter clouds[][CLOUD_PER_LAYER], int indexX, int indexY, bool isTop);
	void SetVisible(bool visible);
	void Update(class SceneGame* scene, float deltaTime);
	void RemoveCloud(class Cloud* cloud);
	void ClearClouds();
	int GetIndexX() { return mIndexX; }
	int GetIndexY() { return mIndexY; }
private:
	int mIndexX = 0;
	int mIndexY = 0;
	int mCloudCount[CLOUD_LAYER_NUM] = {};
	class Cloud* mClouds[CLOUD_LAYER_NUM][CLOUD_PER_LAYER] = {};
	bool mIsFirst = true;
	bool mIsTop = true;
	bool mIsVisible = false;
};

class CloudManager : public Singleton<CloudManager>
{

public:
	friend Singleton<CloudManager>;
	CloudManager();
	void Update(class SceneGame* scene, float deltaTime);
	void Reset();
	void AssignCloudGenerator(class Scene* scene, int indexX, int indexY, bool isTop);
	Vector2 GetTopCloudGenPos();
	Vector2 GetBottomCloudGenPos();
	void ClearClouds() { mBottomCloud->ClearClouds(); }	//BottomClouds�̏㕔���������ς݂ɂ���B(2�X�e�[�W�ڈȍ~����J�n����Ƃ��p)
private:
	class CloudGenerator* mTopCloud;
	class CloudGenerator* mBottomCloud;
	CloudParameter mSaveTopCloudsParam[CloudGenerator::CLOUD_LAYER_NUM][CloudGenerator::CLOUD_PER_LAYER] = {};
	CloudParameter mSaveBottomCloudsParam[CloudGenerator::CLOUD_LAYER_NUM][CloudGenerator::CLOUD_PER_LAYER] = {};
	int mSaveTopCloudCount[CloudGenerator::CLOUD_LAYER_NUM] = {};
	int mSaveBottomCloudCount[CloudGenerator::CLOUD_LAYER_NUM] = {};
};

#include"Object.h"
class Cloud : public Object
{
public:
	Cloud(class Scene* scene, Vector2 position, CloudParameter& param, CloudGenerator* center);
	~Cloud() { mCenter->RemoveCloud(this); }
	void SetLifeTimeScale(float scale) { mTimeScale = scale; }
	void SetSpeedScale(float scale) { mSpeedScale = scale; }
	void UpdateObject(float deltaTime) override;
	void SetAlpha(unsigned a);
	CloudParameter GetParameter();
private:
	CloudGenerator* mCenter;
	CloudParameter mParameter;
	float mTimeScale = 1;
	float mSpeedScale = 1;
};

class CloudGeneratorDummy : public Object
{
public:
	CloudGeneratorDummy(class Scene* scene, int indexX, int indexY, bool isTop);
	void UpdateObject(float deltaTime) override {}
};