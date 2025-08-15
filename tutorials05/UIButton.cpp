#include "UIButton.h"

SDL_Color UIButton::m_default_backgroud_color[UIState::UIStateLength] = { 
	SDL_Color{3, 194, 252,255} , //normal
	SDL_Color{91, 149, 235,255} , //hover
	SDL_Color{54, 100, 168,255} , //pressd
	SDL_Color{204, 204, 204,255} , //disable
};

UIButton::UIButton(SDL_Texture** _text)
{
	/*Our example is simple. Here, the caller manages resource allocation and ensures that the parameters are correct.
	Of course, resource management will be involved in the following content.*/
	if (_text && _text[0]) {
		std::memcpy(m_tex, _text, 4 * sizeof(void*));
		auto [w,h]=sRender->GetTextureSize(_text[0]);
		m_size.x = w;
		m_size.y = h;
	}
	else
	{
		m_size = { 75,20 };
	}
}

UIButton::UIButton()
	: UIButton(nullptr)
{
}

void UIButton::Render()
{
	SDL_Rect rc = GetGlobalRect();
	if (m_tex[0])
		sRender->DrawImage(m_tex[m_state], rc.x, rc.y);
	else
		sRender->FillRect( rc, m_default_backgroud_color[m_state]);
	if (!m_text.empty())
		sRender->DrawString2(m_text.c_str(), 14, rc, SDL_Color{ 0,0,0,255 });
}

void UIButton::SetResource(SDL_Texture** _tex)
{
	std::memcpy(m_tex, _tex, 4 * sizeof(void*));
}
