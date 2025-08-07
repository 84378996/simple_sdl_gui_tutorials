#include "Node.h"
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
    for (auto it = m_children.rbegin(); it != m_children.rend(); ++it) {
        if (it->node->Process(e)) {
            return true;
        }
    }
    return false;
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