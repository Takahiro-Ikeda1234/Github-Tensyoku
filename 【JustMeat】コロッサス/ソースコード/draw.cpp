//=============================================================================
//
// [Draw.cpp] 描画処理
//
//=============================================================================

//インクルードファイル
#include"Draw.h"
#include"Vector2.h"
#include"myMath.h"
#include"Windows_core.h"
#include"DX9_draw.h"
#include"DX9_font.h"
//プロトタイプ宣言
static int SCREEN_OFFSET_X;
static int SCREEN_OFFSET_Y;
static float TEXTURE_SCALE_X;
static float TEXTURE_SCALE_Y;
static int SCREEN_OFFSET_UI_X;
static int SCREEN_OFFSET_UI_Y;
static float g_TextureScale = 1;
static void DrawVertex(VERTEX_3D vertex[], void* texture = NULL);
static bool CheckInCamera(Vector2 pos, Vector2 size);
static void DrawVertexLine(VERTEX_3D vertex[]);
static void DrawVertexTriangle(VERTEX_3D vertex[]);

Float4 MakeFloat4(float x, float y, float z, float w)
{
	Float4 temp;
	temp.x = x;
	temp.y = y;
	temp.z = z;
	temp.w = w;
	return temp;
}

Float2 MakeFloat2(float x, float y)
{
	Float2 temp;
	temp.x = x;
	temp.y = y;
	return temp;
}

Float3 MakeFloat3(float x, float y, float z)
{
	Float3 temp;
	temp.x = x;
	temp.y = y;
	temp.z = z;
	return Float3();
}

void SetTextureScale(float scale)
{
	g_TextureScale = scale;
}
void InitDraw()
{
	SCREEN_OFFSET_X = -GetWindowWidth() / 2;
	SCREEN_OFFSET_Y = -GetWindowHeight() / 2;
	TEXTURE_SCALE_X = GetWindowScaleX();
	TEXTURE_SCALE_Y = GetWindowScaleY();
	SCREEN_OFFSET_UI_X = GetWindowWidth() / 2 + SCREEN_OFFSET_X;
	SCREEN_OFFSET_UI_Y = GetWindowHeight() / 2 + SCREEN_OFFSET_Y;
}


bool CheckInCamera(Vector2 pos, Vector2 size)
{
	if (pos.x + size.x < 0 || pos.x - size.x > GetWindowWidth()) return false;
	if (pos.y + size.y < 0 || pos.y - size.y > GetWindowHeight()) return false;
	return true;
}

void BlendDefault()
{
	BlendDefaultDX9();
}

void BlendAddBlend()
{
	BlendAddBlendDX9();
}
void BlendMultipleBlend()
{
	BlendMultipleBlendDX9();
}

// ---------------------------------------------------------------------------
// 関数名: void DrawStart() 
// 説明:   描画開始時の初期化を行う関数
// ---------------------------------------------------------------------------
void DrawStart(int r, int g, int b)
{
	DrawStartDX9(r, g, b);
}

// ---------------------------------------------------------------------------
// 関数名: void SwapBuffer()
// 説明:   描画完了時のバッファ切り替えを行う関数
// ---------------------------------------------------------------------------
void SwapBuffer()
{
	SwapBufferDX9();
}

// ---------------------------------------------------------------------------
// 関数名: void DrawVertex(VERTEX_3D vertex[], void* texture)
// 引数:   VERTEX_3D vertex[] 頂点情報
// 引数:   void* texture テクスチャのポインタ
// 説明:   描画を行う関数
// ---------------------------------------------------------------------------
void DrawVertex(VERTEX_3D vertex[], void* texture)
{
	DrawVertexDX9(vertex, texture);
}
void DrawVertexLine(VERTEX_3D vertex[])
{
	DrawVertexLineDX9(vertex);
}
void DrawVertexTriangle(VERTEX_3D vertex[])
{
	DrawVertexTriangleDX9(vertex);
}

