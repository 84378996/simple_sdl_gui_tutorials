#pragma once

#include "Node.h"

enum UIState {
    Normal,
    Hovered,
    Pressed,
    Disabled,
    UIStateLength
};

class UIControl : public Node
{
public:
    UIControl();
    virtual ~UIControl() = default;

    // 更新与渲染
    virtual void Render() override = 0;
    virtual bool Handle(SDL_Event* e) override;

    // 文本相关接口
    void SetText(const std::string& text);
    const std::string& GetText() const;

    void SetFontSize(int size);
    int GetFontSize() const;

    void SetTextColor(const SDL_Color& color);
    SDL_Color GetTextColor() const;

    // 背景颜色
    void SetBackgroundColor(const SDL_Color& color);
    SDL_Color GetBackgroundColor() const;

    // 控件状态
    UIState GetState() const { return m_state; }
    void SetState(UIState state) { m_state = state; }

    bool SetFocus(bool _focus);
    inline bool IsFocused() const { return m_isFocus; }

    void SetEnabled(bool enabled) override;
    void OnMouseOut() override;

    virtual void OnTextChanged(const std::string& oldText) {};
    virtual void OnFontSizeChanged(const int oldFontsize) {};
    virtual void OnFocusChanged() {};

protected:
    std::string m_text;
    int m_fontSize = 16;
    SDL_Color m_textColor = { 0, 0, 0, 255 };
    SDL_Color m_backgroundColor = { 255, 255, 255, 255 };
    UIState m_state = UIState::Normal;
    bool m_isFocus{ false };
    
};