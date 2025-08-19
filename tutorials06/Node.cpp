#include "Node.h"
#include "UITextEdit.h"
#include "SceneManager.h"
#include <algorithm>

Node::Node() {
    static uint32_t _seed = 0;
    m_node_id = ++_seed;
}

void Node::Update(uint32_t dt) {
    if (!m_enabled) return;
    for (auto& child : m_children) {
        child.node->Update(dt);
    }
}

void Node::Render() {
    if (!m_visible) return;
    std::sort(m_children.begin(), m_children.end(), [](const ChildEntry& a, const ChildEntry& b) {
        return a.zOrder < b.zOrder;
        });
    for (auto& child : m_children) {
        child.node->Render();
    }
}

bool Node::Process(SDL_Event* e) {
    if (!m_enabled) return false;

    if (e->type == SDL_MOUSEMOTION)
    {
        //处理mouseover和mouseout状态
        SDL_Point point{ e->motion.x, e->motion.y };
        HandleMouseOverOut(point);
    }
    
    SDL_Point pt = GetMousePosition();
    bool eventHandled = false;
    SDL_Rect rc = GetGlobalRect();

    if (e->type == SDL_MOUSEBUTTONDOWN)
    {
        if (auto uicontrol = dynamic_cast<UIControl*>(this))
        {
            if (uicontrol->IsFocused())
            {
                if (!SDL_PointInRect(&pt, &rc))
                {
                    if (auto ses = sSceneMgr->GetCurrentScene())
                    {
                        if (ses->GetFocusedControl())
                        {
                            ses->SetFocusControl(nullptr);
                        }
                    }
                }
            }
        }
    }

    auto tb = dynamic_cast<UITextEdit*>(this);

    bool inputevent = e->type == SDL_KEYDOWN
        || e->type == SDL_KEYUP
        || e->type == SDL_TEXTINPUT
        || e->type == SDL_TEXTEDITING;

    //检测鼠标是否在控件区域，只有在控件区域时才处理事件
    if ((SDL_PointInRect(&pt, &rc) && !inputevent) || (tb && tb->IsFocused() && inputevent))
    {
        if (!IsEnabled() || !IsVisible())
            return true;

        //从顶层子控件依次处理事件
        for (auto it = m_children.rbegin(); it != m_children.rend(); ++it)
        {
            if ((*it).node->Process(e))
            {
                eventHandled = true;
                //如果控件设置了事件穿透，继续向下传播，否则处理完成
                if (!(*it).node->IsAllowEventPassThrogh())
                    return true;
            }
        }

        //将消息路由给真正的处理函数
        if (Handle(e))
        {
            eventHandled = true;
            if (!IsAllowEventPassThrogh())
                return true;
        }
    }

    return eventHandled;

}

void Node::HandleMouseOverOut(const SDL_Point& pt)
{
    if (!IsEnabled() || !IsVisible()) return;
    SDL_Rect rc = GetGlobalRect();
    if (SDL_PointInRect(&pt, &rc))
    {
        if (!m_mouseover)
        {
            m_mouseover = true;
            OnMouseOver();
        }
    }
    else
    {
        if (m_mouseover)
        {
            m_mouseover = false;
            OnMouseOut();
        }
    }
    for (auto& n : m_children)
        n.node->HandleMouseOverOut(pt);
}

void Node::SetPosition(const Vec2& pos) {
    m_pos = pos;
}

void Node::SetSize(const Vec2& size) {
    m_size = size;
}

Vec2 Node::GetPosition() const {
    return m_pos;
}

Vec2 Node::GetSize() const {
    return m_size;
}

Vec2 Node::GetGlobalPosition() const {
    if (m_parent) {
        Vec2 p = m_parent->GetGlobalPosition();
        return { p.x + m_pos.x, p.y + m_pos.y };
    }
    return m_pos;
}

void Node::SetVisible(bool visible) {
    m_visible = visible;
}

void Node::SetEnabled(bool enabled) {
    m_enabled = enabled;
}

bool Node::IsVisible() const {
    return m_visible;
}

bool Node::IsEnabled() const {
    return m_enabled;
}

void Node::SetParent(Node* parent) {
    m_parent = parent;
}

Node* Node::GetParent() const {
    return m_parent;
}

void Node::AddChild(std::unique_ptr<Node> child, int zOrder) {
    child->SetParent(this);
    m_children.push_back({ std::move(child), zOrder });
}

void Node::RemoveChild(Node* child) {
    m_children.erase(std::remove_if(m_children.begin(), m_children.end(),
        [child](const ChildEntry& entry) { return entry.node.get() == child; }),
        m_children.end());
}

void Node::RemoveAllChildren() {
    m_children.clear();
}

uint32_t Node::GetNodeID() const {
    return m_node_id;
}

const std::string& Node::GetName() const {
    return m_name;
}

void Node::SetName(const std::string& name) {
    m_name = name;
}

void Node::AddEvent(EventType et, std::function<void(void*)> _fn)
{
    m_events[et] = _fn;
}

void Node::RemoveEvent(EventType et)
{
    if (m_events.find(et) != m_events.end())
        m_events.erase(et);
}

void Node::AddClick(std::function<void(void*)> _fn)
{
    AddEvent(EventType::ET_LCLICK, _fn);
}

void Node::AddRClick(std::function<void(void*)> _fn)
{
    AddEvent(EventType::ET_RCLICK, _fn);
}


SDL_Point Node::GetMousePosition() const
{
    int x{};
    int y{};
    SDL_GetMouseState(&x, &y);
    return { x,y };
}

SDL_Rect Node::GetGlobalRect() const
{
    auto pos = GetGlobalPosition();
    return SDL_Rect{ (int)pos.x,(int)pos.y, (int)m_size.x, (int)m_size.y };
}

void Node::FireEvent(EventType et, void* args)
{
    auto it = m_events.find(et);
    if (it != m_events.end())
        if (it->second)
            it->second(args);
}

bool Node::HasEvent(EventType et)
{
    return m_events.find(et) != m_events.end();
}
