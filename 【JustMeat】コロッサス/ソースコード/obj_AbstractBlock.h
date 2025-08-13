#pragma once
#include"Object.h"
class Scene;
class AbstractBlock : public Object
{
public:
	AbstractBlock(Scene* scene, int indexX, int indexY, int id);
	AbstractBlock(Scene* scene, Vector2 pos, int id);
	virtual ~AbstractBlock();
	void UpdateObject(float deltaTime) {}
	int GetIndexX() { return mIndexX; }
	int GetIndexY() { return mIndexY; }
	virtual void UpdateBlockTexture();
	void ExportObjectTexture();
private:
	int mIndexX;
	int mIndexY;
	class ComponentSprite* mBlockSprite;
	class ComponentSprite* mCornerSprite;
	int mID;
};