#include "obj_warpGate.h"
#include "StageManager.h"
#include "ComponentAnimationSprite.h"
#include "obj_player.h"
#include"file.h"

WarpGate::WarpGate(Scene * scene, Vector2 pos, int ID, bool enter, bool right)
	: Object(scene, pos)
	, mID(ID)
	, mCanEnter(enter)
{

	mCASprite = new ComponentAnimationSprite(this, TextureTag::Entrance, LAYER_OBJECT);
	mCASprite->SetColorRate(0.7f);
	
	SetObjectType(Type::eWarpGate);
	SetSize(Vector2(2, 2) *BLOCKSIZE);
	Translate(Vector2(0.5f, -0.5f)*BLOCKSIZE);
	//Ú‘±‚ðŠm—§
	auto objects = GetScene()->FindObjects(this, Type::eWarpGate);
	for(auto it : objects)
	{
		if (((WarpGate*)it)->GetID() == mID)
		{
			mConnect = ((WarpGate*)it)->CreateConnection(this);
			break;
		}
	}

	if (!enter)
	{
		mCASprite->SetAnimation(1, 2, 1, 0, false);
		if (right)
		{
			SetDirection(Dir::eLeft);
			Translate(Vector2(BLOCKSIZE * 0.1f, 0));
		}
		else
		{
			SetDirection(Dir::eRight);
			Translate(Vector2(-BLOCKSIZE * 0.1f, 0));
		}
	}
}

WarpGate::WarpGate(Scene * scene, int indexX, int indexY, int ID, bool enter, bool right)
	: WarpGate(scene, StageManager::ToPosition(indexX, indexY), ID, enter, right)
{
}


