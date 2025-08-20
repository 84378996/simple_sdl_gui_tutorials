#pragma once
#include <vector>

class UIRadioButton;
class UIRadioButtonGroup
{
public:
	void SetRadioButtonCheck(UIRadioButton* btn, bool _check);
	void Add(UIRadioButton* btn);
	void Remove(UIRadioButton* btn);
	int Count() const;
private:
	std::vector<UIRadioButton*> m_rbtns;
};