// ---------------------------------------------------------------------------
// 関数名: void SetVertex(VERTEX_3D vertex[], const Vector2& pos, const Vector2& size, float rotationDeg)
// 引数:   VERTEX_3D vertex[] 頂点情報の出力先
// 引数:   const Vector2& pos オブジェクトの中心位置
// 引数:   const Vector2& size オブジェクトのサイズ
// 引数:   float rotationDeg オブジェクトの回転角度
// 説明:   オブジェクトの情報をもとに頂点情報の座標を設定する関数
// ---------------------------------------------------------------------------
void SetVertex(VERTEX_3D vertex[], const Vector2& pos, const Vector2& size, float rotationDeg)
{
	vertex[0].Position = MakeFloat4(pos.x - size.x / 2, pos.y - size.y / 2, 0, 1);
	vertex[1].Position = MakeFloat4(pos.x + size.x / 2, pos.y - size.y / 2, 0, 1);
	vertex[2].Position = MakeFloat4(pos.x - size.x / 2, pos.y + size.y / 2, 0, 1);
	vertex[3].Position = MakeFloat4(pos.x + size.x / 2, pos.y + size.y / 2, 0, 1);

	if (rotationDeg == 0)return;
	float cosTheta = degCos(rotationDeg);
	float sinTheta = degSin(rotationDeg);
	for (int i = 0; i < 4; i++)
	{
		vertex[i].Position.x -= pos.x;
		vertex[i].Position.y -= pos.y;
		vertex[i].Position = MakeFloat4(vertex[i].Position.x * cosTheta - vertex[i].Position.y * sinTheta, vertex[i].Position.x * sinTheta + vertex[i].Position.y * cosTheta, 0, 1);
		vertex[i].Position.x += pos.x;
		vertex[i].Position.y += pos.y;
	}
}

// ---------------------------------------------------------------------------
// 関数名: void SetTexCoord(VERTEX_3D vertex[], int useID, int texX, int texY)
// 引数:   VERTEX_3D vertex[] 頂点情報
// 引数:   int useID 使用するフレーム
// 引数:   int texX テクスチャ横方向分割数
// 引数:   int texY テクスチャ縦方向分割数
// 説明:   テクスチャの分割数、使用するフレームをもとに頂点情報のuv座標を設定する関数
// ---------------------------------------------------------------------------
void SetTexCoord(VERTEX_3D vertex[], int useID, int texX, int texY)
{
	if (texX == 1)
	{
		float x = 1.0f / texX;
		float y = 1.0f / texY;
		float left = (useID % texX) * x;
		float top = (useID / texX) * y;
		vertex[0].TexCoord = MakeFloat2(left, top);
		vertex[1].TexCoord = MakeFloat2(left + x, top);
		vertex[2].TexCoord = MakeFloat2(left, top + y);
		vertex[3].TexCoord = MakeFloat2(left + x, top + y);
	}
	else
	{
		float x = 1.0f / texX;
		float y = 1.0f / texY;
		float left = (useID % texX) * x + 0.01f;
		float top = (useID / texX) * y;
		vertex[0].TexCoord = MakeFloat2(left, top);
		vertex[1].TexCoord = MakeFloat2(left + x - 0.01f, top);
		vertex[2].TexCoord = MakeFloat2(left, top + y);
		vertex[3].TexCoord = MakeFloat2(left + x - 0.01f, top + y);
	}
}

// ---------------------------------------------------------------------------
// 関数名: void SetVertex_uv(VERTEX_3D vertex[], const Vector2& pos, const Vector2& size, float rotationDeg, float uPercentage, float vPercentage)
// 引数:   VERTEX_3D vertex[] 頂点情報の出力先
// 引数:   const Vector2& pos オブジェクトの中心位置
// 引数:   const Vector2& size オブジェクトのサイズ
// 引数:   float rotationDeg オブジェクトの回転角度
// 引数:   float uPercentage テクスチャの表示率
// 引数:   float vPercentage テクスチャの表示率
// 説明:   オブジェクトの情報をもとに頂点情報の座標を設定する関数。全体ではなく一部を表示できる
// ---------------------------------------------------------------------------
void SetVertex_uvPercentage(VERTEX_3D vertex[], const Vector2& pos, const Vector2& size, float rotationDeg, float uPercentage, float vPercentage)
{
	vertex[0].Position = MakeFloat4(pos.x - size.x / 2, pos.y - size.y / 2, 0, 1);
	vertex[1].Position = MakeFloat4(pos.x + size.x / 2 * (-1 + 2 * uPercentage), pos.y - size.y / 2, 0, 1);
	vertex[2].Position = MakeFloat4(pos.x - size.x / 2, pos.y + size.y / 2 * (-1 + 2 * vPercentage), 0, 1);
	vertex[3].Position = MakeFloat4(pos.x + size.x / 2 * (-1 + 2 * uPercentage), pos.y + size.y / 2 * (-1 + 2 * vPercentage), 0, 1);

	if (rotationDeg == 0)return;
	float cosTheta = degCos(rotationDeg);
	float sinTheta = degSin(rotationDeg);
	for (int i = 0; i < 4; i++)
	{
		vertex[i].Position.x -= pos.x;
		vertex[i].Position.y -= pos.y;
		vertex[i].Position = MakeFloat4(vertex[i].Position.x * cosTheta - vertex[i].Position.y * sinTheta, vertex[i].Position.x * sinTheta + vertex[i].Position.y * cosTheta, 0, 1);
		vertex[i].Position.x += pos.x;
		vertex[i].Position.y += pos.y;
	}
}

