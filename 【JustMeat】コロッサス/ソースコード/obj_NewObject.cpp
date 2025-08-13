#include "obj_NewObject.h"
#include "StageManager.h"	//StageManager::ToPosition(indexX, indexY)に必要
#include "ComponentAnimationSprite.h"	// new ComponentAnimationSpriteに必要

//Vector2はfloat x,float yの値を持つ構造体(クラス)。位置や大きさ、速度等に使う
//引数posのワールド座標のにオブジェクトが生成される。
NewObject::NewObject(Scene * scene, Vector2 pos)
	: Object(scene, pos)
{
	//Componentと名の付くクラスはnewで作成するだけでフレームごとに更新や描画処理が行われる。
	//ComponentAnimationSpriteは画像表示に必要な機能が含まれている。
	//引数1は必ずthis。引数2は使用する画像のタグ。
	//引数3(LAYER_OBJECT)は描画優先度。値が小さいほど手前に表示される。LAYER_OBJECTの値は100
	mCASprite = new ComponentAnimationSprite(this, TextureTag::Block, LAYER_OBJECT);
	//上のように書くだけでオブジェクトの位置にブロックの画像が表示される。

}

//indexをワールド座標に変換して別のコンストラクタを呼び出す
NewObject::NewObject(Scene * scene, int indexX, int indexY)
	: NewObject(scene, StageManager::ToPosition(indexX, indexY))
{
	//ここには何も処理を書かない
}

void NewObject::UpdateObject(float deltaTime)
{
	//フレームごとに実行される関数。ここにオブジェクトの処理を記述していく。
	//deltaTimeは前フレームからの経過時間(60FPSであればおよそ0.0166f)
}