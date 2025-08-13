//=============================================================================
//
// [draw.h] �`�揈��
//
//=============================================================================
#ifndef _DRAW_H_
#define _DRAW_H_

//�C���N���[�h�t�@�C��
#include<stdio.h>
#include<string>
#include"Vector2.h"

//�\���̒�`
typedef struct
{
	float x;
	float y;
}Float2;
typedef struct
{
	float x;
	float y;
	float z;
}Float3;
typedef struct
{
	float x;
	float y;
	float z;
	float w;
}Float4;

Float4 MakeFloat4(float x, float y, float z, float w);
Float2 MakeFloat2(float x, float y);
Float3 MakeFloat3(float x, float y, float z);

void InitDraw();

typedef struct
{
	Float4 Position = MakeFloat4(0.0f, 0.0f, 0.0f, 0.0f);	//���_�ʒu
	unsigned Color = 0xFFFFFFFF;	//�F
	Float2 TexCoord = MakeFloat2(0.0f, 0.0f);	//uv���W
}VERTEX_3D; //���_���

//�v���g�^�C�v�錾
void BlendDefault();
void BlendAddBlend();
void BlendMultipleBlend();
void DrawStart(int r, int g, int b);
void SwapBuffer();
void DrawTexture_uv(void* texture, const Vector2& pos, const Vector2& size, float rotationDeg, int useID, int texX, int texY, float uPercentage, float vPercentage, unsigned colorARGB = 0xFFFFFFFF);
void DrawTexture(void* texture, const Vector2& pos, const Vector2& size, float rotationDeg, int useID, int texX = 1, int texY = 1, unsigned colorARGB = 0xFFFFFFFF);
void DrawRect(const Vector2& pos, const Vector2& size, unsigned colorARGB = 0xFFFFFFFF);
void DrawScreen(void* texture, unsigned colorARGB);
void DrawUITexture(void * texture, const Vector2 & pos, const Vector2 & size, float rotationDeg, int useID, int texX, int texY, unsigned colorARGB = 0xFFFFFFFF);
void DrawUITexture_uv(void * texture, const Vector2 & pos, const Vector2 & size, float rotationDeg, int useID, int texX, int texY, float uPercentage, float vPercentage, unsigned colorARGB = 0xFFFFFFFF);
void SetTextureScale(float scale);
void DrawString(std::string str, float charactersPerLine, Vector2 origin, Vector2 fontSize, unsigned color);
void DrawUIString(std::string str, float charactersPerLine, Vector2 origin, Vector2 fontSize, unsigned color);
void DrawUIString(std::string str, float charactersPerLine, Vector2 origin, Vector2 fontSize, unsigned color[]);
void DrawTriangle(Vector2 v[3], unsigned color = 0xFFFFFFFF);
void DrawUITriangle(Vector2 v[3], unsigned color = 0xFFFFFFFF);
#endif