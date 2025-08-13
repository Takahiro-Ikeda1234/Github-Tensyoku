#pragma once
#include<Windows.h>
#include"Singleton.h"
#include<string>
#include"DX9_core.h"
#include<unordered_map>
#include "Vector2.h"
typedef struct
{
	LPDIRECT3DTEXTURE9 texture;
	GLYPHMETRICS gm;
	int count;
}FontData;

class FontSystem : public Singleton<FontSystem>
{
public:
	friend Singleton<FontSystem>;
	FontSystem();
	~FontSystem();
	void Acquire(std::string text);
	void Release(std::string text);
	void Draw(std::string text, Vector2 origin, Vector2 fontSize, float charactersPerLine, unsigned color);
	void Draw(std::string text, Vector2 origin, Vector2 fontSize, float charactersPerLine, unsigned color[]);
	static std::wstring ToWstring(const char* c);
	int GetWidth(std::string text, Vector2 fontSize, float charactersPerLine);
	int GetHeight(std::string text, Vector2 fontSize, float charactersPerLine);
private:
	void AcquireFont(std::string c);
	void ReleaseFont(std::string c);
	void CreateFontTexture(std::string c, GLYPHMETRICS *gm, LPDIRECT3DTEXTURE9 *texture);
	void DrawFont(LPDIRECT3DTEXTURE9 texture, float xPos, float yPos, float width, float height, unsigned color);
	std::unordered_map<std::string, FontData> mFonts;
	HFONT mFont;
};