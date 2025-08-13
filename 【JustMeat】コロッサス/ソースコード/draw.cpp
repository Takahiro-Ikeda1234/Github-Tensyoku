//=============================================================================
//
// [Draw.cpp] �`�揈��
//
//=============================================================================

//�C���N���[�h�t�@�C��
#include"Draw.h"
#include"Vector2.h"
#include"myMath.h"
#include"Windows_core.h"
#include"DX9_draw.h"
#include"DX9_font.h"
//�v���g�^�C�v�錾
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
// �֐���: void DrawStart() 
// ����:   �`��J�n���̏��������s���֐�
// ---------------------------------------------------------------------------
void DrawStart(int r, int g, int b)
{
	DrawStartDX9(r, g, b);
}

// ---------------------------------------------------------------------------
// �֐���: void SwapBuffer()
// ����:   �`�抮�����̃o�b�t�@�؂�ւ����s���֐�
// ---------------------------------------------------------------------------
void SwapBuffer()
{
	SwapBufferDX9();
}

// ---------------------------------------------------------------------------
// �֐���: void DrawVertex(VERTEX_3D vertex[], void* texture)
// ����:   VERTEX_3D vertex[] ���_���
// ����:   void* texture �e�N�X�`���̃|�C���^
// ����:   �`����s���֐�
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
// �֐���: void SetVertex(VERTEX_3D vertex[], const Vector2& pos, const Vector2& size, float rotationDeg)
// ����:   VERTEX_3D vertex[] ���_���̏o�͐�
// ����:   const Vector2& pos �I�u�W�F�N�g�̒��S�ʒu
// ����:   const Vector2& size �I�u�W�F�N�g�̃T�C�Y
// ����:   float rotationDeg �I�u�W�F�N�g�̉�]�p�x
// ����:   �I�u�W�F�N�g�̏������Ƃɒ��_���̍��W��ݒ肷��֐�
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
// �֐���: void SetTexCoord(VERTEX_3D vertex[], int useID, int texX, int texY)
// ����:   VERTEX_3D vertex[] ���_���
// ����:   int useID �g�p����t���[��
// ����:   int texX �e�N�X�`��������������
// ����:   int texY �e�N�X�`���c����������
// ����:   �e�N�X�`���̕������A�g�p����t���[�������Ƃɒ��_����uv���W��ݒ肷��֐�
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
// �֐���: void SetVertex_uv(VERTEX_3D vertex[], const Vector2& pos, const Vector2& size, float rotationDeg, float uPercentage, float vPercentage)
// ����:   VERTEX_3D vertex[] ���_���̏o�͐�
// ����:   const Vector2& pos �I�u�W�F�N�g�̒��S�ʒu
// ����:   const Vector2& size �I�u�W�F�N�g�̃T�C�Y
// ����:   float rotationDeg �I�u�W�F�N�g�̉�]�p�x
// ����:   float uPercentage �e�N�X�`���̕\����
// ����:   float vPercentage �e�N�X�`���̕\����
// ����:   �I�u�W�F�N�g�̏������Ƃɒ��_���̍��W��ݒ肷��֐��B�S�̂ł͂Ȃ��ꕔ��\���ł���
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
// �֐���: void SetTexCoord_uvPercentage(VERTEX_3D vertex[], int useID, int texX, int texY, float uPercentage, float vPercentage)
// ����:   VERTEX_3D vertex[] ���_���
// ����:   int useID �g�p����t���[��
// ����:   int texX �e�N�X�`��������������
// ����:   int texY �e�N�X�`���c����������
// ����:   float uPercentage �e�N�X�`���̕\����
// ����:   float vPercentage �e�N�X�`���̕\����
// ����:   �e�N�X�`���̕������A�g�p����t���[�������Ƃɒ��_����uv���W��ݒ肷��֐��B�S�̂ł͂Ȃ��ꕔ��\���ł���
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
// �֐���: void SetColor(VERTEX_3D vertex[], unsigned color)
// ����:   VERTEX_3D vertex[] ���_���
// ����:   unsigned color �F���
// ����:   ���_���̐F��ݒ肷��֐�
// ---------------------------------------------------------------------------
void SetColor(VERTEX_3D vertex[], unsigned color)
{
	for (int i = 0; i < 4; i++)
	{
		vertex[i].Color = color;
	}
}

