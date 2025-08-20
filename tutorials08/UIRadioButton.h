#pragma once
#include "UIControl.h"

//我们简单的定义两个全局的纹理
extern SDL_Texture* rbtn_checked;
extern SDL_Texture* rbtn_unchecked;

class UIRadioButtonGroup;
class UIRadioButton : public UIControl
{
public:
	UIRadioButton();
	UIRadioButton(const std::string& _text);
	~UIRadioButton();

	void Render() override;
	bool Handle(SDL_Event* e) override;
	void OnTextChanged(const std::string& old) override;

	bool IsChecked() const { return m_ischecked; }
	void SetCheck(bool _check);

	std::string GroupName() const;
	void SetGroupName(const std::string& _gp);
	void SetGroup(UIRadioButtonGroup* _gp);

private:
	bool m_ischecked{ false };
	UIRadioButtonGroup* m_gropu{ nullptr };
	std::string m_gpName;
	int m_radioindex{ 0 };
};