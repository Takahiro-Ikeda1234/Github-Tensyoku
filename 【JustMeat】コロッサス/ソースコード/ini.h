#pragma once
#include<string>
std::string GetConfigString(const char section[], const char key[]);
int GetConfigInt(const char section[], const char key[]);
float GetConfigFloat(const char section[], const char key[]);