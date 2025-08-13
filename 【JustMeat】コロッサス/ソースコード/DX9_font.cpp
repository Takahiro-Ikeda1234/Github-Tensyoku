#include "DX9_font.h"
#include "DX9_draw.h"
FontSystem::FontSystem()
	: Singleton()
{
	setlocale(LC_CTYPE, "");
	LOGFONT lf;
	int fontSize = 64;
	lf = {
		fontSize, 0, 0, 0, 500, 0, 0, 0,
		SHIFTJIS_CHARSET, OUT_TT_ONLY_PRECIS, CLIP_DEFAULT_PRECIS,
		PROOF_QUALITY, DEFAULT_PITCH | FF_MODERN,
		"ＭＳ Ｐゴシック"
	};
	mFont = CreateFontIndirect(&lf);
}

FontSystem::~FontSystem()
{
	for (auto it : mFonts)
	{
		if (it.second.texture)
		{
			it.second.texture->Release();
		}
	}
}

void FontSystem::Acquire(std::string text)
{
	while (!text.empty())
	{
		if (IsDBCSLeadByte(text[0]) == 0)
		{
			AcquireFont(text.substr(0, 1).c_str());
			text.erase(0, 1);
		}
		else
		{
			AcquireFont(text.substr(0, 2).c_str());
			text.erase(0, 2);
		}
	}
}

void FontSystem::Release(std::string text)
{
	while (!text.empty())
	{
		if (IsDBCSLeadByte(text[0]) == 0)
		{
			ReleaseFont(text.substr(0, 1).c_str());
			text.erase(0, 1);
		}
		else
		{
			ReleaseFont(text.substr(0, 2).c_str());
			text.erase(0, 2);
		}
	}
}

void FontSystem::Draw(std::string text, Vector2 origin, Vector2 fontSize, float charactersPerLine, unsigned color)
{
	float baseFontWidth = fontSize.x;
	float baseFontHeight = fontSize.y;
	float lineWidth = baseFontWidth * charactersPerLine;
	float lineOriginYPos = baseFontHeight * 0.66f;
	float offsetX = 0;
	float offsetY = 0;
	float scaleX = fontSize.x / 64;
	float scaleY = fontSize.y / 64;
	if (origin.x + lineWidth < 0) return;
	while (!text.empty())
	{
		int topByte = 0;
		if (IsDBCSLeadByte(text[0]) == 0)
		{
			topByte = 1;
		}
		else
		{
			topByte = 2;
		}
		std::string st = text.substr(0, topByte).c_str();
		if (st == "\n")
		{
			offsetY += baseFontHeight;
			offsetX = 0;
			text.erase(0, topByte);
			continue;
		}
		FontData* dat = &mFonts[st];
		GLYPHMETRICS gm = dat->gm;
		float width = gm.gmBlackBoxX * scaleX;
		float height = gm.gmBlackBoxY * scaleY;
		if (offsetX != 0 && offsetX + width > lineWidth)
		{
			offsetY += baseFontHeight;
			offsetX = 0;
		}
		if (st != " ")		//スペースに1ドットの点が表示されるので表示しないようにする
		{
			DrawFont(dat->texture, origin.x + width / 2 + offsetX + gm.gmptGlyphOrigin.x * scaleX, origin.y + lineOriginYPos - gm.gmptGlyphOrigin.y * scaleY + offsetY + height / 2, width, height, color);
		}
		offsetX += gm.gmCellIncX  * scaleX;
		text.erase(0, topByte);
	}
}
void FontSystem::Draw(std::string text, Vector2 origin, Vector2 fontSize, float charactersPerLine, unsigned color[])
{
	float baseFontWidth = fontSize.x;
	float baseFontHeight = fontSize.y;
	float lineWidth = baseFontWidth * charactersPerLine;
	float lineOriginYPos = baseFontHeight * 0.66f;
	float offsetX = 0;
	float offsetY = 0;
	float scaleX = fontSize.x / 64;
	float scaleY = fontSize.y / 64;
	int colorCount = 0;
	if (origin.x + lineWidth < 0) return;
	while (!text.empty())
	{
		int topByte = 0;
		if (IsDBCSLeadByte(text[0]) == 0)
		{
			topByte = 1;
		}
		else
		{
			topByte = 2;
		}
		std::string st = text.substr(0, topByte).c_str();
		if (st == "\n")
		{
			offsetY += baseFontHeight;
			offsetX = 0;
			text.erase(0, topByte);
			continue;
		}
		FontData* dat = &mFonts[st];
		GLYPHMETRICS gm = dat->gm;
		float width = gm.gmBlackBoxX * scaleX;
		float height = gm.gmBlackBoxY * scaleY;
		if (offsetX != 0 && offsetX + width > lineWidth)
		{
			offsetY += baseFontHeight;
			offsetX = 0;
		}
		if (st != " " && st != "　")		//スペースに1ドットの点が表示されるので表示しないようにする
		{
			DrawFont(dat->texture, origin.x + width / 2 + offsetX + gm.gmptGlyphOrigin.x * scaleX, origin.y + lineOriginYPos - gm.gmptGlyphOrigin.y * scaleY + offsetY + height / 2, width, height, color[colorCount]);
		}
		else
		{

			int a = 5;
		}
		offsetX += gm.gmCellIncX  * scaleX;
		text.erase(0, topByte);
		colorCount++;
	}
}


