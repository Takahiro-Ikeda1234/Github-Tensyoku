#include "obj_laser.h"
#include "StageObject.h"
#include "ComponentAnimationSprite.h"
#include "obj_golemEvent.h"
#include "MoveFloorObject.h"
#include "ParticleSystem.h"
#include "obj_player.h"
#include "AudioData.h"
//dir...0:ç∂å¸Ç´,1:âEå¸Ç´,2:è„å¸Ç´,3:â∫å¸Ç´
static const float UPDATE_INTERVAL = 0.05f;
LaserCannon::LaserCannon(Scene * scene, int indexX, int indexY, int dir)
	: Object(scene, StageManager::ToPosition(indexX, indexY))
	, mDir(dir)
	, mIndexX(indexX)
	, mIndexY(indexY)
	, mUpdateInterval(UPDATE_INTERVAL)
{
	int rot = 0;
	switch (mDir)
	{
	case 0:
		rot = 90;
		break;
	case 1:
		rot = 270;
		break;
	case 2:
		rot = 180;
		break;
	case 3:
		rot = 0;
		break;
	}
	Rotation(rot);
	SetObjectType(Type::eLaser);
	mCASprite = new ComponentAnimationSprite(this, TextureTag::Laser, LAYER_OBJECT - 1);

	Vector2 pos = GetPosition();
	PlayBGM(BGM_LASER, 5, 0);
}

LaserCannon::~LaserCannon()
{
	StopBGM(5);
}

void LaserCannon::UpdateObject(float deltaTime)
{
	if (mFirstUpdate)
	{
		GenerateLaser();
		mFirstUpdate = false;
	}
	UpdateLaserCollision(deltaTime);
	mUpdateInterval -= deltaTime;
	if (mUpdateInterval.IsFinish())
	{
		int x = 0;
		int y = 0;
		switch (mDir)
		{
		case 0:
			x = -1;
			break;
		case 1:
			x = 1;
			break;
		case 2:
			y = -1;
			break;
		case 3:
			y = 1;
			break;
		}

		float rx = 0;
		float ry = 0;
		switch (mDir)
		{
		case 0:
		case 1:
			ry = 0.05f;
			break;
		case 2:
		case 3:
			rx = 0.05f;
			break;
		}

		for (int j = 1; j < LASER_LAYER; j++)
		{
			if (rand() % 2)continue;
			float randX = randomRange(-rx, rx);
			float randY = randomRange(-ry, ry);
			for (int i = 0; i < mLaserLength; i++)
			{
				mLaser[j][i]->SetPosition(StageManager::ToPosition(mIndexX + x * i, mIndexY + y * i) + Vector2(randX, randY) * BLOCKSIZE);
			}
		}
	}

	if (mUpdateInterval.IsFinish())
	{
		GenerateParticle(mTerminalPos);
	}
}