// ---------------------------------------------------------------------------
// 関数名: void SetTexCoord_uvPercentage(VERTEX_3D vertex[], int useID, int texX, int texY, float uPercentage, float vPercentage)
// 引数:   VERTEX_3D vertex[] 頂点情報
// 引数:   int useID 使用するフレーム
// 引数:   int texX テクスチャ横方向分割数
// 引数:   int texY テクスチャ縦方向分割数
// 引数:   float uPercentage テクスチャの表示率
// 引数:   float vPercentage テクスチャの表示率
// 説明:   テクスチャの分割数、使用するフレームをもとに頂点情報のuv座標を設定する関数。全体ではなく一部を表示できる
// ---------------------------------------------------------------------------
void SetTexCoord_uvPercentage(VERTEX_3D vertex[], int useID, int texX, int texY, float uPercentage, float vPercentage)
{
	float x = 1.0f / texX;
	float y = 1.0f / texY;
	float left = (useID % texX) * x;
	float top = (useID / texX) * y;
	vertex[0].TexCoord = MakeFloat2(left, top);
	vertex[1].TexCoord = MakeFloat2(left + x * uPercentage, top);
	vertex[2].TexCoord = MakeFloat2(left, top + y * vPercentage);
	vertex[3].TexCoord = MakeFloat2(left + x * uPercentage, top + y * vPercentage);
}

// ---------------------------------------------------------------------------
// 関数名: void SetColor(VERTEX_3D vertex[], unsigned color)
// 引数:   VERTEX_3D vertex[] 頂点情報
// 引数:   unsigned color 色情報
// 説明:   頂点情報の色を設定する関数
// ---------------------------------------------------------------------------
void SetColor(VERTEX_3D vertex[], unsigned color)
{
	for (int i = 0; i < 4; i++)
	{
		vertex[i].Color = color;
	}
}

// ---------------------------------------------------------------------------
// 関数名: void DrawRect(const Vector2& pos, const Vector2& size, unsigned colorARGB)
// 引数:   const Vector2& pos 矩形の中心位置
// 引数:   const Vector2& size 矩形のサイズ
// 引数:   unsigned colorARGB 色
// 説明:   枠のみの矩形を描画する関数
// ---------------------------------------------------------------------------
void DrawRect(const Vector2& pos, const Vector2& size, unsigned colorARGB)
{
	Vector2 texSize(size.x * TEXTURE_SCALE_X * g_TextureScale, size.y * TEXTURE_SCALE_Y * g_TextureScale);
	VERTEX_3D vertex[5];
	Vector2 screenPos(pos.x - SCREEN_OFFSET_X, pos.y - SCREEN_OFFSET_Y);
	vertex[0].Position = MakeFloat4(screenPos.x - texSize.x / 2, screenPos.y - texSize.y / 2, 0, 1);
	vertex[1].Position = MakeFloat4(screenPos.x + texSize.x / 2, screenPos.y - texSize.y / 2, 0, 1);
	vertex[2].Position = MakeFloat4(screenPos.x + texSize.x / 2, screenPos.y + texSize.y / 2, 0, 1);
	vertex[3].Position = MakeFloat4(screenPos.x - texSize.x / 2, screenPos.y + texSize.y / 2, 0, 1);
	vertex[4].Position = vertex[0].Position;
	for (int i = 0; i < 5; i++)
	{
		vertex[i].Color = colorARGB;
	}
	DrawVertexLine(vertex);
}

// ---------------------------------------------------------------------------
// 関数名: void DrawTexture(void* texture, const Vector2& pos, const Vector2& size, float rotationDeg, int useID, int texX, int texY, unsigned colorARGB)
// 引数:   void* texture テクスチャのポインタ
// 引数:   const Vector2& pos オブジェクトの中心位置
// 引数:   const Vector2& size オブジェクトのサイズ
// 引数:   float rotationDeg オブジェクトの回転角度
// 引数:   int useID 使用するフレーム
// 引数:   int texX テクスチャの横方向分割数
// 引数:   int texY テクスチャの縦方向分割数
// 引数:   unsigned colorARGB 色
// 説明:   テクスチャ描画関数。中心を0,0として扱う
// ---------------------------------------------------------------------------
void DrawTexture(void* texture, const Vector2& pos, const Vector2& size, float rotationDeg, int useID, int texX, int texY, unsigned colorARGB)
{
	Vector2 texSize(size.x * TEXTURE_SCALE_X * g_TextureScale, size.y * TEXTURE_SCALE_Y * g_TextureScale);
	static VERTEX_3D vertex[4];
	Vector2 screenPos(pos.x - SCREEN_OFFSET_X, pos.y - SCREEN_OFFSET_Y);

	if (!CheckInCamera(screenPos, texSize)) return;

	SetVertex(vertex, screenPos, texSize, rotationDeg);
	SetTexCoord(vertex, useID, texX, texY);
	SetColor(vertex, colorARGB);
	DrawVertex(vertex, texture);
}


