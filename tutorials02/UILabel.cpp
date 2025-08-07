#include "UILabel.h"

UILabel::UILabel(const std::string& text)
{
	m_text = text;
}

void UILabel::Render()
{
	//draw background
	auto pos = GetGlobalPosition();
	SDL_Rect rect = { pos.x, pos.y, m_size.x, m_size.y };
	sRender->FillRect(rect, m_backgroundColor);
	//By default we draw the text to the center
	sRender->DrawString2(m_text.c_str(), m_fontSize, rect, m_textColor);
}
