#include "UIControl.h"
#include <SDL.h>

UIControl::UIControl()
    : m_text(""),
    m_textColor({ 255, 255, 255, 255 }),
    m_fontSize(16),
    m_backgroundColor({ 0, 0, 0, 0 }),
    m_state(UIState::Normal)
{
}

void UIControl::SetText(const std::string& text) {
    m_text = text;
}

const std::string& UIControl::GetText() const
{
    return m_text;
}

void UIControl::SetTextColor(const SDL_Color& color) {
    m_textColor = color;
}

SDL_Color UIControl::GetTextColor() const
{
    return m_textColor;
}

void UIControl::SetFontSize(int size) {
    m_fontSize = size;
}

int UIControl::GetFontSize() const
{
    return m_fontSize;
}

void UIControl::SetBackgroundColor(const SDL_Color& color) {
    m_backgroundColor = color;
}

SDL_Color UIControl::GetBackgroundColor() const
{
    return m_backgroundColor;
}

void UIControl::SetEnabled(bool enabled) {
    m_enabled = enabled;
    m_state = enabled ? UIState::Normal : UIState::Disabled;
}

bool UIControl::Process(SDL_Event* e) {
    if (!m_enabled || m_state == UIState::Disabled) {
        return false;
    }

    int mouseX, mouseY;
    SDL_GetMouseState(&mouseX, &mouseY);

    SDL_Rect bounds = { m_pos.x, m_pos.y, m_size.x, m_size.y };
    auto pt = SDL_Point{ mouseX, mouseY };
    bool isHovering = SDL_PointInRect(&pt, &bounds);

    switch (e->type) {
    case SDL_MOUSEMOTION:
        if (isHovering && m_state != UIState::Hovered) {
            m_state = UIState::Hovered;
        }
        else if (!isHovering && m_state == UIState::Hovered) {
            m_state = UIState::Normal;
        }
        break;

    case SDL_MOUSEBUTTONDOWN:
        if (isHovering && e->button.button == SDL_BUTTON_LEFT) {
            m_state = UIState::Pressed;
        }
        break;

    case SDL_MOUSEBUTTONUP:
        if (m_state == UIState::Pressed && isHovering && e->button.button == SDL_BUTTON_LEFT) {
            m_state = UIState::Hovered;
            OnClick(); // optional callback
        }
        else {
            m_state = isHovering ? UIState::Hovered : UIState::Normal;
        }
        break;

    default:
        break;
    }

    return false;
}
