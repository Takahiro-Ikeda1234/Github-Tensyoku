#pragma once
#include "Object.h"
#include<list>
#include"Counter.h"
#include"myMath.h"
#include"textureData.h"
#include"ComponentSprite.h"
static const float FADEIN_PERCENTAGE = 0.2f;
static const float FADEOUT_PERCENTAGE = 0.8f;
static const float FADEIN_RATE = 1 / FADEIN_PERCENTAGE;
static const float FADEOUT_RATE = 1 / (1 - FADEOUT_PERCENTAGE);
enum class LifetimeChangeType
{
	eNone,		//	一定
	eFadeout,	//生存時間20%以下になると急速に値減少
	eFadein,	//生存時間100%から80%になるにつれて急速に値上昇
	eFadeinout,	//fadein、fadeout両方
	eCos90,		//(1 - 生存時間の残%) * 90度の余弦波
	eSin90,		//(1 - 生存時間の残%) * 90度の正弦波
	eSin180,	//(1 - 生存時間の残%) * 180度の正弦波
	eCos90Reverce, // 1 - eCos90
	eSin90Reverce, // 1 - eSin90
	eSin180Reverce, // 1 - eSin180
	eCos90Half,	//生存50%以下で0になる
	eSin90Half, //生存50%以下で1になる
};
static float (*LifetimeCurve[])(float percentage) =
{
	[](float percentage) {return 1.0f; },
	[](float percentage) {if (percentage < FADEIN_PERCENTAGE) { return percentage * FADEIN_RATE; } return 1.0f; },
	[](float percentage) {if (percentage > FADEOUT_PERCENTAGE) { return (1 - percentage) * FADEOUT_RATE; } return 1.0f; },
	[](float percentage) {if (percentage < FADEIN_PERCENTAGE) { return percentage * FADEIN_RATE; } if (percentage > FADEOUT_PERCENTAGE) { return (1 - percentage) * FADEOUT_RATE; }  return 1.0f; },
	[](float percentage) {return degCos((1 - percentage) * 90); },
	[](float percentage) {return degSin((1 - percentage) * 90); },
	[](float percentage) {return degSin((1 - percentage) * 180); },
	[](float percentage) {return 1 - degCos((1 - percentage) * 90); },
	[](float percentage) {return 1 - degSin((1 - percentage) * 90); },
	[](float percentage) {return 1 - degSin((1 - percentage) * 180); },
	[](float percentage) {if (percentage < 0.5f) return 0.0f; return degCos((1 - percentage) * 90); },
	[](float percentage) {if (percentage < 0.5f) return 1.0f; return degSin((1 - percentage) * 90); },
};
enum class SpawnType
{
	ePoint,	//原点に出現
	eLine,	//原点と長さと傾きと線の幅からなる矩形の中でランダムな個所に出現
	eCircle,//原点と半径からなる円の中でランダムな箇所に出現
};


class ParticleInfo
{
public:
	int drawPriority = 1;	//描画優先度	//済
	TextureTag tex = TextureTag::Plane;	//テクスチャ	//済
	Vector2 size = Vector2(BLOCKSIZE, BLOCKSIZE );	//大きさ	//済

	//大きさ
	float scaleMin = 0.25f;	//サイズ倍率最小値	//済
	float scaleMax = 0.25f;	//サイズ倍率最大値	//済

	//回転	
	float rotationDefaultMin = 0;	//初期回転値最小値	//済
	float rotationDefaultMax = 0;	//初期回転値最大値	//済
	float rotationSpeedMin = 0;	//1秒ごとの回転量最小値	//済
	float rotationSpeedMax = 0;	//1秒ごとの回転量最大値	//済


	//速度・方向
	float degMin = 0;	//済
	float degMax = 360;	//済
	float speedMin = BLOCKSIZE * 5;	//済
	float speedMax = BLOCKSIZE * 5;	//済

	bool moveForward = false;	//回転角度を進行方向に合わせる	//済
	bool moveBackward = false;	//回転角度を進行方向と逆にする	//済

	bool useMoveCurve = false;	//波を描いて進む	//済
	float curveDegRange = 45;	//移動補正左右角度	//済
	float lifetimeCurveCount = 4; //生存時間ごとの往復回数	//済

	//その他
	bool useGravity = false;	//重力の使用	//済
	Vector2 gravity = {0, GRAVITY /10};	//済
	bool fragile = false;	//ブロックに衝突すると消滅する	//済
	
	//色情報
	float alphaRateMin = 1;	//透明度補正最小値	//済
	float alphaRateMax = 1;	//透明度補正最大値	//済
	//float colorRateMin = 1;	//済
	//float colorRateMax = 1;	//済
	float redRateMin = 1;	//済
	float redRateMax = 1;	//済
	float greenRateMin = 1;	//済
	float greenRateMax = 1;	//済
	float blueRateMin = 1;	//済
	float blueRateMax = 1;	//済
	bool multipleBlend = false;	//乗算合成	//済
	bool addBlend = false;	//加算合成	//済

	//生存時間
	float lifetimeMin = 5;	//済
	float lifetimeMax = 5;	//済

	int particlePerGenerateMin = 1;	//生成判定1回あたりに作られるパーティクル数の最小値	//済
	int particlePerGenerateMax = 1;	//生成判定1回あたりに作られるパーティクル数の最大値	//済
	float generateIntervalMin = 1;	//生成判定の間隔の最小値	//済
	float generateIntervalMax = 1;	//生成判定の間隔の最大値	//済

	//生存時間による変化
	LifetimeChangeType scaleLifetimeChangeType = LifetimeChangeType::eNone;	//済
	LifetimeChangeType alphaLifetimeChangeType = LifetimeChangeType::eNone;	//済
	LifetimeChangeType redLifetimeChangeType = LifetimeChangeType::eNone;	//済
	LifetimeChangeType greenLifetimeChangeType = LifetimeChangeType::eNone;	//済
	LifetimeChangeType blueLifetimeChangeType = LifetimeChangeType::eNone;	//済
	LifetimeChangeType rotationSpeedLifetimeChangeType = LifetimeChangeType::eNone;	//済
	LifetimeChangeType speedLifetimeChangeType = LifetimeChangeType::eNone;	//済
	float speedLifetimeChangeBaseValue = 0.0f;	//済

	//出現型
	SpawnType spawnType = SpawnType::ePoint;	//済
	bool spawnLineFan = false;	//線上の位置と角度を比例させる	//済
	float spawnLineDeg = 0;	//傾き(法線方向)	//済
	float spawnLineLength = 0;	//長さ	//済
	float spawnLineWidth = 0;	//線幅	//済
	float spawnCircleRadiusMin = 0;//最小半径	//済
	float spawnCircleRadiusMax = 0;//最大半径	//済
	
};


//mLifetimeが0になると生成を停止する。0を指定したら永続的に生成
//生成停止状態でパーティクル数が0になるとParticleSystemは削除される
class ParticleSystem : public Object
{
public:
	ParticleSystem(Scene* scene, Vector2 position, ParticleInfo* info, float lifetime, bool once = false);
	~ParticleSystem();
	void UpdateObject(float deltaTime) override;
	void Erase();
	virtual void LoadCheckPointData()override { SetState(State::eDead); }
private:
	ParticleInfo mParticleInfo;
	Counter mGenerateInterval;	//生成間隔
	Counter mLifetime;	//パーティクル生成を続ける時間
	bool mPermanent = false;
	std::list<class Particle*> mParticles;
	bool mOnce = false;
	bool mOnceGenerated = false;
};