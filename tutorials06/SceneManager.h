#pragma once
#include "SceneBase.h"
#include <stack>
class Node;
class UIPoint;
class SceneManager
{
private:
	SceneManager();
	~SceneManager();
public:
	static SceneManager* Instance()
	{
		static SceneManager _mgr;
		return &_mgr;
	}

	void Run(SceneBase* s);
	void SwitchScene(SceneBase* s);
	void PushScene(SceneBase* s);
	void PopScene();
	SceneBase* GetCurrentScene() const { return m_currentScene; }
	void Exit();
private:
	std::stack<SceneBase*> m_sceneTrees;
	SceneBase* m_currentScene{ nullptr };
	SceneBase* m_waitSwitchScene{ nullptr };
	SceneBase* m_pushedScene{ nullptr };
	float m_fps{ 60.f };
	//Node* m_popup{ nullptr };
	bool m_waitPopScene{ false };
	bool m_bRun{ true };
};

#define sSceneMgr SceneManager::Instance()