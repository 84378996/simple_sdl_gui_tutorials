
#include "UIRadioButtonGroup.h"
#include "UIRadioButton.h"

void UIRadioButtonGroup::SetRadioButtonCheck(UIRadioButton* btn, bool _check)
{
	for (auto& c : m_rbtns)
	{
		if (c == btn)
		{
			c->SetCheck(_check);
		}
		else
		{
			c->SetCheck(!_check);
		}
	}
}

void UIRadioButtonGroup::Add(UIRadioButton* btn)
{
	m_rbtns.push_back(btn);
}

void UIRadioButtonGroup::Remove(UIRadioButton* btn)
{
	m_rbtns.erase(std::remove(m_rbtns.begin(), m_rbtns.end(), btn));
}

int UIRadioButtonGroup::Count() const
{
	return (int)m_rbtns.size();
}