void LaserCannon::UpdateLaserCollision(float deltaTime)
{
	auto objects = GetScene()->GetMoveFloors();
	Vector2 lasercol;
	if (mDir == 0 || mDir == 1)
	{
		lasercol = Vector2(mLaser[0][0]->GetColSize().y, mLaser[0][0]->GetColSize().x) * 0.9f;
	}
	else
	{
		lasercol = mLaser[0][0]->GetColSize() * 0.9f;
	}
	for (int i = 0; i < mLaserLength; i++)
	{
		mLaser[0][i]->GetCASprite()->SetAlphaRate(1);
		UpdateTerminal(mLaser[0][i]);
		for (int k = 1; k < LASER_LAYER; k++)
		{
			mLaser[k][i]->GetCASprite()->SetAlphaRate(0.5f);
		}
		bool mIsHit = false;
		for (auto it : objects)
		{
			if (Vector2::RectCollision(mLaser[0][i]->GetPosition(), lasercol, it->GetPosition(), it->GetColSize()) && it->GetObjectType()==Type::eSolid)
			{
				mIsHit = true;
				break;
			}
		}
		if (!mIsHit && GetScene()->GetGolem()->HitGolemArm(mLaser[0][i]->GetPosition(), lasercol) != nullptr)
		{
			mIsHit = true;
		}

		if (i != 0 && mIsHit)
		{
			for (int k = 0; k < LASER_LAYER; k++)
			{
				for (int j = i; j < mLaserLength; j++)
				{
					mLaser[k][j]->GetCASprite()->SetAlphaRate(0);
				}
			}
			UpdateTerminal(mLaser[0][i - 1]);
			return;
		}
		Vector2 playerPos = GetScene()->GetPlayer()->GetPosition();
		Vector2 playerCol = GetScene()->GetPlayer()->GetColSize();
		if (!GetScene()->GetPlayer()->IsDying() && Vector2::RectCollision(playerPos, playerCol, mLaser[0][i]->GetPosition(), lasercol))
		{
			int x = 0;
			int y = 0;
			int sx = 0;
			int sy = 0;
			switch (mDir)
			{
			case 0:
				x = -1;
				sy = 1;
				break;
			case 1:
				x = 1;
				sy = 1;
				break;
			case 2:
				sx = 1;
				y = -1;
				break;
			case 3:
				sx = 1;
				y = 1;
				break;
			}


			bool escape = false;
			for (int b = -5; b < 5; b++)
			{
				for (int scan = -2; scan <= 2; scan++)
				{
					if (Vector2::RectCollision(mLaser[0][i]->GetPosition() + Vector2(0.25f * sx, 0.25f * sy) * scan * BLOCKSIZE + Vector2(0.1f * b * x, 0.1f * b * y) * BLOCKSIZE, Vector2(BLOCKSIZE / 10, BLOCKSIZE / 10), playerPos, playerCol))
					{
						Vector2 pos = mLaser[0][i]->GetPosition() + Vector2(0.25f * sx, 0.25f * sy) * scan * BLOCKSIZE + Vector2(0.1f * b * x, 0.1f * b * y) * BLOCKSIZE;
						GenerateParticlePlayerHit(pos);
						GetScene()->GetPlayer()->HitLaser(1);
						escape = true;
						break;
					}
				}
				if (escape)break;
			}
		}
	}
}

void LaserCannon::GenerateLaser()
{
	int x = 0;
	int y = 0;
	switch (mDir)
	{
	case 0:
		x = -1;
		break;
	case 1:
		x = 1;
		break;
	case 2:
		y = -1;
		break;
	case 3:
		y = 1;
		break;
	}
	for (int i = 0; i < LASER_MAX_LENGTH; i++)
	{
		if (GetScene()->GetStageManager()->GetStageData(StageManager::eStageObjectLayerBlock, mIndexX + x * i, mIndexY + y * i) > 0)
		{
			return;
		}
		mLaserLength++;
		for (int j = 0; j < LASER_LAYER; j++)
		{
			int tex = 2;
			int layer = LAYER_OBJECT;
			if (i == 0) tex = 1;
			if (j == 0) layer -= 2;
			int laserColor = 0;
			switch (j)
			{
			case 2:
			case 5:
			case 8:
				laserColor = 2;
				break;
			case 3:
			case 6:
			case 9:
				laserColor = 4;
				break;
			default:
				break;
			}
			mLaser[j][i] = new Laser(GetScene(), StageManager::ToPosition(mIndexX + x * i, mIndexY + y * i), tex + laserColor, mDir, layer);
			UpdateTerminal(mLaser[j][i]);
		}
	}
}

void LaserCannon::UpdateTerminal(Object * laser)
{
	int x = 0;
	int y = 0;
	switch (mDir)
	{
	case 0:
		x = -1;
		break;
	case 1:
		x = 1;
		break;
	case 2:
		y = -1;
		break;
	case 3:
		y = 1;
		break;
	}
	mTerminalPos = laser->GetPosition() + Vector2(x * 0.5f, y * 0.5f) * BLOCKSIZE;
}

