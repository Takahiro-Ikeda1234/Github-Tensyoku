#pragma once
#include<string>
#include<vector>
#include<cstdio>
#include<cstring>

int loadFileAllData(FILE * fp, std::vector<std::string>* data);
bool IsSplit(char c);
std::string NextData(const std::string * str, int* index);