// ---------------------------------------------------------------------------
// �֐���: void DrawRect(const Vector2& pos, const Vector2& size, unsigned colorARGB)
// ����:   const Vector2& pos ��`�̒��S�ʒu
// ����:   const Vector2& size ��`�̃T�C�Y
// ����:   unsigned colorARGB �F
// ����:   �g�݂̂̋�`��`�悷��֐�
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
// �֐���: void DrawTexture(void* texture, const Vector2& pos, const Vector2& size, float rotationDeg, int useID, int texX, int texY, unsigned colorARGB)
// ����:   void* texture �e�N�X�`���̃|�C���^
// ����:   const Vector2& pos �I�u�W�F�N�g�̒��S�ʒu
// ����:   const Vector2& size �I�u�W�F�N�g�̃T�C�Y
// ����:   float rotationDeg �I�u�W�F�N�g�̉�]�p�x
// ����:   int useID �g�p����t���[��
// ����:   int texX �e�N�X�`���̉�����������
// ����:   int texY �e�N�X�`���̏c����������
// ����:   unsigned colorARGB �F
// ����:   �e�N�X�`���`��֐��B���S��0,0�Ƃ��Ĉ���
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
// �֐���: void DrawTexture_uv(void* texture, const Vector2& pos, const Vector2& size, float rotationDeg, int useID, int texX, int texY, float uPercentage, float vPercentage, unsigned colorARGB)
// ����:   void* texture �e�N�X�`���̃|�C���^
// ����:   const Vector2& pos �I�u�W�F�N�g�̒��S�ʒu
// ����:   const Vector2& size �I�u�W�F�N�g�̃T�C�Y
// ����:   float rotationDeg �I�u�W�F�N�g�̉�]�p�x
// ����:   int useID �g�p����t���[��
// ����:   int texX �e�N�X�`���̉�����������
// ����:   int texY �e�N�X�`���̏c����������
// ����:   unsigned colorARGB �F
// ����:   float uPercentage �e�N�X�`���̕\����
// ����:   float vPercentage �e�N�X�`���̕\����
// ����:   �e�N�X�`���`��֐��B���S��0,0�Ƃ��Ĉ����B�S�̂ł͂Ȃ��ꕔ��\���ł���
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
// �֐���: void DrawScreen(void* texture, unsigned colorARGB)
// ����:   void* texture �e�N�X�`���̃|�C���^
// ����:   unsigned colorARGB �F
// ����:   �e�N�X�`������ʃT�C�Y�Ɋg�債�ĕ\������֐�
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
// �֐���: void DrawUITexture(void* texture, const Vector2& pos, const Vector2& size, float rotationDeg, int useID, int texX, int texY, unsigned colorARGB)
// ����:   void* texture �e�N�X�`���̃|�C���^
// ����:   const Vector2& pos �I�u�W�F�N�g�̒��S�ʒu
// ����:   const Vector2& size �I�u�W�F�N�g�̃T�C�Y
// ����:   float rotationDeg �I�u�W�F�N�g�̉�]�p�x
// ����:   int useID �g�p����t���[��
// ����:   int texX �e�N�X�`���̉�����������
// ����:   int texY �e�N�X�`���̏c����������
// ����:   unsigned colorARGB �F
// ����:   UI�`��֐��B�����0,0�Ƃ��Ĉ���
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
// �֐���: void DrawUITexture_uv(void* texture, const Vector2& pos, const Vector2& size, float rotationDeg, int useID, int texX, int texY, float uPercentage, float vPercentage, unsigned colorARGB)
// ����:   void* texture �e�N�X�`���̃|�C���^
// ����:   const Vector2& pos �I�u�W�F�N�g�̒��S�ʒu
// ����:   const Vector2& size �I�u�W�F�N�g�̃T�C�Y
// ����:   float rotationDeg �I�u�W�F�N�g�̉�]�p�x
// ����:   int useID �g�p����t���[��
// ����:   int texX �e�N�X�`���̉�����������
// ����:   int texY �e�N�X�`���̏c����������
// ����:   unsigned colorARGB �F
// ����:   float uPercentage �e�N�X�`���̕\����
// ����:   float vPercentage �e�N�X�`���̕\����
// ����:   UI�`��֐��B�����0,0�Ƃ��Ĉ����B�S�̂ł͂Ȃ��ꕔ��\���ł���
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


//���S��0,0�Ƃ��Ĉ���
void DrawString(std::string str, float charactersPerLine, Vector2 origin, Vector2 fontSize, unsigned color)
{
	Vector2 screenPos(origin.x - SCREEN_OFFSET_X, origin.y - SCREEN_OFFSET_Y);

	if (!CheckInCamera(screenPos, fontSize * charactersPerLine)) return;

	FontSystem::Inst()->Draw(str, screenPos, Vector2(fontSize.x  * TEXTURE_SCALE_X * g_TextureScale, fontSize.y  * TEXTURE_SCALE_Y * g_TextureScale), charactersPerLine, color);
}
//�����0,0�Ƃ��Ĉ���
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