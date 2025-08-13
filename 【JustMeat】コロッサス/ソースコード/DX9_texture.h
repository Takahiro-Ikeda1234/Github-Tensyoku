#pragma once
#include<string>
#include"Vector2.h"
void *LoadTextureDX9(std::string filepath, int width, int height);
void UnloadTextureDX9(void* texture);

void StartExportStageData(int w, int h);
void ExportObject(void * texture, int indexX, int indexY, int useID, int divX, int divY);
void EndExportStageData(const char saveFileName[]);

void BufferToTexture();
void DrawBufferTexture();