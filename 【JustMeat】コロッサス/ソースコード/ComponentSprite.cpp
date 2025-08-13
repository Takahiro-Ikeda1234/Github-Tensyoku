//=============================================================================
//
// [ComponentSprite.cpp] スプライト表示用コンポーネント
//
//=============================================================================

//インクルードファイル
#include"ComponentSprite.h"
#include"Object.h"
#include"Draw.h"
#include"Scene.h"
#include"CameraManager.h"
#include"Vector2.h"

//テクスチャ出力用
#include"DX9_texture.h"
#include"textureData.h"
#include"StageManager.h"

// ---------------------------------------------------------------------------
// 関数名: ComponentSprite::ComponentSprite(AbstractObject* owner, TextureTag tag, int drawPriority)
// 引数:   AbstractObject* owner 所持者のポインタ
// 引数:   TextureTag tag 利用する画像のタグ
// 引数:   int drawPriority 描画優先度。低いほど前に表示される
// 説明:   ComponentSpriteのコンストラクタ
// ---------------------------------------------------------------------------
ComponentSprite::ComponentSprite(AbstractObject* owner, TextureTag tag, int drawPriority)
	: Component(owner)
	, mTexX(1)
	, mTexY(1)
	, mUseID(0)
	, mTexture(nullptr)
	, mColor(0xFFFFFFFF)
	, mDrawPriority(drawPriority)
	, mIsActive(true)
	, mDrawText(false)
	, mTextureScale(Vector2(1, 1))
{
	mOwner->GetScene()->AddSprite(this);
	mTexture = new Texture(tag);
	mTexX = mTexture->mDivX;
	mTexY = mTexture->mDivY;
}

//ComponentSpriteのデストラクタ
ComponentSprite::~ComponentSprite()
{
	//不要になったテクスチャの解放
	delete mTexture;
	mOwner->GetScene()->RemoveSprite(this);
	ResetString();
}

// ---------------------------------------------------------------------------
// 関数名: void ComponentSprite::Draw()
// 説明:   描画を行う
// ---------------------------------------------------------------------------
void ComponentSprite::Draw()
{
	if (!mIsActive)return;
	if (mOwner->GetState() == AbstractObject::State::eInactive)return;
	float cameraRotation = mOwner->GetScene()->GetCameraManager()->GetRotation();
	Vector2 screenPos = mOwner->GetScene()->GetCameraManager()->ToScreenPosition(mOwner->GetPosition());
	switch (mDrawMode)
	{
	case DrawMode::eAdd:
		BlendAddBlend();
		DrawTexture(mTexture->GetTexture(), screenPos, mOwner->GetSize() * mTextureScale, mOwner->GetRotation() + cameraRotation, mUseID, mTexX, mTexY, Color());
		BlendDefault();
		break;
	case DrawMode::eMultiple:
		BlendMultipleBlend();
		DrawTexture(mTexture->GetTexture(), screenPos, mOwner->GetSize() * mTextureScale, mOwner->GetRotation() + cameraRotation, mUseID, mTexX, mTexY, Color());
		BlendDefault();
		break;
	case DrawMode::eDefault:
		DrawTexture(mTexture->GetTexture(), screenPos, mOwner->GetSize() * mTextureScale, mOwner->GetRotation() + cameraRotation, mUseID, mTexX, mTexY, Color());
		break;
	case DrawMode::eLine:
		DrawRect(screenPos, mOwner->GetSize() * mTextureScale, Color());
	}
	if (mDrawText)
	{
		DrawString(mText, mStringInfoCharactersPerLine, mOwner->GetScene()->GetCameraManager()->ToScreenPosition(mStringInfoOrigin), mStringInfoFontSize, TextColor());
	}
}



void ComponentSprite::Update(float deltaTime)
{
	if (!mIsActive)return;
}

void ComponentSprite::ExportObjectTexture()
{
	ExportObject(mTexture->GetTexture(), StageManager::ToIndexX(mOwner->GetPosition().x), StageManager::ToIndexX(mOwner->GetPosition().y), mUseID, mTexture->mDivX, mTexture->mDivY);
}

unsigned ComponentSprite::Color()
{
	unsigned R = ((unsigned)(((mColor & 0x00FF0000) >> 16) * (mRedRate * mColorRate * mForegroundRate)));
	unsigned G = ((unsigned)(((mColor & 0x0000FF00) >> 8) * (mGreenRate * mColorRate * mForegroundRate)));
	unsigned B = ((unsigned)(((mColor & 0x000000FF) >> 0) * (mBlueRate * mColorRate * mForegroundRate)));
	unsigned A = ((unsigned)(((mColor & 0xFF000000) >> 24) * (mAlphaRate * mForegroundRate)));
	return  A << 24 | R << 16 | G << 8 | B << 0;
}

//文字表示機構
#include"DX9_font.h"
void ComponentSprite::SetStringInfo(float charactersPerLine, Vector2 origin, Vector2 fontSize, bool center)
{
	if (center)
	{
		mStringInfoCharactersPerLine = charactersPerLine;
		int width = FontSystem::Inst()->GetWidth(mText, fontSize, charactersPerLine);
		int height = FontSystem::Inst()->GetHeight(mText, fontSize, charactersPerLine);
		mStringInfoOrigin = origin - Vector2(width / 2, height / 2) + Vector2(0, fontSize.y * 0.34f / 2);
		mStringInfoFontSize = fontSize;
	}
	else
	{
		mStringInfoCharactersPerLine = charactersPerLine;
		mStringInfoOrigin = origin;
		mStringInfoFontSize = fontSize;
	}
}
void ComponentSprite::SetString(std::string str)
{
	if (mDrawText)
	{
		FontSystem::Inst()->Release(mText);
	}
	mText = str;
	mDrawText = true;
	FontSystem::Inst()->Acquire(mText);
}

float ComponentSprite::GetStringHeight(std::string text, Vector2 fontSize, int charactersPerLine)
{
	return FontSystem::Inst()->GetHeight(text, fontSize, charactersPerLine);
}

void ComponentSprite::ResetString()
{
	if (mDrawText)
	{
		mDrawText = false;
		FontSystem::Inst()->Release(mText);
	}
}

unsigned ComponentSprite::TextColor()
{
	float a = GetStringAlphaRate();
	float c = mStringColorRate;
	unsigned R = ((unsigned)(GetStringRed() * c));
	unsigned G = ((unsigned)(GetStringGreen() * c));
	unsigned B = ((unsigned)(GetStringBlue() * c));
	unsigned A = ((unsigned)(GetStringAlpha() * a));
	return  A << 24 | R << 16 | G << 8 | B << 0;
}
