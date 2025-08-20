#pragma once
#include "SDLRenderer.h"
#include <vector>
#include <memory>
#include <string>
#include <functional>

const float EPSILON = 0.000001f;

struct Vec2 {
    float x, y;
};

bool operator==(const Vec2& lhs, const Vec2& rhs);
bool operator!=(const Vec2& lhs, const Vec2& rhs);

enum class EventType
{
    ET_LCLICK,
    ET_RCLICK,
    ET_MOUSE_OVER,
    ET_MOUSE_OUT,
    ET_MOUSE_WHEEL,
    ET_MOUSE_MOVE,
    ET_DOUBLE_CLICK,
    ET_SCROLL_CHANGED,
    ET_ITEMSELECTIONCHANGED,
    ET_GOTFOCUS,
    ET_LOSEFOCUS,
    ET_CHECKCHANGED,
    ET_VALUECHANGED,
    ET_VISIABLECHANGED,
    ET_ITEM_HOVER,
    ET_ITEM_OUT,
    ET_ITEM_CLICK,
    ET_ITEM_DBCLICK,
    ET_ITEM_RCLICK,
    ET_NUM_OF_EVENTS,
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

    // Called when the mouse cursor enters the element's area
    virtual void OnMouseOver() {}

    // Called when the mouse cursor leaves the element's area
    virtual void OnMouseOut() {}

    // Handles incoming SDL events (returns true if event was consumed)
    virtual bool Handle(SDL_Event* e) { return false; };

    virtual void OnSize(int _w, int _h) {};
    virtual void OnMove(int _x, int _y) {};

    // Registers a callback function for a specific event type
    void AddEvent(EventType et, std::function<void(void*)> _fn);

    // Unregisters all callbacks for the specified event type
    void RemoveEvent(EventType et);

    // Convenience method for adding click event handlers
    void AddClick(std::function<void(void*)> _fn);
    void AddRClick(std::function<void(void*)> _fn);

    // Enables/disables event propagation to underlying elements
    inline void SetAllowEventPassThrough(bool _allowEventPassthrouth) { m_allowEventPassthrouth = _allowEventPassthrouth; }

    // Checks if events are allowed to pass through to underlying elements
    inline bool IsAllowEventPassThrogh() const { return m_allowEventPassthrouth; }

protected:

    // Handles mouse enter/leave detection based on current cursor position
    void HandleMouseOverOut(const SDL_Point& pt);

    // Gets current mouse cursor position relative to application window
    SDL_Point GetMousePosition() const;

    // Gets element's absolute position and size in screen coordinates
    SDL_Rect GetGlobalRect() const;

    // Triggers all callbacks registered for the specified event type
    void FireEvent(EventType et, void* args);

    // Checks if any callbacks are registered for the specified event type
    bool HasEvent(EventType et);

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
    bool m_mouseover = false;

    uint32_t m_node_id{};
    std::string m_name;
    bool m_allowEventPassthrouth{ false };
    std::unordered_map<EventType, std::function<void(void*)>> m_events;
};
