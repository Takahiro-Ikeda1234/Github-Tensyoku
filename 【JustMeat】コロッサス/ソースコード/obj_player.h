//=============================================================================
//
// [obj_player.h] プレイヤー
//
//=============================================================================
#pragma once

//インクルードファイル
#include"Object.h"
#include<string>
#include"TextureData.h"
#include"Counter.h"
#include"Vector2.h"
class ComponentMove;
class ComponentMoveClimb;
class ComponentClickEvent;

//クラス定義
class Player : public Object
{
public:

	enum class State
	{
		eLandDefault,
		eClimbDefault,
		eFallDefault,
		eAwaking,
		eAwakingClimb,
		eLandDying,
		eFallDying,
		eFreefallDying,
		eClimbJump,
		eEntering,
		eGrab,
		eLanding,
		eLandDown,

		eStage0Event1_Move,
		eStage0Event1_Jump,
		eStage0Event1_Stay,
		eStage0Event1_CameraMove,
		eStage0Event2_Thrust,
		eStage0Event2_Pause,
		eStage0Event2_Grab,
		eStage0Event2_Fall,
	};

	Player(Scene* scene, Vector2 position);
	Player(Scene* scene, int indexX, int indexY);
	~Player() {}
	void UpdateObject(float deltaTime);
	bool IsDead() { return mIsDead; }
	bool IsDying() { if (mIsDead)return true; return mIsDying; }
	bool CanUpdateCheckPoint();
	void HitGolemArm(int hitType);//0=左から右, 1=右から左、2=上から下、3=下から上
	void HitLaser(int hitType);//0=左から右, 1=右から左、2=上から下、3=下から上

	bool UseStamina(float stamina) { if (mStamina < stamina) { return false; }; mStamina -= stamina; return true; }
	bool JumpUseStamina();
	void HealStamina(float stamina) { mStamina += stamina; if (mStamina > mMaxStamina) mStamina = mMaxStamina; }
	void AddHasBlock() { mHasBlockCount+=10; if (mHasBlockCount > 99) mHasBlockCount = 99; }
	int GetHasBlockCount() { return mHasBlockCount; }

	void StageChangeSave();
	void StageChangeLoad();
	void WoodJump();
	State GetPlayerState() { return mState; }

	bool IsLanding();
	bool IsUsingDummy() { return mIsUsingDummy; }
	float DummyCTPercentage() { return mDummyCoolTime.Percentage(); }
	void CancelDummy(bool makeEffect);
	int GetGolemCount() { return mGolemCount; }

	void SetCanControl(bool control) { mCanControl = control; }
	bool InCave() { return mInCave; }
	int GetCheckpointID()const { return mSavedCheckPoint; }
private:
	void DyingEffect();
	void UpdateBGMVolume(float deltaTime);
	void UpdateDummy();
	void UpdateAnimation();
	void UpdateMoveFloor();
	void UpdateMoveLandAndFall(float deltaTime);
	void CheckWarpGate();
	ComponentAnimationSprite* mCASprite_Foreground;
	ComponentClickEvent* mClick;
	ComponentMove* mMove;
	ComponentMoveClimb* mClimb;

	State mState;
	Counter mNextMotionTime;
	float mMovingDirection;
	Counter mCannotClimbTime;

	bool mCanControl = true;
	bool mIsDying = false;
	bool mIsDead = false;
	bool mIsStaminaZero = false;
	void ClimbToFall();
	void LandToClimb();
	class StaminaGauge* mStaminaGauge;
	class ComponentSystemMessage* mSystemMessage;
	float mStamina = 100;
	float mMaxStamina = 100;

	int mHasBlockCount = 10;

	Counter mJumpingTime;
	Counter mCannotStaminaHealTime;

	Counter mDummyCoolTime;
	bool mIsUsingDummy = false;
	int mGolemCount = 0;
public:
	virtual void SaveCheckPointData() override;
	virtual void LoadCheckPointData() override;
private:
	std::unordered_map <std::string, int> mCheckPointData;


	typedef struct
	{
		float animframe;
		Vector2 offsetCloudGenerator;	//雲からの相対位置
	}StageChangeData;
	static StageChangeData mStageChangeData;


	//状態
private:
	void StateAwaking(float deltaTime);
	void StateAwakingClimb(float deltaTime);
	void StateLandDying(float deltaTime);
	void StateFallDying(float deltaTime);
	void StateFreefallDying(float deltaTime);
	void StateLandDefault(float deltaTime);
	void StateFallDefault(float deltaTime);
	void StateClimbDefault(float deltaTime);
	void StateClimbJump(float deltaTime);
	void StateEntering(float deltaTime);
	void StateGrab(float deltaTime);
	void StateLanding(float deltaTime);
	void StateLandDown(float deltaTime);
	//使用する当たり判定
	void CollisionUpdateLandDefault(float deltaTime);
	void CollisionUpdateClimbDefault(float deltaTime);
	void CollisionUpdateClimbJump(float deltaTime);
	void CollisionUpdateFall(float deltaTime);
	//イベント用
public:
	void Stage0Event1Start(Vector2 destination);
private:
	void StateStage0Event1Move(float deltaTime);
	void StateStage0Event1Jump(float deltaTime);
	void StateStage0Event1Stay(float deltaTime);
	void StateStage0Event1CameraMove(float deltaTime);
	void StateStage0Event2Thrust(float deltaTime);
	void StateStage0Event2Pause(float deltaTime);
	void StateStage0Event2Grab(float deltaTime);
	void StateStage0Event2Fall(float deltaTime);
	Vector2 mStage0Event1Move_Destination;
	float mStage0Event1MoveX;
	bool mStage0Event1FadeoutFlag = false;


private:
	static void LoadSettingFile();
	static bool mAlreadyLoadFile;


	bool mInCave = false;
	bool mCPInCave = false;
	int mSavedCheckPoint = 0;	//どのチェックポイントを通ったか。
	Dir mCPDir = Dir::eRight;
	Counter mFootstepTime;

};