//=============================================================================
//
// [ComponentSprite.h] スプライト表示用コンポーネント
//
//=============================================================================
#ifndef _COMPONENT_SPRITE_H_
#define _COMPONENT_SPRITE_H_


//インクルードファイル
#include"Component.h"
#include"AbstractObject.h"
#include"Scene.h"
#include"TextureData.h"

#define LAYER_OBJECT 100
#define LAYER_BACKGROUND 10000

//クラス定義
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
		eAdd,	//加算合成
		eMultiple,	//乗算合成
		eLine	//枠線のみ
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
	int mTexX;	//テクスチャの横方向分割数
	int mTexY;	//テクスチャの縦方向分割数
	int mUseID;	//テクスチャの使用する位置
	int mDrawPriority;	//描画優先度、値が小さいほど前面に表示される
	Texture* mTexture;	//使用するテクスチャ
	unsigned mColor;	//ARGB
	DrawMode mDrawMode;	//描画モード
	bool mIsActive;		//描画するか否か
	Vector2 mTextureScale;		//オブジェクトの大きさに対するテクスチャの大きさ
	Vector2 mTextureOffset;	//
	float mAlphaRate = 1.0f;
	float mColorRate = 1.0f;
	float mRedRate = 1.0f;
	float mGreenRate = 1.0f;
	float mBlueRate = 1.0f;
	unsigned Color();
	//文字表示機構
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
	std::string mText;	//描画テキスト
	bool mDrawText;		//テキストが設定されてるか否か
	float mStringInfoCharactersPerLine;
	Vector2 mStringInfoOrigin;
	Vector2 mStringInfoFontSize;
	unsigned mTextColor = 0xFFFFFFFF;
	float mStringAlphaRate = 1.0f;
	float mStringColorRate = 1.0f;
	unsigned TextColor();

	//前景透け
public:
	void SetForegroundRate(float rate) { if (rate > 1) rate = 1; if (rate < 0) rate = 0; mForegroundRate = rate; }
	void SetForeground(bool isForeground) { mIsForeground = isForeground; }
protected:
	bool mIsForeground = false;
	float mForegroundRate = 1.0f;
};
#endif