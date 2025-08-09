#include "SceneBase.h"
#include "Node.h"
#include "UIControl.h"

void SceneBase::AddNode(std::shared_ptr<Node> n)
{
    m_nodes.push_back(n);
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