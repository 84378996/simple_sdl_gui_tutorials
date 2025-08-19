#include "SceneBase.h"
#include "Node.h"
#include "UIControl.h"
#include "UIRadioButtonGroup.h"
#include "UIRadioButton.h"

void SceneBase::AddNode(std::shared_ptr<Node> n)
{
    m_nodes.push_back(n);

    if (auto r = dynamic_cast<UIRadioButton*>(n.get()))
    {
        std::string gp = r->GroupName();
        auto it = m_radiobtngroup.find(gp);
        UIRadioButtonGroup* bg = nullptr;
        if (it != m_radiobtngroup.end())
        {
            bg = it->second;
        }
        else
        {
            bg = new UIRadioButtonGroup;
            m_radiobtngroup.emplace(gp, bg);
        }
        r->SetGroup(bg);
    }

}

void SceneBase::RemoveNode(std::shared_ptr<Node> n)
{
    auto it = std::find_if(m_nodes.begin(), m_nodes.end(), [=](std::shared_ptr<Node>& nd) { return n.get() == nd.get(); });
    if (it != m_nodes.end()) {
        m_nodes.erase(it);
    }
}

void SceneBase::SetFocusControl(UIControl* n)
{
    if (n != m_focusControl)
    {
        if (m_focusControl)
            m_focusControl->SetFocus(false);
        m_focusControl = n;
    }
}

void SceneBase::Render()
{
    for (auto& c : m_nodes)
    {
        if (c->IsVisible())
            c->Render();
    }
}

bool SceneBase::Process(SDL_Event* e)
{
    for (auto it = m_nodes.rbegin(); it != m_nodes.rend(); ++it)
    {
        if (!(*it)->IsVisible())continue;
        if ((*it)->Process(e))
            return false;
    }

    return true;
}

void SceneBase::Update(uint32_t dt)
{
    for (auto& c : m_nodes)
        c->Update(dt);
}

void SceneBase::RadioGroupNameChanged(UIRadioButton* btn, const std::string& _gp)
{
    auto old = btn->GroupName();
    auto gpp = m_radiobtngroup.find(old);
    if (gpp != m_radiobtngroup.end())
    {
        auto _gpp = gpp->second;
        _gpp->Remove(btn);
        if (_gpp->Count() == 0)
        {
            delete _gpp;
            m_radiobtngroup.erase(gpp);
        }
    }

    auto newgpp = m_radiobtngroup.find(_gp);
    if (newgpp == m_radiobtngroup.end())
    {
        auto ptr = new UIRadioButtonGroup;
        m_radiobtngroup.emplace(_gp, ptr);
        newgpp = m_radiobtngroup.find(_gp);
    }
    newgpp->second->Add(btn);
}