// ---------------------------------------------------------------------------
// 関数名: void DrawTexture_uv(void* texture, const Vector2& pos, const Vector2& size, float rotationDeg, int useID, int texX, int texY, float uPercentage, float vPercentage, unsigned colorARGB)
// 引数:   void* texture テクスチャのポインタ
// 引数:   const Vector2& pos オブジェクトの中心位置
// 引数:   const Vector2& size オブジェクトのサイズ
// 引数:   float rotationDeg オブジェクトの回転角度
// 引数:   int useID 使用するフレーム
// 引数:   int texX テクスチャの横方向分割数
// 引数:   int texY テクスチャの縦方向分割数
// 引数:   unsigned colorARGB 色
// 引数:   float uPercentage テクスチャの表示率
// 引数:   float vPercentage テクスチャの表示率
// 説明:   テクスチャ描画関数。中心を0,0として扱う。全体ではなく一部を表示できる
// ---------------------------------------------------------------------------
void DrawTexture_uv(void* texture, const Vector2& pos, const Vector2& size, float rotationDeg, int useID, int texX, int texY, float uPercentage, float vPercentage, unsigned colorARGB)
{
	Vector2 texSize(size.x * TEXTURE_SCALE_X * g_TextureScale, size.y * TEXTURE_SCALE_Y * g_TextureScale);
	VERTEX_3D vertex[4];
	Vector2 screenPos(pos.x - SCREEN_OFFSET_X, pos.y - SCREEN_OFFSET_Y);

	if (!CheckInCamera(screenPos, texSize)) return;

	SetVertex_uvPercentage(vertex, screenPos, texSize, 0, uPercentage, vPercentage);
	SetTexCoord_uvPercentage(vertex, useID, texX, texY, uPercentage, vPercentage);
	SetColor(vertex, colorARGB);
	DrawVertex(vertex, texture);
}


// ---------------------------------------------------------------------------
// 関数名: void DrawScreen(void* texture, unsigned colorARGB)
// 引数:   void* texture テクスチャのポインタ
// 引数:   unsigned colorARGB 色
// 説明:   テクスチャを画面サイズに拡大して表示する関数
// ---------------------------------------------------------------------------
void DrawScreen(void* texture, unsigned colorARGB)
{
	VERTEX_3D vertex[4];
	Vector2 size(GetWindowWidth(), GetWindowHeight());
	Vector2 pos(-SCREEN_OFFSET_X, -SCREEN_OFFSET_Y);
	SetVertex(vertex, pos, size, 0);
	vertex[0].TexCoord = MakeFloat2(0, 1);
	vertex[1].TexCoord = MakeFloat2(1, 1);
	vertex[2].TexCoord = MakeFloat2(0, 0);
	vertex[3].TexCoord = MakeFloat2(1, 0);
	SetColor(vertex, colorARGB);
	DrawVertex(vertex, texture);
}


// ---------------------------------------------------------------------------
// 関数名: void DrawUITexture(void* texture, const Vector2& pos, const Vector2& size, float rotationDeg, int useID, int texX, int texY, unsigned colorARGB)
// 引数:   void* texture テクスチャのポインタ
// 引数:   const Vector2& pos オブジェクトの中心位置
// 引数:   const Vector2& size オブジェクトのサイズ
// 引数:   float rotationDeg オブジェクトの回転角度
// 引数:   int useID 使用するフレーム
// 引数:   int texX テクスチャの横方向分割数
// 引数:   int texY テクスチャの縦方向分割数
// 引数:   unsigned colorARGB 色
// 説明:   UI描画関数。左上を0,0として扱う
// ---------------------------------------------------------------------------
void DrawUITexture(void* texture, const Vector2& pos, const Vector2& size, float rotationDeg, int useID, int texX, int texY, unsigned colorARGB)
{

	Vector2 texSize(size.x, size.y);
	VERTEX_3D vertex[4];
	Vector2 screenPos(pos.x + SCREEN_OFFSET_UI_X, pos.y + SCREEN_OFFSET_UI_Y);
	SetVertex(vertex, screenPos, texSize, rotationDeg);
	SetTexCoord(vertex, useID, texX, texY);
	SetColor(vertex, colorARGB);
	DrawVertex(vertex, texture);
}

