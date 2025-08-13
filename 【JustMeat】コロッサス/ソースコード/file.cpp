#include "file.h"
#define LINE_BUFF_LENGTH 64
static bool loadFileLineData(FILE *fp, std::string* data);

int loadFileAllData(FILE * fp, std::vector<std::string>* data)
{
	int row = 0;
	while (1)
	{
		(*data).push_back("");
		if (loadFileLineData(fp, &(*data)[row++]))
			break;
	}
	return row;
}


bool loadFileLineData(FILE *fp, std::string* data)
{
	char buffer[LINE_BUFF_LENGTH];
	while (1)
	{
		if (fgets(buffer, sizeof(buffer), fp) == NULL) return true;
		char* p = strchr(buffer, '\n');
		if (p)
		{
			*p = '\0';
		}
		*data += buffer;
		if (p)break;
	}
	return false;
}

bool IsSplit(char c)
{
	if (c == ',' || c == '\n' || c == '\0' || c == '#')
	{
		return true;
	}
	return false;
}

std::string NextData(const std::string * str, int* index)
{
	std::string temp;
	while (!IsSplit((*str)[*index]))
	{
		temp += (*str)[*index];
		(*index)++;
	}
	(*index)++;
	return temp;
}