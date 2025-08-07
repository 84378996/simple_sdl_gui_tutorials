#pragma once

#include "Node.h"

enum class UIState {
    Normal,
    Hovered,
    Pressed,
    Disabled
};

class UIControl : public Node
{
public:
    UIControl();
    virtual ~UIControl() = default;

    // ��������Ⱦ
    virtual void Render() override = 0;
    virtual bool Process(SDL_Event* e) override;

    // �ı���ؽӿ�
    void SetText(const std::string& text);
    const std::string& GetText() const;

    void SetFontSize(int size);
    int GetFontSize() const;

    void SetTextColor(const SDL_Color& color);
    SDL_Color GetTextColor() const;

    // ������ɫ
    void SetBackgroundColor(const SDL_Color& color);
    SDL_Color GetBackgroundColor() const;

    // �ؼ�״̬
    UIState GetState() const { return m_state; }
    void SetState(UIState state) { m_state = state; }

    void SetEnabled(bool enabled) override;

    virtual void OnClick() {};
    virtual void OnMouseHover() {};
    virtual void OnMouseOut() {};

protected:
    std::string m_text;
    int m_fontSize = 16;
    SDL_Color m_textColor = { 0, 0, 0, 255 };
    SDL_Color m_backgroundColor = { 255, 255, 255, 255 };
    UIState m_state = UIState::Normal;
};