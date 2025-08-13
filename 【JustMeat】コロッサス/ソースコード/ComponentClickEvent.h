#pragma once
#include"Component.h"
#include"Counter.h"
#include"Vector2.h"
class AbstractObject;
#define CLICKTIME 0.2f	//�N���b�N���������������̎��Ԉȓ��ɂ����Ȃ�ꂽ��N���b�N�Ɣ��肷��
class ComponentClickEvent : public Component
{
public:
	ComponentClickEvent(AbstractObject* owner);
	virtual ~ComponentClickEvent() {}
	virtual bool OnMouse() = 0;		//�}�E�X���I�u�W�F�N�g�Əd�Ȃ��Ă���
	bool OnHit();	//�}�E�X���I�u�W�F�N�g�Əd�Ȃ��Ă����Ԃō��N���b�N�������ꂽ
	bool Released();	//�}�E�X���I�u�W�F�N�g�Əd�Ȃ��Ă����ԂŃN���b�N����A�C�ӂ̏ꏊ�ŗ����ꂽ
	bool OnClicked();		//�}�E�X���I�u�W�F�N�g�����N���b�N����

	bool OnHitRight();	//�}�E�X���I�u�W�F�N�g�Əd�Ȃ��Ă����Ԃō��N���b�N�������ꂽ
	bool ReleasedRight();	//�}�E�X���I�u�W�F�N�g�Əd�Ȃ��Ă����ԂŃN���b�N����A�C�ӂ̏ꏊ�ŗ����ꂽ
	bool OnClickedRight();	//�}�E�X���I�u�W�F�N�g���E�N���b�N����

	bool OuterHit();
	bool OuterClicked();

	bool OuterHitRight();
	bool OuterClickedRight();

	bool LeftHit();		//�}�E�X���{�^�����C�ӂ̏ꏊ�ŉ����ꂽ
	bool RightHit();	//�}�E�X�E�{�^�����C�ӂ̏ꏊ�ŉ����ꂽ
	Vector2 GetWorldPos();	//�}�E�X�̃Q�[�����W���擾
	void Update(float deltaTime);
private:
	Counter mClickTime;	//�N���b�N�̔��莞��
	Counter mRightClickTime;	//�N���b�N�̔��莞��
	Counter mOuterClickTime;
	Counter mOuterRightClickTime;
};

//�I�u�W�F�N�g�̒��S������͈�
class ComponentClickEventCircle : public ComponentClickEvent
{
public:
	ComponentClickEventCircle(AbstractObject* owner, float radius);
	~ComponentClickEventCircle() {}
	bool OnMouse() override;
private:
	float mRadius;
};

//�I�u�W�F�N�g�̓����蔻��𗘗p����
//�͈͂̓R���W�����Ɠ�����`
class ComponentClickEventObject : public ComponentClickEvent
{
public:
	ComponentClickEventObject(AbstractObject* owner);
	~ComponentClickEventObject() {}
	bool OnMouse() override;
};

class ComponentClickEventRectUI : public ComponentClickEvent
{
public:
	ComponentClickEventRectUI(AbstractObject* owner);
	~ComponentClickEventRectUI() {}
	bool OnMouse() override;
};

class ComponentClickEventCircleUI : public ComponentClickEvent
{
public:
	ComponentClickEventCircleUI(AbstractObject* owner, float radius);
	~ComponentClickEventCircleUI() {}
	bool OnMouse() override;
private:
	float mRadius;
};