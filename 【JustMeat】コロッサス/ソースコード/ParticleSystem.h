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
	eNone,		//	���
	eFadeout,	//��������20%�ȉ��ɂȂ�Ƌ}���ɒl����
	eFadein,	//��������100%����80%�ɂȂ�ɂ�ċ}���ɒl�㏸
	eFadeinout,	//fadein�Afadeout����
	eCos90,		//(1 - �������Ԃ̎c%) * 90�x�̗]���g
	eSin90,		//(1 - �������Ԃ̎c%) * 90�x�̐����g
	eSin180,	//(1 - �������Ԃ̎c%) * 180�x�̐����g
	eCos90Reverce, // 1 - eCos90
	eSin90Reverce, // 1 - eSin90
	eSin180Reverce, // 1 - eSin180
	eCos90Half,	//����50%�ȉ���0�ɂȂ�
	eSin90Half, //����50%�ȉ���1�ɂȂ�
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
	ePoint,	//���_�ɏo��
	eLine,	//���_�ƒ����ƌX���Ɛ��̕�����Ȃ��`�̒��Ń����_���Ȍ��ɏo��
	eCircle,//���_�Ɣ��a����Ȃ�~�̒��Ń����_���ȉӏ��ɏo��
};


class ParticleInfo
{
public:
	int drawPriority = 1;	//�`��D��x	//��
	TextureTag tex = TextureTag::Plane;	//�e�N�X�`��	//��
	Vector2 size = Vector2(BLOCKSIZE, BLOCKSIZE );	//�傫��	//��

	//�傫��
	float scaleMin = 0.25f;	//�T�C�Y�{���ŏ��l	//��
	float scaleMax = 0.25f;	//�T�C�Y�{���ő�l	//��

	//��]	
	float rotationDefaultMin = 0;	//������]�l�ŏ��l	//��
	float rotationDefaultMax = 0;	//������]�l�ő�l	//��
	float rotationSpeedMin = 0;	//1�b���Ƃ̉�]�ʍŏ��l	//��
	float rotationSpeedMax = 0;	//1�b���Ƃ̉�]�ʍő�l	//��


	//���x�E����
	float degMin = 0;	//��
	float degMax = 360;	//��
	float speedMin = BLOCKSIZE * 5;	//��
	float speedMax = BLOCKSIZE * 5;	//��

	bool moveForward = false;	//��]�p�x��i�s�����ɍ��킹��	//��
	bool moveBackward = false;	//��]�p�x��i�s�����Ƌt�ɂ���	//��

	bool useMoveCurve = false;	//�g��`���Đi��	//��
	float curveDegRange = 45;	//�ړ��␳���E�p�x	//��
	float lifetimeCurveCount = 4; //�������Ԃ��Ƃ̉�����	//��

	//���̑�
	bool useGravity = false;	//�d�͂̎g�p	//��
	Vector2 gravity = {0, GRAVITY /10};	//��
	bool fragile = false;	//�u���b�N�ɏՓ˂���Ə��ł���	//��
	
	//�F���
	float alphaRateMin = 1;	//�����x�␳�ŏ��l	//��
	float alphaRateMax = 1;	//�����x�␳�ő�l	//��
	//float colorRateMin = 1;	//��
	//float colorRateMax = 1;	//��
	float redRateMin = 1;	//��
	float redRateMax = 1;	//��
	float greenRateMin = 1;	//��
	float greenRateMax = 1;	//��
	float blueRateMin = 1;	//��
	float blueRateMax = 1;	//��
	bool multipleBlend = false;	//��Z����	//��
	bool addBlend = false;	//���Z����	//��

	//��������
	float lifetimeMin = 5;	//��
	float lifetimeMax = 5;	//��

	int particlePerGenerateMin = 1;	//��������1�񂠂���ɍ����p�[�e�B�N�����̍ŏ��l	//��
	int particlePerGenerateMax = 1;	//��������1�񂠂���ɍ����p�[�e�B�N�����̍ő�l	//��
	float generateIntervalMin = 1;	//��������̊Ԋu�̍ŏ��l	//��
	float generateIntervalMax = 1;	//��������̊Ԋu�̍ő�l	//��

	//�������Ԃɂ��ω�
	LifetimeChangeType scaleLifetimeChangeType = LifetimeChangeType::eNone;	//��
	LifetimeChangeType alphaLifetimeChangeType = LifetimeChangeType::eNone;	//��
	LifetimeChangeType redLifetimeChangeType = LifetimeChangeType::eNone;	//��
	LifetimeChangeType greenLifetimeChangeType = LifetimeChangeType::eNone;	//��
	LifetimeChangeType blueLifetimeChangeType = LifetimeChangeType::eNone;	//��
	LifetimeChangeType rotationSpeedLifetimeChangeType = LifetimeChangeType::eNone;	//��
	LifetimeChangeType speedLifetimeChangeType = LifetimeChangeType::eNone;	//��
	float speedLifetimeChangeBaseValue = 0.0f;	//��

	//�o���^
	SpawnType spawnType = SpawnType::ePoint;	//��
	bool spawnLineFan = false;	//����̈ʒu�Ɗp�x���Ⴓ����	//��
	float spawnLineDeg = 0;	//�X��(�@������)	//��
	float spawnLineLength = 0;	//����	//��
	float spawnLineWidth = 0;	//����	//��
	float spawnCircleRadiusMin = 0;//�ŏ����a	//��
	float spawnCircleRadiusMax = 0;//�ő唼�a	//��
	
};


//mLifetime��0�ɂȂ�Ɛ������~����B0���w�肵����i���I�ɐ���
//������~��ԂŃp�[�e�B�N������0�ɂȂ��ParticleSystem�͍폜�����
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
	Counter mGenerateInterval;	//�����Ԋu
	Counter mLifetime;	//�p�[�e�B�N�������𑱂��鎞��
	bool mPermanent = false;
	std::list<class Particle*> mParticles;
	bool mOnce = false;
	bool mOnceGenerated = false;
};