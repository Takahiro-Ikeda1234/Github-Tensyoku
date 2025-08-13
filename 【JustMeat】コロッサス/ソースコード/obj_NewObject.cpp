#include "obj_NewObject.h"
#include "StageManager.h"	//StageManager::ToPosition(indexX, indexY)�ɕK�v
#include "ComponentAnimationSprite.h"	// new ComponentAnimationSprite�ɕK�v

//Vector2��float x,float y�̒l�����\����(�N���X)�B�ʒu��傫���A���x���Ɏg��
//����pos�̃��[���h���W�̂ɃI�u�W�F�N�g�����������B
NewObject::NewObject(Scene * scene, Vector2 pos)
	: Object(scene, pos)
{
	//Component�Ɩ��̕t���N���X��new�ō쐬���邾���Ńt���[�����ƂɍX�V��`�揈�����s����B
	//ComponentAnimationSprite�͉摜�\���ɕK�v�ȋ@�\���܂܂�Ă���B
	//����1�͕K��this�B����2�͎g�p����摜�̃^�O�B
	//����3(LAYER_OBJECT)�͕`��D��x�B�l���������قǎ�O�ɕ\�������BLAYER_OBJECT�̒l��100
	mCASprite = new ComponentAnimationSprite(this, TextureTag::Block, LAYER_OBJECT);
	//��̂悤�ɏ��������ŃI�u�W�F�N�g�̈ʒu�Ƀu���b�N�̉摜���\�������B

}

//index�����[���h���W�ɕϊ����ĕʂ̃R���X�g���N�^���Ăяo��
NewObject::NewObject(Scene * scene, int indexX, int indexY)
	: NewObject(scene, StageManager::ToPosition(indexX, indexY))
{
	//�����ɂ͉��������������Ȃ�
}

void NewObject::UpdateObject(float deltaTime)
{
	//�t���[�����ƂɎ��s�����֐��B�����ɃI�u�W�F�N�g�̏������L�q���Ă����B
	//deltaTime�͑O�t���[������̌o�ߎ���(60FPS�ł���΂��悻0.0166f)
}