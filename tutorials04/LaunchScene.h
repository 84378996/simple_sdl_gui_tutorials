#pragma once
#include "SceneBase.h"

class UILabel;
class UIButton;
class LaunchScene : public SceneBase
{
public:
	bool init() override;
	void Update(uint32_t dt) override;
	std::shared_ptr<UILabel> m_lb;
	std::shared_ptr<UILabel> m_lbmsg;
	std::shared_ptr<UIButton> m_btn;
};