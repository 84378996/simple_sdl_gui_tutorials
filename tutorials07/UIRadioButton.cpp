#include "UIRadioButton.h"
#include "UIRadioButtonGroup.h"
#include "SceneManager.h"
#include "SceneBase.h"

#define RBTN_CHECK_RES "D:/Projects/res/checked.jpeg"
#define RBTN_UNCHECK_RES "D:/Projects/res/uncheck.jpeg"
SDL_Texture* rbtn_checked{ nullptr };
SDL_Texture* rbtn_unchecked{ nullptr };

UIRadioButton::UIRadioButton()
    : UIRadioButton("")
{
}

UIRadioButton::UIRadioButton(const std::string& _text)
    : UIControl()
{
    m_text = _text;
    static unsigned int _ix = 1;
    _ix++;
    m_radioindex = _ix;

    if (!rbtn_checked)
        rbtn_checked = sRender->LoadTextureFromFile(RBTN_CHECK_RES);

    if (!rbtn_unchecked)
        rbtn_unchecked = sRender->LoadTextureFromFile(RBTN_UNCHECK_RES);
}

UIRadioButton::~UIRadioButton()
{
}

void UIRadioButton::Render()
{
    auto rc = GetGlobalRect();
    auto rcCheck = SDL_Rect{ rc.x, rc.y, CHECK_BOX_SIZE, CHECK_BOX_SIZE };
    auto [w, h] = sRender->GetTextSize(m_text.c_str(), m_fontSize);
    int dism = (rc.h - h) / 2;
    auto rcText = SDL_Rect{ rcCheck.x + rcCheck.w + 4, rc.y + dism, w, h };
    //sRender->DrawCircle(rcCheck, m_textColor);
    sRender->DrawString(m_text.c_str(), m_fontSize, rcText, m_textColor);

    if (m_ischecked)
    {
        //auto rcSel = SDL_Rect{ rcCheck.x + 4, rcCheck.y + 4, rcCheck.w - 8, rcCheck.h - 8 };
        //sRender->FillCircle(rcSel, m_textColor);
        sRender->DrawImage(rbtn_checked, rcCheck.x, rcCheck.y);
    }
    else
    {
        sRender->DrawImage(rbtn_unchecked, rcCheck.x, rcCheck.y);
    }
}

bool UIRadioButton::Handle(SDL_Event* e)
{
    if (e->type == SDL_MOUSEBUTTONDOWN)
    {
        if (e->button.button == SDL_BUTTON_LEFT)
        {
            if (!m_ischecked)
            {
                m_gropu->SetRadioButtonCheck(this, !m_ischecked);
                FireEvent(EventType::ET_CHECKCHANGED, nullptr);
            }
        }
    }
    return UIControl::Handle(e);
}

void UIRadioButton::OnTextChanged(const std::string& old)
{
    auto [w, h] = sRender->GetTextSize(m_text.c_str(), m_fontSize);
    Vec2 sz = { w + CHECK_BOX_SIZE ,SDL_max(h, CHECK_BOX_SIZE) };
    SetSize(sz);
}

void UIRadioButton::SetCheck(bool _check)
{
    m_ischecked = _check;
}

std::string UIRadioButton::GroupName() const
{
    if (m_gpName.empty())
    {
        return std::string("group") + std::to_string(m_radioindex);
    }
    return m_gpName;
}

void UIRadioButton::SetGroupName(const std::string& _gp)
{
    if (GroupName() != _gp)
    {
        if (auto s = sSceneMgr->GetCurrentScene())
            s->RadioGroupNameChanged(this, _gp);
        m_gpName = _gp;
    }
}

void UIRadioButton::SetGroup(UIRadioButtonGroup* _gp)
{
    m_gropu = _gp;
    _gp->Add(this);
}