#include "UICheckBox.h"

UICheckBox::UICheckBox()
    : UIControl()
{
}

UICheckBox::UICheckBox(const std::string& _text)
    : UIControl()
{
    m_text = _text;
    OnTextChanged("");
}

void UICheckBox::Render()
{
    auto rc = GetGlobalRect();
    auto rcCheck = SDL_Rect{ rc.x, rc.y, CHECK_BOX_SIZE, CHECK_BOX_SIZE };
    auto [w, h] = sRender->GetTextSize(m_text.c_str(), m_fontSize);
    int dism = (rc.h - h) / 2;
    auto rcText = SDL_Rect{ rcCheck.x+rcCheck.w + 4, rc.y, w, h };
    sRender->DrawRect(rcCheck, m_textColor);
    sRender->DrawString(m_text.c_str(), m_fontSize, rcText, m_textColor);

    if (m_ischecked)
    {
        SDL_Point p1, p2, p3;
        p1 = { rcCheck.x + 2, rc.y + rc.h / 2 };
        p2 = { rcCheck.x + rcCheck.w / 2, (rc.y + rc.h) - 4 };
        p3 = { rcCheck.x + rcCheck.w - 4, rcCheck.y + 4 };
        sRender->DrawLine(p1, p2, m_textColor);
        sRender->DrawLine(p2, p3, m_textColor);
    }
}

bool UICheckBox::Handle(SDL_Event* e)
{
    if (e->type == SDL_MOUSEBUTTONDOWN)
    {
        if (e->button.button == SDL_BUTTON_LEFT)
        {
            m_ischecked = !m_ischecked;
            FireEvent(EventType::ET_CHECKCHANGED, nullptr);
        }
    }
    return UIControl::Handle(e);
}

void UICheckBox::OnTextChanged(const std::string& old)
{
    auto [w, h] = sRender->GetTextSize(m_text.c_str(), m_fontSize);
    Vec2 sz = { w + CHECK_BOX_SIZE ,SDL_max(h, CHECK_BOX_SIZE) };
    SetSize(sz);
}

void UICheckBox::SetCheck(bool _check)
{
    m_ischecked = _check;
}