// ---------------------------------------------------------------------------
// 関数名: void DrawUITexture_uv(void* texture, const Vector2& pos, const Vector2& size, float rotationDeg, int useID, int texX, int texY, float uPercentage, float vPercentage, unsigned colorARGB)
// 引数:   void* texture テクスチャのポインタ
// 引数:   const Vector2& pos オブジェクトの中心位置
// 引数:   const Vector2& size オブジェクトのサイズ
// 引数:   float rotationDeg オブジェクトの回転角度
// 引数:   int useID 使用するフレーム
// 引数:   int texX テクスチャの横方向分割数
// 引数:   int texY テクスチャの縦方向分割数
// 引数:   unsigned colorARGB 色
// 引数:   float uPercentage テクスチャの表示率
// 引数:   float vPercentage テクスチャの表示率
// 説明:   UI描画関数。左上を0,0として扱う。全体ではなく一部を表示できる
// ---------------------------------------------------------------------------
void DrawUITexture_uv(void* texture, const Vector2& pos, const Vector2& size, float rotationDeg, int useID, int texX, int texY, float uPercentage, float vPercentage, unsigned colorARGB)
{
	Vector2 texSize(size.x, size.y);
	VERTEX_3D vertex[4];
	Vector2 screenPos(pos.x + SCREEN_OFFSET_UI_X, pos.y + SCREEN_OFFSET_UI_Y);
	SetVertex_uvPercentage(vertex, screenPos, texSize, rotationDeg, uPercentage, vPercentage);
	SetTexCoord_uvPercentage(vertex, useID, texX, texY, uPercentage, vPercentage);
	SetColor(vertex, colorARGB);
	DrawVertex(vertex, texture);
}


//中心を0,0として扱う
void DrawString(std::string str, float charactersPerLine, Vector2 origin, Vector2 fontSize, unsigned color)
{
	Vector2 screenPos(origin.x - SCREEN_OFFSET_X, origin.y - SCREEN_OFFSET_Y);

	if (!CheckInCamera(screenPos, fontSize * charactersPerLine)) return;

	FontSystem::Inst()->Draw(str, screenPos, Vector2(fontSize.x  * TEXTURE_SCALE_X * g_TextureScale, fontSize.y  * TEXTURE_SCALE_Y * g_TextureScale), charactersPerLine, color);
}
//左上を0,0として扱う
void DrawUIString(std::string str, float charactersPerLine, Vector2 origin, Vector2 fontSize, unsigned color)
{
	Vector2 screenPos(origin.x + SCREEN_OFFSET_UI_X, origin.y + SCREEN_OFFSET_UI_Y);
	FontSystem::Inst()->Draw(str, screenPos, Vector2(fontSize.x, fontSize.y), charactersPerLine, color);
}
void DrawUIString(std::string str, float charactersPerLine, Vector2 origin, Vector2 fontSize, unsigned color[])
{
	Vector2 screenPos(origin.x + SCREEN_OFFSET_UI_X, origin.y + SCREEN_OFFSET_UI_Y);
	FontSystem::Inst()->Draw(str, screenPos, Vector2(fontSize.x, fontSize.y), charactersPerLine, color);
}

void DrawTriangle(Vector2 v[3], unsigned color)
{
	VERTEX_3D vertex[3] = {
		{v[0].x - SCREEN_OFFSET_X,v[0].y - SCREEN_OFFSET_Y, 0, 1},
		{v[1].x - SCREEN_OFFSET_X,v[1].y - SCREEN_OFFSET_Y, 0, 1},
		{v[2].x - SCREEN_OFFSET_X,v[2].y - SCREEN_OFFSET_Y, 0, 1},
	};
	SetColor(vertex, color);
	DrawVertexTriangle(vertex);
}
void DrawUITriangle(Vector2 v[3], unsigned color)
{
	VERTEX_3D vertex[3] = {
		{v[0].x + SCREEN_OFFSET_UI_X,v[0].y + SCREEN_OFFSET_UI_Y, 0, 1},
		{v[1].x + SCREEN_OFFSET_UI_X,v[1].y + SCREEN_OFFSET_UI_Y, 0, 1},
		{v[2].x + SCREEN_OFFSET_UI_X,v[2].y + SCREEN_OFFSET_UI_Y, 0, 1},
	};
	SetColor(vertex, color);
	DrawVertexTriangle(vertex);
}