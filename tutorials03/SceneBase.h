#pragma once
#include <SDL2/SDL.h>
#include "SDLRenderer.h"
#include <type_traits>
#include <memory>

class Node;
class SceneBase
{
public:
	virtual bool init() = 0;
	virtual bool Process(SDL_Event* e);
	virtual void Update(uint32_t dt);
	virtual void Render();
	virtual void OnUpdate(uint32_t dt) {}
	template<typename T, typename... Args, std::enable_if_t<std::is_base_of_v<SceneBase, T>>* = nullptr>
	inline static T* Create(Args&&... args)
	{
		T* t = new T(std::forward<Args>(args)...);
		if (!t->init())
		{
			delete t;
			return nullptr;
		}
		return t;
	}

	void AddNode(std::shared_ptr<Node> n);
	void RemoveNode(std::shared_ptr<Node> n);

protected:
	std::vector<std::shared_ptr<Node>> m_nodes;
};
