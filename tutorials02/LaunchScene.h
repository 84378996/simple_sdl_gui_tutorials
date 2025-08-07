#pragma once
#include "SceneBase.h"

class UILabel;
class LaunchScene : public SceneBase
{
public:
	bool init() override;
	void Update(uint32_t dt) override;
	std::shared_ptr<UILabel> m_lb;
};