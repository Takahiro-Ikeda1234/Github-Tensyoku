#include "StageData.h"
#include "StageManager.h"
#include "SceneEditor.h"
static const char stageDataPath[] = "resources/stages/";
void StageData::CreateStage(StageManager * stageManager, int stageID)
{
	std::string path;
	path += stageDataPath + mStageName[stageID] + ".txt";
	FILE* stageDataFile = fopen(path.c_str(), "r");
	std::vector<std::string> stageData;
	int row = loadFileAllData(stageDataFile, &stageData);
	int layer = 0;
	int width = 0;
	int height = 0;
	for (int i = 0; i < 2; i++)
	{
		if (stageData[i][0] == '\n') continue;
		char* p = strchr((char*)stageData[i].c_str(), '=');
		if (p)
		{
			*p = '\0';
			if (!strcmp(stageData[i].c_str(), "width"))
				width = atoi(p + 1);
			else if (!strcmp(stageData[i].c_str(), "height"))
				height = atoi(p + 1);
		}
	}
	stageManager->SetStageSize(width, height);
	for (int i = 2; i < row; i++)
	{
		if (stageData[i][0] == '\n') continue;
		char* p = strchr((char*)stageData[i].c_str(), '=');
		if (p)
		{
			*p = '\0';
			if (!strcmp(stageData[i].c_str(), "changeLayer"))
				layer = atoi(p + 1);
			else
			{
				int objectID = atoi(stageData[i].c_str());
				if (OBJECT_DATA[objectID].useCh)
				{
					CreateObjectsCh(stageManager, objectID, p + 1, layer);
				}
				else
				{
					CreateObjects(stageManager, objectID, p + 1, layer);
				}
			}
		}
	}
	fclose(stageDataFile);
}

void StageData::CreateEditorStage(SceneEditor::StageObjectData ** editorStageData, int * width, int * height, int stageID)
{
	std::string path;
	path += stageDataPath + mStageName[stageID] + ".txt";
	FILE* stageDataFile = fopen(path.c_str(), "r");
	std::vector<std::string> stageData;
	int row = loadFileAllData(stageDataFile, &stageData);
	int layer = 0;
	for (int i = 0; i < 2; i++)
	{
		if (stageData[i][0] == '\n') continue;
		char* p = strchr((char*)stageData[i].c_str(), '=');
		if (p)
		{
			*p = '\0';
			if (!strcmp(stageData[i].c_str(), "width"))
				*width = atoi(p + 1);
			else if (!strcmp(stageData[i].c_str(), "height"))
				*height = atoi(p + 1);
		}
	}
	for (int i = 0; i < EDITOR_LAYER_NUM; i++)
	{
		editorStageData[i] = new SceneEditor::StageObjectData[*width * *height];
		for (int x = 0; x < *width; x++)
		{
			for (int y = 0; y < *height; y++)
			{
				editorStageData[i][y * *width + x].id = -1;
				editorStageData[i][y * *width + x].ch = -1;
			}
		}
	}
	for (int i = 2; i < row; i++)
	{
		if (stageData[i][0] == '\n') continue;
		char* p = strchr((char*)stageData[i].c_str(), '=');
		if (p)
		{
			*p = '\0';
			if (!strcmp(stageData[i].c_str(), "changeLayer"))
				layer = atoi(p + 1);
			else
			{
				int objectID = atoi(stageData[i].c_str());
				if (OBJECT_DATA[objectID].useCh)
				{
					CreateEditorObjectsCh(editorStageData[layer], objectID, p + 1, *width, *height);
				}
				else
				{
					CreateEditorObjects(editorStageData[layer], objectID, p + 1, *width, *height);
				}
			}
		}
	}
	fclose(stageDataFile);
}

void StageData::SaveStageFile(SceneEditor::StageObjectData ** editorStageData, int width, int height, int stageID)
{
	std::string path;
	path += stageDataPath + mStageName[stageID] + ".txt";
	FILE* stageDataFile = fopen(path.c_str(), "w");
	fprintf(stageDataFile, "width=%d\n", width);
	fprintf(stageDataFile, "height=%d\n", height);

	for (int i = 0; i < EDITOR_LAYER_NUM; i++)
	{
		int count[OBJECT_ID_MAX] = {};
		std::string exportData[OBJECT_ID_MAX];
		fprintf(stageDataFile, "changeLayer=%d\n", i);
		for (int x = 0; x < width; x++)
		{
			for (int y = 0; y < height; y++)
			{
				int id = editorStageData[i][y * width + x].id;
				int ch = editorStageData[i][y * width + x].ch;
				if (id > -1)
				{
					if (ch == -1)
					{
						char buf[64];
						sprintf(buf, "(%d,%d)", x, y);
						exportData[id] += buf;
						count[id]++;
					}
					else
					{
						char buf[64];
						sprintf(buf, "(%d,%d,%d)", x, y, ch);
						exportData[id] += buf;
						count[id]++;
					}
				}
			}
		}

		for (int j = 0; j < OBJECT_ID_MAX; j++)
		{
			if (count[j] > 0)
			{
				fprintf(stageDataFile, "%d={%s}\n", j, exportData[j].c_str());
			}
		}
	}
	fclose(stageDataFile);
}

