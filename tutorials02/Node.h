#pragma once
#include "SDLRenderer.h"
#include <vector>
#include <memory>
#include <string>

struct Vec2 {
    float x, y;
};

class Node {
public:
    Node();
    virtual ~Node() = default;
    virtual void Update(uint32_t dt);
    virtual void Render();
    virtual bool Process(SDL_Event* e);
    void SetPosition(const Vec2& pos);
    void SetSize(const Vec2& size);
    Vec2 GetPosition() const;
    Vec2 GetSize() const;
    Vec2 GetGlobalPosition() const;
    virtual void SetVisible(bool visible);
    virtual void SetEnabled(bool enabled);
    bool IsVisible() const;
    bool IsEnabled() const;
    void SetParent(Node* parent);
    Node* GetParent() const;
    void AddChild(std::unique_ptr<Node> child, int zOrder = 0);
    void RemoveChild(Node* child);
    void RemoveAllChildren();
    uint32_t GetNodeID() const;
    const std::string& GetName() const;
    void SetName(const std::string& name);

protected:
    struct ChildEntry {
        std::unique_ptr<Node> node;
        int zOrder;
    };

    Vec2 m_pos{};
    Vec2 m_size{};
    Node* m_parent = nullptr;
    std::vector<ChildEntry> m_children;
    bool m_visible = true;
    bool m_enabled = true;

    uint32_t m_node_id;
    std::string m_name;
};
