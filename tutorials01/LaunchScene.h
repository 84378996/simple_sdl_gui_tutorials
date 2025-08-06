#pragma once
#include "SceneBase.h"

class LaunchScene : public SceneBase
{
public:
	bool init() override;
	int ProcessEvent(SDL_Event* e) override;
	int Update(uint32_t dt) override;
	int Render() override;
};