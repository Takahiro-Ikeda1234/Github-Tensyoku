#pragma once
class GameProcess
{
public:
	GameProcess();
	~GameProcess();
	bool Update();
private:
	class SceneManager* mSceneManager;
};