void FontSystem::AcquireFont(std::string c)
{
	int ct = mFonts[c].count;
	if (ct == 0)
	{
		CreateFontTexture(c, &mFonts[c].gm, &mFonts[c].texture);
	}
	mFonts[c].count++;
}

void FontSystem::ReleaseFont(std::string c)
{
	mFonts[c].count--;
	if (mFonts[c].count == 0)
	{
		mFonts[c].texture->Release();
		mFonts[c].texture = NULL;
	}
}

void FontSystem::CreateFontTexture(std::string c, GLYPHMETRICS* gm, LPDIRECT3DTEXTURE9 *texture)
{
	HDC hdc = GetDC(NULL);
	HFONT selectFont = (HFONT)SelectObject(hdc, mFont);
	// フォントビットマップ取得
	std::wstring wide_string = ToWstring(c.c_str());
	const wchar_t* result = wide_string.c_str();
	UINT code = (UINT)result[0];
	const int gradFlag = GGO_GRAY4_BITMAP; // GGO_GRAY2_BITMAP or GGO_GRAY4_BITMAP or GGO_GRAY8_BITMAP
	int grad = 0; // 階調の最大値
	switch (gradFlag) {
	case GGO_GRAY2_BITMAP: grad = 4; break;
	case GGO_GRAY4_BITMAP: grad = 16; break;
	case GGO_GRAY8_BITMAP: grad = 64; break;
	}
	if (grad == 0) {
		GetD3DDevice()->Release();
		return;
	}

	TEXTMETRIC tm;
	GetTextMetrics(hdc, &tm);
	CONST MAT2 mat = { {0,1},{0,0},{0,0},{0,1} };
	DWORD size = GetGlyphOutlineW(hdc, code, gradFlag, gm, 0, NULL, &mat);
	BYTE *pMono = new BYTE[size];
	GetGlyphOutlineW(hdc, code, gradFlag, gm, size, pMono, &mat);

	SelectObject(hdc, selectFont);
	ReleaseDC(NULL, hdc);

	// テクスチャ作成
	*texture = 0;
	int fontWidth = (gm->gmBlackBoxX + 3) / 4 * 4;
	int fontHeight = gm->gmBlackBoxY;
	GetD3DDevice()->CreateTexture(fontWidth, fontHeight, 1, 0, D3DFMT_A8R8G8B8, D3DPOOL_MANAGED, texture, NULL);

	// テクスチャにフォントビットマップ情報を書き込み
	D3DLOCKED_RECT lockedRect;
	(*texture)->LockRect(0, &lockedRect, NULL, 0);  // ロック
	DWORD *pTexBuf = (DWORD*)lockedRect.pBits;   // テクスチャメモリへのポインタ
	for (int y = 0; y < fontHeight; y++) {
		for (int x = 0; x < fontWidth; x++) {
			DWORD alpha = pMono[y * fontWidth + x] * 255 / grad;
			pTexBuf[y * fontWidth + x] = (alpha << 24) | 0x00ffffff;
		}
	}

	(*texture)->UnlockRect(0);  // アンロック
	delete[] pMono;
}

