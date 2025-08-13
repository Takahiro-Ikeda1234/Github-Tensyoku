#include<Windows.h>
#include"ini.h"
static const char FILEPATH[] = "resources/config.ini";
static const int BUFFER_SIZE = 128;
std::string GetConfigString(const char section[], const char key[])
{
	char buf[BUFFER_SIZE] = "";
	GetPrivateProfileString(section, key, "notfound", buf, BUFFER_SIZE, FILEPATH);
	return buf;
}

int GetConfigInt(const char section[], const char key[])
{
	return GetPrivateProfileInt(section, key, 0, FILEPATH);
}

float GetConfigFloat(const char section[], const char key[])
{
	return (float)atof(GetConfigString(section, key).c_str());
}