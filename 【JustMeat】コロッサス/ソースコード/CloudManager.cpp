#include "CloudManager.h"
#include "ComponentAnimationSprite.h"
#include "StageManager.h"
#include "SceneGame.h"
#include "myMath.h"
#include "obj_player.h"
#include "CameraManager.h"
#include "AudioData.h"
CloudManager::CloudManager()
	: Singleton()
{
	mTopCloud = new CloudGenerator;
	mBottomCloud = new CloudGenerator;
	PlayBGM(BGM_WIND, 6, 0);
}

void CloudManager::Update(SceneGame * scene, float deltaTime)
{
	mTopCloud->Update(scene, deltaTime);
	mBottomCloud->Update(scene, deltaTime);
	if (StageManager::ToIndexY(scene->GetPlayer()->GetPosition().y) < mTopCloud->GetIndexY())
	{
		scene->NextStage();
		mTopCloud->SaveClouds(mSaveTopCloudCount, mSaveTopCloudsParam);
		mBottomCloud->SaveClouds(mSaveBottomCloudCount, mSaveBottomCloudsParam);
	}
	if (StageManager::ToIndexY(scene->GetPlayer()->GetPosition().y) < mTopCloud->GetIndexY() + 10)
	{
		scene->GetCameraManager()->SetFollowCenter(true);
	}
	else
	{
		scene->GetCameraManager()->SetFollowCenter(false);
	}
}

void CloudManager::Reset()
{
	mTopCloud->SetVisible(false);
	mBottomCloud->SetVisible(false);
}

void CloudManager::AssignCloudGenerator(Scene * scene, int indexX, int indexY, bool isTop)
{
	if (isTop)	//ã‰º“ü‚ê‘Ö‚¦‚é
	{
		mTopCloud->LoadClouds(scene, mSaveBottomCloudCount, mSaveBottomCloudsParam, indexX, indexY, true);
	}
	else
	{
		mBottomCloud->LoadClouds(scene, mSaveTopCloudCount, mSaveTopCloudsParam, indexX, indexY, false);
	}
}

Vector2 CloudManager::GetTopCloudGenPos()
{
	return StageManager::ToPosition(mTopCloud->GetIndexX(), mTopCloud->GetIndexY());
}

Vector2 CloudManager::GetBottomCloudGenPos()
{
	return StageManager::ToPosition(mBottomCloud->GetIndexX(), mBottomCloud->GetIndexY());
}



CloudGenerator::CloudGenerator()
{
}

void CloudGenerator::LoadClouds(Scene* scene, int cloudCount[], CloudParameter clouds[][CLOUD_PER_LAYER], int indexX, int indexY, bool isTop)
{
	mIsTop = isTop;
	mIndexX = indexX;
	mIndexY = indexY;
	SetVisible(true);

	for (int i = 0; i < CLOUD_LAYER_NUM; i++)
	{
		if (cloudCount)
			mCloudCount[i] = cloudCount[i];
		else
			mCloudCount[i] = 0;
		
		for (int j = 0; j < mCloudCount[i]; j++)
		{
			CloudParameter param = clouds[i][j];
			mClouds[i][j] = new Cloud(scene, StageManager::ToPosition(mIndexX, mIndexY), param, this);
			mClouds[i][j]->SetAlpha(mIsVisible ? 0xFF : 0x00);
		}
	}

}

void CloudGenerator::SetVisible(bool visible)
{
	mIsVisible = visible;
	for (int i = 0; i < CLOUD_LAYER_NUM; i++)
	{
		for (int j = 0; j < mCloudCount[i]; j++)
		{
			mClouds[i][j]->SetAlpha(mIsVisible ? 0xFF : 0x00);
		}
	}
}

void CloudGenerator::Update(SceneGame * scene, float deltaTime)
{
	if (mIsFirst)
	{
		mIsFirst = false;
		for (int i = 0; i < CLOUD_LAYER_NUM; i++)
		{
			mCloudCount[i] = 0;
			for (int j = 0; j < CLOUD_PER_LAYER; j++)
			{
				CloudParameter param;
				param.xSpeed = BLOCKSIZE * randomRange(0.5f, 2.5f) * (rand() % 2 ? 1 : -1);
				param.currentLifetime = param.maxLifetime = CLOUD_MOVE * BLOCKSIZE / fabsf(param.xSpeed);
				param.scale = randomRange(0.5f, 1.5f);
				mClouds[i][j] = new Cloud(scene, StageManager::ToPosition(param.xSpeed > 0 ? mIndexX - CLOUD_OFFSET_X : mIndexX + CLOUD_OFFSET_X, mIndexY + (i - CLOUD_LAYER_NUM / 2.0f) * 4) + Vector2(0, randomRange(-BLOCKSIZE * 2, BLOCKSIZE * 2)), param, this);
				mClouds[i][j]->UpdateObject(randomRange(5, 20));
				mClouds[i][j]->SetAlpha(mIsVisible ? 0xFF : 0x00);
				mCloudCount[i]++;
			}
		}
	}
	if (mIsTop)
	{
		for (int i = 0; i < CLOUD_LAYER_NUM; i++)
		{
			if (mCloudCount[i] < CLOUD_PER_LAYER)
			{
				CloudParameter param;
				param.xSpeed = BLOCKSIZE * randomRange(0.5f, 2.5f) * (rand() % 2 ? 1 : -1);
				param.currentLifetime = param.maxLifetime = CLOUD_MOVE * BLOCKSIZE / fabsf(param.xSpeed);
				param.scale = randomRange(0.5f, 1.5f);
				mClouds[i][mCloudCount[i]] = new Cloud(scene, StageManager::ToPosition(param.xSpeed > 0 ? mIndexX - CLOUD_OFFSET_X : mIndexX + CLOUD_OFFSET_X, mIndexY + (i - CLOUD_LAYER_NUM / 2.0f) * 4) + Vector2(0, randomRange(-BLOCKSIZE * 2, BLOCKSIZE * 2)), param, this);
				mClouds[i][mCloudCount[i]]->SetAlpha(mIsVisible ? 0xFF : 0x00);
				mCloudCount[i]++;
			}
		}
	}
	else
	{
		for (int i = 0; i < CLOUD_LAYER_NUM - LOWER_CLOUD; i++)
		{
			for (int j = 0; j < mCloudCount[i]; j++)
			{
				mClouds[i][j]->SetLifeTimeScale(((CLOUD_LAYER_NUM - LOWER_CLOUD) - i) * 5);
				mClouds[i][j]->SetSpeedScale(-((CLOUD_LAYER_NUM - LOWER_CLOUD) - i) * 2);
			}
		}
		for (int i = CLOUD_LAYER_NUM - LOWER_CLOUD; i < CLOUD_LAYER_NUM; i++)
		{
			if (mCloudCount[i] < CLOUD_PER_LAYER)
			{
				CloudParameter param;
				param.xSpeed = BLOCKSIZE * randomRange(0.5f, 2.5f) * (rand() % 2 ? 1 : -1);
				param.currentLifetime = param.maxLifetime = CLOUD_MOVE * BLOCKSIZE / fabsf(param.xSpeed);
				param.scale = randomRange(0.5f, 1.5f);
				mClouds[i][mCloudCount[i]] = new Cloud(scene, StageManager::ToPosition(param.xSpeed > 0 ? mIndexX - CLOUD_OFFSET_X : mIndexX + CLOUD_OFFSET_X, mIndexY + (i - CLOUD_LAYER_NUM / 2.0f) * 4), param, this);
				mCloudCount[i]++;
			}
		}
	}
}