void LaserCannon::GenerateParticle(Vector2 pos)
{
	ParticleInfo info;
	info.useGravity = true;
	info.generateIntervalMin = 0;
	info.generateIntervalMax = 0;
	info.tex = TextureTag::Plane;
	info.addBlend = true;
	info.moveForward = true;
	info.lifetimeMin = 0.1f;
	info.lifetimeMax = 0.2f;
	info.particlePerGenerateMin = 5;
	info.particlePerGenerateMax = 5;
	info.spawnType = SpawnType::eLine;
	info.spawnLineLength = BLOCKSIZE * 0.25f;
	info.spawnLineWidth = 0;
	info.size = Vector2(BLOCKSIZE, BLOCKSIZE) * 0.1f;
	info.scaleMin = 0.25f;
	info.scaleMax = 1;
	info.speedMin = BLOCKSIZE * 2.5f;
	info.speedMax = BLOCKSIZE * 5;
	info.alphaRateMin = 0.9f;
	info.alphaRateMax = 1.0f;
	info.greenRateMin = 0.25f;
	info.greenRateMax = 0.5f;
	info.blueRateMin = 0;
	info.blueRateMax = 0;
	if (mDir == 0 || mDir == 1)
	{
		info.spawnLineDeg = 90;
	}

	switch (mDir)
	{
	case 0:
		info.degMin = -90;
		info.degMax = 90;
		break;
	case 1:
		info.degMin = 90;
		info.degMax = 270;
		break;
	case 2:
		info.degMin = 0;
		info.degMax = 180;
		break;
	case 3:
		info.degMin = 180;
		info.degMax = 360;
		break;
	}

	new ParticleSystem(GetScene(), pos, &info, 1, true);
}
void LaserCannon::GenerateParticlePlayerHit(Vector2 pos)
{
	ParticleInfo info;
	info.useGravity = true;
	info.gravity = Vector2(0, GRAVITY);
	info.generateIntervalMin = 0;
	info.generateIntervalMax = 0;
	info.tex = TextureTag::Plane;
	info.addBlend = true;
	info.moveForward = true;
	info.lifetimeMin = 0.1f;
	info.lifetimeMax = 0.15f;
	info.particlePerGenerateMin = 50;
	info.particlePerGenerateMax = 50;
	info.spawnType = SpawnType::eLine;
	info.spawnLineLength = BLOCKSIZE * 0.25f;
	info.spawnLineWidth = 0;
	info.size = Vector2(BLOCKSIZE, BLOCKSIZE) * 0.1f;
	info.scaleMin = 0.25f;
	info.scaleMax = 1;
	info.speedMin = BLOCKSIZE * 5;
	info.speedMax = BLOCKSIZE * 10;
	info.alphaRateMin = 0.5f;
	info.alphaRateMax = 1.0f;
	info.greenRateMin = 0.25f;
	info.greenRateMax = 0.5f;
	info.blueRateMin = 0;
	info.blueRateMax = 0;
	if (mDir == 0 || mDir == 1)
	{
		info.spawnLineDeg = 90;
	}

	new ParticleSystem(GetScene(), pos, &info, 1, true);
}



Laser::Laser(Scene * scene, Vector2 pos, int texID, int dir, int layer)
	: Object(scene, pos)
{
	mCASprite = new ComponentAnimationSprite(this, TextureTag::Laser, layer);
	mCASprite->SetAnimation(texID, texID, 1, 0);
	if (layer == LAYER_OBJECT)
	{
		mCASprite->SetAlpha(0x1F);
		mCASprite->SetDrawMode(ComponentSprite::DrawMode::eAdd);
		mCASprite->SetScale(4, 1.000f);
	}
	else
	{
		mCASprite->SetAlpha(0xFF);		
		mCASprite->SetScale(3, 1.001f);
	}
	int rot = 0;
	SetSize(Vector2(0.25f, 1) * BLOCKSIZE);
	

	switch (dir)
	{
	case 0:
		rot = 90;
		break;
	case 1:
		rot = 270;
		break;
	case 2:
		rot = 180;
		break;
	case 3:
		rot = 0;
		break;
	}
	Rotation(rot);
}

void Laser::UpdateObject(float deltaTime)
{
}
