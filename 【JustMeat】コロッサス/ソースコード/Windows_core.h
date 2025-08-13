#pragma once
#include<Windows.h>
static const int gDefaultWidth = 1920;
static const int gDefaultHeight = 1080;
int GetWindowWidth();
int GetWindowHeight();
float GetWindowScaleX();
float GetWindowScaleY();
HWND GetHWND();
HWND InitWINAPI(HINSTANCE hInstance, int nCmdShow);
bool GameLoopWINAPI();
void MessageErrorOK(const char msg[]);
void UninitWINAPI();