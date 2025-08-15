#pragma once
#include "UIControl.h"

class UICheckBox : public UIControl
{
public:
	UICheckBox();
	UICheckBox(const std::string& _text);
	virtual ~UICheckBox() = default;

	void Render() override;
	bool Handle(SDL_Event* e) override;
	void OnTextChanged(const std::string& old) override;

	bool IsChecked() const { return m_ischecked; }
	void SetCheck(bool _check);

protected:
	bool m_ischecked{ false };
};