void StageData::CreateObjects(StageManager *stageManager, int id, const char str[], int layer)
{
	int bufferIndex = 0;
	int digit = 0;
	char numBuffer[4] = "";
	int indexX = 0;
	int indexY = 0;
	while (1)
	{
		char c = str[bufferIndex++];
		if (c == '}') break;
		switch (c)
		{
		case '(':
			indexX = 0;
			indexY = 0;
			break;
		case ',':
			indexX = atoi(numBuffer);
			digit = 0;
			memset(numBuffer, 0, sizeof(numBuffer));
			break;
		case ')':
			indexY = atoi(numBuffer);
			digit = 0;
			memset(numBuffer, 0, sizeof(numBuffer));
			stageManager->CreateObject(id, indexX, indexY, layer);
			break;
		case '{':
			break;
		default:
			numBuffer[digit] = c;
			digit++;
			break;
		}
	}
}

void StageData::CreateEditorObjects(SceneEditor::StageObjectData * editorStageData, int id, const char str[], int width, int height)
{
	int bufferIndex = 0;
	int digit = 0;
	char numBuffer[4] = "";
	int indexX = 0;
	int indexY = 0;
	while (1)
	{
		char c = str[bufferIndex++];
		if (c == '}') break;
		switch (c)
		{
		case '(':
			indexX = 0;
			indexY = 0;
			break;
		case ',':
			indexX = atoi(numBuffer);
			digit = 0;
			memset(numBuffer, 0, sizeof(numBuffer));
			break;
		case ')':
			indexY = atoi(numBuffer);
			digit = 0;
			memset(numBuffer, 0, sizeof(numBuffer));
			editorStageData[indexY * width + indexX].id = id;
			break;
		case '{':
			break;
		default:
			numBuffer[digit] = c;
			digit++;
			break;
		}
	}
}

void StageData::LoadStageList()
{
	std::string path;
	path += stageDataPath;
	path += "stages.txt";
	FILE *stageFile = fopen(path.c_str(), "r");
	mStageName.push_back({});
	mStageCount = loadFileAllData(stageFile, &mStageName);
	fclose(stageFile);
}


void StageData::CreateObjectsCh(StageManager *stageManager, int id, const char str[], int layer)
{
	int bufferIndex = 0;
	int digit = 0;
	char numBuffer[4] = "";
	int indexX = 0;
	int indexY = 0;
	int ch = -1;
	int count = 0;
	while (1)
	{
		char c = str[bufferIndex++];
		if (c == '}') break;
		switch (c)
		{
		case '(':
			indexX = 0;
			indexY = 0;
			count = 0;
			ch = -1;
			break;
		case ',':
			if (count == 0)
				indexX = atoi(numBuffer);
			else if(count == 1)
				indexY = atoi(numBuffer);

			count++;
			digit = 0;
			memset(numBuffer, 0, sizeof(numBuffer));
			break;
		case ')':
			ch = atoi(numBuffer);
			digit = 0;
			memset(numBuffer, 0, sizeof(numBuffer));
			stageManager->CreateObject(id, indexX, indexY, layer, ch);
			break;
		case '{':
			break;
		default:
			numBuffer[digit] = c;
			digit++;
			break;
		}
	}
}

void StageData::CreateEditorObjectsCh(SceneEditor::StageObjectData * editorStageData, int id, const char str[], int width, int height)
{
	int bufferIndex = 0;
	int digit = 0;
	char numBuffer[4] = "";
	int indexX = 0;
	int indexY = 0;
	int ch = -1;
	int count = 0;
	while (1)
	{
		char c = str[bufferIndex++];
		if (c == '}') break;
		switch (c)
		{
		case '(':
			indexX = 0;
			indexY = 0;
			count = 0;
			ch = -1;
			break;
		case ',':
			if (count == 0)
				indexX = atoi(numBuffer);
			else if (count == 1)
				indexY = atoi(numBuffer);

			count++;
			digit = 0;
			memset(numBuffer, 0, sizeof(numBuffer));
			break;
		case ')':
			ch = atoi(numBuffer);
			digit = 0;
			memset(numBuffer, 0, sizeof(numBuffer));
			editorStageData[indexY * width + indexX].id = id;
			editorStageData[indexY * width + indexX].ch = ch;
			break;
		case '{':
			break;
		default:
			numBuffer[digit] = c;
			digit++;
			break;
		}
	}
}


void StageData::StageSizeFromStageFile(int id, int * x, int * y)
{
	std::string path;
	path += stageDataPath + mStageName[id] + ".txt";
	FILE* stageDataFile = fopen(path.c_str(), "r");
	std::vector<std::string> stageData;
	int row = loadFileAllData(stageDataFile, &stageData);
	int layer = 0;
	int width = 0;
	int height = 0;
	for (int i = 0; i < 2; i++)
	{
		if (stageData[i][0] == '\n') continue;
		char* p = strchr((char*)stageData[i].c_str(), '=');
		if (p)
		{
			*p = '\0';
			if (!strcmp(stageData[i].c_str(), "width"))
				width = atoi(p + 1);
			else if (!strcmp(stageData[i].c_str(), "height"))
				height = atoi(p + 1);
		}
	}
	fclose(stageDataFile);
	*x = width;
	*y = height;
}


void StageData::LoadStageTitle()
{
	std::string path;
	path += stageDataPath;
	path += "stageTitle.txt";
	FILE *stageFile = fopen(path.c_str(), "r");
	std::vector<std::string> stageData;
	int row = loadFileAllData(stageFile, &stageData);
	fclose(stageFile);
	for (int i = 0; i < STAGE_MAX; i++)
	{
		if (IsSplit(stageData[i][0])) continue;

		int index = 0;
		int stageID = 0;
		int wallID = 0;
		int typeID = 0;

		std::string temp = NextData(&stageData[i], &index);
		mStageTitle[i][0] = temp;
		temp = NextData(&stageData[i], &index);
		mStageTitle[i][1] = temp;
	}
}