void CloudGenerator::RemoveCloud(Cloud * cloud)
{
	for (int i = 0; i < CLOUD_LAYER_NUM; i++)
	{
		for (int j = 0; j < mCloudCount[i]; j++)
		{
			if (mClouds[i][j] == cloud)
			{
				mClouds[i][j] = nullptr;
				mCloudCount[i]--;
				mClouds[i][j] = mClouds[i][mCloudCount[i]];
				return;
			}
		}
	}
}

void CloudGenerator::ClearClouds()
{
	for (int i = 0; i < CLOUD_LAYER_NUM - LOWER_CLOUD; i++)
	{
		for (int j = 0; j < mCloudCount[i]; j++)
		{

			mClouds[i][j]->SetAlpha(0x00);
		}
	}
}

void CloudGenerator::SaveClouds(int count[], CloudParameter clouds[][CLOUD_PER_LAYER])
{

	for (int i = 0; i < CLOUD_LAYER_NUM; i++)
	{
		count[i] = mCloudCount[i];
		for (int j = 0; j < mCloudCount[i]; j++)
		{
			clouds[i][j] = mClouds[i][j]->GetParameter();
		}
	}
}


Cloud::Cloud(Scene * scene, Vector2 position, CloudParameter& param, CloudGenerator* center)
	: Object(scene, position)
	, mParameter(param)
	, mCenter(center)
{
	mCASprite = new ComponentAnimationSprite(this, TextureTag::Cloud, 0);
	int useID = rand() % 3;
	mCASprite->SetAnimation(useID * 2, useID * 2 + 1, 1, 0, false);
	SetSize(Vector2(12, 6)*BLOCKSIZE * param.scale);
	Translate(param.pos);
	if (param.xSpeed > 0) SetDirection(Dir::eRight);
}

void Cloud::UpdateObject(float deltaTime)
{
	mParameter.currentLifetime -= deltaTime * mTimeScale;

	Translate(Vector2(mParameter.xSpeed * deltaTime * mSpeedScale, 0));

	float percentage = mParameter.currentLifetime / mParameter.maxLifetime;
	if (percentage > 1)percentage = 1;
	if (percentage < 0)percentage = 0;
	if (percentage > 0.8f)
	{
		mCASprite->SetAlphaRate((1 - percentage) * 5);
	}
	else if (percentage > 0.6f)
	{
		mCASprite->SetAlphaRate((percentage - 0.6f) * 5);
	}
	else if (percentage > 0.4f)
	{
		mCASprite->SetAlphaRate(0);
	}
	else if (percentage > 0.2f)
	{
		mCASprite->SetAlphaRate((0.4f - percentage) * 5);
	}
	else
	{
		mCASprite->SetAlphaRate((percentage) * 5);
	}
	//mCASprite->SetAlphaRate(1 - fabsf(2 * percentage - 1));
	if (mParameter.currentLifetime < 0)
	{
		SetState(State::eDead);
	}

}

void Cloud::SetAlpha(unsigned a)
{
	mCASprite->SetAlpha(a);
	if (a == 0xFF)
	{
		mCASprite->SetAlpha(0x7F);
	}
}

CloudParameter Cloud::GetParameter()
{
	CloudParameter param = mParameter;
	param.pos = GetPosition() - StageManager::ToPosition(mCenter->GetIndexX(), mCenter->GetIndexY());
	return param;
}


CloudGeneratorDummy::CloudGeneratorDummy(Scene * scene, int indexX, int indexY, bool isTop)
	: Object(scene, StageManager::ToPosition(indexX, indexY))
{
	CloudManager::Inst()->AssignCloudGenerator(scene, indexX, indexY, isTop);
	//SetState(State::eDead);
	mCASprite = new ComponentAnimationSprite(this, TextureTag::Invisible, -999);
}