void FontSystem::DrawFont(LPDIRECT3DTEXTURE9 texture, float xPos, float yPos, float width, float height, unsigned color)
{
	VERTEX_3D_DX9 vertex[4] = {
		{D3DXVECTOR4(xPos - width / 2, yPos - height / 2, 0, 1), color, D3DXVECTOR2(0, 0)},
		{D3DXVECTOR4(xPos + width / 2, yPos - height / 2, 0, 1), color, D3DXVECTOR2(1, 0)},
		{D3DXVECTOR4(xPos - width / 2, yPos + height / 2, 0, 1), color, D3DXVECTOR2(0, 1)},
		{D3DXVECTOR4(xPos + width / 2, yPos + height / 2, 0, 1), color, D3DXVECTOR2(1, 1)},
	};
	GetD3DDevice()->SetTexture(0, texture);
	GetD3DDevice()->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, vertex, sizeof(VERTEX_3D_DX9));
}

std::wstring FontSystem::ToWstring(const char* c)
{

	std::wstring tmps;

	if (c == nullptr)
		return tmps;

	size_t sz = strlen(c);

	tmps.reserve(sz);//メモリを確保し、newが走りすぎないようにする

	const size_t CNT_MAX = 50;
	char tmpc[CNT_MAX];
	wchar_t tmpw[CNT_MAX];


	const char* p = c;
	while (*p != '\0') {
		int L = IsDBCSLeadByte(p[0]) + 1;//pが指し示すマルチバイト文字のバイト数を取得

		strncpy(tmpc, p, L);//tmpcにその一文字をコピーする
		tmpc[L] = '\0';
		mbstowcs(tmpw, tmpc, CNT_MAX);//tmpcの終端を0にしてあるので1文字だけ変換する
		tmps += tmpw;

		p += L;

	}

	return tmps;
}

int FontSystem::GetWidth(std::string text, Vector2 fontSize, float charactersPerLine)
{
	float baseFontWidth = fontSize.x;
	float baseFontHeight = fontSize.y;
	float lineWidth = baseFontWidth * charactersPerLine;
	float lineOriginYPos = baseFontHeight * 0.66f;
	float offsetX = 0;
	float offsetY = 0;
	float scaleX = fontSize.x / 64;
	float scaleY = fontSize.y / 64;
	int maxOffsetX = 0;
	while (!text.empty())
	{
		int topByte = 0;
		if (IsDBCSLeadByte(text[0]) == 0)
		{
			topByte = 1;
		}
		else
		{
			topByte = 2;
		}
		std::string st = text.substr(0, topByte).c_str();
		if (st == "\n")
		{
			if (offsetX != 0 && offsetX > maxOffsetX)
			{
				maxOffsetX = offsetX;
			}
			offsetY += baseFontHeight;
			offsetX = 0;
			text.erase(0, topByte);
			continue;
		}
		FontData* dat = &mFonts[st];
		GLYPHMETRICS gm = dat->gm;
		float width = gm.gmBlackBoxX * scaleX;
		float height = gm.gmBlackBoxY * scaleY;
		if (offsetX != 0 && offsetX + width > lineWidth)
		{
			return lineWidth;
			offsetX = 0;
		}
		offsetX += gm.gmCellIncX  * scaleX;
		text.erase(0, topByte);
		if (offsetX > maxOffsetX) maxOffsetX = offsetX;
	}
	return maxOffsetX;
}

int FontSystem::GetHeight(std::string text, Vector2 fontSize, float charactersPerLine)
{
	float baseFontWidth = fontSize.x;
	float baseFontHeight = fontSize.y;
	float lineWidth = baseFontWidth * charactersPerLine;
	float lineOriginYPos = baseFontHeight * 0.66f;
	float offsetX = 0;
	float offsetY = 0;
	float scaleX = fontSize.x / 64;
	float scaleY = fontSize.y / 64;
	while (!text.empty())
	{
		int topByte = 0;
		if (IsDBCSLeadByte(text[0]) == 0)
		{
			topByte = 1;
		}
		else
		{
			topByte = 2;
		}
		std::string st = text.substr(0, topByte).c_str();
		if (st == "\n")
		{
			offsetY += baseFontHeight;
			offsetX = 0;
			text.erase(0, topByte);
			continue;
		}
		FontData* dat = &mFonts[st];
		GLYPHMETRICS gm = dat->gm;
		float width = gm.gmBlackBoxX * scaleX;
		float height = gm.gmBlackBoxY * scaleY;
		if (offsetX != 0 && offsetX + width > lineWidth)
		{
			offsetY += baseFontHeight;
			offsetX = 0;
		}
		offsetX += gm.gmCellIncX  * scaleX;
		text.erase(0, topByte);
	}
	return offsetY + baseFontHeight;
}
