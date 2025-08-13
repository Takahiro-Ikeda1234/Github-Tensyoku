//=============================================================================
//
// [obj_Background.h] 背景オブジェクト
//
//=============================================================================
#ifndef _OBJ_WALL_H_
#define _OBJ_WALL_H_

//インクルードファイル
#include"Object.h"
#include<string>
#include"TextureData.h"
class Scene;
class ComponentSprite;

//クラス定義
class Wall : public Object
{
public:
	Wall(Scene* scene, int indexX, int indexY);
	Wall(Scene* scene, Vector2 pos);
	virtual ~Wall();
	void UpdateObject(float deltaTime) {}
	int GetIndexX() { return mIndexX; }
	int GetIndexY() { return mIndexY; }
private:
	int mIndexX;
	int mIndexY;
};

#endif