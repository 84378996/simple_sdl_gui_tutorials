#pragma once
#include <SDL2/SDL.h>
#include "SDLRenderer.h"
#include <type_traits>

class SceneBase
{
public:
	virtual bool init() = 0;
	virtual int ProcessEvent(SDL_Event* e) { return 0; }
	virtual int Update(uint32_t dt) { return 0; }
	virtual int Render() { return 0; }
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

protected:
	//....
};
