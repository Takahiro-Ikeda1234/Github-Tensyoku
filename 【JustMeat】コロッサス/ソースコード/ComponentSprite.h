//=============================================================================
//
// [ComponentSprite.h] �X�v���C�g�\���p�R���|�[�l���g
//
//=============================================================================
#ifndef _COMPONENT_SPRITE_H_
#define _COMPONENT_SPRITE_H_


//�C���N���[�h�t�@�C��
#include"Component.h"
#include"AbstractObject.h"
#include"Scene.h"
#include"TextureData.h"

#define LAYER_OBJECT 100
#define LAYER_BACKGROUND 10000

//�N���X��`
class ComponentSprite : public Component
{
public:
	ComponentSprite(class AbstractObject* owner, TextureTag tag, int drawPriority = LAYER_OBJECT);
	virtual ~ComponentSprite();
	virtual void Update(float deltaTime) override;
	virtual void Draw();
	void SetUseID(int useID) { if (useID != mUseID)mUseID = useID; }
	void SetColor(unsigned color) { mColor = color; }
	void SetAlpha(unsigned a) { if (a > 0xFF) { a = 0xFF; } if (a < 0) { a = 0x00; } mColor = (mColor & 0x00FFFFFF) | a << 24; }
	void SetRed(unsigned r) { mColor = (mColor & 0xFF00FFFF) | r << 16; }
	void SetGreen(unsigned g) { mColor = (mColor & 0xFFFF00FF) | g << 8; }
	void SetBlue(unsigned b) { mColor = (mColor & 0xFFFFFF00) | b; }
	unsigned GetAlpha() { return (mColor & 0xFF000000) >> 24; }
	unsigned GetRed() { return (mColor & 0x00FF0000) >> 16; }
	unsigned GetGreen() { return (mColor & 0x0000FF00) >> 8; }
	unsigned GetBlue() { return (mColor & 0x000000FF) >> 0; }
	void SetTexture(TextureTag tag) { if (mTexture != nullptr)delete mTexture; mTexture = new Texture(tag); mTexX = mTexture->mDivX; mTexY = mTexture->mDivY; }
	int GetDrawPriority() { return mDrawPriority; }
	int GetTexX() { return mTexX; }
	int GetTexY() { return mTexY; }
	void SetActive(bool active) { mIsActive = active; }
	bool IsActive() { return mIsActive; }
	void SetScale(float scaleX, float scaleY) { mTextureScale = Vector2(scaleX, scaleY); }
	enum class DrawMode
	{
		eDefault = 0,
		eAdd,	//���Z����
		eMultiple,	//��Z����
		eLine	//�g���̂�
	};
	void SetDrawMode(DrawMode drawMode) { mDrawMode = drawMode; }
	void SetAlphaRate(float rate) { if (rate > 1) rate = 1; if (rate < 0) rate = 0; mAlphaRate = rate; }
	void SetColorRate(float rate) { if (rate > 1) rate = 1; if (rate < 0) rate = 0; mColorRate = rate; }
	void SetRedRate(float rate) { if (rate > 1) rate = 1; if (rate < 0) rate = 0; mRedRate = rate; }
	void SetGreenRate(float rate) { if (rate > 1) rate = 1; if (rate < 0) rate = 0; mGreenRate = rate; }
	void SetBlueRate(float rate) { if (rate > 1) rate = 1; if (rate < 0) rate = 0; mBlueRate = rate; }
	float GetAlphaRate() { return mAlphaRate * mForegroundRate; }
	float GetRedRate() { return mRedRate * mColorRate * mForegroundRate; }
	float GetGreenRate() { return mGreenRate * mColorRate * mForegroundRate; }
	float GetBlueRate() { return mBlueRate * mColorRate * mForegroundRate; }
	void ExportObjectTexture();
protected:
	int mTexX;	//�e�N�X�`���̉�����������
	int mTexY;	//�e�N�X�`���̏c����������
	int mUseID;	//�e�N�X�`���̎g�p����ʒu
	int mDrawPriority;	//�`��D��x�A�l���������قǑO�ʂɕ\�������
	Texture* mTexture;	//�g�p����e�N�X�`��
	unsigned mColor;	//ARGB
	DrawMode mDrawMode;	//�`�惂�[�h
	bool mIsActive;		//�`�悷�邩�ۂ�
	Vector2 mTextureScale;		//�I�u�W�F�N�g�̑傫���ɑ΂���e�N�X�`���̑傫��
	Vector2 mTextureOffset;	//
	float mAlphaRate = 1.0f;
	float mColorRate = 1.0f;
	float mRedRate = 1.0f;
	float mGreenRate = 1.0f;
	float mBlueRate = 1.0f;
	unsigned Color();
	//�����\���@�\
public:
	void SetStringAlpha(unsigned a) { if (a > 0xFF) { a = 0xFF; } if (a < 0) { a = 0x00; } mTextColor = (mTextColor & 0x00FFFFFF) | a << 24; }
	void SetStringColor(unsigned color) { mTextColor = color; }
	void SetStringInfo(float charactersPerLine, Vector2 origin, Vector2 fontSize, bool center = false);
	virtual void SetString(std::string str);
	static float GetStringHeight(std::string text, Vector2 fontSize, int charactersPerLine);
	void ResetString();
	void SetStringAlphaRate(float rate) { if (rate > 1) rate = 1; if (rate < 0) rate = 0; mStringAlphaRate = rate; }
	void SetStringColorRate(float rate) { if (rate > 1) rate = 1; if (rate < 0) rate = 0; mStringColorRate = rate; }
	unsigned GetStringAlpha() { return (mTextColor & 0xFF000000) >> 24; }
	unsigned GetStringRed() { return (mTextColor & 0x00FF0000) >> 16; }
	unsigned GetStringGreen() { return (mTextColor & 0x0000FF00) >> 8; }
	unsigned GetStringBlue() { return (mTextColor & 0x000000FF) >> 0; }
	float GetStringAlphaRate() { return mStringAlphaRate * mForegroundRate; }
protected:
	std::string mText;	//�`��e�L�X�g
	bool mDrawText;		//�e�L�X�g���ݒ肳��Ă邩�ۂ�
	float mStringInfoCharactersPerLine;
	Vector2 mStringInfoOrigin;
	Vector2 mStringInfoFontSize;
	unsigned mTextColor = 0xFFFFFFFF;
	float mStringAlphaRate = 1.0f;
	float mStringColorRate = 1.0f;
	unsigned TextColor();

	//�O�i����
public:
	void SetForegroundRate(float rate) { if (rate > 1) rate = 1; if (rate < 0) rate = 0; mForegroundRate = rate; }
	void SetForeground(bool isForeground) { mIsForeground = isForeground; }
protected:
	bool mIsForeground = false;
	float mForegroundRate = 1.0f;
};
#endif