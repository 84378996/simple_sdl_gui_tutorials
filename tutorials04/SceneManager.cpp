#include "SceneManager.h"
#include "SceneBase.h"
#include "UITextEdit.h"
#include <chrono>
#include <thread>

using namespace std::chrono;

SceneManager::SceneManager()
{

}

SceneManager::~SceneManager()
{

}

void SceneManager::Run(SceneBase* s)
{
	auto now = system_clock::now();
	auto prevTime = now;
	int64_t ticks = 0;
	m_currentScene = s;
	SDL_assert(s);
	while (m_bRun)
	{
		now = system_clock::now();
		ticks = duration_cast<milliseconds> (now - prevTime).count();
		SDL_Event e;
		while (SDL_PollEvent(&e))
		{
			if (e.type == SDL_QUIT)
			{
				m_bRun = false;
				break;
			}
			m_currentScene->Process(&e);
		}


		m_currentScene->Update(ticks);
		{
			NewFrame _{};
			m_currentScene->Render();

			if (auto uicontrol = m_currentScene->GetFocusedControl())
			{
				if (auto _tb = dynamic_cast<UITextEdit*>(uicontrol))
				{
					_tb->DrawImgList();
				}
			}
		}



		if (m_waitSwitchScene)
		{
			auto ts = m_currentScene;
			m_currentScene = m_waitSwitchScene;
			m_waitSwitchScene = nullptr;
			if (ts)
				delete ts;
		}

		if (m_pushedScene)
		{
			m_currentScene = m_pushedScene;
			m_pushedScene = nullptr;
		}

		if (m_waitPopScene)
		{
			if (!m_sceneTrees.empty())
			{
				auto _ts = m_currentScene;
				m_currentScene = m_sceneTrees.top();
				m_sceneTrees.pop();
				if (_ts)
					delete _ts;
			}
			m_waitPopScene = false;
		}

		prevTime = now;
		std::this_thread::sleep_for((milliseconds)((int)(1000 / m_fps - ticks)));
	}

	while (!m_sceneTrees.empty())
	{
		auto s = m_sceneTrees.top();
		m_sceneTrees.pop();
		delete s;
	}
}

void SceneManager::SwitchScene(SceneBase* s)
{
	m_waitSwitchScene = s;
}

void SceneManager::PushScene(SceneBase* s)
{
	m_sceneTrees.push(m_currentScene);
	m_pushedScene = s;
}

void SceneManager::PopScene()
{
	if (m_sceneTrees.empty()) return;
	m_waitPopScene = true;
}


void SceneManager::Exit()
{
	m_bRun = false;
}
