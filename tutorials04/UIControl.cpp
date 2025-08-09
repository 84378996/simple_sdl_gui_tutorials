#include "UIControl.h"
#include "SceneManager.h"

UIControl::UIControl()
    : m_text(""),
    m_state(UIState::Normal)
{
}

void UIControl::SetText(const std::string& text) {
    if (m_text != text)
    {
        std::string oldtext = m_text;
        m_text = text;
        OnTextChanged(oldtext);
    }
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
    if (m_fontSize != size) {
        int oldsize = m_fontSize;
        m_fontSize = size;
        OnFontSizeChanged(oldsize);
    }
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

bool UIControl::Handle(SDL_Event* e) {

    int mouseX, mouseY;
    SDL_GetMouseState(&mouseX, &mouseY);

    SDL_Rect bounds = GetGlobalRect();
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
    {
        if (auto ses = sSceneMgr->GetCurrentScene())
        {
            ses->SetFocusControl(this);
            SetFocus(true);
        }

        if (isHovering && e->button.button == SDL_BUTTON_LEFT) {
            m_state = UIState::Pressed;
        }
        break;
    }

    case SDL_MOUSEBUTTONUP:
        if (m_state == UIState::Pressed && isHovering && e->button.button == SDL_BUTTON_LEFT) {
            m_state = UIState::Hovered;
            FireEvent(EventType::ET_LCLICK, this);
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

void UIControl::OnMouseOut()
{
    m_state = Normal;
    FireEvent(EventType::ET_MOUSE_OUT, nullptr);
}


bool UIControl::SetFocus(bool _focus)
{
    if (m_isFocus != _focus)
    {
        m_isFocus = _focus;
        if (m_isFocus)
        {
            FireEvent(EventType::ET_GOTFOCUS, this);
            if (auto ses = sSceneMgr->GetCurrentScene())
            {
                ses->SetFocusControl(this);
            }
        }
        else
        {
            if (auto ses = sSceneMgr->GetCurrentScene())
            {
                ses->SetFocusControl(nullptr);
            }
            FireEvent(EventType::ET_LOSEFOCUS, this);
        }

        OnFocusChanged();
        return true;
    }
    return false;
}